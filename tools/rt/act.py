"""act.py NAME STATE 'cross,cross,w60,down,...' [--after N] [--shots]
Loads STATE, resets coverage, performs the inputs (button[+button][:hold], wN = wait N frames), runs --after
frames, saves coverage as cov_NAME.json and a screenshot."""
import os as _os
ROOT=_os.path.dirname(_os.path.dirname(_os.path.dirname(_os.path.abspath(__file__))))
import sys, argparse, json
sys.path.insert(0,ROOT+'/tools/rt')
from emu_lib import *
p=argparse.ArgumentParser(); p.add_argument('name'); p.add_argument('state'); p.add_argument('seq')
p.add_argument('--after',type=int,default=0); p.add_argument('--shots',action='store_true'); p.add_argument('--nocov',action='store_true'); p.add_argument('--trace',default=''); p.add_argument('--watch',default=''); p.add_argument('--show',type=int,default=12); p.add_argument('--max',type=int,default=300)
a=p.parse_args()
S=ROOT+'/build/rt/'; _os.makedirs(S,exist_ok=True)
if a.state!='-':
    emu.http('/state/load?name='+a.state); emu.http('/execution-flow?function=pause','POST')
if not a.nocov: emu.lua('PCSX.settings.emulator.Debug.Debug = true; return Cov.reset()')
ids={}
for t in [t for t in a.trace.split(',') if t]:
    ids[B('addBp',emu.parse_addr(t),'Exec',4,'log',a.max,t)]=t
for w in [w for w in a.watch.split(',') if w]:
    parts=w.split(':'); kind='Read' if len(parts)>2 and parts[2]=='r' else 'Write'
    ids[B('addBp',emu.parse_addr(parts[0]),kind,int(parts[1]) if len(parts)>1 else 4,'log',a.max,w)]=w
if ids: emu.lua('PCSX.settings.emulator.Debug.Debug = true; return 1')
i=0
for tok in [t for t in a.seq.split(',') if t]:
    if tok.startswith('w'): run(int(tok[1:])); continue
    if tok=='s':
        import subprocess; subprocess.run([ROOT+'/tools/emu.py','shot',S+f'{a.name}_s{i}.png'],stdout=subprocess.DEVNULL); i+=1; continue
    hold=4; wait=12
    if ':' in tok:
        tok,rest=tok.split(':',1); parts=rest.split(':'); hold=int(parts[0]); wait=int(parts[1]) if len(parts)>1 else wait
    names=[{'x':'CROSS','o':'CIRCLE'}.get(b,b.upper()) for b in tok.split('+')]
    B('hold',names,hold); run(hold+wait)
    if a.shots:
        import subprocess; subprocess.run([ROOT+'/tools/emu.py','shot',S+f'{a.name}_{i}.png'],stdout=subprocess.DEVNULL)
    i+=1
if a.after: run(a.after)
if not a.nocov:
    r=emu.lua('return Cov.result()')
    json.dump(r,open(f'{ROOT}/build/rt/cov_{a.name}.json','w'))
    print(len(r),'functions hit')
res=emu.resident_overlays(); sy=emu.syms()
for bp,t in ids.items():
    log=B('takeLog',bp); B('removeBp',bp)
    json.dump(log,open(f'{ROOT}/build/rt/trace_{a.name}_{t.replace(":","_")}.json','w'))
    print(f'== {t}: {log["hits"]} hits')
    for h in (log['log'] or [])[:a.show]:
        print(f'   f{h["frame"]} pc={sy.describe(h["pc"],res)} ra={sy.describe(h["ra"],res)} a0={h["a0"]:08X} a1={h["a1"]:08X} a2={h["a2"]:08X} a3={h["a3"]:08X}')
if ids: emu.lua('PCSX.settings.emulator.Debug.Debug = true; return 1')
import subprocess
subprocess.run([ROOT+'/tools/emu.py','shot',S+a.name+'.png'],stdout=subprocess.DEVNULL)
print('frame',B('status')['frame'])
