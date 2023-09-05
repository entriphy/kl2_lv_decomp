# Exports all functions to a JSON file
import os
import json

directory = askDirectory("Select output directory", "Ok")

functions = []
program = getCurrentProgram()
listing = program.getListing()
symbolTable = program.getSymbolTable()


for symbol in list(symbolTable.getAllSymbols(False)):
    if symbol.symbolType.equals(symbol.symbolType.FUNCTION):
        address = symbol.address.unsignedOffset
        function = getFunctionAt(symbol.address)
        name = None if symbol.name.startswith("FUN") else symbol.getName(address >= 0x0023A3E8)
        params = function.getParameters(None)
        return_type = function.getReturnType().getName()
        functions.append({
            "name": name or "func_%08X" % address,
            "address": address,
            "return_type": return_type,
            "params": [{
                "name": param.getName() or "",
                "type": param.getDataType().getName()
            } for param in params],
            "refs": symbol.getReferenceCount()
        })

with open(os.path.join(directory.absolutePath, "functions.json"), "w") as f:
    json.dump({"functions": functions}, f)