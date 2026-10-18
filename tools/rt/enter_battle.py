"""Enter encounter N on PSX from a loaded, paused field state, keeping the save's own party.
Mirrors ff9-ds tools/psx_battle_fixture.py without the character/event import.
usage: enter_battle.py ENC [--disc 3] [--field 70] [--base-state S] [--summons-seen V] [--frames F]
"""
import os as _os
ROOT=_os.path.dirname(_os.path.dirname(_os.path.dirname(_os.path.abspath(__file__))))
import sys, argparse, struct
sys.path.insert(0,ROOT+'/tools/rt'); sys.path.insert(0,_os.environ.get('FF9DS',ROOT+'/../ff9-ds')+'/tools')
from emu_lib import *
from scenario_catalog import block, SECTOR
def ranges(path, encounter):
    with open(path,'rb') as image:
        header=image.read(SECTOR)
        kind,count,table,_=struct.unpack_from('<4I',header,16+5*16)
        image.seek(table*SECTOR);entries=image.read((count+1)*8)
        for i in range(count):
            key,sector=struct.unpack_from('<2I',entries,i*8)
            if key!=encounter:continue
            end=struct.unpack_from('<I',entries,(i+1)*8+4)[0]
            image.seek(sector*SECTOR);data=image.read((end-sector)*SECTOR)
            deps=block(data,20);n=struct.unpack_from('<H',data,deps+2)[0]
            bgs=[]
            for j in range(n):
                k,start,length,_=struct.unpack_from('<4I',data,deps+4+j*16)
                if k&0xff0000==0xc0000:bgs.append((start,length))
            assert len(bgs)==1, bgs
            return [sector,bgs[0][0]],[end-sector,bgs[0][1]]
    raise ValueError('encounter absent')
p=argparse.ArgumentParser()
p.add_argument('enc',type=int); p.add_argument('--disc',type=int,default=3); p.add_argument('--field',type=int,default=70)
p.add_argument('--base-state'); p.add_argument('--summons-seen',type=lambda s:int(s,0))
p.add_argument('--party',help='e.g. 0,1,2,6');p.add_argument('--frames',type=int,default=0, help='run N frames after mode switch')
a=p.parse_args()
sectors,lengths=ranges(f'{ROOT}/disks/us/disc{a.disc}/FF9.IMG',a.enc)
if a.base_state:
    emu.http('/state/load?name='+a.base_state); emu.http('/execution-flow?function=pause','POST')
g_s=gs(); g=gd()
assert rd(g_s+8,1)==b'\x01', 'not in field'
bp=B('addBp',0x80012834,'Exec',4,'break',1,'after_field_cleanup')
try:
    wr(g_s+8,b'\x02'); wr(g_s+10,b'\x01'); w32(g_s, r32(g_s)|2)
    emu.run_frames(600)
    assert B('status')['pc']==0x80012834, 'field cleanup did not reach dispatcher'
finally:
    B('removeBp',bp)
if a.summons_seen is not None: wr(g+0x55e,struct.pack('<H',a.summons_seen))
for i in range(2):
    w32(g+0x7ec+i*4,r32(g+0x7fc+i*4)); w32(g+0x7f4+i*4,r32(g+0x804+i*4))
    w32(g+0x7fc+i*4,sectors[i]); w32(g+0x804+i*4,lengths[i])
if a.party:
    for i,ch in enumerate(int(x) for x in a.party.split(',')): w32(g+0x54c+i*4, 0 if ch<0 else g+0x3c+ch*0x90)
wr(g+12,struct.pack('<2H',a.field,a.enc))
wr(g_s+8,b'\x02'); wr(g_s+10,b'\x01')
if a.frames: run(a.frames)
print('ok', sectors, lengths)
