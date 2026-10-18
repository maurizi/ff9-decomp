#!/usr/bin/env python3
"""Copy a matched effect-module function body into other modules that contain the same code
(tools/fxmod_dups.py groups bodies that are identical apart from relocated fields).

  tools/docker.sh python3 tools/fxmod_propagate.py fx_09b:func_801E7704 fx_0ea:func_801E7738 [--dry] [--no-verify]
  tools/docker.sh python3 tools/fxmod_propagate.py --all [--only fx_0ea,fx_114] [--dry]

Runs in the container (needs objdump/nm and a built tree: `make build` first). For one copy it
  1. reads the relocations of the source function from its compiled object (symbol + addend) and the symbol
     references of the destination's INCLUDE_ASM'd .s, pairs them by instruction offset and maps every source
     symbol to the destination symbol at the same address relative to the reference (so `&D_801E9A34[1]`
     in the source becomes `&D_801E7E64[1]` when the destination loads D_801E7E6C);
  2. renames module types Fx<src>Name -> Fx<dst>Name and copies the declarations the body needs (typedefs,
     macros, externs, prototypes, recursively) from include/fxmod/<src>.h into include/fxmod/<dst>.h, unless
     the destination already declares that name (a differing destination typedef gets a _<src> suffix);
  3. replaces the INCLUDE_ASM line in src/fxmod/<dst>/<dst>.c, rebuilds build/us/<dst>.bin and compares its
     sha1 with config/checksums.fxmod.us.sha1. A failed copy is rolled back (--no-verify keeps it).
Before copying it compares the two original images outside relocated fields: bodies that fxmod_dups puts in
one group can still differ in struct offsets off non-sp bases ("skeleton twins"), which is reported.
--all propagates every unmatched copy of every group that has a matched copy and prints a summary."""
import argparse, os, re, subprocess, struct, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, 'tools'))
OBJDUMP = 'mipsel-linux-gnu-objdump'
NM = 'mipsel-linux-gnu-nm'


def run(cmd, **kw):
    return subprocess.run(cmd, cwd=ROOT, capture_output=True, text=True, **kw)


def tag(mod):
    return 'Fx' + mod[3:].replace('_', '')


class Fail(Exception):
    pass


# ---------------------------------------------------------------- symbols and relocations

_nm_cache = {}


def elf_syms(mod):
    """address -> [names], name -> address of build/us/<mod>.elf"""
    if mod not in _nm_cache:
        out = run([NM, '-n', f'build/us/{mod}.elf']).stdout
        by_addr, by_name = {}, {}
        for line in out.splitlines():
            p = line.split()
            if len(p) != 3 or p[1] in 'aA' or '.' in p[2] or p[2].endswith(('_START', '_END', '_VRAM', '_SIZE')):
                continue
            if p[2] in ('gcc2_compiled', '__gnu_compiled_c'):
                continue
            a = int(p[0], 16)
            by_addr.setdefault(a, []).append(p[2])
            by_name[p[2]] = a
        _nm_cache[mod] = (by_addr, by_name)
    return _nm_cache[mod]


def obj_relocs(mod, func):
    """{offset in func: (type, symbol, addend)} from the compiled source object."""
    obj = f'build/us/src/fxmod/{mod}/{mod}.c.o'
    out = run([OBJDUMP, '-dr', f'--disassemble={func}', obj]).stdout
    start = None
    words = {}
    rel = {}
    for line in out.splitlines():
        m = re.match(r'^([0-9a-f]+) <(\S+)>:', line)
        if m:
            start = int(m.group(1), 16) if m.group(2) == func else None
            continue
        if start is None:
            continue
        m = re.match(r'^\s+([0-9a-f]+):\s+([0-9a-f]{8})\s', line)
        if m:
            words[int(m.group(1), 16)] = int(m.group(2), 16)
            continue
        m = re.match(r'^\s+([0-9a-f]+): (R_MIPS_\w+)\s+(\S+)', line)
        if m:
            off = int(m.group(1), 16)
            typ, sym = m.group(2), m.group(3)
            w = words.get(off, 0)
            if typ == 'R_MIPS_LO16':
                add = (w & 0xFFFF) - ((w & 0x8000) << 1)
            elif typ == 'R_MIPS_26':
                add = (w & 0x3FFFFFF) << 2
            else:
                add = None
            rel[off - start] = (typ, sym, add)
    if start is None and not words:
        raise Fail(f'{func} not found in {obj} (build it first)')
    return rel


