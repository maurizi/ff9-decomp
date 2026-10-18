#!/usr/bin/env python3
"""List battle-effect-module calls through the export table whose $v0 is read although the export is void in
ovl_0b (or main / PsyQ), i.e. the module-side analogue of tools/void_returns.py.

    tools/docker.sh python3 tools/fxmod_void_returns.py [--all] [--md]

Modules call ovl_0b only through the export table (BtlfxExports in include/btlfx_mod.h). On the PS1 a module
that reads $v0 after calling an export that is void in ovl_0b gets whatever that export's last callee left in
$v0; a port that calls native ovl_0b code through wrappers with ovl_0b's own prototypes gets nothing. This tool
works from the built module binaries (build/us/fx_*.elf), so it covers modules that are still INCLUDE_ASM.

How a call is recognised: a forward dataflow over each function's control flow tracks which registers (and
stack slots) may hold the export table, i.e. a word loaded from offset 0x10 of something (env->unk10,
BTLFX_EXPORTS), and copies of it.
`lw rF, OFF(table)` followed by `jalr rF` is a call to the export at OFF. From the instruction after the delay
slot, every path (both sides of branches, through jumps) is followed until $v0 is written (or a call clobbers
it). A path that reads $v0 is a "read" hit; one that reaches `jr ra` with $v0 untouched is a "return" hit (the
module function returns the export's $v0), reported only when that result is used: the function is an op
handler or init (ovl_0b reads their result) or a `jal` caller in the module reads its $v0. Void-ness comes from the export's definition or prototype in
src/ovl_0b, src/main, include/ovl_0b.h, include/ff9.h and include/psyq.h.

--all lists every hit site (default: one line per export with counts); --md prints a Markdown table.
"""
import argparse
import glob
import re
import subprocess
from collections import defaultdict

INS = re.compile(r"^\s*([0-9a-f]+):\s+(\S+)\s*(.*)$")
FUNC = re.compile(r"^([0-9a-f]+) <([\w.]+)>:")
REGS = r"\b(zero|at|v[01]|a[0-3]|t[0-9]|s[0-8]|k[01]|gp|sp|fp|ra)\b"
BRANCH = {"beq", "bne", "beqz", "bnez", "blez", "bgtz", "bltz", "bgez", "b", "bltzal", "bgezal"}


def regs(ops):
    return re.findall(REGS, ops)


def reads_writes(mn, ops):
    r = regs(ops)
    if not r:
        return [], []
    if mn.startswith(("s", "beq", "bne", "bl", "bg", "mt", "j", "mult", "div", "ctc", "b")) and \
            not mn.startswith(("sll", "srl", "sra", "slt", "sub")):
        return r, []  # stores, branches, jr/jalr, mthi/mtlo/mtc2/ctc2, mult/div: every register is a source
    return r[1:], r[:1]


def exports():
    """export offset -> name, from the BtlfxExports struct."""
    src = open("include/btlfx_mod.h").read()
    body = src[src.index("typedef struct BtlfxExports"):src.index("} BtlfxExports;")]
    out = {}
    for m in re.finditer(r"\(\*(\w+)\)\s*\(.*?/\*\s*0x([0-9A-Fa-f]+)", body, re.S):
        out[int(m.group(2), 16)] = m.group(1)
    return out


def return_types():
    """function name -> 'void' / 'value', from ovl_0b's and main's definitions and prototypes."""
    files = sorted(glob.glob("src/ovl_0b/*.c")) + sorted(glob.glob("src/main/*.c")) + \
        ["include/ovl_0b.h", "include/ff9.h", "include/psyq.h"]
    out = {}
    pat = re.compile(r"^(?:static\s+|extern\s+|inline\s+)*([A-Za-z_][\w\s]*?[\s*]+)(\w+)\s*\(", re.M)
    for f in files:
        for m in pat.finditer(open(f).read()):
            ty, name = m.group(1).strip(), m.group(2)
            if ty in ("return", "else", "if", "while", "for", "switch", "do", "case", "goto", "sizeof"):
                continue
            kind = "void" if re.fullmatch(r"void", ty) else "value"
            # a definition beats a prototype; otherwise keep the first seen
            out.setdefault(name, kind)
    # header names that are stale addresses (func_80013228 = ClearImage): resolve through the symbol files
    alias = {}
    for f in glob.glob("config/symbols*.us.txt"):
        for line in open(f):
            m = re.match(r"(\w+) = 0x([0-9A-Fa-f]{8});", line)
            if m:
                alias["func_" + m.group(2).upper()] = m.group(1)
    for k, v in alias.items():
        if k not in out and v in out:
            out[k] = out[v]
    # still INCLUDE_ASM in ovl_0b: void when the asm never writes $v0
    for f in glob.glob("asm/us/ovl_0b/nonmatchings/*/func_*.s"):
        name = f.split("/")[-1][:-2]
        if name not in out:
            out[name] = "value" if re.search(r"\s\w+\s+\$v0,", open(f).read()) else "void"
    return out


