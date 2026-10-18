#include "fxmod/fx_07a.h"

s32 func_801E77A8(s32 curve, s32 n, s32 t, SVECTOR *from, SVECTOR *to, SVECTOR *out) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E94F4);
    s32 k = 0x20000000 / (n * n);
    s32 r = 0;

    switch (curve) {
    case 0:
        r = ((0x10000000 / n) * t) >> 16;
        break;
    case 1:
        r = 0x1000 - ((k * (n - t) * (n - t)) >> 17);
        break;
    case 2:
        r = (k * t * t) >> 17;
        break;
    }
    return ex->btlfx_lerpVec((s16 *)from, (s16 *)to, 0x1000 - r, (s16 *)out);
}

s32 func_801E78E4(s32 curve, s32 n, s32 t, s32 from, s32 to) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E94F4);
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

INCLUDE_RODATA("asm/us/fx_07a/nonmatchings/fx_07a", fx_07a_headerPtr);

INCLUDE_ASM("asm/us/fx_07a/nonmatchings/fx_07a", fx_07a_op80);

void func_801E8A88(SVECTOR *pos, s32 ot, s32 tpage, s32 clut, s32 abr) {
    Unk800F78A4 *env = D_801E94F4;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    u8 uv[4][8];
    SVECTOR p[4];
    SVECTOR rot;
    MATRIX m;
    s32 dr;
    s32 dk;
    POLY_FT4 *ft4;
    POLY_G4 *g4;
    s32 a;
    s32 j;
    s32 r;
    s32 k;
    s32 b;

    p[3].vy = 0;
    p[2].vy = 0;
    p[1].vy = 0;
    p[0].vy = 0;
    rot.vz = 0;
    rot.vy = 0;
    rot.vx = 0;
    gte_SetRotMatrix(env->unk14);
    gte_SetTransMatrix(env->unk14);
    gte_ldv0(pos);
    ft4 = (POLY_FT4 *)env->unk24;
    tpage |= (abr & 3) << 5;
    g4 = (POLY_G4 *)(ft4 + 64);
    env->unk24 = (u8 *)(g4 + 32);
    gte_rtv0tr_b();
    ex->func_800316D8(&rot, &m);
    gte_stlvnl(m.t);
    ex->MulRotMatrix(&m);
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    a = 0;
    dr = D_801E94F8 >> 2;
    dk = (D_801E9500 - D_801E94FC) >> 2;
    for (; a < 0x1000; a += 0x100) {
        r = 0;
        k = 0;
        for (j = 0; j < 4; j++) {
            b = a + D_801E9514[j];
            uv[0][0] = ((ex->math_cos(a) * (D_801E94FC + k)) >> 16) + 0x38;
            uv[0][1] = ((ex->math_sin(a) * (D_801E94FC + k)) >> 16) - 0x48;
            uv[1][0] = ((ex->math_cos(a + 0x100) * (D_801E94FC + k)) >> 16) + 0x38;
            uv[1][1] = ((ex->math_sin(a + 0x100) * (D_801E94FC + k)) >> 16) - 0x48;
            p[0].vx = (ex->math_cos(b) * r) >> 16;
            p[0].vz = -((ex->math_sin(b) * r) >> 16) - 0x20;
            b += 0x100;
            p[1].vx = (ex->math_cos(b) * r) >> 16;
            p[1].vz = -((ex->math_sin(b) * r) >> 16) - 0x20;
            b = a + D_801E9514[j + 1];
            k += dk;
            uv[2][0] = ((ex->math_cos(a) * (D_801E94FC + k)) >> 16) + 0x38;
            uv[2][1] = ((ex->math_sin(a) * (D_801E94FC + k)) >> 16) - 0x48;
            uv[3][0] = ((ex->math_cos(a + 0x100) * (D_801E94FC + k)) >> 16) + 0x38;
            r += dr;
            uv[3][1] = ((ex->math_sin(a + 0x100) * (D_801E94FC + k)) >> 16) - 0x48;
            p[2].vx = (ex->math_cos(b) * r) >> 16;
            p[2].vz = -((ex->math_sin(b) * r) >> 16) - 0x20;
            b += 0x100;
            p[3].vx = (ex->math_cos(b) * r) >> 16;
            p[3].vz = -((ex->math_sin(b) * r) >> 16) - 0x20;
            gte_ldv3c(p);
            gte_rtpt();
            ft4->b0 = 0x40;
            ft4->g0 = 0x40;
            ft4->r0 = 0x40;
            ft4->code = abr == 0xFF ? 0x2C : 0x2E;
            *(u16 *)&ft4->u0 = *(u16 *)uv[0];
            *(u16 *)&ft4->u1 = *(u16 *)uv[1];
            *(u16 *)&ft4->u2 = *(u16 *)uv[2];
            *(u16 *)&ft4->u3 = *(u16 *)uv[3];
            gte_stsxy3(&ft4->x0, &ft4->x1, &ft4->x2);
            gte_ldv0(&p[3]);
            gte_rtps();
            ft4->clut = clut;
            ft4->tpage = tpage;
            gte_stsxy(&ft4->x3);
            ex->btlfx_addPrim(0x09000000, ot * 4, ft4);
            ft4++;
        }
        for (j = 0; j < 2; j++) {
            if (j != 0) {
                p[2].vx = (ex->math_cos(a) * D_801E9508) >> 16;
                p[2].vz = -((ex->math_sin(a) * D_801E9508) >> 16) - 0x20;
                p[3].vx = (ex->math_cos(a + 0x100) * D_801E9508) >> 16;
                p[3].vz = -((ex->math_sin(a + 0x100) * D_801E9508) >> 16) - 0x20;
                p[0].vx = (ex->math_cos(a) * D_801E94F8) >> 16;
                p[0].vz = -((ex->math_sin(a) * D_801E94F8) >> 16) - 0x20;
                p[1].vx = (ex->math_cos(a + 0x100) * D_801E94F8) >> 16;
                p[1].vz = -((ex->math_sin(a + 0x100) * D_801E94F8) >> 16) - 0x20;
                g4->b1 = 0x80;
                g4->g1 = 0x80;
                g4->r1 = 0x80;
                g4->b0 = 0x80;
                g4->g0 = 0x80;
                g4->r0 = 0x80;
                g4->b3 = 0;
                g4->g3 = 0;
                g4->r3 = 0;
                g4->b2 = 0;
                g4->g2 = 0;
                g4->r2 = 0;
            } else {
                p[2].vx = (ex->math_cos(a) * D_801E94F8) >> 16;
                p[2].vz = -((ex->math_sin(a) * D_801E94F8) >> 16) - 0x20;
                p[3].vx = (ex->math_cos(a + 0x100) * D_801E94F8) >> 16;
                p[3].vz = -((ex->math_sin(a + 0x100) * D_801E94F8) >> 16) - 0x20;
                p[0].vx = (ex->math_cos(a) * D_801E950C) >> 12;
                p[0].vz = -((ex->math_sin(a) * D_801E950C) >> 12) - 0x20;
                p[1].vx = (ex->math_cos(a + 0x100) * D_801E950C) >> 12;
                p[1].vz = -((ex->math_sin(a + 0x100) * D_801E950C) >> 12) - 0x20;
                g4->b1 = 0;
                g4->g1 = 0;
                g4->r1 = 0;
                g4->b0 = 0;
                g4->g0 = 0;
                g4->r0 = 0;
                g4->b3 = 0xFF;
                g4->g3 = 0xFF;
                g4->r3 = 0xFF;
                g4->b2 = 0xFF;
                g4->g2 = 0xFF;
                g4->r2 = 0xFF;
            }
            gte_ldv3c(p);
            gte_rtpt();
            g4->code = 0x3A;
            gte_stsxy3(&g4->x0, &g4->x1, &g4->x2);
            gte_ldv0(&p[3]);
            gte_rtps();
            gte_stsxy(&g4->x3);
            ex->btlfx_addPrim(0x08000000, ot * 4 - 4, g4);
            g4++;
        }
    }
    ex->btlfx_addPrim(0x01000000, ot * 4 - 4, ex->btlfx_allocDrMode(0, 2));
}
