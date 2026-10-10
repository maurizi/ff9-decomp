/* WIP func_800D1FFC: 27 differing rows (build/cmp.py-style aligned diff), same length. New in pass 9 (3D sprite draw, sibling of func_800D1D74 whose WIP has the same header-block problem). Ctx is 0x28 bytes with z as its own local right after it (sp+0x70); size/mode switch, func_800D090C tpage and the addPrimLenIn tail all match. Left: header block registers (original: n in a2, i in v1, i*2 computed right after n*2+8 and added to arg0 later); arg4 used directly gets the late i*2 but loads arg4 in the prologue. */
#include "ovl_0b.h"

typedef struct {
    u16 unk0;
    u16 unk2;
    u16 unk4;
    u8 unk6;
    u8 unk7;
    u16 unk8[1];
} Unk800D1D74Hdr;

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
} Unk800D1FFCCtx;

void func_800D40CC(u8 *, Unk800D1FFCCtx *, u8 *, s32 *);
void func_800D4228(u8 *, Unk800D1FFCCtx *, u8 *, s32 *);
void func_800D437C(u8 *, Unk800D1FFCCtx *, u8 *, s32 *);
DR_TPAGE *func_800D090C(s32 arg0, s32 arg1);

void func_800D1FFC(u8 *arg0, SVECTOR *arg1, SVECTOR *arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    Unk800D1D74Hdr *h = (Unk800D1D74Hdr *)arg0;
    VECTOR s;
    SVECTOR r;
    MATRIX m;
    Unk800D1FFCCtx c;
    s32 z;
    s32 n;
    u16 *p;
    u16 *o;
    u8 *prim;
    s32 i;
    s32 k;
    s32 mode;
    s32 size;
    DR_TPAGE *tp;

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
    o = &h->unk8[i];
    p = (u16 *)(arg0 + n);
    k = *p++;
    c.unk0 = p;
    c.unk4 = p + k + 1;
    c.unk14 = h->unk4;
    mode = h->unk2;
    c.unkC = h->unk6 - 1;
    c.unk8 = h->unk7 - 1;
    z = *(u16 *)&h->unk6;
    n = *o;
    c.unk1C = arg0[n++];
    arg0 += n;
    gte_SetRotMatrix(D_800F78A4->unk14);
    gte_SetTransMatrix(D_800F78A4->unk14);
    gte_ldv0(arg1);
    if (mode == 0) {
        size = 0xC;
        z = 0x68;
    } else if (z == 0x808) {
        size = 0x10;
        z = 0x74;
    } else if (z == 0x1010) {
        size = 0x10;
        z = 0x7C;
    } else {
        size = 0x14;
        z = 0x64;
    }
    prim = D_800F78A4->unk24;
    D_800F78A4->unk24 = prim + size * c.unk1C;
    gte_rtv0tr_nn();
    func_800316D8(arg2, &m);
    if (arg3 >= 0) {
        s.vz = arg3;
        s.vy = arg3;
        s.vx = arg3;
        ScaleMatrix(&m, &s);
    }
    gte_stlvnl(m.t);
    if (arg5 != 0xFF) {
        z |= 2;
    }
    gte_rtps_nn();
    c.unk24 = z;
    tp = func_800D090C(mode, arg5);
    gte_stszotz(&z);
    if (arg2->pad != 0) {
        MulRotMatrix(&m);
    }
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    c.unk18 = arg6;
    z = (z - D_800F78A4->unk28) >> 2;
    if ((u32)z < 0x1000 && c.unk1C != 0) {
        if (size == 0x14) {
            func_800D40CC(arg0, &c, prim, &D_800F78A4->unk20[z]);
        } else if (size == 0xC) {
            func_800D437C(arg0, &c, prim, &D_800F78A4->unk20[z]);
        } else {
            func_800D4228(arg0, &c, prim, &D_800F78A4->unk20[z]);
        }
        addPrimLenIn(&D_800F78A4->unk20[z], tp, 0x1000000);
    }
}