def asm_refs(mod, func):
    """{offset in func: (kind, symbol)} from the destination's .s, and the function's vram."""
    path = f'{ROOT}/asm/us/{mod}/nonmatchings/{mod}/{func}.s'
    if not os.path.exists(path):
        raise Fail(f'{path} missing')
    refs, base = {}, None
    for line in open(path):
        m = re.match(r'\s*/\* [0-9A-F]+ ([0-9A-F]{8}) [0-9A-F]{8} \*/\s+(\S+)\s*(.*)', line)
        if not m:
            continue
        va = int(m.group(1), 16)
        if base is None:
            base = va
        op, args = m.group(2), m.group(3)
        r = re.search(r'%(hi|lo)\(([\w.]+)\)', args)
        if r:
            refs[va - base] = (r.group(1), r.group(2))
        elif op in ('jal', 'j') and not args.startswith('.'):
            refs[va - base] = ('26', args.strip())
    return refs, base


def func_words(mod, addr, size):
    vram = {}
    for line in open(f'{ROOT}/config/fxmods.us.txt'):
        f = line.split('#')[0].split()
        if f:
            vram[f[0]] = int(f[1], 16)
    data = open(f'{ROOT}/disks/us/fxmod/{mod}.bin', 'rb').read()
    return struct.unpack_from(f'<{size // 4}I', data, addr - vram[mod])


def symbol_map(sm, sf, dm, df):
    srel = obj_relocs(sm, sf)
    drefs, dbase = asm_refs(dm, df)
    d_addr, d_name = elf_syms(dm)
    mp, notes = {sf: df}, []
    for off, (typ, sym, add) in sorted(srel.items()):
        if sym.startswith('.'):          # section-relative: local labels, jump tables, literals
            if off in drefs and not drefs[off][1].startswith(('.L', 'jtbl_')):
                notes.append(f'+{off:#x}: source uses {sym}, destination {drefs[off][1]}')
            continue
        if off not in drefs:
            raise Fail(f'+{off:#x}: no symbol reference in the destination for {sym}')
        kind, dsym = drefs[off]
        if typ == 'R_MIPS_HI16':
            continue
        if dsym not in d_name:
            raise Fail(f'+{off:#x}: destination symbol {dsym} not in build/us/{dm}.elf')
        target = d_name[dsym] - (add or 0)
        names = d_addr.get(target)
        if not names:
            raise Fail(f'+{off:#x}: {sym}{add:+#x} -> {dsym}, but no destination symbol at {target:#010X}')
        name = names[0]
        if sym in mp and mp[sym] != name:
            raise Fail(f'{sym} maps to both {mp[sym]} and {name}: not the same body')
        mp[sym] = name
    # hi16 sanity: every hi reloc symbol must be mapped by its lo partner
    for off, (typ, sym, add) in srel.items():
        if typ == 'R_MIPS_HI16' and not sym.startswith('.') and sym not in mp:
            raise Fail(f'{sym}: %hi without a mapped %lo')
    return mp, notes, dbase


def twin_check(sm, sf, dm, df, dbase):
    """Words that differ between the two original images outside the destination's relocated fields."""
    _, s_name = elf_syms(sm)
    drefs, _ = asm_refs(dm, df)
    size = None
    for line in open(f'{ROOT}/config/functions.fxmod.us.txt'):
        p = line.split()
        if len(p) >= 4 and p[0] == dm and p[1] == df:
            size = int(p[2], 16)
    if size is None or sf not in s_name:
        return []
    a = func_words(sm, s_name[sf], size)
    b = func_words(dm, dbase, size)
    return [i * 4 for i, (x, y) in enumerate(zip(a, b))
            if x != y and i * 4 not in drefs and not (x >> 26 == y >> 26 and x >> 26 in (2, 3))]  # j to local labels


# ---------------------------------------------------------------- headers

def strip_comments(t):
    return re.sub(r'/\*.*?\*/|//[^\n]*', ' ', t, flags=re.S)


