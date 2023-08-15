from elftools.elf.elffile import ELFFile, SymbolTableSection, RelocationSection
from colorama import Fore, Style
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
    relocation_addrs: list[int]

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
        relocations = next(s for s in elf.iter_sections() if isinstance(s, RelocationSection))
        function_symbols = list(sorted(
            [sym for sym in symbol_table.iter_symbols() if sym.entry.st_info.type == "STT_FUNC"],
            key=lambda sym: sym.entry.st_value  # Sort by address
        ))
        self.functions = {func.name: Function(func.name, func.entry.st_value, func.entry.st_size) for func in function_symbols}
        self.relocation_addrs = [r.entry.r_offset for r in relocations.iter_relocations()]


class ElfWithJson(GenericElf):
    def __init__(self, elf: ELFFile, funcs_json: dict):
        super().__init__(elf)
        self.functions = {func["name"]: Function(func["name"], func["address"], -1) for func in funcs_json.get("functions")}


class NonMatchingFunction:
    orig_func: Function
    decomp_func: Function
    orig_func_instrs: list[rabbitizer.Instruction]
    decomp_func_instrs: list[rabbitizer.Instruction]
    idx: int

    def __init__(self, orig_func, decomp_func, orig_func_instrs, decomp_func_instrs):
        self.orig_func = orig_func
        self.decomp_func = decomp_func
        self.orig_func_instrs = orig_func_instrs
        self.decomp_func_instrs = decomp_func_instrs
        self.idx = -1


def compare_functions(orig_elf: GenericElf, decomp_elf: GenericElf, name: str) -> NonMatchingFunction | None:
    orig_func = orig_elf.functions[name]
    decomp_func = decomp_elf.functions[name]
    func_size = decomp_func.size
    orig_func_instrs = orig_elf.get_function_instructions(name, func_size)
    decomp_func_instrs = decomp_elf.get_function_instructions(name)

    non_matching = NonMatchingFunction(orig_func, decomp_func, orig_func_instrs, decomp_func_instrs)
    for i, instr in enumerate(orig_func_instrs):
        if not instr.sameOpcode(decomp_func_instrs[i]):
            non_matching.idx = i
            break
        if instr.sameOpcodeButDifferentArguments(decomp_func_instrs[i]):
            if instr.isJump():
                continue
            if orig_func.address + i * 4 in orig_elf.relocation_addrs or decomp_func.address + i * 4 in decomp_elf.relocation_addrs:
                continue
            non_matching.idx = i
            break
    return None if non_matching.idx == -1 else non_matching


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
        result = compare_functions(orig_elf, decomp_elf, func_name)
        if result is not None:
            orig_address = f"0x{result.orig_func.address + result.idx * 4:08x}"
            decomp_address = f"0x{decomp_elf.functions[func_name].address + result.idx * 4:08x}"
            print(Fore.RED + f"\t{func_name}: FAIL" + Style.RESET_ALL)
            print(f"\t\t{orig_address}: {result.orig_func_instrs[result.idx]}")
            print(f"\t\t{decomp_address}: {result.decomp_func_instrs[result.idx]}")
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
