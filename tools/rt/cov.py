"""cov.py start [--bin ovl_0b ...] [--cap N] | reset | save NAME | stop"""
import os as _os
ROOT=_os.path.dirname(_os.path.dirname(_os.path.dirname(_os.path.abspath(__file__))))
import sys, json, argparse
sys.path.insert(0,ROOT+'/tools/rt')
from emu_lib import *
from syms import funcs as mfuncs
def funcs(bins):
    out=[]
    for l in open(ROOT+'/config/functions.us.txt'):
        if l.startswith('#'): continue
        b,name,size,addr=l.split()[:4]
        if b in bins: out.append((int(addr,16),name,b))
    return out
p=argparse.ArgumentParser(); p.add_argument('cmd'); p.add_argument('name',nargs='?')
p.add_argument('--bin',action='append'); p.add_argument('--cap',type=int,default=2000)
p.add_argument('--exclude-lib',action='store_true',default=True)
a=p.parse_args()
if a.cmd=='start':
    emu.lua(open(ROOT+'/tools/rt/cov.lua').read())
    bins=a.bin or ['ovl_0b']
    lib=set()
    # library_funcs also lists Square's hand-written asm in ovl_0a/0b/0c (effect emitters etc.): cover those,
    # skip only SDK code (main, ovl_64)
    for l in open(ROOT+'/config/library_funcs.us.txt'):
        if l.strip() and not l.startswith('#') and l.split()[0] in ('main', 'ovl_64'): lib.add(l.split()[1])
    for f in ['symbols.psyq.us.txt','symbols.psyq.ovl_64.us.txt']:
        for l in open(ROOT+'/config/'+f):
            if '=' in l: lib.add(l.split('=')[0].strip())
    fs=[f for f in mfuncs(bins) if f[1] not in lib and not f[1].startswith('__')]
    addrs=[f[0] for f in fs]
    code='return Cov.start({'+','.join(str(x) for x in addrs)+'},%d)'%a.cap
    print(emu.lua(code))
elif a.cmd=='reset': print(emu.lua('return Cov.reset()'))
elif a.cmd=='save':
    r=emu.lua('return Cov.result()')
    _os.makedirs(ROOT+'/build/rt',exist_ok=True); json.dump(r,open(f'{ROOT}/build/rt/cov_{a.name}.json','w'))
    print(len(r),'functions hit')
elif a.cmd=='stop': print(emu.lua('return Cov.stop()'))