def disasm(elf):
    dis = subprocess.run(["mipsel-linux-gnu-objdump", "-d", "--no-show-raw-insn", elf],
                         capture_output=True, text=True).stdout
    funcs = defaultdict(list)
    func = None
    for line in dis.splitlines():
        m = FUNC.match(line)
        if m:
            func = m.group(2).split(".")[0]
            continue
        m = INS.match(line)
        if m and func:
            funcs[func].append((int(m.group(1), 16), m.group(2), m.group(3)))
    return funcs


def branch_target(mn, ops):
    m = re.search(r"\b([0-9a-f]+)\s*(?:<[^>]*>)?\s*$", ops)
    if mn in BRANCH or mn == "j":
        if m:
            return int(m.group(1), 16)
    return None


def follow(ins, start):
    """Paths from index start with $v0 holding the export's result: returns set of ('read', addr) / ('return', addr)."""
    idx = {a: i for i, (a, _, _) in enumerate(ins)}
    hits = set()
    seen = set()
    stack = [start]
    while stack:
        i = stack.pop()
        while 0 <= i < len(ins) and i not in seen:
            seen.add(i)
            a, mn, ops = ins[i]
            rd, wr = reads_writes(mn, ops)
            ctrl = mn in BRANCH or mn in ("j", "jr", "jal", "jalr")
            if "v0" in rd:
                hits.add(("read", a))
                break
            if "v0" in wr:
                break
            if not ctrl:
                i += 1
                continue
            # the delay slot runs before the transfer (and before a callee clobbers v0)
            if i + 1 < len(ins):
                da, dmn, dops = ins[i + 1]
                drd, dwr = reads_writes(dmn, dops)
                if "v0" in drd:
                    hits.add(("read", da))
                    break
                if "v0" in dwr:
                    break
            if mn in ("jal", "jalr"):
                break
            if mn == "jr":
                if ops.strip() == "ra":
                    hits.add(("return", a))
                break
            t = branch_target(mn, ops)
            if t is not None and t in idx:
                stack.append(idx[t])
            if mn in ("j", "b"):
                break
            i += 2
    return hits


def result_used(funcs):
    """Module functions whose $v0 is used by their caller: op handlers / init (called by ovl_0b, which reads
    the result) and functions with a `jal` caller in the module that reads $v0 afterwards."""
    used = set(fn for fn in funcs if re.search(r"_(op[0-9A-Fa-f]+|init)$", fn))
    addr = {ins[0][0]: fn for fn, ins in funcs.items() if ins}
    for fn, ins in funcs.items():
        for i, (a, mn, ops) in enumerate(ins):
            if mn == "jal":
                t = branch_target("j", ops)
                if t in addr and any(k == "read" for k, _ in follow(ins, i + 2)):
                    used.add(addr[t])
    return used


CALLER_SAVED = ("v0", "v1", "a0", "a1", "a2", "a3", "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7", "t8", "t9",
                "at", "ra")


def step(state, ins, exp):
    """Transfer function: state = (table regs, table stack slots, {reg: export offset})."""
    table_regs, table_slots, fptr = set(state[0]), set(state[1]), set(state[2])  # fptr: (reg, offset)
    a, mn, ops = ins
    r = regs(ops)
    m = re.match(r"(\w+),(-?\d+)\((\w+)\)", ops.replace(" ", ""))
    new_table = new_fptr = None
    if mn == "lw" and m:
        rd, off, base = m.group(1), int(m.group(2)), m.group(3)
        if base == "sp" and off in table_slots:
            new_table = rd
        elif base in table_regs and off in exp:
            new_fptr = (rd, off)
        elif off == 0x10 and base != "sp":
            new_table = rd
    elif mn == "sw" and m and m.group(3) == "sp":
        if m.group(1) in table_regs:
            table_slots.add(int(m.group(2)))
        else:
            table_slots.discard(int(m.group(2)))
    elif mn == "move" and len(r) == 2 and r[1] in table_regs:
        new_table = r[0]
    _, wr = reads_writes(mn, ops)
    for w in wr:
        table_regs.discard(w)
        fptr = set(x for x in fptr if x[0] != w)
    if mn in ("jal", "jalr"):
        for t in CALLER_SAVED:
            table_regs.discard(t)
            fptr = set(x for x in fptr if x[0] != t)
    if new_table:
        table_regs.add(new_table)
    if new_fptr:
        fptr.add(new_fptr)
    return (frozenset(table_regs), frozenset(table_slots), frozenset(fptr))


