#!/usr/bin/env python3
"""bj_force.py [--base-state S] [--keep-movie] [--save S]: from a disc-4 field state, force the field exit, set
GameState.mode 4 (GS_MODE_ENDING) at main's dispatcher, and (unless --keep-movie) return from ending_playMovie at
once (FMV059 is the ~10-minute credits movie); then run to the "THE END" screen."""
import argparse, os, sys, subprocess
ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, ROOT + '/tools/rt')
from emu_lib import *
DISPATCH = 0x80012834
PLAYMOVIE = 0x800A7378  # ovl_15 ending_playMovie
p = argparse.ArgumentParser(); p.add_argument('--base-state', default='ds_perf_disc4_treno')
p.add_argument('--keep-movie', action='store_true'); p.add_argument('--frames', type=int, default=600)
p.add_argument('--save'); p.add_argument('--shot')
a = p.parse_args()
emu.http('/state/load?name=' + a.base_state); emu.http('/execution-flow?function=pause', 'POST')
g = gs()
bp = B('addBp', DISPATCH, 'Exec', 4, 'break', 1, 'bj_force')
try:
    wr(g + 10, b'\x01'); w32(g, r32(g) | 2); run(600)
    if B('status')['pc'] != DISPATCH: raise SystemExit('no dispatcher break')
finally:
    B('removeBp', bp)
wr(g + 8, b'\x04')
if not a.keep_movie:
    bp = B('addBp', PLAYMOVIE, 'Exec', 4, 'break', 1, 'playMovie')
    try:
        for _ in range(60):
            run(20)
            if B('status')['pc'] == PLAYMOVIE: break
        else:
            raise SystemExit('ending_playMovie not reached')
    finally:
        B('removeBp', bp)
    r = B('regs'); print('ending_playMovie(%#x, %#x) skipped, ra %#x' % (r['a0'], r['a1'], r['ra']))
    B('setReg', 'v0', 0); B('setReg', 'pc', r['ra'])
run(a.frames)
print('mode', rd(gs() + 8, 1).hex(), 'frame', B('status')['frame'])
if a.shot: subprocess.run([ROOT + '/tools/emu.py', 'shot', a.shot], stdout=subprocess.DEVNULL)
if a.save: print(emu.http('/state/save?name=' + a.save).decode().strip())
