# Exports all symbols to a symbol_addrs.txt file
import os

all_funcs = askYesNo("meow", "Export unnamed functions?")
directory = askDirectory("Select output directory", "Ok")
program = getCurrentProgram()
listing = program.getListing()
symbolTable = program.getSymbolTable()

with open(os.path.join(directory.absolutePath, "symbol_addrs.txt" if not all_funcs else "symbol_addrs_all.txt"), "w") as f:
    for symbol in list(symbolTable.getAllSymbols(False)):
        address = symbol.address.unsignedOffset
        if symbol.symbolType.equals(symbol.symbolType.FUNCTION):
            if symbol.name.startswith("FUN"):
                if all_funcs:
                    name = "func_%08X" % address
                else:
                    continue
            else:
                name = symbol.getName(address >= 0x0023A3E8)
            f.write("%s = 0x%08X; // type:func\n" % (name, address))
        elif symbol.symbolType.equals(symbol.symbolType.LABEL):
            name = symbol.getName(False)
            data = listing.getDataAt(symbol.address)
            if data is None:
                continue
            data_type = data.getDataType()
            data_size = data_type.getLength()
            if data_size == -1:
                continue
            f.write("%s = 0x%08X; // type:symbol size:0x%X\n" % (name, address, data_size))