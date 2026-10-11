#!/usr/bin/env python3
"""Rename struct members everywhere they're used, guided by the compiler.

    1. Rename the members in the struct definition (include/*.h) by hand.
    2. python3 tools/rename_fields.py unk10=cur.hp,unk12=cur.mp,unkB=level

Builds (in Docker), and on every line gcc rejects with "structure has no member named
`unkB'" rewrites that member, but only when the line uses it exactly once: the error proves
that use is on the renamed struct. Lines using it more than once are listed for a hand fix
(`a->unkB = b->unkB` where only one side is the renamed struct); fix them and run again.
Repeats until the build has no member errors. Then `make verify`.

Member names that other structs also use are safe: their uses compile and are never touched.
Before step 1, check that no other struct already has a member with one of the NEW names,
or a wrong base could compile silently.
"""
import re
import subprocess
import sys

ERR = re.compile(r"^([\w/.]+):(\d+): structure has no member named `(\w+)'", re.M)


def main():
    if len(sys.argv) != 2 or "=" not in sys.argv[1]:
        sys.exit(__doc__)
    pairs = dict(p.split("=") for p in sys.argv[1].split(","))
    for _ in range(20):
        out = subprocess.run(["tools/docker.sh", "make", "-j8", "build"], capture_output=True, text=True)
        text = out.stdout + out.stderr
        errs = {}
        for f, ln, m in set(ERR.findall(text)):
            errs.setdefault(f, {}).setdefault(int(ln), set()).add(m)
        if not errs:
            other = [l for l in text.splitlines() if re.search(r"error|Error \d", l)]
            print("no member errors" + ("; other errors:\n" + "\n".join(other[:10]) if other else ""))
            return
        fixed, manual = 0, []
        for f, lines in errs.items():
            src = open(f).read().split("\n")
            for ln, members in lines.items():
                s = src[ln - 1]
                for m in sorted(members):
                    occ = list(re.finditer(rf"(\.|->){m}\b", s))
                    if m in pairs and len(occ) == 1:
                        o = occ[0]
                        s = s[:o.start()] + o.group(1) + pairs[m] + s[o.end():]
                        fixed += 1
                    else:
                        manual.append(f"{f}:{ln}: {m}: {s.strip()}")
                src[ln - 1] = s
            open(f, "w").write("\n".join(src))
        print(f"fixed {fixed}")
        if not fixed:
            print("fix by hand, then run again:\n" + "\n".join(sorted(set(manual))))
            sys.exit(1)


if __name__ == "__main__":
    main()
