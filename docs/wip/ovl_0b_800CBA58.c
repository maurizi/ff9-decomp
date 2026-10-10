/* WIP func_800CBA58: 60 differing rows (build/cmp.py-style aligned diff). semantic draft only: arg0/arg1 s-regs swapped, pc reloaded per byte, dx/dz reloaded from the stack. */
#include "ovl_0b.h"

s32 func_800CBA58(BtlScript *arg0, BtlUnit *arg1) {
    u8 *pc = arg0->unk8;
    s32 a;
    s32 ang;
    s32 b;
    s16 d[2];
    s32 v;

    a = pc[1] | (pc[2] << 8);
    ang = a;
    b = pc[3] | (pc[4] << 8);
    arg0->unk21 = pc[5];
    arg0->unk20 = 1;
    arg0->unk1C = arg1->unk86[1];
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
