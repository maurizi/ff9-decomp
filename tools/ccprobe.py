#!/usr/bin/env python3
"""Compile a C file with every candidate compiler and compare each function
against the original SLUS_012.51, masking relocated fields (jal targets,
%hi/%lo immediates). Run inside the container:

    tools/docker.sh python3 tools/ccprobe.py probe.c [--flags "-O2 -G0"] [--aspsx 2.67,2.77] [--bin ovl_0a]

Functions in the C file must keep their func_XXXXXXXX names (address is taken
from the name). Prints a matrix of compiler/aspsx -> matching words per function.
"""
import argparse
import glob
import os
import re
import struct
import subprocess
import sys
import tempfile

EXE = "disks/us/SLUS_012.51"
ASM_DIR = "asm/us/main/nonmatchings"
VRAM = 0x80010000
FILE_OFF = 0x800
CC_DIR = os.environ.get("CC_DIR", "/opt/cc")
ASFLAGS = "-march=r3000 -mabi=32 -EL -no-pad-sections -O0 -Iinclude".split()
CPP_FLAGS = "-nostdinc -Iinclude -undef -D__GNUC__=2 -Dmips -D__mips__ -D_LANGUAGE_C -DLANGUAGE_C".split()


def run(cmd, **kw):
    return subprocess.run(cmd, check=True, capture_output=True, text=True, **kw).stdout


def symbols(obj):
    out = {}
    for line in run(["mipsel-linux-gnu-nm", "-S", obj]).splitlines():
        parts = line.split()
        if len(parts) == 4 and parts[2] in "tT":
            out[parts[3]] = (int(parts[0], 16), int(parts[1], 16))
    return out


def reloc_masks(obj):
    masks = {}
    for line in run(["mipsel-linux-gnu-readelf", "-rW", obj]).splitlines():
        m = re.match(r"^([0-9a-f]{8})\s+\S+\s+(R_MIPS_\w+)", line)
        if not m:
            continue
        off, kind = int(m.group(1), 16), m.group(2)
        if kind == "R_MIPS_26":
            masks[off] = 0xFC000000
        elif kind in ("R_MIPS_HI16", "R_MIPS_LO16", "R_MIPS_GPREL16"):
            masks[off] = 0xFFFF0000
    return masks


def compile_c(src, cc, flags, aspsx, expand_div, tmp):
    i, s, o = (os.path.join(tmp, n) for n in ("x.i", "x.s", "x.o"))
    run(["cpp", *CPP_FLAGS, src, "-o", i])
    run([f"{CC_DIR}/{cc}/cc1", "-quiet", *flags.split(), i, "-o", s])
    run(["sed", "-i", r"s/^\t\.align\t3$/\t.align\t2/", s])  # see Makefile: jump-table alignment
    maspsx = ["python3", "tools/maspsx/maspsx.py", f"--aspsx-version={aspsx}"]
    if expand_div:
        maspsx.append("--expand-div")
    with open(s) as f:
        subprocess.run(maspsx + ["--run-assembler", *ASFLAGS, "-o", o], stdin=f, check=True,
                       capture_output=True, text=True)
    text = os.path.join(tmp, "x.bin")
    run(["mipsel-linux-gnu-objcopy", "-O", "binary", "-j", ".text", o, text])
    return open(text, "rb").read(), symbols(o), reloc_masks(o)


def score(exe, code, syms, masks):
    res = {}
    for name, (off, size) in syms.items():
        m = re.match(r"func_([0-9A-Fa-f]{8})$", name)
        if not m:
            continue
        addr = int(m.group(1), 16)
        good = 0
        n = size // 4
        for k in range(n):
            mine = struct.unpack_from("<I", code, off + k * 4)[0]
            theirs = struct.unpack_from("<I", exe, addr - VRAM + FILE_OFF + k * 4)[0]
            mask = masks.get(off + k * 4, 0xFFFFFFFF)
            good += (mine & mask) == (theirs & mask)
        res[name] = (good, n)
    return res


def show(name, obj):
    dis = run(["mipsel-linux-gnu-objdump", "-d", "-r", "--no-show-raw-insn", f"--disassemble={name}", obj])
    ours = []
    for line in dis.splitlines():
        if "R_MIPS" in line:
            if ours:
                ours[-1] += "  <" + line.split()[-1] + ">"
            continue
        m = re.match(r"^\s+[0-9a-f]+:\s+(.*)$", line)
        if m:
            ours.append(re.sub(r"\s+", " ", m.group(1)).strip())
    theirs = []
    path = next(iter(glob.glob(f"{ASM_DIR}/*/{name}.s")))
    for line in open(path):
        m = re.match(r"^\s+/\* [0-9A-F]+ [0-9A-F]+ [0-9A-F]+ \*/\s+(.*)$", line)
        if m:
            theirs.append(re.sub(r"\s+", " ", m.group(1)).strip())
    for i in range(max(len(ours), len(theirs))):
        o = ours[i] if i < len(ours) else ""
        t = theirs[i] if i < len(theirs) else ""
        print(f"{i*4:4x}  {o:46.46} | {t}")


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("src")
    ap.add_argument("--flags", default="-O2 -G0")
    ap.add_argument("--aspsx", default="2.56,2.67,2.77,2.79")
    ap.add_argument("--cc", default=",".join(sorted(os.listdir(CC_DIR))))
    ap.add_argument("--no-expand-div", action="store_true")
    ap.add_argument("--show", help="print ours vs original side by side for this function")
    ap.add_argument("--bin", default="main", help="binary the functions belong to: main or ovl_XX")
    a = ap.parse_args()
    global VRAM, FILE_OFF, ASM_DIR
    if a.bin == "main":
        exe = open(EXE, "rb").read()
    else:
        ovl = a.bin[4:]
        exe = open(f"disks/us/ovl/{ovl}.bin", "rb").read()
        for line in open("config/overlays.us.txt"):
            f = line.split()
            if f and f[0] == ovl:
                VRAM, FILE_OFF = int(f[1], 16), 0
    ASM_DIR = f"asm/us/{a.bin}/nonmatchings"
    with tempfile.TemporaryDirectory() as tmp:
        for cc in a.cc.split(","):
            for asv in a.aspsx.split(","):
                try:
                    code, syms, masks = compile_c(a.src, cc, a.flags, asv, not a.no_expand_div, tmp)
                except subprocess.CalledProcessError as e:
                    print(f"{cc:18} aspsx {asv}: FAILED {e.stderr.strip()[:200]}")
                    continue
                if a.show:
                    show(a.show, os.path.join(tmp, "x.o"))
                    return
                res = score(exe, code, syms, masks)
                cells = "  ".join(f"{k[5:]}:{g}/{n}{'*' if g == n else ''}" for k, (g, n) in sorted(res.items()))
                print(f"{cc:18} aspsx {asv}: {cells}")


if __name__ == "__main__":
    sys.exit(main())
