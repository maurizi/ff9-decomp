#!/usr/bin/env python3
"""wm_force.py WORLD [--disc N] [--base-state S] [--poke ADDR=HEX ...] [--frames N] [--save S] [--shot P]
From a paused field state: force the field exit (as enter_field.py), break at main's dispatcher 0x80012834, write the
world file's sector/length (FF9.IMG dir 3 file WORLD-9000) into GameData+0x800/+0x808, GameData.fieldId = WORLD,
apply the pokes (story, event bytes) and set GameState.mode 3 (GS_MODE_WORLD); then let wm_entry run."""
import argparse, os, sys, struct, subprocess
ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, ROOT + '/tools/rt'); sys.path.insert(0, ROOT + '/tools')
from emu_lib import *
from encounter_catalog import Img, find_image
DISPATCH = 0x80012834
BASE = {1: 'field_dali_inn', 2: 'field_iifa_tree', 3: 'disc3_pandemonium_lv99_field', 4: 'ds_perf_disc4_treno'}
p = argparse.ArgumentParser()
p.add_argument('world', type=int); p.add_argument('--disc', type=int, default=3); p.add_argument('--base-state')
p.add_argument('--poke', action='append', default=[]); p.add_argument('--frames', type=int, default=600)
p.add_argument('--save'); p.add_argument('--shot')
a = p.parse_args()
f = Img(find_image(a.disc)).files(3)[a.world - 9000]
emu.http('/state/load?name=' + (a.base_state or BASE[a.disc])); emu.http('/execution-flow?function=pause', 'POST')
g = gs(); d = gd()
bp = B('addBp', DISPATCH, 'Exec', 4, 'break', 1, 'wm_force')
try:
    wr(g + 10, b'\x01'); w32(g, r32(g) | 2); run(600)
    if B('status')['pc'] != DISPATCH: raise SystemExit('no dispatcher break')
finally:
    B('removeBp', bp)
w32(d + 0x800, f[1]); w32(d + 0x808, f[2]); wr(d + 12, struct.pack('<H', a.world))
wr(d + 0x7DC, struct.pack('<hH', a.world, 68))  # GameData.worldId / worldAreaId (every world file links area 68)
for pk in a.poke:
    ad, h = pk.split('='); wr(emu.parse_addr(ad), bytes.fromhex(h))
wr(g + 8, b'\x03')
run(a.frames)
print('world', a.world, 'file', hex(f[0]), 'mode', rd(gs() + 8, 1).hex(), 'story', r16(0x8007AEF0), 'b2', rd(0x8007AEF2, 2).hex(),
      'be', rd(0x8007AFAE, 1).hex(), 'bf', rd(0x8007AFAF, 1).hex(), 'frame', B('status')['frame'])
if a.shot: subprocess.run([ROOT + '/tools/emu.py', 'shot', a.shot], stdout=subprocess.DEVNULL)
if a.save: print(emu.http('/state/save?name=' + a.save).decode().strip())
