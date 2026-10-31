"""Event-script disassembler (field scripts). Arg widths from build/3ds3/optab.json (derived from evt_execInstr's C)
plus scheduler ops (src_equiv/main/script_text.c evt_runScripts) and evt_modelOpField.
Usage: evtdis.py FIELD [--disc N] [--entry E] [--tag T]"""
import os, sys, json, struct, argparse
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from encounter_catalog import Img, scripts, script_functions, find_image
HERE = os.path.dirname(os.path.abspath(__file__))
_t = json.load(open(os.path.join(HERE, 'optab.json')))
ARGS = {int(k, 16): v['args'] for k, v in _t.items() if int(k, 16) != 0xFF}
ARGS.update({0x7F: '', 0x80: '', 0x81: 'bb', 0x82: 'ww', 0x83: 'b', 0x84: '', 0x85: 'b', 0xD8: 'bb'})
ARGS[0x29] = 'N'          # n, then n x (s16 x, s16 z)
ARGS[0xFE] = 'bbbbb'
ARGS[0x4B] = 'bbb'; ARGS[0xDE] = 'bw'; ARGS[0x4F] = 'r'      # PARTY_JOIN: chr, b, c, class, row (reads the last two in either branch)
NAMES = {0x10: 'CALL', 0x12: 'CALL_WAIT_FREE', 0x14: 'CALL_SYNC', 0x16: 'CALL_CALLER', 0x18: 'CALL_CALLER_WAIT_FREE',
         0x1A: 'CALL_CALLER_SYNC', 0x75: 'MENU', 0x2D: 'DISABLE_CONTROL', 0x2E: 'ENABLE_CONTROL', 0x1F: 'MSG_WAIT',
         0x20: 'MSG', 0x21: 'MSG_CLOSE', 0x2B: 'GOTO_FIELD', 0x07: 'NEW_CODE', 0x08: 'NEW_REGION', 0x09: 'NEW_ACTOR',
         0x2C: 'SET_PLAYER', 0x51: 'TURN_TO_OBJ', 0x50: 'WAIT_TURN', 0x91: 'SET_TALK_TURN', 0x2F: 'SET_MODEL',
         0x1D: 'SET_POS_XZ', 0x36: 'SET_DIR', 0x40: 'ANIM', 0x41: 'WAIT_ANIM', 0x22: 'WAIT', 0x95: 'MSG_OBJ_WAIT',
         0x96: 'MSG_OBJ', 0x54: 'MSG_WAIT_WIN', 0x43: 'RUN_THREAD', 0x44: 'WAIT_THREAD', 0x4B: 'SET_RADIUS',
         0xA7: 'TURN_TO_ANGLE', 0x33: 'SET_STAND_ANIM', 0x34: 'SET_WALK_ANIM', 0x35: 'SET_RUN_ANIM', 0x26: 'SET_WALK_SPEED',
         0x52: 'SET_IDLE_ANIM', 0x23: 'MOVE_XZ', 0x48: 'ADD_ITEM'}


def expr(code, i):
    """Skip an expression starting at i; returns (end, text)."""
    s = i
    while True:
        b = code[i]; i += 1
        if b == 0x7F:
            break
        if b == 0x7D or b == 0x78:
            i += 2
        elif b == 0x7E:
            i += 4
        elif b in (0x79, 0x7A, 0x29, 0x5F):
            i += 1
        elif b >= 0x80:
            i += 2 if b & 0x20 else 1
    return i, 'expr[' + code[s:i].hex(' ') + ']'


def dis(code):
    """Yields (offset, op, flags or None, [args], raw bytes)."""
    i = 0
    n = len(code)
    while i < n:
        s = i
        op = code[i]; i += 1
        flags = None
        args = []
        if op in (1, 2, 3):
            args = [struct.unpack_from('<h', code, i)[0]]; i += 2
            args = [f'->{i + args[0]:#x}']
        elif op == 4:
            pass
        elif op == 5:
            i, t = expr(code, i); args = [t]
        elif op == 6:
            k = code[i]; dflt = struct.unpack_from('<H', code, i + 1)[0]; base = i + 3
            args = [f'n={k}', f'default->{base + dflt:#x}']
            for j in range(k):
                v, o = struct.unpack_from('<HH', code, i + 3 + 4 * j); args.append(f'{v}->{base + o:#x}')
            i += 3 + 4 * k
        elif op in (0x0B, 0x0D):
            if op == 0x0D:
                k = struct.unpack_from('<H', code, i)[0]; i += 1
            else:
                k = code[i]
            bse = code[i + 1] | (struct.unpack_from('<b', code, i + 2)[0] << 8)
            args = [f'n={k}', f'base={bse}']
            i += 5 + 2 * k
        elif op in (0x30, 0x31, 0x6C, 0x6D):
            pass
        elif op == 0x4E:  # debug string, NUL, var ref, 1 byte
            while code[i] != 0: i += 1
            i += 1
            b = code[i]; i += 1 + (2 if b & 0x20 else 1)
            i += 1
        elif op in (0x07, 0x08, 0x09):
            args = [code[i], code[i + 1]]; i += 2
        else:
            if op == 0xFF:
                op = 0xFF00 | code[i]; i += 1
            if op == 0x1C:
                spec = 'b'
            elif op == 0x22:
                spec = 'b'
            elif op == 0x32:
                spec = 'bb'
            else:
                spec = ARGS.get(op)
            if spec is None:
                yield s, op, None, ['??'], code[s:i]
                return
            if spec:
                flags = code[i]; i += 1
                f = flags
                def arg(w):
                    nonlocal i, f
                    if f & 1:
                        i, t = expr(code, i)
                    else:
                        t = int.from_bytes(code[i:i + w], 'little'); i += w
                    f >>= 1
                    return t
                for c in spec:
                    if c == 'b': args.append(arg(1))
                    elif c == 'w': args.append(arg(2))
                    elif c == 't': args.append(arg(3))
                    elif c == 'r': args.append(code[i]); i += 1
                    elif c == 'N':
                        cnt = arg(1); args.append(cnt)
                        for _ in range(cnt if isinstance(cnt, int) else 0):
                            args.append((arg(2), arg(2)))
        yield s, op, flags, args, code[s:i]


def fmt(s, op, flags, args, raw):
    nm = NAMES.get(op, '')
    return f'  {s:04x}: {raw.hex(" "):<36s} {op:02X} {nm:<16s} ' + (f'flags={flags:02x} ' if flags is not None else '') + \
        ', '.join(a if isinstance(a, str) else (f'{a}' if not isinstance(a, int) else f'{a:#x}') for a in args)


def field_script(img, field):
    for fid, sec, n in img.files(4):
        data = img.read(sec * 2048, n * 2048)
        for k, s in scripts(data):
            if k == 0x50000 | field:
                return data, s
    return None, None


if __name__ == '__main__':
    p = argparse.ArgumentParser(); p.add_argument('field', type=int); p.add_argument('--disc', type=int, default=1)
    p.add_argument('--entry', type=int, action='append'); p.add_argument('--tag', type=int, action='append')
    a = p.parse_args()
    img = Img(find_image(a.disc))
    data, s = field_script(img, a.field)
    print(f'field {a.field}: script at file+{s:#x}, {data[s + 3]} entries, name {data[s+4:s+0x2C].split(bytes(1))[0]}')
    for e, tag, code in script_functions(data, s):
        if a.entry and e not in a.entry: continue
        if a.tag and tag not in a.tag: continue
        print(f'entry {e} tag {tag} ({len(code)} bytes)')
        for x in dis(code):
            print(fmt(*x))
