"""encscan.py: scan encounter files (FF9.IMG dir 5) of every disc for btlseq effect ops (0x08/0x1A id16, 0x06 id16) with given effect ids."""
import os, struct, sys
ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
SECTOR=2048
ids=[int(x,0) for x in sys.argv[1].split(',')]
def encs(path):
    with open(path,'rb') as im:
        h=im.read(SECTOR); kind,count,table,_=struct.unpack_from('<4I',h,16+5*16)
        im.seek(table*SECTOR); e=im.read((count+1)*8)
        for i in range(count):
            key,sec=struct.unpack_from('<2I',e,i*8); end=struct.unpack_from('<I',e,(i+1)*8+4)[0]
            if end<=sec: continue
            im.seek(sec*SECTOR); yield key, im.read((end-sec)*SECTOR)
for d in (1,2,3,4):
    seen=set()
    for key,data in encs(f'{ROOT}/disks/us/disc{d}/FF9.IMG'):
        for fx in ids:
            lo,hi=fx&0xff,fx>>8
            for op in (6,8,0x1a):
                pat=bytes([op,lo,hi]); i=data.find(pat)
                while i>=0:
                    seen.add((key,hex(fx),op,i)); i=data.find(pat,i+1)
    for s in sorted(seen): print('disc',d,'enc',s[0],'fx',s[1],'op',hex(s[2]),'off',hex(s[3]))
