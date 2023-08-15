from elf_diff import diff
import argparse
from colorama import Style
import json
import os
import re

if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        prog="build_diff",
        description="Compares object files against an ELF file",
    )
    parser.add_argument("orig_elf")
    parser.add_argument("build_path")
    parser.add_argument("-j", "--json")
    args = parser.parse_args()

    # Requires -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
    with open(os.path.join(args.build_path, "compile_commands.json"), "r") as f:
        compile_comands = json.load(f)
    objects = []
    for command in compile_comands:
        # Older versions of CMake don't have the "output" field, so we have to regex it D:
        o = re.findall(r"(CMakeFiles/kl2_lv_decomp\.dir/src/.+?\.o)", command["command"])
        if len(o) > 0:
            objects.append(o[0])
    for o in objects:
        object_path = os.path.join(args.build_path, o)
        try:
            diff(args.orig_elf, object_path, args.json)
        except Exception as e:
            print(Style.BRIGHT + f"{os.path.basename(args.orig_elf)} -> {'/'.join(os.path.abspath(object_path).split('/')[-2:])[:-2]}: FAILED" + Style.RESET_ALL)
            print("\t" + str(e))
        print()
