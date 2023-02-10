# Exports all functions and their addresses + parameters to a csv file
import os

directory = askDirectory("Select output directory", "Ok")
f = open(os.path.join(directory.absolutePath, "functions.csv"), "w")

for i in list(getCurrentProgram().getSymbolTable().getAllSymbols(False)):
    if i.symbolType.equals(i.symbolType.FUNCTION):
        address = i.address.unsignedOffset
        function = getFunctionAt(i.address)
        name = None if i.name.startswith("FUN") else i.name
        params = function.getParameters(None)
        return_type = function.getReturnType().getName()

        # Address (dec), Address (hex), Name, Return, Parameters
        f.write('%d,,%s,%s,"%s"\n' % (address, name or "", return_type, ", ".join([(param.getDataType().getName()) + " " + (param.getName() or "") for param in params])))

f.close()