def flow_states(ins, exp):
    """Forward may-dataflow over the function's CFG (union at joins); returns the state before each insn."""
    n = len(ins)
    idx = {a: i for i, (a, _, _) in enumerate(ins)}
    succ = {}
    for i, (a, mn, ops) in enumerate(ins):
        if mn in BRANCH or mn in ("j", "jr"):
            out = []
            t = branch_target(mn, ops)
            if t is not None and t in idx:
                out.append(idx[t])
            if mn not in ("j", "b", "jr") and i + 2 < n:
                out.append(i + 2)
            succ[i + 1] = out  # the transfer happens after the delay slot
            succ.setdefault(i, [i + 1])
        else:
            succ.setdefault(i, [i + 1] if i + 1 < n else [])
    empty = (frozenset(), frozenset(), frozenset())
    before = [None] * n
    before[0] = empty
    work = [0]
    while True:
        while work:
            i = work.pop()
            out = step(before[i], ins[i], exp)
            for j in succ.get(i, []):
                if before[j] is None:
                    before[j] = out
                else:
                    merged = (before[j][0] | out[0], before[j][1] | out[1], before[j][2] | out[2])
                    if merged == before[j]:
                        continue
                    before[j] = merged
                work.append(j)
        # code reached only through a jump table (jr reg): seed it with the state at the jr instructions
        rest = [i for i in range(n) if before[i] is None]
        if not rest:
            return before
        seed = empty
        for i, (a, mn, ops) in enumerate(ins):
            if mn == "jr" and ops.strip() != "ra" and before[i] is not None:
                st = step(before[i], ins[i], exp)
                seed = (seed[0] | st[0], seed[1] | st[1], seed[2] | st[2])
        before[rest[0]] = seed
        work = [rest[0]]


def scan(elf, exp):
    calls = []  # (func, addr, export offset, hits)
    unresolved = []
    funcs = disasm(elf)
    used = result_used(funcs)
    for fn, ins in funcs.items():
        if not ins:
            continue
        before = flow_states(ins, exp)
        for i, (a, mn, ops) in enumerate(ins):
            if mn != "jalr" or before[i] is None:
                continue
            tgt = regs(ops)[-1]
            offs = set(off for reg, off in before[i][2] if reg == tgt)
            if offs:
                # several offsets: cross-jumping shares one jalr between paths that load different exports
                hits = set(h for h in follow(ins, i + 2) if h[0] == "read" or fn in used)
                for off in sorted(offs):
                    calls.append((fn, a, off, hits))
            else:
                unresolved.append((fn, a, " ; ".join("%s %s" % x[1:] for x in ins[max(0, i - 3):i + 1])))
    return calls, unresolved


DOC = "docs/effect-modules.md"
HEADING = "## Void exports whose result is read"


def tail_calls(name):
    """For a void ovl_0b function: the calls that end its paths (a call statement followed by a closing brace),
    with their return types."""
    for f in sorted(glob.glob("src/ovl_0b/*.c")):
        m = re.search(r"^void " + name + r"\(.*?\{\n(.*?\n)\}", open(f).read(), re.M | re.S)
        if not m:
            continue
        out = []
        for c in re.findall(r"(?:^|[;{}])\s*(\w+)\([^;]*\);\s*(?:\}\s*)+(?:else\b|$)", m.group(1) + "}"):
            ty = "?"
            for g in sorted(glob.glob("src/ovl_0b/*.c")) + sorted(glob.glob("src/main/*.c")):
                d = re.search(r"^(?:static\s+)?([A-Za-z_][\w ]*?[\s*]+)" + c + r"\(", open(g).read(), re.M)
                if d:
                    ty = d.group(1).strip()
                    break
            if (c, ty) not in out:
                out.append((c, ty))
        return out
    return []


