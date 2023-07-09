from elftools.elf.elffile import ELFFile, SymbolTableSection
from colorama import Fore, Back, Style
import argparse
import json
import os
import rabbitizer
import struct


class Function:
    name: str
    address: int
    size: int

    def __init__(self, name: str, address: int, size: int):
        self.name = name
        self.address = address
        self.size = size


class GenericElf:
    elf: ELFFile
    text: bytes
    text_offset: int
    functions: dict[str, Function]

    def __init__(self, elf: ELFFile):
        self.elf = elf
        text_section = next(t for t in elf.iter_sections() if t.name == ".text")
        self.text = text_section.data()
        self.text_offset = text_section.header.sh_addr

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
        function_symbols = list(sorted(
            [sym for sym in symbol_table.iter_symbols() if sym.entry.st_info.type == "STT_FUNC"],
            key=lambda sym: sym.entry.st_value  # Sort by address
        ))
        self.functions = {func.name: Function(func.name, func.entry.st_value, func.entry.st_size) for func in function_symbols}


class ElfWithJson(GenericElf):
    def __init__(self, elf: ELFFile, funcs_json: dict):
        super().__init__(elf)
        self.functions = {func["name"]: Function(func["name"], func["address"], -1) for func in funcs_json.get("functions")}


def compare_functions(x: list[rabbitizer.Instruction], y: list[rabbitizer.Instruction]) -> int | None:
    for i, instr in enumerate(x):
        if not instr.sameOpcode(y[i]):
            return i
        if instr.sameOpcodeButDifferentArguments(y[i]):
            if instr.isJump() or instr.canBeLo() or instr.canBeHi():
                continue
            if (instr.doesLoad() or instr.doesStore()) and instr.rs == rabbitizer.RegGprO32.gp:
                continue
            return i
    return None


def read_elf(elf_path: str, json_path: str = None) -> GenericElf:
    if json_path is not None:
        with open(json_path, "r") as f:
            js = json.load(f)
        with open(elf_path, "rb") as f:
            elf = ElfWithJson(ELFFile(f), js)
    else:
        with open(elf_path, "rb") as f:
            elf = ElfWithSymbols(ELFFile(f))
    return elf


def diff(orig_elf_path, decomp_elf_path, orig_json):
    orig_elf = read_elf(orig_elf_path, orig_json)
    decomp_elf = read_elf(decomp_elf_path)
    print(Style.BRIGHT + f"{os.path.basename(orig_elf_path)} -> {'/'.join(os.path.abspath(decomp_elf_path).split('/')[-2:])[:-2]}:" + Style.RESET_ALL)

    for func_name in orig_elf.functions.keys():
        if func_name not in decomp_elf.functions:
            continue
        func_size = decomp_elf.functions[func_name].size
        orig_func = orig_elf.get_function_instructions(func_name, func_size)
        decomp_func = decomp_elf.get_function_instructions(func_name)
        result = compare_functions(orig_func, decomp_func)
        if result is not None:
            orig_address = f"0x{orig_elf.functions[func_name].address + result * 4:08x}"
            decomp_address = f"0x{decomp_elf.functions[func_name].address + result * 4:08x}"
            print(Fore.RED + f"\t{func_name}: FAIL" + Style.RESET_ALL)
            print(f"\t\t{orig_address}: {orig_func[result]}")
            print(f"\t\t{decomp_address}: {decomp_func[result]}")
        else:
            print(Fore.GREEN + f"\t{func_name}: PASS" + Style.RESET_ALL)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        prog="elf_diff",
        description="Compares an ELF and a decompiled ELF's functions",
    )
    parser.add_argument("orig_elf")
    parser.add_argument("decomp_elf")
    parser.add_argument("-j", "--json")
    args = parser.parse_args()

    diff(args.orig_elf, args.decomp_elf, args.json)
