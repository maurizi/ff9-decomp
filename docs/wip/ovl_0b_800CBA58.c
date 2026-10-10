/* WIP func_800CBA58: 45 differing rows (build/cmp.py-style aligned diff). Pass 8 (permuter): b reused for pc[2] << 8 and unk86[1] read into v before the unk20 store took it from 60 to 45 rows. Left: arg0/arg1 s-regs swapped (s0/s1), dx/dz reloads, one instruction short. */
#include "ovl_0b.h"

s32 func_800CBA58(BtlScript *arg0, BtlUnit *arg1) {
    u8 *pc = arg0->unk8;
    s32 a;
    s32 ang;
    s32 b;
    s16 d[2];
    s32 v;

    b = pc[2] << 8;
    a = pc[1] | b;
    ang = a;
    b = pc[3] | (pc[4] << 8);
    arg0->unk21 = pc[5];
    v = arg1->unk86[1];
    arg0->unk20 = 1;
    arg0->unk1C = v;
    if (a & 0x8000) {
        switch (a & 0x7FFF) {
        case 0:
            ang = (u16)arg1->unkA0->unk22;
            break;
        case 1:
            func_800CB1D4(arg0->unk4->unkC, (u16 *)&d[0], (u16 *)&d[1]);
            d[0] = (arg1->unk8C[0] >> 16) - d[0];
            d[1] = (arg1->unk8C[2] >> 16) - d[1];
            if (d[0] != 0 || d[1] != 0) {
                ang = ratan2(d[0], d[1]);
                break;
            }
        default:
            ang = (u16)arg1->unk86[1];
            break;
        }
    }
    v = ((u16)arg1->unk86[1] - ((ang + b) & 0xFFF)) & 0xFFF;
    if (v > 0x800) {
        arg0->unk1E = 0x1000 - v;
    } else {
        arg0->unk1E = -v;
    }
    arg0->unk8 += 6;
    return 1;
}
