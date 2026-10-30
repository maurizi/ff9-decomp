"""varscan.py SPEC [--discs 1,2,3,4] [--dirs 3,4,5]: find event-script references to a global variable.
SPEC: bit:INDEX (global bit var, INDEX = byte*8+bit) or byte:OFFSET (global byte var g_evtGlobalVars.b[OFFSET],
also 16/24-bit vars covering it). Var ref byte: 0x80 marker, 0x20 wide index, 0x10/0x08/0x04 type, 0x03 bank (0 global)
(evt_runExpr/func_80051F18/evt_popValue/evt_readVar in main). Prints dir, file id, entry, tag, offset in function, context."""
import os, sys, struct, argparse
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from encounter_catalog import Img, scripts, script_functions, db_items
p = argparse.ArgumentParser(); p.add_argument('spec'); p.add_argument('--discs', default='1,2,3,4'); p.add_argument('--dirs', default='3,4,5')
a = p.parse_args()
kind, v = a.spec.split(':'); v = int(v, 0)
pats = []
if kind == 'bit':
    for b in (0x80, 0x84, 0xC0, 0xC4):
        if v > 0xFF: pats.append(bytes([b | 0x20, v & 0xFF, v >> 8]))
        else: pats.append(bytes([b, v]))
else:
    for b in (0x90, 0x94, 0xD0, 0xD4): pats.append(bytes([b, v]) if v <= 0xFF else bytes([b | 0x20, v & 0xFF, v >> 8]))
    for o in (v - 1, v):
        for b in (0x98, 0x9C, 0xD8, 0xDC): pats.append(bytes([b, o]))
    for o in (v - 2, v - 1, v):
        for b in (0x88, 0x8C, 0xC8, 0xCC): pats.append(bytes([b, o]))
for disc in [int(x) for x in a.discs.split(',')]:
    img = Img(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))) + f'/disks/us/disc{disc}/FF9.IMG')
    for d in [int(x) for x in a.dirs.split(',')]:
        for fid, sec, n in img.files(d):
            data = img.read(sec * 2048, n * 2048)
            try: ss = scripts(data) + [(k, o) for k, o in db_items(data) if data[o:o + 2] == b'EV']
            except Exception: continue
            for k2, s in ss:
                try:
                    for e, tag, code in script_functions(data, s):
                        for pt in pats:
                            i = code.find(pt)
                            while i >= 0:
                                print(f'disc{disc} dir{d} file{fid} script{k2:#x} entry{e} tag{tag} +{i:#x} pat {pt.hex()} ctx {code[max(0,i-6):i+8].hex()}')
                                i = code.find(pt, i + 1)
                except Exception as ex:
                    pass
