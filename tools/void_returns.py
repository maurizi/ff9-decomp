#!/usr/bin/env python3
"""List functions declared `void` whose callers use $v0 after the call.

    tools/docker.sh python3 tools/void_returns.py

On the PS1 such code works (and matches) because $v0 still holds whatever the function's last callee
returned; a port compiled for another CPU loses that value. The fix is a real return type and
`return f(...)` where the function ends in a call.

For every `jal` in the 20 built binaries, the instructions after the delay slot are scanned in order until
$v0 is written, read, or control flow leaves the straight line (a branch that tests $v0 counts as a read).
Delay slots count, including the one of the branch or call that ends the scan. A callee is reported when a caller reads its $v0 and our headers or
sources declare it void. Calls through pointers (jalr) aren't checked.
"""
import glob
import re
import subprocess
from collections import defaultdict

ELFS = ["build/us/slus_012.51.elf"] + sorted(glob.glob("build/us/ovl_*.elf"))
INS = re.compile(r"^\s*([0-9a-f]+):\s+(?:[0-9a-f]{8}\s+)?(\S+)\s*(.*)$")
JAL = re.compile(r"<([\w.]+)>")
CTRL = {"j", "jr", "jal", "jalr", "b", "beq", "bne", "beqz", "bnez", "blez", "bgtz", "bltz", "bgez",
        "bal", "bltzal", "bgezal", "syscall", "break"}


def regs(ops):
    return re.findall(r"\b(zero|at|v[01]|a[0-3]|t[0-9]|s[0-8]|k[01]|gp|sp|fp|ra)\b", ops)


def reads_writes(mn, ops):
    r = regs(ops)
    if not r:
        return [], []
    if mn.startswith(("s", "beq", "bne", "bl", "bg", "mt", "j", "mult", "div")) and not mn.startswith(("sll", "srl", "sra", "slt", "sub")):
        return r, []  # stores, branches, mthi/mtlo/mtc2, mult/div: every register is a source
    return r[1:], r[:1]


def void_decls():
    """binary -> names declared void where that binary can see them (main's declarations count everywhere)."""
    def scan(files):
        out = set()
        for f in files:
            for m in re.finditer(r"^\s*(?:static\s+)?void\s+(\w+)\s*\(", open(f).read(), re.M):
                out.add(m.group(1))
        return out
    common = scan(["include/ff9.h", "include/psyq.h"] + glob.glob("src/main/*.c"))
    out = defaultdict(lambda: set(common))
    for elf in ELFS[1:]:
        b = elf.split("/")[-1][:-4]
        out[b] = common | scan(glob.glob(f"include/{b}.h") + glob.glob(f"src/{b}/*.c"))
    return out


def main():
    voids = void_decls()
    used = defaultdict(set)
    for elf in ELFS:
        binary = "main" if "slus" in elf else elf.split("/")[-1][:-4]
        dis = subprocess.run(["mipsel-linux-gnu-objdump", "-d", "--no-show-raw-insn", elf],
                             capture_output=True, text=True).stdout
        func = None
        lines = []
        for line in dis.splitlines():
            m = re.match(r"^[0-9a-f]+ <([\w.]+)>:", line)
            if m:
                func = m.group(1)
                continue
            m = INS.match(line)
            if m:
                lines.append((func, int(m.group(1), 16), m.group(2), m.group(3)))
        for i, (fn, addr, mn, ops) in enumerate(lines):
            if mn != "jal":
                continue
            t = JAL.search(ops)
            if not t:
                continue
            callee = t.group(1)
            # the delay slot runs before the callee, so start after it
            seq = lines[i + 2:i + 40]
            for k, (fn2, a2, mn2, ops2) in enumerate(seq):
                if fn2 != fn:
                    break
                rd, wr = reads_writes(mn2, ops2)
                if "v0" in rd:
                    used[(binary, callee)].add(f"{fn}@{a2:08X}")
                    break
                if "v0" in wr:
                    break
                if mn2 in CTRL:
                    # its delay slot still runs on the straight line (before a jal's callee clobbers v0)
                    if k + 1 < len(seq) and "v0" in reads_writes(seq[k + 1][2], seq[k + 1][3])[0]:
                        used[(binary, callee)].add(f"{fn}@{seq[k + 1][1]:08X}")
                    break
    hits = sorted((k, v) for k, v in used.items() if k[1] in voids[k[0]])
    for (binary, callee), sites in hits:
        print(f"{binary:7} {callee:28} {len(sites):3} caller(s): {' '.join(sorted(sites)[:6])}")
    print(f"{len(hits)} void-declared functions have callers that read $v0")


if __name__ == "__main__":
    main()
