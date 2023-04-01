import json
import gspread
import os
import sys

def main():
    if len(sys.argv) > 1:
        json_file = sys.argv[1]
    else:
        json_file = input("Enter path to stdump json file: ")
    if not os.path.exists(json_file):
        print("File does not exist; exiting...")
        exit(1)
    
    gc = gspread.service_account()
    wks = gc.open_by_key("190e6yPZklUfM2ye2Pkpta4B5KNp9lJFB0Ldx90eCVzU").worksheet("IOP Functions")
    addresses = [int(i) for i in wks.col_values(1)[1:]]

    update = [[] for _ in range(10000)]
    with open(json_file) as f:
        stdump = json.load(f)
        for file in stdump["files"]:
            filename = file["relative_path"]
            for function in file["functions"]:
                address = function["address_range"]["low"]
                name = function["name"]
                return_type = get_type_str(function["type"]["return_type"])
                parameters = ", ".join([f"{get_type_str(param['type'])} {param['name']}" for param in function["type"]["parameters"]])
                locals = ", ".join([f"{get_type_str(local['type'])} {local['name']}" for local in function["locals"]])
                line_numbers = ", ".join([str(line[1]) for line in function["line_numbers"]])
                try:
                    # Check if function already exists in table
                    index = addresses.index(address)
                    if index == -1:
                        raise "dne"
                except:
                    index = len(addresses)
                    addresses.append(address)
                update[index] = [address, None, name, return_type, parameters, locals, line_numbers, filename]
    
    wks.update("A2", update)

def get_type_str(t) -> str:
    descriptor = t["descriptor"]
    match descriptor:
        case "type_name":
            return t["type_name"]
        case "pointer":
            return t["value_type"]["type_name"] + " *"
        case "array":
            return f"{t['element_type']['type_name']}[{t['element_count']}]"
        case _:
            raise Exception(descriptor)

if __name__ == "__main__":
    main()