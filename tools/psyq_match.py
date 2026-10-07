#!/usr/bin/env python3
"""Find PsyQ SDK library objects in SLUS_012.51 using lab313ru/psx_psyq_signatures.

    git clone --depth 1 https://github.com/lab313ru/psx_psyq_signatures /path/sigs
    python3 tools/psyq_match.py /path/sigs              # rank SDK versions
    python3 tools/psyq_match.py /path/sigs --version 430 --symbols   # symbol_addrs lines

Only matches that are unique in .text and at least MIN_LEN bytes long are kept.
"""
import argparse
import ast
import json
import os
import re
import sys

EXE = "disks/us/SLUS_012.51"
VRAM = 0x80010000
FILE_OFF = 0x800
TEXT = (0x2FA0, 0x56DE0)  # file offsets
MIN_LEN = 24


def sig_regex(sig):
    out = b""
    for tok in sig.split():
        out += b"." if tok == "??" else re.escape(bytes([int(tok, 16)]))
    return re.compile(out, re.S)


def load(version_dir):
    for fn in sorted(os.listdir(version_dir)):
        if not fn.endswith(".json"):
            continue
        for obj in json.load(open(os.path.join(version_dir, fn))):
            yield fn[:-5], obj


def match_version(text, version_dir):
    hits = []
    for lib, obj in load(version_dir):
        if "sig" not in obj:
            continue
        n = len(obj["sig"].split())
        if n < MIN_LEN:
            continue
        found = [m.start() for m in sig_regex(obj["sig"]).finditer(text)]
        # finditer skips overlaps; good enough for code. Require word alignment.
        found = [f for f in found if f % 4 == 0]
        if len(found) != 1:
            continue
        labels = obj["labels"]
        if isinstance(labels, str):
            labels = ast.literal_eval(labels)
        hits.append((found[0], n, lib, obj["name"], labels))
    return hits


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("sigdir")
    ap.add_argument("--version", help="comma-separated, e.g. 470,460")
    ap.add_argument("--symbols", action="store_true")
    a = ap.parse_args()
    data = open(EXE, "rb").read()
    text = data[TEXT[0]:TEXT[1]]
    versions = a.version.split(",") if a.version else sorted(
        d for d in os.listdir(a.sigdir) if d.isdigit())
    if not a.symbols:
        for ver in versions:
            hits = match_version(text, os.path.join(a.sigdir, ver))
            print(f"{ver}: {len(hits):4d} objects, {sum(h[1] for h in hits):7d} bytes")
        return
    # Union over the given versions: longest match wins, no overlaps.
    allhits = []
    for ver in versions:
        allhits += [h + (ver,) for h in match_version(text, os.path.join(a.sigdir, ver))]
    taken, keep = [], []
    for h in sorted(allhits, key=lambda h: -h[1]):
        if any(h[0] < e and s < h[0] + h[1] for s, e in taken):
            continue
        taken.append((h[0], h[0] + h[1]))
        keep.append(h)
    print(f"// {len(keep)} objects, {sum(h[1] for h in keep)} bytes", file=sys.stderr)
    seen = set()
    for off, n, lib, obj, labels, ver in sorted(keep):
        base = VRAM + TEXT[0] - FILE_OFF + off
        for lab in labels:
            name = lab["name"]
            if re.match(r"(loc|text|data|bss|rdata|sdata|sbss)_[0-9A-Fa-f]+$", name) or lab["offset"] >= n:
                continue
            if name in seen:
                continue
            seen.add(name)
            print(f"{name} = 0x{base + lab['offset']:08X}; // type:func psyq{ver} {lib[:-4].lower()}/{obj}")

if __name__ == "__main__":
    sys.exit(main())
