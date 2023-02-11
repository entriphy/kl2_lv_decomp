import csv
import gspread
import os
import sys

if __name__ == "__main__":
    if len(sys.argv) > 1:
        csv_file = sys.argv[1]
    else:
        csv_file = input("Enter path to csv file: ")
    if not os.path.exists(csv_file):
        print("csv file does not exist; exiting...")
        exit(1)

    gc = gspread.service_account()
    wks = gc.open_by_key("190e6yPZklUfM2ye2Pkpta4B5KNp9lJFB0Ldx90eCVzU").sheet1
    addresses = [int(i) for i in wks.col_values(1)[1:]]

    update = [[] for _ in range(10000)]
    with open(csv_file, "r") as f:
        # Address,,Name,Return Type,Parameters,Reference Count
        csv_reader = csv.reader(f, delimiter=',')
        for function in csv_reader:
            address = int(function[0])
            function[1] = None
            if function[2] == "":
                # Function does not have a name
                function[2] = None
            try:
                # Check if function already exists in table
                index = addresses.index(address)
                if index == -1:
                    raise "dne"
            except:
                index = len(addresses)
                addresses.append(address)
            update[index] = function
    wks.update("A2", update)
