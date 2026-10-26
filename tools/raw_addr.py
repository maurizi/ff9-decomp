#!/usr/bin/env python3
"""List PS1 RAM addresses written as numbers in C outside PSX_RAM(type, addr).

    python3 tools/raw_addr.py [--all]

A port compiles src/ natively, where 0x801CDF00 is not a buffer: every fixed address the code uses as memory
goes through PSX_RAM (ff9.h), which a port redefines. Square's code loads these addresses as constants
(lui/ori), so they can't be linker symbols without changing the PS1 output. Reported: hex literals in
0x80010000-0x801FFFFF in code (comments skipped) that aren't the address argument of PSX_RAM. Should be 0.
--all also lists the PSX_RAM uses (the port's mapping list).
"""
import argparse
import glob
import re


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--all", action="store_true")
    args = ap.parse_args()
    bad = uses = 0
    for f in sorted(glob.glob("src/**/*.c", recursive=True) + glob.glob("include/**/*.h", recursive=True)):
        s = open(f).read()
        code = re.sub(r"/\*.*?\*/", lambda m: re.sub(r"[^\n]", " ", m.group(0)), s, flags=re.S)
        code = re.sub(r"//[^\n]*", "", code)
        for m in re.finditer(r"\b0x(80[0-9A-Fa-f]{6})\b", code):
            v = int(m.group(1), 16)
            if not 0x80010000 <= v < 0x80200000:
                continue
            line = code.count("\n", 0, m.start()) + 1
            text = s.split("\n")[line - 1].strip()
            if re.search(r"PSX_RAM\([^()]*,\s*$", code[max(0, m.start() - 60):m.start()]):
                uses += 1
                if args.all:
                    print(f"{f}:{line}: {text}")
                continue
            bad += 1
            print(f"{f}:{line}: RAW 0x{v:08X}: {text}")
    print(f"{bad} raw addresses ({uses} through PSX_RAM)")


if __name__ == "__main__":
    main()
