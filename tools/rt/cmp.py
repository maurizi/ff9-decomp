"""cmp.py A B [C...] : functions hit in A but not in any of B..; with -i: intersection info"""
import os as _os
ROOT=_os.path.dirname(_os.path.dirname(_os.path.dirname(_os.path.abspath(__file__))))
import sys, json
sys.path.insert(0,ROOT+'/tools/rt')
from syms import funcs
def names():
    return {a:(b,n,s) for a,n,b,s in funcs(['ovl_0b','main'])}
M=names()
def load(n): return {e[0]:e for e in json.load(open(f'{ROOT}/build/rt/cov_{n}.json'))}
A=load(sys.argv[1]); others=[load(x) for x in sys.argv[2:]]
for a in sorted(A):
    if any(a in o for o in others): continue
    b,n,s=M.get(a,('?','?',0)); e=A[a]
    print(f'{b:7} {n:28} size {s:5x} hits {e[1]:5} first {e[2]:6} ra {" ".join("%08X"%r for r in e[3][:4])}')
