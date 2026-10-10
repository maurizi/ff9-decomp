/* WIP func_800D2938: 8 differing rows (temp regs v0/v1/a0 for k+0x14, arg4*4 and e). Needs the header
   prototype changed to Unk800D2938 * (see the define below). e is reused for the primitive pointer. */
#define func_800D2938 func_800D2938_hdr
#include "ovl_0b.h"
#undef func_800D2938

typedef struct {
    u16 unk0; /* frame count */
    s16 unk2;
    u16 unk4; /* gouraud flag */
    u16 unk6;
} Unk800D2938;

typedef struct {
    s32 unk0; /* primitive code */
    s32 unk4; /* ot base z */
    s32 unk8;
    s32 unkC;
    u16 *unk10;
    s32 unk14;
    s32 unk18; /* tpage */
    u8 pad1C[0x28 - 0x1C];
} Unk800D57D0Ctx;

typedef struct {
    s32 unk0; /* primitive code */
    s32 unk4; /* ot base z */
    s32 unk8;
    s32 unkC; /* tpage */
    s32 unk10;
    s32 unk14;
} Unk800D59D0Ctx;

void func_800D57D0(u8 *, Unk800D57D0Ctx *, u8 *, s32 *);
void func_800D59D0(u8 *, Unk800D59D0Ctx *, u8 *, s32 *);

void func_800D2938(Unk800D2938 *arg0, SVECTOR *arg1, SVECTOR *arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    VECTOR s;
    union {
        SVECTOR v;
        s32 w[2];
    } zero;
    MATRIX m;
    Unk800D57D0Ctx c1;
    Unk800D59D0Ctx c2;
    s32 k;
    s32 n;
    s32 t;
    s32 z;
    s32 z2;
    u16 *base;
    u16 *e;
    u8 *prim;
    Unk800D2938 *h;

    if (arg2 == NULL) {
        arg2 = &zero.v;
        zero.w[0] = 0;
        zero.w[1] = 0;
    }
    n = arg0->unk0 - 1;
    if (n < arg4) {
        arg4 = n;
    }
    k = arg0->unk6;
    if (n == 0) {
        t = 0;
    } else {
        t = arg4 * (k - 1) / n;
    }
    n++;
    z = arg0->unk2 * 4;
    if (arg0->unk4 == 0) {
        k *= 4;
    } else {
        k *= 8;
    }
    base = (u16 *)((u8 *)arg0 + (k + 0x14));
    e = base + arg4 * 2;
    s.vy = 0x1000;
    s.vz = e[0];
    s.vx = e[1];
    if (arg3 >= 0) {
        gte_lddp(arg3);
        gte_ldir(&s);
        gte_gpf12();
        gte_stir(&s);
    }
    gte_SetRotMatrix(D_800F78A4->unk14);
    gte_SetTransMatrix(D_800F78A4->unk14);
    gte_ldv0(arg1);
    gte_rtv0tr();
    func_800316D8(arg2, &m);
    ScaleMatrix(&m, &s);
    gte_stlvnl(m.t);
    if (arg2->pad != 0) {
        MulRotMatrix(&m);
    }
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    arg0++;
    if (arg0[-1].unk4 == 0) {
        c1.unk10 = base + n * 2 + arg4;
        c1.unk18 = (arg5 & 3) << 5;
        c1.unk14 = t;
        c1.unkC = z;
        c1.unk0 = (arg5 != 0xFF) ? 0x2E000000 : 0x2C000000;
        c1.unk8 = arg6;
        c1.unk4 = D_800F78A4->unk28;
        e = (u16 *)D_800F78A4->unk24;
        D_800F78A4->unk24 = (u8 *)e + 0x28;
        func_800D57D0((u8 *)arg0, &c1, (u8 *)e, D_800F78A4->unk20);
    } else {
        c2.unkC = (arg5 & 3) << 5;
        c2.unk14 = t;
        c2.unk10 = z;
        c2.unk0 = (arg5 != 0xFF) ? 0x32000000 : 0x30000000;
        c2.unk8 = arg6;
        c2.unk4 = D_800F78A4->unk28;
        e = (u16 *)D_800F78A4->unk24;
        D_800F78A4->unk24 = (u8 *)e + 0x5C;
        func_800D59D0((u8 *)arg0, &c2, (u8 *)e, D_800F78A4->unk20);
    }
}
