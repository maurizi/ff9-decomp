import os as _os
ROOT=_os.path.dirname(_os.path.dirname(_os.path.dirname(_os.path.abspath(__file__))))
import importlib.util, struct, sys
spec=importlib.util.spec_from_file_location('ff9_emu',ROOT+'/tools/emu.py')
emu=importlib.util.module_from_spec(spec);spec.loader.exec_module(emu)
B=emu.bridge
def r32(a): return B('r32',a)
def rd(a,n): return bytes.fromhex(B('read',a,n))
def r16(a): return struct.unpack('<H',rd(a,2))[0]
def wr(a,b): return B('write',a,b.hex())
def w32(a,v): wr(a,struct.pack('<I',v&0xffffffff))
def gs(): return r32(0x8006794c)
def gd(): return r32(gs()+0x1c)
import time
def run(n, timeout=None):
    B('runFrames', n)
    if not emu.wait_paused(timeout or max(30.0, n/5.0)):
        emu.http('/execution-flow?function=pause','POST'); raise RuntimeError('timeout')
    return B('status')['frame']