def split_decls(text):
    """Top-level declarations of a header: list of (text, kind). Leading comment lines stay with their decl."""
    out, cur, depth, incom = [], '', 0, False
    lines = text.splitlines(keepends=True)
    i = 0
    while i < len(lines):
        line = lines[i]
        i += 1
        if depth == 0 and not incom and _only_comments(cur):
            if line.startswith('#'):
                while line.rstrip('\n').endswith('\\') and i < len(lines):
                    line += lines[i]
                    i += 1
                out.append((cur + line, 'pp'))
                cur = ''
                continue
        cur += line
        code = line
        if incom:
            if '*/' not in code:
                continue
            code = code[code.index('*/') + 2:]
            incom = False
        code = re.sub(r'/\*.*?\*/|//[^\n]*', ' ', code)
        if '/*' in code:
            incom = True
            code = code[:code.index('/*')]
        depth += code.count('{') - code.count('}')
        if depth == 0 and code.rstrip().endswith((';', '}')) and not incom:
            out.append((cur, 'decl'))
            cur = ''
    if cur.strip():
        out.append((cur, 'tail'))
    return out


def _only_comments(t):
    return not strip_comments(t).strip()


def defines(decl, kind, name):
    if declared_name(decl, kind) == name:
        return True
    return kind != 'pp' and re.search(r'\bstruct\s+%s\s*\{' % re.escape(name), strip_comments(decl)) is not None


IDENT = re.compile(r'\b[A-Za-z_]\w*\b')


def needed_decls(body, hdr_decls, skip):
    """Header declarations the body depends on, transitively, in header order."""
    want = set(IDENT.findall(strip_comments(body))) - skip
    chosen = set()
    changed = True
    while changed:
        changed = False
        for k, (d, kind) in enumerate(hdr_decls):
            if k in chosen or kind == 'tail':
                continue
            c = strip_comments(d)
            if kind == 'pp' and re.match(r'\s*#\s*(ifndef|endif|include|if|else|elif)\b', c):
                continue
            if kind == 'pp' and re.match(r'\s*#\s*define\s+FX_\w+_H\b', c):
                continue
            for name in want:
                if defines(d, kind, name):
                    chosen.add(k)
                    new = set(IDENT.findall(c)) - want - skip
                    want |= new
                    changed = True
                    break
    return [hdr_decls[k] for k in sorted(chosen)]


def declared_name(decl, kind):
    c = strip_comments(decl).strip()
    if kind == 'pp':
        m = re.match(r'#\s*define\s+(\w+)', c)
        return m and m.group(1)
    if c.startswith('typedef'):
        m = re.search(r'(\w+)\s*(\[[^\]]*\])*\s*;\s*$', c)
        m2 = re.search(r'\(\s*\*\s*(\w+)\s*\)', c)
        if m and not c.rstrip(';').rstrip().endswith(')'):
            return m.group(1)
        return m2 and m2.group(1)
    m = re.search(r'\(\s*\*\s*(\w+)', c)
    head = c.split('(')[0] if '(' in c else c
    if m and c.index(m.group(0)) <= len(head):
        return m.group(1)
    m = re.search(r'(\w+)\s*(\[[^\]]*\])*\s*(=[^;]*)?;\s*$', head if '(' in c else c)
    if '(' in c:
        m = re.search(r'(\w+)\s*$', head)
    return m and m.group(1)


def norm(t):
    return re.sub(r'\s+', ' ', strip_comments(t)).strip()


# ---------------------------------------------------------------- one copy

def extract_body(src, func):
    m = re.search(r'^(?:static |INCLUDE_ASM)?[A-Za-z][^\n;{}]*?\b%s\([^;{]*\)\s*\{.*?^\}\n' % re.escape(func), src, re.S | re.M)
    if not m:
        raise Fail(f'C body of {func} not found')
    return m.group(0)


