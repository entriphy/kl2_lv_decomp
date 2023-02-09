import typing
import ghidra_bridge
import gspread

if __name__ == "__main__":
    if typing.TYPE_CHECKING:
        import ghidra
        from ghidra.ghidra_builtins import *
    else:
        b = ghidra_bridge.GhidraBridge(namespace=globals())
    
    gc = gspread.service_account()
    wks = gc.open_by_key("190e6yPZklUfM2ye2Pkpta4B5KNp9lJFB0Ldx90eCVzU").sheet1
    addresses = [int(i) for i in wks.col_values(1)[1:]]

    for i in getCurrentProgram().getSymbolTable().getAllSymbols(False):
        if i.symbolType.equals(i.symbolType.FUNCTION):
            address = i.address.unsignedOffset
            function = getFunctionAt(i.address)
            name = None if i.name.startswith("FUN") else i.name
            params = function.getParameters(None)
            return_type = function.getReturnType().getName()

            try:
                # Check if function already exists in table
                index = addresses.index(address) + 2
            except:
                # Function does not exist, add it to table
                index = len(addresses) + 2
                addresses.append(address)

            # Address (dec), Address (hex), Name, Return, Parameters
            wks.update("A" + str(index), [[address, None, name, return_type, ", ".join([f"{param.getDataType().getName()} {param.getName() or ''}" for param in params])]])
