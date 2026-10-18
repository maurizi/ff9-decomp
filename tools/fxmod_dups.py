#!/usr/bin/env python3
"""Group effect-module functions whose code is identical apart from relocated fields (jal/j targets, lui and
load/store offsets off non-sp/zero bases), so each distinct body is matched once and copied to the others.

  python3 tools/fxmod_dups.py                 # summary
  python3 tools/fxmod_dups.py --func fx_0b2 func_801E7704   # the group of one function, with each copy's status
  python3 tools/fxmod_dups.py --groups        # every group with more than one copy: owner first

The owner of a group is its copy in the lowest-numbered module. Needs disks/us/fxmod/ (make extract-fxmod)."""
import argparse, collections, hashlib, os, re, struct

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


def load():
    funcs = collections.defaultdict(list)
    for line in open(f'{ROOT}/config/functions.fxmod.us.txt'):
        if line.startswith('#') or not line.strip():
            continue
        b, n, size, addr = line.split()[:4]
        funcs[b].append((int(addr, 16), int(size, 16), n))
    vram = {}
    for line in open(f'{ROOT}/config/fxmods.us.txt'):
        f = line.split('#')[0].split()
        if f:
            vram[f[0]] = int(f[1], 16)
    groups = collections.defaultdict(list)
    for b in sorted(funcs):
        data = open(f'{ROOT}/disks/us/fxmod/{b}.bin', 'rb').read()
        for addr, size, n in funcs[b]:
            ws = struct.unpack_from(f'<{size // 4}I', data, addr - vram[b])
            m = []
            for w in ws:
                op = w >> 26
                if op in (2, 3):
                    w &= 0xFC000000
                elif op == 0xF:
                    w &= 0xFFFF0000
                elif op in (0x09, 0x20, 0x21, 0x23, 0x24, 0x25, 0x28, 0x29, 0x2B) and ((w >> 21) & 31) not in (29, 0):
                    w &= 0xFFFF0000
                m.append(w)
            groups[hashlib.sha1(struct.pack(f'<{len(m)}I', *m)).hexdigest()[:12]].append((b, n, size))
    return groups


def matched(b, n):
    """True when module b's C defines n (no INCLUDE_ASM for it)."""
    path = f'{ROOT}/src/fxmod/{b}/{b}.c'
    if not os.path.exists(path):
        return False
    src = open(path).read()
    return re.search(r'INCLUDE_ASM\([^)]*\b%s\)' % re.escape(n), src) is None and re.search(r'\b%s\s*\(' % re.escape(n), src) is not None


def main():
    p = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument('--func', nargs=2, metavar=('BIN', 'NAME'))
    p.add_argument('--groups', action='store_true')
    a = p.parse_args()
    groups = load()
    if a.func:
        for h, v in groups.items():
            if (a.func[0], a.func[1]) in [(b, n) for b, n, _ in v]:
                print(f'group {h}: {len(v)} copies, {v[0][2]} bytes')
                for b, n, _ in v:
                    print(f'  {b:10} {n:28} {"matched" if matched(b, n) else "asm"}')
                return
        raise SystemExit('not found')
    if a.groups:
        for h, v in sorted(groups.items(), key=lambda kv: (-len(kv[1]), kv[1][0])):
            if len(v) > 1:
                done = [f'{b}:{n}' for b, n, _ in v if matched(b, n)]
                print(f'{h} {len(v):3}x {v[0][2]:6}  owner {v[0][0]}:{v[0][1]}' + (f'  matched in {done[0]}' if done else ''))
        return
    n = sum(len(v) for v in groups.values())
    print(f'{n} functions, {len(groups)} distinct bodies, {sum(v[0][2] for v in groups.values())} distinct bytes; '
          f'{sum(1 for v in groups.values() if len(v) > 1)} bodies occur more than once')


if __name__ == '__main__':
    main()
