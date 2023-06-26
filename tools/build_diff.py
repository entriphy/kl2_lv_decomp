from elf_diff import diff
import argparse
import glob
import os

if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        prog="build_diff",
        description="Compares object files against an ELF file",
    )
    parser.add_argument("orig_elf")
    parser.add_argument("build_path")
    parser.add_argument("-j", "--json")
    args = parser.parse_args()

    object_files = glob.glob(os.path.join(args.build_path, "**/*.c.o"), recursive=True)
    object_files += glob.glob(os.path.join(args.build_path, "**/*.cc.o"), recursive=True)
    for object in object_files:
        diff(args.orig_elf, object, args.json)