def write_doc(sites, nmods, ncalls, nunres):
    per = defaultdict(list)
    for site in sites:
        per[(site[3], site[4])].append(site)
    md = [HEADING, "",
          "Written by `tools/docker.sh python3 tools/fxmod_void_returns.py --doc` from the built modules, so it",
          "covers INCLUDE_ASM code too. These module call sites read `$v0` after calling an export that is `void`",
          "in ovl_0b's C. On the PS1 they get the value the export's last callee left in `$v0`; a port that calls",
          "native ovl_0b code through wrappers with ovl_0b's prototypes gets nothing, so these exports need a real",
          "return type there. Each of them ends by calling a function that returns a value, so `return callee(...)`",
          "is the likely original (ovl_0b's C has to be rechecked for a match with it).",
          f"Scan: {nmods} modules, {ncalls} export calls; {nunres} other indirect calls (callbacks passed as",
          "arguments). No module function hands a void export's `$v0` on as its own result to a caller that uses it.",
          "",
          "| export | ovl_0b definition | `$v0` actually holds | sites | modules |",
          "|---|---|---|---|---|"]
    for (off, name), ss in sorted(per.items()):
        tails = tail_calls(name)
        mods = sorted(set(x[0] for x in ss))
        held = " or ".join(f"{c}()'s result ({ty})" for c, ty in tails) or "?"
        md.append(f"| 0x{off:03X} {name} | void | {held} | {len(ss)} | {len(mods)} |")
    md.append("")
    md.append("Sites per export (function, call address, then the address that reads `$v0`):")
    md.append("")
    for (off, name), ss in sorted(per.items()):
        md.append(f"**0x{off:03X} {name}**")
        md.append("")
        bymod = defaultdict(list)
        for mod, fn, a, _, _, kind, ha in ss:
            bymod[mod].append(f"{fn} {a:08X}" + (f"→{ha:08X}" if kind == "read" else f" (returned at {ha:08X})"))
        for mod in sorted(bymod):
            md.append(f"- {mod}: " + ", ".join(bymod[mod]))
        md.append("")
    text = open(DOC).read()
    if HEADING in text:
        text = text[:text.index(HEADING)]
    open(DOC, "w").write(text.rstrip("\n") + "\n\n" + "\n".join(md).rstrip("\n") + "\n")


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--all", action="store_true", help="list every site")
    ap.add_argument("--doc", action="store_true",
                    help="(re)write the 'Void exports whose result is read' section of docs/effect-modules.md")
    ap.add_argument("--unresolved", action="store_true", help="list jalr not recognised as export calls")
    args = ap.parse_args()
    exp = exports()
    rt = return_types()
    unknown = sorted(set(n for n in exp.values() if n not in rt))
    elfs = sorted(glob.glob("build/us/fx_*.elf"))
    if not elfs:
        raise SystemExit("no build/us/fx_*.elf: build the effect modules first (make; needs disks/us/fxmod)")
    sites = []
    ncalls = nunres = 0
    for elf in elfs:
        mod = elf.split("/")[-1][:-4]
        calls, unres = scan(elf, exp)
        ncalls += len(calls)
        nunres += len(unres)
        if args.unresolved:
            for fn, a, ctx in unres:
                print(f"unresolved {mod} {fn} 0x{a:08X}: {ctx}")
        for fn, a, off, hits in calls:
            name = exp[off]
            if rt.get(name) != "void":
                continue
            for kind, ha in sorted(hits):
                sites.append((mod, fn, a, off, name, kind, ha))
    if args.doc:
        write_doc(sites, len(elfs), ncalls, nunres)
    elif args.all:
        for mod, fn, a, off, name, kind, ha in sites:
            print(f"{mod} {fn:28} call 0x{a:08X}  0x{off:03X} {name:26} {kind} at 0x{ha:08X}")
    else:
        per = defaultdict(lambda: defaultdict(set))
        for mod, fn, a, off, name, kind, ha in sites:
            per[(off, name)][kind].add((mod, fn, a))
        for (off, name), kinds in sorted(per.items()):
            for kind, ss in sorted(kinds.items()):
                mods = sorted(set(s[0] for s in ss))
                print(f"0x{off:03X} {name:26} {kind:6} {len(ss):3} site(s) in {len(mods)} module(s): {' '.join(mods[:8])}"
                      + (" ..." if len(mods) > 8 else ""))
    reads = sum(1 for s in sites if s[5] == "read")
    rets = sum(1 for s in sites if s[5] == "return")
    print(f"# {len(elfs)} modules, {ncalls} export calls ({nunres} jalr not through the table), "
          f"void exports: {reads} read site(s), {rets} return site(s)"
          + (f"; exports with no ovl_0b/main declaration found: {' '.join(unknown)}" if unknown else ""))


if __name__ == "__main__":
    main()