def propagate(sm, sf, dm, df, dry=False, verify=True, log=print, prefer='dst', sig='src'):
    mp, notes, dbase = symbol_map(sm, sf, dm, df)
    diffs = twin_check(sm, sf, dm, df, dbase)
    if diffs:
        log(f'  warning: images differ outside relocations at ' + ', '.join(f'+{o:#x}' for o in diffs[:8]))
    for n in notes:
        log('  note: ' + n)
    ssrc = open(f'{ROOT}/src/fxmod/{sm}/{sm}.c').read()
    body = extract_body(ssrc, sf)
    shpath = f'{ROOT}/include/fxmod/{sm}.h'
    shdr = open(shpath).read() if os.path.exists(shpath) else ''
    sdecls = split_decls(shdr)

    stag, dtag = tag(sm), tag(dm)
    dhpath = f'{ROOT}/include/fxmod/{dm}.h'
    if os.path.exists(dhpath):
        dhdr = open(dhpath).read()
    else:
        g = dm.upper() + '_H'
        dhdr = f'#ifndef {g}\n#define {g}\n\n/* Effect module {dm} (dir 13 file 0x{dm[3:6].upper()}). */\n#include "btlfx_mod.h"\n\n#endif\n'
    dcpath = f'{ROOT}/src/fxmod/{dm}/{dm}.c'
    dsrc = open(dcpath).read()
    ddecls = split_decls(dhdr)

    # type renames: Fx<src>X -> Fx<dst>X, or Fx<dst>X_<src> where the destination has a different X
    tmap = {}
    for d, kind in needed_decls(body, sdecls, set()):
        nm = declared_name(d, kind)
        for st, dt in ((stag, dtag), (stag.upper(), dtag.upper()), (stag.lower(), dtag.lower())):
            if nm and nm.startswith(st):
                tmap[nm] = dt + nm[len(st):]
    names = dict(mp)
    names.update(tmap)

    def rename(t):
        return re.sub(r'\b\w+\b', lambda m: names.get(m.group(0), m.group(0)), t)

    # pick the declarations to copy
    need = needed_decls(body, sdecls, set())
    add = []
    for d, kind in need:
        snm = declared_name(d, kind)
        if snm == sf:
            continue
        nd = rename(d)
        dnm = names.get(snm, snm)
        exist = [x for x, k in ddecls if dnm and (defines(x, k, dnm))]
        cfunc = False
        if not exist and dnm and re.search(r'^\S[^\n]*\b%s\s*\(' % re.escape(dnm), dsrc, re.M) and not \
                re.search(r'INCLUDE_ASM\([^)]*\b%s\)' % re.escape(dnm), dsrc):
            # a C function of the destination: use its own signature
            m = re.search(r'^(\S[^\n;{]*\b%s\s*\([^;{]*\))\s*\{' % re.escape(dnm), dsrc, re.M)
            if m:
                exist = [m.group(1)]
                cfunc = True
        if exist:
            if norm(exist[0]).rstrip(';') != norm(nd).rstrip(';'):
                if kind != 'pp' and strip_comments(d).lstrip().startswith('typedef') and snm in tmap:
                    # a different type of that name in the destination: suffix ours
                    names[snm] = dnm + '_' + sm[3:]
                    tmap[snm] = names[snm]
                elif prefer == 'src' and not cfunc and kind != 'pp':
                    log(f'  note: replacing {dm}\'s declaration of {dnm}: {norm(exist[0])[:100]}')
                    dhdr = dhdr.replace(exist[0], '', 1)
                else:
                    log(f'  note: keeping {dm}\'s declaration of {dnm}: {norm(exist[0])[:100]}')
                    continue
            else:
                continue
        add.append((snm, d, kind))
    nbody = rename(body)
    adds = [rename(d) for _, d, _ in add]
    # a data table of the source may be several symbols in the destination: leave the extent open
    adds = [re.sub(r'^(extern [^(;]*?\b\w+)\[[^\]]+\]', r'\1[]', d, flags=re.M)
            if d.lstrip().startswith('extern') and '(' not in strip_comments(d) else d for d in adds]
    # the copied function's own prototype: drop an existing one, add ours
    protore = re.compile(r'^[A-Za-z][\w \*]*\b%s\s*\([^;{]*\);[^\n]*\n' % re.escape(df), re.M)
    if sig == 'dst':
        # the destination's own prototype (its callers' view), with the body's parameter names
        m = protore.search(dhdr)
        if not m:
            raise Fail('no destination prototype')
        nbody = resign(nbody, m.group(0), df)
    proto = re.match(r'([^{]*?)\s*\{', nbody, re.S).group(1).strip() + ';'
    dhdr = protore.sub('', dhdr)
    adds.append(proto + '\n')
    # leftovers: source-module names that survived the rename
    left = [w for w in set(IDENT.findall(strip_comments(nbody + ''.join(adds))))
            if (w.startswith(stag) and stag != dtag) or (re.match(r'(D|func)_801[EF][0-9A-F]{4}$', w) and w not in mp.values()
                                                         and not re.search(r'\b%s\b' % w, dhdr + dsrc))]
    if left:
        raise Fail('unmapped source names: ' + ', '.join(sorted(left)))

    block = ''.join(a if a.endswith('\n') else a + '\n' for a in adds)
    marker = '/* propagated */\n'
    i = dhdr.rindex('#endif')
    head = dhdr[:i].rstrip('\n') + '\n'
    if marker not in dhdr:
        head += '\n' + marker
    dhdr = head + block + '\n' + dhdr[i:]
    pat = re.compile(r'INCLUDE_ASM\("[^"]*", %s\);\n' % re.escape(df))
    if not pat.search(dsrc):
        raise Fail(f'no INCLUDE_ASM for {df} in {dm}.c')
    ndsrc = pat.sub(lambda _: nbody, dsrc)
    if f'#include "fxmod/{dm}.h"' not in ndsrc:
        ndsrc = ndsrc.replace('#include "btlfx_mod.h"', f'#include "fxmod/{dm}.h"')
    if dry:
        log(block + nbody)
        return []
    old_h = open(dhpath).read() if os.path.exists(dhpath) else None
    open(dhpath, 'w').write(dhdr)
    open(dcpath, 'w').write(ndsrc)
    if not verify:
        return []
    ok, msg = build_check(dm)
    if not ok:
        restore(dm, dsrc, old_h)
        raise Fail(msg)
    if msg:
        log('  warnings: ' + ' | '.join(msg[:3]))
    return msg


