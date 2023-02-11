# Exports all functions and their addresses + parameters to a csv file
import os

directory = askDirectory("Select output directory", "Ok")
f = open(os.path.join(directory.absolutePath, "functions.csv"), "w")

for symbol in list(getCurrentProgram().getSymbolTable().getAllSymbols(False)):
    if symbol.symbolType.equals(symbol.symbolType.FUNCTION):
        address = symbol.address.unsignedOffset
        function = getFunctionAt(symbol.address)
        name = None if symbol.name.startswith("FUN") else symbol.name
        params = function.getParameters(None)
        return_type = function.getReturnType().getName()

        # Address (dec), Address (hex), Name, Return, Parameters, Reference Count
        f.write('%d,,%s,%s,"%s",%d\n' % (address, name or "", return_type, ", ".join([(param.getDataType().getName()) + " " + (param.getName() or "") for param in params]), symbol.getReferenceCount()))

f.close()