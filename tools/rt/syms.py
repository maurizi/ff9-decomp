import os as _os
ROOT=_os.path.dirname(_os.path.dirname(_os.path.dirname(_os.path.abspath(__file__))))
import re
MAPS={'main':'slus_012.51','ovl_0b':'ovl_0b'}
def funcs(bins):
    """[(addr,name,bin,size)] for game functions in the index, addresses from the build maps"""
    idx={}
    for l in open(ROOT+'/config/functions.us.txt'):
        if l.startswith('#'): continue
        b,n,s=l.split()[:3]
        idx[(b,n)]=int(s,16)
    out=[]
    for b in bins:
        seen=set()
        for l in open(f'{ROOT}/build/us/{MAPS.get(b,b)}.map'):
            m=re.match(r'\s+0x([0-9a-f]{8,16})\s+(\w+)\s*$',l)
            if not m: continue
            n=m.group(2)
            if (b,n) in idx and n not in seen:
                seen.add(n); out.append((int(m.group(1),16)&0xffffffff,n,b,idx[(b,n)]))
    return out