def restore(dm, csrc, hsrc):
    open(f'{ROOT}/src/fxmod/{dm}/{dm}.c', 'w').write(csrc)
    hp = f'{ROOT}/include/fxmod/{dm}.h'
    if hsrc is None:
        if os.path.exists(hp):
            os.remove(hp)
    else:
        open(hp, 'w').write(hsrc)


def split_params(t):
    out, depth, cur = [], 0, ''
    for ch in t:
        if ch == ',' and depth == 0:
            out.append(cur.strip())
            cur = ''
            continue
        depth += (ch == '(') - (ch == ')')
        cur += ch
    if cur.strip():
        out.append(cur.strip())
    return out


def resign(body, dproto, df):
    """body with the return and parameter types of dproto (a prototype of df) but its own parameter names."""
    m = re.match(r'([^{]*?)\b%s\s*\((.*?)\)\s*\{' % re.escape(df), body, re.S)
    dm_ = re.match(r'\s*(.*?)\b%s\s*\((.*)\)\s*;' % re.escape(df), strip_comments(dproto), re.S)
    if not m or not dm_:
        raise Fail('cannot parse signatures')
    sp, dp = split_params(m.group(2)), split_params(dm_.group(2))
    if len(sp) != len(dp) or any('(' in x for x in sp + dp):
        raise Fail('signatures differ in shape')
    new = []
    for a, b in zip(sp, dp):
        if a == 'void':
            new.append(b)
            continue
        name = re.search(r'(\w+)\s*$', a).group(1)
        mb = re.match(r'^(.*?[\s*])(\w+)$', b)
        t = mb.group(1).strip() if mb and mb.group(1).strip() not in ('', 'unsigned', 'signed', 'struct') else b
        new.append(t + ('' if t.endswith('*') else ' ') + name)
    head = dm_.group(1).strip() + (' ' if not dm_.group(1).strip().endswith('*') else '') + df + '(' + ', '.join(new) + ') {'
    return head + body[m.end():]


def build_check(dm):
    os.utime(f'{ROOT}/src/fxmod/{dm}/{dm}.c')
    r = run(['make', f'build/us/{dm}.bin'])
    warn = [l for l in (r.stdout + r.stderr).splitlines() if f'{dm}.c' in l and 'warning' in l and 'memset' not in l]
    if r.returncode:
        err = [l for l in (r.stdout + r.stderr).splitlines() if 'error' in l.lower() or f'{dm}.c:' in l]
        return False, 'build failed: ' + ' | '.join(err[:4])
    want = None
    for line in open(f'{ROOT}/config/checksums.fxmod.us.sha1'):
        h, p = line.split()
        if p == f'build/us/{dm}.bin':
            want = h
    got = run(['sha1sum', f'build/us/{dm}.bin']).stdout.split()[0]
    if got != want:
        return False, 'sha1 mismatch' + (' (warnings: ' + ' | '.join(warn[:3]) + ')' if warn else '')
    return True, warn


