/* WIP func_800D1D74: 22 differing rows (build/cmp.py-style aligned diff). New in pass 8 (GTE sprite-set draw, sibling of func_800D198C/D1FFC; ctx for func_800D3ED4). Fixed: arg4 copied to i (an unmodified stack param is rematerialised late, as in the original), n reused for n*2+8, separate count k. Left: register choice in the header block: the original has n in a2, i in v1 and computes i*2 (sll a0) right after n*2+8, adding arg0 later; ours keeps i in a0 and sinks the sll. Moving &h->unk8[i] earlier / reusing prim as the temp (permuter idea): no better. */
#include "ovl_0b.h"

typedef struct {
    u16 *unk0;
    u16 *unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
} Unk800D3ED4Ctx;

void func_800D3ED4(u8 *, Unk800D3ED4Ctx *, u8 *, s32 *);

typedef struct {
    u16 unk0;
    u16 unk2;
    u16 unk4;
    u8 unk6;
    u8 unk7;
    u16 unk8[1];
} Unk800D1D74Hdr;

void func_800D1D74(u8 *arg0, SVECTOR *arg1, SVECTOR *arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    Unk800D1D74Hdr *h = (Unk800D1D74Hdr *)arg0;
    VECTOR s;
    SVECTOR r;
    MATRIX m;
    Unk800D3ED4Ctx c;
    s32 n;
    u16 *p;
    u8 *prim;
    s32 i;
    s32 k;

    if (arg2 == NULL) {
        arg2 = &r;
        *(s32 *)&r.vx = 0;
        *(s32 *)&r.vz = 0;
    }
    n = h->unk0 & 0x7FFF;
    i = arg4;
    if (i >= n) {
        i %= n;
    }
    n = n * 2 + 8;
    i *= 2;
    p = (u16 *)(arg0 + n);
    k = *p++;
    c.unk0 = p;
    c.unk4 = p + k + 1;
    c.unk10 = h->unk2 | ((arg5 & 3) << 5);
    c.unk14 = h->unk4;
    c.unkC = (h->unk6 - 1) * 2;
    c.unk8 = (h->unk7 - 1) * 2;
    n = *(u16 *)(arg0 + i + 8);
    c.unk1C = arg0[n++];
    arg0 += n;
    gte_SetRotMatrix(D_800F78A4->unk14);
    gte_SetTransMatrix(D_800F78A4->unk14);
    gte_ldv0(arg1);
    prim = D_800F78A4->unk24;
    D_800F78A4->unk24 = prim + c.unk1C * 40;
    gte_rtv0tr_nn();
    func_800316D8(arg2, &m);
    if (arg3 >= 0) {
        s.vz = arg3;
        s.vy = arg3;
        s.vx = arg3;
        ScaleMatrix(&m, &s);
    }
    gte_stlvnl(m.t);
    if (arg2->pad != 0) {
        MulRotMatrix(&m);
    }
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    c.unk24 = (arg5 == 0xFF) ? 0x2C : 0x2E;
    c.unk18 = arg6;
    c.unk20 = D_800F78A4->unk28;
    if (c.unk1C != 0) {
        func_800D3ED4(arg0, &c, prim, D_800F78A4->unk20);
    }
}
