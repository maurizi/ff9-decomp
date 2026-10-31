"""edis.py FIELD [--disc N] [--entry E] [--tag T]: evtdis with infix expressions and EVT_OP_* names."""
import os, sys, re, struct, argparse
ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.join(ROOT, 'tools', 'evtscript')); sys.path.insert(0, os.path.join(ROOT, 'tools'))
import evtdis
from evtdis import dis, field_script, Img, find_image, script_functions
NM = {}
for l in open(os.path.join(ROOT, 'include', 'ff9.h')):
    m = re.match(r'#define EVT_OP_(\w+) (0x[0-9A-Fa-f]+)', l)
    if m: NM[int(m.group(2), 16)] = m.group(1)
BIN = {0x11:'*',0x12:'/',0x13:'%',0x14:'+',0x15:'-',0x16:'<<',0x17:'>>',0x18:'<',0x19:'>',0x1A:'<=',0x1B:'>=',
       0x20:'==',0x21:'!=',0x24:'&',0x25:'^',0x26:'|',0x27:'&&',0x28:'||',0x2F:'*=',0x30:'/=',0x31:'%=',0x32:'+=',
       0x33:'-=',0x34:'<<=',0x35:'>>=',0x3D:'&=',0x3E:'^=',0x3F:'|=',0x2C:'='}
UN = {0x0E:'!',0x10:'~',0x0D:'-'}
POST = {0x04:'++',0x05:'--'}; PRE = {0x06:'++',0x07:'--'}
FN1 = {0x62:'sin',0x63:'cos',0x50:'sin2',0x51:'cos2',0x4F:'padPressed',0x58:'padReleased',0x59:'padHeld',0x6B:'isInParty',
       0x6D:'joinParty',0x6A:'animFrames',0x64:'itemCount',0x65:'bgAnimFrame',0x52:'hp',0x53:'maxHp',0x6E:'mp',0x6F:'maxMp',
       0x70:'actorTriX',0x71:'actorTriY',0x61:'distToObj',0x60:'angleToObj',0x4D:'randomBit',0x2A:'bitCount'}
FN2 = {0x66:'atan2',0x5E:'distTo',0x5D:'angleTo'}
TY = {0:'bit',4:'bit',8:'s24',0xC:'u24',0x10:'s8',0x14:'u8',0x18:'s16',0x1C:'u16'}
BK = {0:'G',1:'F',2:'L',3:'L'}
SYS = {2:'controlEnabled',4:'talkAngle',6:'gil',7:'steps',8:'msgSyncTag',9:'msgChoice',12:'scrX',13:'scrY',0:'rand',
       24:'battleId',27:'battleResult',20:'playTime'}


def var(b, idx):
    return f'{BK[b&3]}{TY[b&0x1C]}[{idx:#x}]'


def ex(code, i):
    st = []
    while True:
        b = code[i]; i += 1
        if b == 0x7F: break
        if b >= 0x80:
            if b & 0x20: idx = code[i] | code[i+1] << 8; i += 2
            else: idx = code[i]; i += 1
            st.append(var(b, idx))
        elif b == 0x7D: st.append(str(struct.unpack_from('<h', code, i)[0])); i += 2
        elif b == 0x7E: st.append(hex(struct.unpack_from('<I', code, i)[0] & 0x3FFFFFF)); i += 4
        elif b == 0x78: st.append(f'obj({code[i]:#x}).p{code[i+1]:#x}'); i += 2
        elif b == 0x79: st.append(f'btlvar{code[i]:#x}'); i += 1
        elif b == 0x7A: st.append(f'sys.{SYS.get(code[i], code[i])}'); i += 1
        elif b == 0x29: st.append(f'self.p{code[i]:#x}'); i += 1
        elif b == 0x5F: st.append(f'objid({code[i]:#x})'); i += 1
        elif b in BIN:
            r = st.pop() if st else '?'; l = st.pop() if st else '?'; st.append(f'({l} {BIN[b]} {r})')
        elif b in UN: st.append(f'{UN[b]}{st.pop()}')
        elif b in POST: st.append(f'{st.pop()}{POST[b]}')
        elif b in PRE: st.append(f'{PRE[b]}{st.pop()}')
        elif b in FN1: st.append(f'{FN1[b]}({st.pop()})')
        elif b in FN2: r = st.pop(); l = st.pop(); st.append(f'{FN2[b]}({l}, {r})')
        else: st.append(f'op{b:#x}({st.pop() if st else ""})')
    return i, '{' + ' ; '.join(st) + '}'


evtdis.expr = ex


def fmt(s, op, flags, args, raw):
    nm = NM.get(op, evtdis.NAMES.get(op, ''))
    a = ', '.join(x if isinstance(x, str) else (hex(x) if isinstance(x, int) else str(x)) for x in args)
    if op == 0x0B:
        n = raw[1]; base = raw[2] | (struct.unpack_from('<b', raw, 3)[0] << 8); r = s + 1
        d = struct.unpack_from('<H', raw, 4)[0]
        a = f'n={n} base={base} default->{r + d:#x} ' + ' '.join(
            f'{base + j}->{r + struct.unpack_from("<H", raw, 6 + 2 * j)[0]:#x}' for j in range(n))
    if op == 5: a = a[1:-1]
    return f'  {s:04x}: {raw[:12].hex(" "):<36s} {op:02X} {nm:<18s} {a}'


if __name__ == '__main__':
    p = argparse.ArgumentParser(); p.add_argument('field', type=int); p.add_argument('--disc', type=int, default=1)
    p.add_argument('--entry', type=int, action='append'); p.add_argument('--tag', type=int, action='append')
    a = p.parse_args()
    img = Img(find_image(a.disc)); data, s = field_script(img, a.field)
    for e, tag, code in script_functions(data, s):
        if a.entry and e not in a.entry: continue
        if a.tag and tag not in a.tag: continue
        print(f'entry {e} tag {tag} ({len(code)} bytes)')
        for x in dis(code): print(fmt(*x))