def try_copy(sm, sf, dm, df, dry=False, verify=True):
    """propagate() in up to four variants: the body's own signature or the destination's existing prototype
    (what its callers were written against), and the destination's differing declarations kept or replaced.
    The first variant that matches without adding compiler warnings wins, else the matching one with the fewest.
    Returns the log lines of that attempt; raises Fail with the first attempt's error."""
    run(['make', f'build/us/src/fxmod/{sm}/{sm}.c.o'])
    if dry or not verify:
        lines = []
        propagate(sm, sf, dm, df, dry, verify, log=lines.append)
        return lines
    csrc = open(f'{ROOT}/src/fxmod/{dm}/{dm}.c').read()
    hp = f'{ROOT}/include/fxmod/{dm}.h'
    hsrc = open(hp).read() if os.path.exists(hp) else None
    ok, base = build_check(dm)
    base = len(base) if ok else 1 << 20
    err, best = None, None
    for sig in ('src', 'dst'):
        for prefer in ('dst', 'src'):
            lines = []
            try:
                warn = propagate(sm, sf, dm, df, dry, verify, log=lines.append, prefer=prefer, sig=sig)
            except Fail as e:
                err = err or e
                continue
            if len(warn) <= base:
                return lines
            if best is None or len(warn) < best[0]:
                best = (len(warn), sig, prefer)
            restore(dm, csrc, hsrc)
    if best is None:
        build_check(dm)
        raise err
    lines = []
    propagate(sm, sf, dm, df, dry, verify, log=lines.append, prefer=best[2], sig=best[1])
    return lines


def cost(sm, sf, dm, df):
    """How many destination symbols a copy from sm:sf would have to declare anew (fewer is better: a source
    that indexes the same table as the destination's existing C gives `&D_x[2]`, not a new symbol per element)."""
    try:
        run(['make', f'build/us/src/fxmod/{sm}/{sm}.c.o'])
        mp, _, _ = symbol_map(sm, sf, dm, df)
    except Fail:
        return 1 << 20
    text = ''
    for path in (f'{ROOT}/include/fxmod/{dm}.h', f'{ROOT}/src/fxmod/{dm}/{dm}.c'):
        if os.path.exists(path):
            text += open(path).read()
    text = re.sub(r'INCLUDE_ASM\([^)]*\);', '', text)
    return sum(1 for k, v in mp.items() if k != sf and not re.search(r'\b%s\b' % re.escape(v), text))


# ---------------------------------------------------------------- --all

def main():
    p = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument('src', nargs='?', help='MOD:FUNC (matched)')
    p.add_argument('dst', nargs='?', help='MOD:FUNC (INCLUDE_ASM)')
    p.add_argument('--all', action='store_true')
    p.add_argument('--only', help='comma-separated destination modules (--all)')
    p.add_argument('--dry', action='store_true')
    p.add_argument('--no-verify', action='store_true')
    a = p.parse_args()
    if not a.all:
        if not (a.src and a.dst):
            p.error('need SRC:FUNC DST:FUNC or --all')
        (sm, sf), (dm, df) = a.src.split(':'), a.dst.split(':')
        try:
            for l in try_copy(sm, sf, dm, df, a.dry, not a.no_verify):
                print(l)
            print(f'{sm}:{sf} -> {dm}:{df} ' + ('(dry)' if a.dry else 'OK'))
        except Fail as e:
            print(f'{sm}:{sf} -> {dm}:{df} FAILED: {e}')
            sys.exit(1)
        return
    import fxmod_dups as fd
    only = set(a.only.split(',')) if a.only else None
    groups = fd.load()
    done, failed = [], []
    for h, v in sorted(groups.items(), key=lambda kv: kv[1][0]):
        if len(v) < 2:
            continue
        srcs = [(b, n) for b, n, _ in v if fd.matched(b, n)]
        if not srcs:
            continue
        for b, n, _ in v:
            if fd.matched(b, n) or (only and b not in only):
                continue
            errs = []
            for sm, sf in sorted(srcs, key=lambda x: cost(x[0], x[1], b, n)):
                try:
                    for l in try_copy(sm, sf, b, n, a.dry, not a.no_verify):
                        print(l)
                    print(f'{sm}:{sf} -> {b}:{n} OK', flush=True)
                    done.append((b, n))
                    if not a.dry:
                        srcs.append((b, n))
                    break
                except Fail as e:
                    errs.append(f'{sm}:{sf}: {e}')
            else:
                print(f'{b}:{n} FAILED\n    ' + '\n    '.join(errs), flush=True)
                failed.append((b, n, errs))
    print(f'{len(done)} propagated, {len(failed)} failed')


if __name__ == '__main__':
    main()
