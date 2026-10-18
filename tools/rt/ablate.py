"""ablate.py STATE 'pre-seq' FUNC[,FUNC] ... : for each func: load STATE, run pre-seq, stub func (jr ra), run --frames, shot.
Writes scratchpad/abl_<func>.png and a labelled montage abl.png"""
import os as _os
ROOT=_os.path.dirname(_os.path.dirname(_os.path.dirname(_os.path.abspath(__file__))))
import sys, argparse, subprocess
sys.path.insert(0,ROOT+'/tools/rt')
from emu_lib import *
from PIL import Image, ImageDraw
S=ROOT+'/build/rt/'; _os.makedirs(S,exist_ok=True)
p=argparse.ArgumentParser(); p.add_argument('state'); p.add_argument('seq'); p.add_argument('funcs',nargs='+')
p.add_argument('--frames',type=int,default=8); p.add_argument('--ret',default=None, help='v0 to return (hex)'); p.add_argument('--out',default='abl'); p.add_argument('--crop',default=None,help='x0,y0,x1,y1'); p.add_argument('--cols',type=int,default=4)
a=p.parse_args()
def seq(s):
    for tok in [t for t in s.split(',') if t]:
        if tok.startswith('w'): run(int(tok[1:])); continue
        hold,wait=4,12
        if ':' in tok:
            tok,rest=tok.split(':',1); parts=rest.split(':'); hold=int(parts[0]); wait=int(parts[1]) if len(parts)>1 else wait
        B('hold',[{'x':'CROSS'}.get(b,b.upper()) for b in tok.split('+')],hold); run(hold+wait)
ims=[]
for f in ['none']+a.funcs:
    emu.http('/state/load?name='+a.state); emu.http('/execution-flow?function=pause','POST')
    seq(a.seq)
    if f!='none':
        for g in f.split('+'):
            ad=emu.parse_addr(g)
            if a.ret is not None:
                v=int(a.ret,16); stub=struct.pack('<III',0x03E00008,0x24020000|(v&0xffff),0)  # jr ra; li v0,v
            else: stub=struct.pack('<II',0x03E00008,0)
            wr(ad,stub)
    run(a.frames)
    out=S+f'abl_{f}.png'
    subprocess.run([ROOT+'/tools/emu.py','shot',out],stdout=subprocess.DEVNULL)
    im=Image.open(out).convert('RGB')
    if a.crop: im=im.crop(tuple(int(v) for v in a.crop.split(','))); im=im.resize((im.width*2,im.height*2))
    d=ImageDraw.Draw(im); d.rectangle([0,0,130,10],fill=(0,0,0)); d.text((2,0),f[:22],fill=(255,255,0)); ims.append(im)
cols=a.cols; w,h=ims[0].size; rows=(len(ims)+cols-1)//cols
o=Image.new('RGB',(w*cols,h*rows))
for i,im in enumerate(ims): o.paste(im,((i%cols)*w,(i//cols)*h))
o.save(S+a.out+'.png'); print('wrote',S+a.out+'.png')
