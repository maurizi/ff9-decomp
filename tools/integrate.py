#!/usr/bin/env python3
"""Move function definitions from a scratch batch file into src/main/main.c.

    python3 tools/integrate.py build/batchN.c [func_X ...]

Each definition replaces its INCLUDE_ASM line. Without names, every function in
the batch is moved. Declarations are NOT copied: put types/externs/prototypes in
include/ headers so the batch and main.c share them. Run make verify afterwards.
"""
import re
import sys

sys.path.insert(0, "tools")
from resplit import split_c  # noqa: E402

TARGET = "src/main/main.c"


def main():
    if len(sys.argv) < 2:
        sys.exit(__doc__)
    _, bodies = split_c(open(sys.argv[1]).read())
    names = sys.argv[2:] or list(bodies)
    src = open(TARGET).read()
    moved, missing = [], []
    for name in names:
        if name not in bodies:
            sys.exit(f"{name} not defined in {sys.argv[1]}")
        pat = re.compile(r'^INCLUDE_ASM\("[^"]+",\s*' + re.escape(name) + r"\);\n", re.M)
        src, n = pat.subn(lambda _: bodies[name], src)
        (moved if n == 1 else missing).append(name)
    open(TARGET, "w").write(src)
    print(f"moved {len(moved)} functions into {TARGET}")
    if missing:
        sys.exit(f"no INCLUDE_ASM slot for: {missing}")


if __name__ == "__main__":
    main()
