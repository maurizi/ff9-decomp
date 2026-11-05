#!/usr/bin/env python3
"""List data words that look like PS1 RAM pointers (0x80000000-0x801FFFFF) in the split asm.

Ports that relocate data treat every such `.word` as a pointer. This lists, per data block
(dlabel), the in-range `.word`s splat emits: `sym` (resolved to a symbol), `raw` (left as a
hex constant: a pointer into another binary or section, or a false pointer inside byte data)
and, with --hidden, aligned in-range words that splat emits as `.short`/`.byte` (real
pointers a relocation pass reading `.word`s would miss). Jump-table entries (`.word .L...`)
are skipped. Which blocks are false pointers is recorded by hand in
docs/wip/false_pointer_words.md.

Usage: python3 tools/false_pointers.py [--bin main] [--hidden] [--words]
Needs asm/ from `make resplit`.
"""
import argparse
import collections
import os
import re

LO, HI = 0x80000000, 0x80200000
WORD = re.compile(r"/\* [0-9A-F]+ ([0-9A-F]{8}) ([0-9A-F]{8}) \*/\s+\.word (\S.*)$")
SMALL = re.compile(r"/\* [0-9A-F]+ ([0-9A-F]{8}) \*/\s+\.(short|byte) (0x[0-9A-Fa-f]+)\s*$")
LABEL = re.compile(r"\s*(?:dlabel|glabel)\s+(\S+)")


def scan(path, hidden):
    blk = None
    small = {}
    for line in open(path, errors="replace"):
        m = LABEL.match(line)
        if m:
            blk = m.group(1)
            small = {}
            continue
        m = WORD.search(line)
        if m:
            v = int.from_bytes(bytes.fromhex(m.group(2)), "little")
            tgt = m.group(3).strip()
            if LO <= v < HI and not tgt.startswith(".L"):
                yield blk, int(m.group(1), 16), v, "raw" if tgt.startswith("0x") else "sym", tgt
            continue
        if hidden:
            m = SMALL.search(line)
            if m:
                a = int(m.group(1), 16)
                small[a] = (int(m.group(3), 16), 2 if m.group(2) == "short" else 1)
                base = a & ~3
                if a + small[a][1] == base + 4:
                    v, ok, p = 0, True, base
                    while p < base + 4:
                        if p not in small:
                            ok = False
                            break
                        x, n = small[p]
                        v |= x << (8 * (p - base))
                        p += n
                    if ok and p == base + 4 and LO <= v < HI:
                        yield blk, base, v, "hidden", "0x%08X" % v


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--bin", help="only this binary (asm/us/<bin>)")
    ap.add_argument("--hidden", action="store_true", help="also list in-range words emitted as .short/.byte")
    ap.add_argument("--words", action="store_true", help="print every word, not one line per block")
    ap.add_argument("--root", default="asm/us")
    a = ap.parse_args()
    blocks = collections.OrderedDict()
    for b in sorted(os.listdir(a.root)):
        if a.bin and b != a.bin:
            continue
        for dp, _, fn in sorted(os.walk(os.path.join(a.root, b))):
            for f in sorted(fn):
                if f.endswith(".s"):
                    for blk, addr, v, kind, tgt in scan(os.path.join(dp, f), a.hidden):
                        blocks.setdefault((b, blk), []).append((addr, v, kind, tgt))
    for (b, blk), ws in blocks.items():
        if a.words:
            for addr, v, kind, tgt in ws:
                print("%s %s %08X %08X %s %s" % (b, blk, addr, v, kind, tgt))
            continue
        c = collections.Counter(k for _, _, k, _ in ws)
        print("%-8s %-40s %s" % (b, blk, " ".join("%s=%d" % kv for kv in sorted(c.items()))))


if __name__ == "__main__":
    main()
