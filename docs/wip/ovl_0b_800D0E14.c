/* WIP func_800D0E14: 3 differing rows (build/cmp.py-style aligned diff). New in pass 7, modelled on func_800D12C4. Left: the D_800F78A4->unk28 load must come before the c.unk2C store (ours: after it, one extra nop); all 720 orders of the six ctx stores in the n != 0 block and an 8-byte struct copy for unk28/unk2C don't get it. */
#include "ovl_0b.h"

void func_800D0E14(s32 arg0, s32 *arg1, s32 n, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 p,
                   u32 *c1, u32 *c2, s32 tp) {
    Unk800D50A0Ctx c;
    u8 *prim;
    s32 inv;

    if (p < 0) {
        p = 0x1000;
    }
    gte_DpqColorFar_nst(c1, p);
    prim = D_800F78A4->unk24;
    D_800F78A4->unk24 = prim + n * 0x34;
    gte_strgb(&c.unkC);
    gte_DpqColorFar_nst(c2, p);
    c.unk26 = (arg4 + arg6) << 8;
    c.unk24 = arg4 << 8;
    gte_strgb(&c.unk10);
    c.unk18 = (((tp & 3) << 5) | arg7) << 16;
    c.unk1C = arg8 << 16;
    c.unkC.b[3] = (tp == 0xFF) ? 0x3C : 0x3E;
    gte_SetRotMatrix(D_800F78A4->unk14);
    gte_SetTransMatrix(D_800F78A4->unk14);
    if (n != 0) {
        inv = 0x1000 / n;
        c.unk20 = inv;
        c.unk8 = inv;
        c.unk28 = arg1[0];
        c.unk2C = arg1[1];
        c.unk4 = arg0;
        c.unk14 = arg3;
        c.unk16 = arg5;
        c.unk0 = D_800F78A4->unk28;
        func_800D50A0(n, &c, prim, D_800F78A4->unk20);
    }
}
