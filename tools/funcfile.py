#!/usr/bin/env python3
"""Where does each function live now? Maps function names (current, or older names via
docs/wip/names_from_port.txt) to the source file that defines them or INCLUDE_ASMs them.

  python3 tools/funcfile.py                    # every function: bin name file
  python3 tools/funcfile.py func_800B73F8 akao_calcPitch ...   # look up names (old or new)
  python3 tools/funcfile.py --tsv > out.tsv    # bin, name, file, old names (comma-separated)

For ports that patch decomp files by path: after a pull, look up the functions a patch touches."""
import argparse, collections, glob, os, re

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
DEF = re.compile(r'^[A-Za-z_][\w \*]*?\b(\w+)\s*\([^;]*\)\s*\{', re.M)
ASM = re.compile(r'INCLUDE_ASM\(\s*"[^"]*"\s*,\s*(\w+)\s*\)')


def index():
    where = {}
    for path in sorted(glob.glob(f'{ROOT}/src/*/*.c') + glob.glob(f'{ROOT}/src/fxmod/*/*.c')):
        rel = os.path.relpath(path, ROOT)
        b = rel.split('/')[1] if not rel.startswith('src/fxmod/') else rel.split('/')[2]
        text = open(path, errors='replace').read()
        for m in DEF.finditer(text):
            if m.group(1) not in ('if', 'while', 'for', 'switch'):
                where.setdefault((b, m.group(1)), rel)
        for m in ASM.finditer(text):
            where.setdefault((b, m.group(1)), rel)
    return where


def renames():
    """old -> new across every rename list (chains followed)."""
    old2new = {}
    p = f'{ROOT}/docs/wip/names_from_port.txt'
    if os.path.exists(p):
        for line in open(p):
            m = re.match(r'(\w+)\s+(\w+)\s*(#|$)', line)
            if m and m.group(1) != m.group(2) and not line.startswith('#'):
                old2new[m.group(1)] = m.group(2)
    def final(n, seen=()):
        return final(old2new[n], seen + (n,)) if n in old2new and n not in seen else n
    return {o: final(o) for o in old2new}


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('names', nargs='*')
    ap.add_argument('--tsv', action='store_true')
    a = ap.parse_args()
    where, ren = index(), renames()
    byname = collections.defaultdict(list)
    for (b, n), f in where.items():
        byname[n].append((b, f))
    byaddr = collections.defaultdict(list)
    for idx in ('functions.us.txt', 'functions.fxmod.us.txt'):
        pth = f'{ROOT}/config/{idx}'
        if os.path.exists(pth):
            for line in open(pth):
                f = line.split()
                if len(f) >= 4 and not line.startswith('#'):
                    byaddr[int(f[3], 16)].append((f[0], f[1]))
    if a.names:
        for n in a.names:
            cands = [ren.get(n, n)]
            m = re.match(r'(?:func|D)_([0-9A-Fa-f]{8})$', n)
            if m:  # a placeholder: every binary's function at that address (overlays share addresses)
                cands += [nm for _, nm in byaddr.get(int(m.group(1), 16), [])]
            hits = []
            for c in dict.fromkeys(cands):
                for b, f in byname.get(c, []) + byname.get(c + '_def', []):
                    hits.append(f'{c} in {b} {f}')
            print(f'{n}: ' + ('; '.join(hits) if hits else 'not found'))
        return
    olds = collections.defaultdict(list)
    for o, n in ren.items():
        olds[n].append(o)
    for (b, n), f in sorted(where.items(), key=lambda kv: (kv[0][0], kv[1], kv[0][1])):
        if a.tsv:
            print(f'{b}\t{n}\t{f}\t{",".join(sorted(olds.get(n, [])))}')
        else:
            print(f'{b} {n} {f}')


if __name__ == '__main__':
    main()
