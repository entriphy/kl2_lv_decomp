import argparse
import os
import re
import struct
import subprocess
from binary_reader import BinaryReader, Whence
from elftools.elf.elffile import ELFFile

VUDIS_PATH = ""  # Enter path to vudis binary here (built from https://github.com/chaoticgd/vutrace)
PROG_NAMES = [
    # Klonoa 2 program names
    # TODO: Automatically get these names from debug symbols
    # "hr_vu1c_mp",
    # "hr_vu1m_avi",
    # "hr_vu1m_av0",
    # "hr_vu1m_av1",
    # "hr_vu1m_av1o",
    # "hr_vu1m_av2",
    # "hr_vu1_vpa",
    # "hr_vu1_vpa_v",
    # "hr_vu1m_iv",
    # "hr_vu1m_bg",
    # "hr_vu1m_mirr",
    # "hr_vu1_vpo",
    # "hr_vu1m_wire",
    # "hr_vu1m_ioff",
    # "hr_vu1m_fogc",
    # "p_polhit",
    # "p_vu1m_iv",
    # "p_vu1o_iv",
    # "Ao_particle_start"
]

if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        prog="microprog_disasm",
        description="Extracts VU microprograms from an ELF file"
    )
    parser.add_argument("elf_path", help="Path to the ELF file to extract from")
    parser.add_argument("section", help="Section of the ELF to parse (i.e. vutext, vudata)")
    parser.add_argument("output_dir", help="Name of directory to write all micropgrams to")
    args = parser.parse_args()

    # Create out directory if it does not exist
    output_dir = args.output_dir
    if not os.path.exists(output_dir):
        os.mkdir(output_dir)

    # Read ELF file and VU section
    with open(args.elf_path, "rb") as f:
        elf = ELFFile(f)
        vu_section = elf.get_section_by_name("." + args.section)
        vu_address = vu_section.header.sh_addr
        vu_bytes = vu_section.data()
        with open(f"{output_dir}/MAIN_{args.section}.bin", "wb") as v:
            v.write(vu_bytes)
            
        reader = BinaryReader(vu_bytes)
    
    # Read each microprogram
    prog_num = 0
    while True:
        prog_name = PROG_NAMES[prog_num] if prog_num < len(PROG_NAMES) else f"prg{prog_num}"
        if reader.pos() >= len(reader.buffer()):
            break
        address = vu_address + reader.pos()
        dmatag = reader.read_uint32()
        qwc = dmatag & 0xFFFF
        tag = (dmatag >> 28) & 0x7
        if tag == 1 or tag == 7:
            reader.seek(4, Whence.CUR)
        elif tag == 3:
            reader.seek(12, Whence.CUR)
        else:
            raise Exception(f"Unexpected DMAtag id (got {tag})")

        microprog = bytes()
        while True:
            nop = reader.read_uint32()
            vifcode = reader.read_uint32()
            imm = vifcode & 0xFFFF
            num = (vifcode >> 16) & 0xFF
            cmd = (vifcode >> 24) & 0xFF
            if cmd != 0x4A:  # MPG
                raise Exception(f"Unexpected VIFcode (got {cmd})")
            size = 2048 if num == 0 else num * 8
            microprog += reader.read_bytes(size)
            if num != 0:
                break
        reader.align_pos(0x10)
        if reader.pos() < len(vu_bytes):
            dmatag_end = reader.read_uint32()
            reader.seek(12, Whence.CUR)
            if tag == 3 and ((dmatag_end >> 28) & 0x7) != 7:
                raise Exception("Could not find end of REF DMAtag")

        # Write raw microprogram
        with open(f"{output_dir}/{prog_name}.bin", "wb") as f:
            f.write(microprog)

        # Write output from vudis
        vudis = subprocess.run([VUDIS_PATH, f"{output_dir}/{prog_name}.bin"], capture_output=True, text=True)
        with open(f"{output_dir}/{prog_name}.vsm_vudis", "w") as f:
            f.write(vudis.stdout)

        # Parse vudis output to make it compilable
        instructions = re.findall(r"([0-9a-f]{4}): \(([0-9a-f]{8})\) (.+?) [ \n]", vudis.stdout)
        labels = set()
        instr_map = {}
        for instr in instructions:
            address = instr[0]
            op_bytes = int(instr[1], 16)
            op = instr[2].replace("$", "0x").split(" ")
            opcode = op[0]
            args = " ".join(op[1:]).split(", ") if len(op) > 1 else []
            flag_bit = None

            if opcode[0].isdigit() or opcode[0] == "-":
                args.append(str(struct.unpack("!f", op_bytes.to_bytes(4))[0]))
                opcode = "LOI"

            if len(args) > 0:
                if " + " in args[-1]:  # Load/save
                    r = re.findall(r"(.+) \+ (.+)", args[-1])[0]
                    args[-1] = f"{r[1]}({r[0]})"
                if args[-1].endswith("++") or args[-1].startswith("--"):  # Load/save with increment/decrement
                    args[-1] = f"({args[-1]})"
                if args[-1].endswith("]"):
                    bracket = re.findall(r"\[(.+?)]", args[-1])[0]
                    if len(bracket) == 1:  # Flag bit
                        flag_bit = bracket
                        args[-1] = args[-1][:-4]
                        if len(args[-1]) == 0:
                            args.pop()
                    elif len(bracket) == 4:  # Branch/jump
                        if opcode == "JR":
                            args[-1] = bracket
                        else:
                            labels.add(bracket)
                            if opcode == "B":
                                args.pop()
                            else:
                                args[-1] = args[-1][:-7]
                            args.append(f"LAB_{prog_num}_{bracket}")
                    else:
                        raise "Unknown thingy"

            match opcode:
                case "MOVE.xyzw":
                    if args[0] == "vf00" and args[1] == "vf00":
                        opcode = "NOP"
                        args.clear()
                case "LQ" | "SQ" | "ILW" | "ISW":
                    r = re.findall(r"(.+) \+ (.+)", args[-1])[0]
                    args[-1] = f" {r[1]}({r[0].strip()})"
                case "ELENG" | "ERLENG":
                    fs = (op_bytes >> 11) & 0x1F
                    args.append(f"vf{fs:02}")
                case "ESIN" | "EEXP":
                    fs = (op_bytes >> 11) & 0x1F
                    fsf = (op_bytes >> 21) & 0x3
                    args.append(f"vf{fs:02}{'xyzw'[fsf]}")

            text = opcode
            if flag_bit is not None:
                split = text.split(".")
                split[0] += f"[{flag_bit}]"
                text = ".".join(split)
            if len(args) > 0:
                text += " " + ", ".join(args)
            instr_map[address] = text

        with open(f"{output_dir}/{prog_name}.vsm", "w") as f:
            if "0000" not in labels:
                f.write(f"LAB_{prog_num}_0000:\n")
            for address, instr in instr_map.items():
                if address in labels:
                    f.write(f"\nLAB_{prog_num}_{address}:\n")

                # Fix upper/lower instruction order
                if int(address, 16) % 8 == 0:
                    instructions = ["", instr.ljust(32)]
                else:
                    instructions[0] = instr.ljust(32)
                    f.write(f"    {instructions[0]} {instructions[1]}\n")

        prog_num += 1
        if reader.pos() < len(vu_bytes):
            reader.align_pos(0x100)

