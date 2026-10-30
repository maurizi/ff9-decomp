"""fxscan.py OPS [--discs ..]: list effect-script instructions (3 bytes op,a,b; script = effect file sector 0 + 0x400,
g_btlfxFileScript) of every dir-13 effect file whose op is in OPS (hex, comma separated). Ends at op 0."""
import os, sys, argparse
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from ff9img import Img
from effect_modules import fx_files
p = argparse.ArgumentParser(); p.add_argument('ops'); p.add_argument('--discs', default='1,2,3,4'); p.add_argument('--all', action='store_true')
a = p.parse_args(); ops = {int(x, 16) for x in a.ops.split(',')}
seen = {}
for d in [int(x) for x in a.discs.split(',')]:
    img = Img(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))) + f'/disks/us/disc{d}/FF9.IMG')
    for fid, sec, n in fx_files(img):
        s = img.read(sec * 2048 + 0x400, 0x400)
        hits = []
        for i in range(0, 0x3FF, 3):
            op, x, y = s[i], s[i + 1], s[i + 2]
            if op in ops: hits.append(f'+{i:#x}:{op:02x} {x:02x} {y:02x}')
            if op == 0: break
        if hits and (fid not in seen):
            seen[fid] = 1
            print(f'disc{d} fx {fid:#x}', ' '.join(hits))
