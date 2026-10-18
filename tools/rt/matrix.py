import os as _os
ROOT=_os.path.dirname(_os.path.dirname(_os.path.dirname(_os.path.abspath(__file__))))
import sys, json, os
sys.path.insert(0,ROOT+'/tools/rt')
from syms import funcs
scen=sys.argv[1].split(',')
filt=sys.argv[2] if len(sys.argv)>2 else None
D={s:{e[0]:e for e in json.load(open(f'{ROOT}/build/rt/cov_{s}.json'))} for s in scen}
print(' '*36+' '.join(f'{s[:7]:>7}' for s in scen))
for a,n,b,sz in sorted(funcs(['ovl_0b','main'])):
    row=[D[s].get(a,[0,0])[1] for s in scen]
    if not any(row): continue
    if filt and filt not in n and filt!=b: continue
    print(f'{b[:6]:6} {n:24} {sz:5x} '+' '.join(f'{x:7}' for x in row))
