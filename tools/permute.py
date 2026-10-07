#!/usr/bin/env python3
"""Set up (and optionally run) decomp-permuter for one function. Inside the container:

    tools/docker.sh python3 tools/permute.py build/batchN.c func_X            # set up build/permuter/func_X
    tools/docker.sh python3 tools/permute.py build/batchN.c func_X --run -j8  # set up and run

The C file must define func_X; other functions are stripped. Best results land in
build/permuter/func_X/output-*/source.c.
"""
import argparse
import os
import shutil
import stat
import subprocess
import sys

PERMUTER = "tools/decomp-permuter"
ASM_GLOB = "asm/us/*/nonmatchings/*/{func}.s"
CPP_FLAGS = "-nostdinc -Iinclude -undef -D__GNUC__=2 -Dmips -D__mips__ -D_LANGUAGE_C -DLANGUAGE_C"
ASFLAGS = "-march=r3000 -mabi=32 -EL -no-pad-sections -O0"

COMPILE_SH = """#!/bin/sh
# invoked as ./compile.sh input.c -o output.o
set -e
ROOT={root}
IN="$1"; OUT="$3"; TMP="$OUT.cc1.s"
{cc1} -quiet {cflags} "$IN" -o "$TMP"
sed -i 's/^\t\.align\t3$/\t.align\t2/' "$TMP"
python3 $ROOT/tools/maspsx/maspsx.py --aspsx-version={aspsx} --expand-div --run-assembler {asflags} -I$ROOT/include -o "$OUT" < "$TMP"
rm -f "$TMP"
"""


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("c_file")
    ap.add_argument("func")
    ap.add_argument("--cc", default="gcc-2.8.0-psx")
    ap.add_argument("--cflags", default="-O2 -G0")
    ap.add_argument("--aspsx", default="2.77")
    ap.add_argument("--run", action="store_true")
    ap.add_argument("--bin", help="binary the function belongs to (main, ovl_XX); needed when names repeat")
    ap.add_argument("-j", default="4")
    a = ap.parse_args()

    root = os.getcwd()
    d = os.path.join("build/permuter", a.func)
    shutil.rmtree(d, ignore_errors=True)
    os.makedirs(d)

    base = os.path.join(d, "base.c")
    subprocess.run(f"cpp -P {CPP_FLAGS} {a.c_file} -o {base}", shell=True, check=True)
    subprocess.run([sys.executable, f"{PERMUTER}/strip_other_fns.py", base, a.func], check=True)

    with open(os.path.join(d, "target.s"), "w") as f:
        f.write('.include "macro.inc"\n.set noat\n.set noreorder\n.section .text\n')
        import glob
        hits = glob.glob(ASM_GLOB.format(func=a.func))
        if a.bin:
            hits = [h for h in hits if h.split("/")[2] == a.bin]
        if len(hits) != 1:
            sys.exit(f"need exactly one asm file for {a.func} (use --bin main|ovl_XX): {hits}")
        f.write(open(hits[0]).read())
    subprocess.run(f"mipsel-linux-gnu-as {ASFLAGS} -Iinclude {d}/target.s -o {d}/target.o",
                   shell=True, check=True)

    sh = os.path.join(d, "compile.sh")
    with open(sh, "w") as f:
        f.write(COMPILE_SH.format(root=root, cc1=f"/opt/cc/{a.cc}/cc1", cflags=a.cflags,
                                  aspsx=a.aspsx, asflags=ASFLAGS))
    os.chmod(sh, os.stat(sh).st_mode | stat.S_IEXEC)
    with open(os.path.join(d, "settings.toml"), "w") as f:
        f.write(f'func_name = "{a.func}"\ncompiler_type = "gcc"\n')

    print(f"set up {d}")
    if a.run:
        os.execvp(sys.executable, [sys.executable, f"{PERMUTER}/permuter.py", d, "-j", a.j,
                                   "--stop-on-zero"])


if __name__ == "__main__":
    main()
