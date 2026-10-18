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

void func_801E9D80(SVECTOR *pos, SVECTOR *rot, s32 n, s32 tpage, s32 clut, s32 abr) {
    SVECTOR v[4];
    SVECTOR t;
    MATRIX m;
    Fx0b8UV uv0;
    Fx0b8UV uv1;
    Fx0b8UV uv2;
    Fx0b8UV uv3;
    CVECTOR c0;
    CVECTOR c1;
    Unk800F78A4 *env;
    BtlfxExports *ex;
    POLY_GT4 *p;
    s32 i;
    s32 j;
    s32 k0;
    s32 k1;

    env = D_801EA7A0;
    ex = BTLFX_EXPORTS(env);
    uv0.u = uv2.u = 0;
    uv1.u = uv3.u = 0x5F;
    uv0.v = uv1.v = 0;
    uv2.v = uv3.v = 0x5F;
    v[0].vy = v[1].vy = v[2].vy = v[3].vy = 0;
    gte_SetRotMatrix(env->unk14);
    gte_SetTransMatrix(env->unk14);
    gte_ldv0(pos);
    tpage |= (abr & 3) << 5;
    p = (POLY_GT4 *)env->unk24;
    env->unk24 = (u8 *)(p + 32);
    gte_rtv0tr_b();
    ex->func_800316D8(rot, &m);
    gte_stlvnl(m.t);
    ex->MulRotMatrix(&m);
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    n /= 8;
    k0 = 0;
    k1 = n;
    for (j = 0; j < 8; j++) {
        t.vx = D_801EA680[j].vx;
        t.vy = D_801EA680[j].vy;
        t.vz = D_801EA680[j].vz;
        c0.r = (t.vx * k0) >> 12;
        c0.g = (t.vy * k0) >> 12;
        c0.b = (t.vz * k0) >> 12;
        t.vx = D_801EA680[j + 1].vx;
        t.vy = D_801EA680[j + 1].vy;
        t.vz = D_801EA680[j + 1].vz;
        c1.r = (t.vx * k1) >> 12;
        c1.g = (t.vy * k1) >> 12;
        c1.b = (t.vz * k1) >> 12;
        k0 += n;
        k1 += n;
        v[0].vz = v[1].vz = j * 0xA00 - 0x2800;
        v[2].vz = v[3].vz = j * 0xA00 - 0x1E00;
        for (i = 0; i < 4; i++) {
            v[0].vx = v[2].vx = i * 0x1400 - 0x2800;
            v[1].vx = v[3].vx = i * 0x1400 - 0x1400;
            gte_ldv3c(v);
            gte_rtpt();
            p->code = (abr != 0xFF) ? 0x3E : 0x3C;
            p->r0 = p->r1 = c0.r;
            p->g0 = p->g1 = c0.g;
            p->b0 = p->b1 = c0.b;
            p->r2 = p->r3 = c1.r;
            p->g2 = p->g3 = c1.g;
            p->b2 = p->b3 = c1.b;
            gte_stsxy3(&p->x0, &p->x1, &p->x2);
            gte_ldv0(&v[3]);
            gte_rtps();
            *(u16 *)&p->u0 = *(u16 *)&uv0;
            *(u16 *)&p->u1 = *(u16 *)&uv1;
            *(u16 *)&p->u2 = *(u16 *)&uv2;
            *(u16 *)&p->u3 = *(u16 *)&uv3;
            p->clut = clut;
            p->tpage = tpage;
            gte_stsxy(&p->x3);
            ex->btlfx_addPrim(0x0C000000, 0, p);
            p++;
        }
    }
}

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
