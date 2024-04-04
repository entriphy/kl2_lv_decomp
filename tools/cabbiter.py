from elftools.elf.elffile import ELFFile, SymbolTableSection, RelocationSection
from colorama import Fore, Style
import argparse
import json
import os
import rabbitizer
import re
import requests
import struct


class Function:
    name: str
    address: int
    size: int

    def __init__(self, name: str, address: int, size: int):
        self.name = name
        self.address = address
        self.size = size


class SymbolAddr:
    name: str
    address: int
    params: dict

    def __init__(self, line: str):
        symbol = re.match(r"(.+) +?= *(0x[0-9a-fA-F]+);(?: *\/\/\s*(.+:.+)+)?", line)
        self.name = symbol[1]
        self.address = int(symbol[2], 16)
        self.params = { param[0]: param[1] for param in list(map(lambda x: x.split(":"), symbol[3].split(" "))) }


class GenericElf:
    elf: ELFFile
    text: bytes
    text_offset: int
    functions: dict[str, Function]
    relocation_addrs: list[int]
    path: str

    def __init__(self, elf: ELFFile):
        self.elf = elf
        text_section = next(t for t in elf.iter_sections() if t.name == ".text")
        self.text = text_section.data()
        self.text_offset = text_section.header.sh_addr
        self.relocation_addrs = []

    def get_function_instructions(self, name: str, size: int = -1) -> list[rabbitizer.Instruction]:
        func = self.functions.get(name)
        if func.size != -1:
            size = func.size
        if func is None:
            raise Exception("Function not found: " + name)
        return self.__get_function(func.address - self.text_offset, size)

    def __get_function(self, address: int, size: int) -> list[rabbitizer.Instruction]:
        if size == -1:
            raise Exception("Function has no defined size")

        instructions = []
        for i in range(size // 4):
            instr_word = struct.unpack("i", self.text[address + i * 4:address + (i + 1) * 4])[0]
            instr = rabbitizer.Instruction(instr_word, category=rabbitizer.InstrCategory.R5900)
            instructions.append(instr)
        return instructions


class ElfWithSymbols(GenericElf):
    def __init__(self, elf: ELFFile):
        super().__init__(elf)
        symbol_table = next(s for s in elf.iter_sections() if isinstance(s, SymbolTableSection))
        relocation_sections = [s for s in elf.iter_sections() if isinstance(s, RelocationSection)]
        if len(relocation_sections) > 0:
            relocations = relocation_sections[0]
        function_symbols = list(sorted(
            [sym for sym in symbol_table.iter_symbols() if sym.entry.st_info.type == "STT_FUNC"],
            key=lambda sym: sym.entry.st_value  # Sort by address
        ))
        self.functions = {func.name: Function(func.name, func.entry.st_value, func.entry.st_size) for func in function_symbols}
        self.relocation_addrs = [r.entry.r_offset for r in relocations.iter_relocations()] if len(relocation_sections) > 0 else []


class ElfWithSymbolAddrs(GenericElf):
    def __init__(self, elf: ELFFile, symbol_addrs: dict[int, SymbolAddr]):
        super().__init__(elf)
        self.functions = { symbol.name: Function(symbol.name, symbol.address, -1) for symbol in symbol_addrs.values() if symbol.params["type"] == "func" }


class FunctionCompareResult:
    target_func: Function
    decomp_func: Function
    target_func_instrs: list[rabbitizer.Instruction]
    decomp_func_instrs: list[rabbitizer.Instruction]
    nonmatching_idx: int

    def __init__(self, target_func, decomp_func, target_func_instrs, decomp_func_instrs):
        self.target_func = target_func
        self.decomp_func = decomp_func
        self.target_func_instrs = target_func_instrs
        self.decomp_func_instrs = decomp_func_instrs
        self.nonmatching_idx = -1


class ParuuUpdate:
    address: int
    matching: bool
    source_file: str

    def __init__(self, address: int, matching: bool, source_file: str) -> None:
        self.address = address
        self.matching = matching
        self.source_file = source_file


def compare_functions(target_elf: GenericElf, decomp_elf: GenericElf, name: str) -> FunctionCompareResult:
    target_func = target_elf.functions[name]
    decomp_func = decomp_elf.functions[name]
    func_size = decomp_func.size
    target_func_instrs = target_elf.get_function_instructions(name, func_size)
    decomp_func_instrs = decomp_elf.get_function_instructions(name)

    result = FunctionCompareResult(target_func, decomp_func, target_func_instrs, decomp_func_instrs)
    for i, instr in enumerate(target_func_instrs):
        if not instr.sameOpcode(decomp_func_instrs[i]):
            result.nonmatching_idx = i
            break
        if instr.sameOpcodeButDifferentArguments(decomp_func_instrs[i]):
            if instr.isJump():
                continue
            if target_func.address + i * 4 in target_elf.relocation_addrs or decomp_func.address + i * 4 in decomp_elf.relocation_addrs:
                continue
            result.nonmatching_idx = i
            break
    return result


def read_elf(elf_path: str, symbol_addrs: dict[int, SymbolAddr] = None) -> GenericElf:
    with open(elf_path, "rb") as f:
        elf = ElfWithSymbolAddrs(ELFFile(f), symbol_addrs) if symbol_addrs is not None else ElfWithSymbols(ELFFile(f))
    elf.path = elf_path
    return elf


def diff(target_elf: GenericElf, decomp_elf: GenericElf, symbol_addrs: dict[int, SymbolAddr]) -> dict[str, FunctionCompareResult]:
    ret = {}
    for func_name in decomp_elf.functions.keys():
        if func_name not in target_elf.functions:
            continue
        result = compare_functions(target_elf, decomp_elf, func_name)
        ret[func_name] = result
    return ret


def print_diff(target_elf: GenericElf, decomp_elf: GenericElf, results: dict[str, FunctionCompareResult]):
    print(Style.BRIGHT + f"{'/'.join(os.path.abspath(decomp_elf.path).split('/')[-2:])[:-2]} -> {os.path.basename(target_elf.path)}:" + Style.RESET_ALL)

    for func_name, result in results.items():
        if result.nonmatching_idx == -1:
            print(Fore.GREEN + f"\t{func_name}: PASS" + Style.RESET_ALL)
        else:
            target_address = f"0x{result.target_func.address + result.nonmatching_idx * 4:08x}"
            decomp_address = f"0x{decomp_elf.functions[func_name].address + result.nonmatching_idx * 4:08x}"
            print(Fore.RED + f"\t{func_name}: FAIL" + Style.RESET_ALL)
            print(f"\t\t{target_address}: {result.target_func_instrs[result.nonmatching_idx]}")
            print(f"\t\t{decomp_address}: {result.decomp_func_instrs[result.nonmatching_idx]}")


def read_symbol_addrs(symbol_addrs_path: str) -> dict[int, SymbolAddr]:
    symbol_addrs = {}
    with open(symbol_addrs_path, "r") as f:
        for line in f.readlines():
            if not line:
                continue
            symbol = SymbolAddr(line)
            symbol_addrs[symbol.address] = symbol
    return symbol_addrs


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        prog="cabbiter",
        description="Compares an ELF and a decompiled ELF's functions",
    )
    parser.add_argument("--target",
                        help="The original ELF file to compare against", required=True)
    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument("--decomp", 
                       help="The ELF file with decompiled functions")
    group.add_argument("--cmake",
                       help="Path to CMake build directory (requires -DCMAKE_EXPORT_COMPILE_COMMANDS=ON)")
    parser.add_argument("--symbol-addrs",
                        default=os.path.join(os.path.dirname(__file__), "symbol_addrs_all.txt"),
                        required=False,
                        help="Path to symbol_addrs.txt file")
    args = parser.parse_args()

    symbol_addrs = read_symbol_addrs(args.symbol_addrs)
    target_elf = read_elf(args.target, symbol_addrs)
    if args.decomp is not None:
        decomp_elf = read_elf(args.decomp)
        diff(target_elf, decomp_elf, symbol_addrs)
    else:
        with open(os.path.join(args.cmake, "compile_commands.json"), "r") as f:
            compile_comands = json.load(f)
        
        objects: list[tuple[str, str]] = []
        for command in compile_comands:
            # Older versions of CMake don't have the "output" field, so we have to regex it D:
            o = re.findall(r"-o (.+?) -c (.+?)(?:$| )", command["command"])
            if len(o) > 0:
                o = o[0]
                objects.append((o[1], o[0]))
        
        paruu_results: list[ParuuUpdate] = []
        for in_file, out_file in objects:
            object_path = os.path.join(args.cmake, out_file)
            try:
                object_elf = read_elf(object_path)
                results = diff(target_elf, object_elf, symbol_addrs)
                print_diff(target_elf, object_elf, results)
                for result in results.values():
                    paruu_results.append(ParuuUpdate(result.target_func.address, result.nonmatching_idx == -1, "/".join(in_file.split("/")[-2:])))
            except Exception as e:
                print(Fore.RED + f"Error when diffing file: {e}" + Style.RESET_ALL)
            print()
        
        paruu_api_url = os.environ.get("PARUU_API_URL")
        paruu_token = os.environ.get("PARUU_TOKEN")
        if paruu_api_url is not None and paruu_token is not None:
            print("Updating Paruu entries...")
            response = requests.post(f"{paruu_api_url}/entry/lv", json=[result.__dict__ for result in paruu_results], headers={"Authorization": paruu_token})
            print(f"API response {response.status_code}: {response.text}")
