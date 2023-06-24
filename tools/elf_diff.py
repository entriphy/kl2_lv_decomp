from elftools.elf.elffile import ELFFile, SymbolTableSection
import rabbitizer
import struct
import json
import argparse


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

    def get_function_instructions(self, name: str) -> list[rabbitizer.Instruction]:
        func = self.functions.get(name)
        if func is None:
            raise Exception("Function not found: " + name)
        return self.__get_function(func.address - self.text_offset, func.size)

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


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        prog="elf_diff",
        description="Compares an ELF and a decompiled ELF's functions",
    )
    parser.add_argument("orig_elf")
    parser.add_argument("decomp_elf")
    parser.add_argument("-j", "--json")
    args = parser.parse_args()

    orig_elf = read_elf(args.orig_elf, args.json)
    decomp_elf = read_elf(args.decomp_elf)

    for func_name in orig_elf.functions.keys():
        orig_func = orig_elf.get_function_instructions(func_name)
        decomp_func = decomp_elf.get_function_instructions(func_name)
        result = compare_functions(orig_func, decomp_func)
        if result is not None:
            orig_address = f"0x{orig_elf.functions[func_name].address:08x}"
            decomp_address = f"0x{decomp_elf.functions[func_name].address:08x}"
            print(f"{func_name}: FAIL ({orig_func[result]} @ {orig_address} != {decomp_func[result]} @ {decomp_address})")
        else:
            print(f"{func_name}: PASS")

