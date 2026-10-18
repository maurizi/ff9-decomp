#include "fxmod/fx_0b8.h"

s32 func_801E7704(s32 curve, s32 n, s32 t, s32 from, s32 to) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EA7A0);
    s32 k = 0x20000000 / (n * n);
    s32 r = 0;

    switch (curve) {
    case 0:
        r = 0x1000 - (((0x10000000 / n) * t) >> 16);
        break;
    case 1:
        r = (k * (n - t) * (n - t)) >> 17;
        break;
    case 2:
        r = 0x1000 - ((k * t * t) >> 17);
        break;
    }
    return ex->btlfx_lerp(from, to, r);
}

INCLUDE_ASM("asm/us/fx_0b8/nonmatchings/fx_0b8", fx_0b8_op80);

INCLUDE_ASM("asm/us/fx_0b8/nonmatchings/fx_0b8", func_801E9D80);

INCLUDE_ASM("asm/us/fx_0b8/nonmatchings/fx_0b8", func_801EA1E0);

void func_801EA424(s32 ot) {
    Unk800F78A4 *env = D_801EA7A0;
    POLY_G4 *p = (POLY_G4 *)env->unk24;
    BtlfxExports *ex;

    env->unk24 = (u8 *)(p + 1);
    ex = BTLFX_EXPORTS(env);
    p->code = 0x38;
    p->r0 = p->r1 = 0xE6;
    p->g0 = p->g1 = 0xCD;
    p->b0 = p->b1 = 0x96;
    p->r2 = p->r3 = 0xB4;
    p->g2 = p->g3 = 0xA0;
    p->b2 = p->b3 = 0x78;
    p->x1 = p->x3 = 320;
    p->x0 = p->x2 = 0;
    p->y0 = p->y1 = 0;
    p->y2 = p->y3 = 240;
    ex->btlfx_addPrim(0x08000000, ot * 4, p);
}

void func_801EA4E0(s32 a, s32 b, s32 t, s32 n, s32 mode, s32 ot) {
    Unk800F78A4 *env = D_801EA7A0;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 v = ex->btlfx_lerpRatio(a, b, t, n);

    ex->btlfx_drawScreenTint((u32 *)(env->unk20 + ot), mode, v, v, v);
}

s32 func_801EA558(Pool30Entry *pt, s32 a1, SVECTOR *pos, s32 a3, s32 frame, s32 n) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EA7A0);
    SVECTOR v;
    s32 r;
    SVECTOR off;

    SVECTOR *a = &D_801EA7B0[(pt->unk1F - 1) & 0x1F];

    off.vx = 0;
    off.vy = -0xC00;
    off.vz = 0x418;
    ex->btlfx_lerpVec((s16 *)a, (s16 *)&off,
                                             ((0x1000000 / n) * frame) >> 12, (s16 *)&v);
    pos->vx = v.vx;
    pos->vy = v.vy;
    pos->vz = v.vz;
    pt->unk0.vx = v.vx << 12;
    pt->unk0.vy = v.vy << 12;
    pt->unk0.vz = v.vz << 12;
    if (frame >= n) {
        return -1;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0b8/nonmatchings/fx_0b8", fx_0b8_headerPtr);
