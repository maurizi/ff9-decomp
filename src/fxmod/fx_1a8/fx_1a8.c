#include "fxmod/fx_1a8.h"

void func_801E7704(s32 arg0, s32 arg1, SVECTOR *pos, SVECTOR *rot, s32 *scale, s32 arg5, s32 arg6, s32 t, s32 n) {
    s32 r = (t << 12) / n;
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E97CC);
    VECTOR p;
    VECTOR s;
    s32 a;
    s32 c;

    s.vx = s.vy = s.vz = *scale >> 3;
    if (r >= 0xC00) {
        a = 0x1000 - (r - 0xC00) * 4;
    } else {
        a = 0x1000;
    }
    p.vx = pos->vx;
    p.vy = pos->vy;
    p.vz = pos->vz;
    ex->Hi_DrawEffModel(rot, &p, (s32)&s, D_801E97D4[0]);
    if (a < 0x1000) {
        ex->Hi_ModifyEffModelAbr(D_801E97D4[0], 1);
        c = (a >> 5) - 0x80;
        ex->Hi_ModifyEffModelRGB(D_801E97D4[0], c, c, c);
    }
}

void func_801E7828(s32 arg0, s32 arg1, SVECTOR *pos, SVECTOR *rot, s32 *scale, s32 arg5, s32 arg6, s32 t, s32 n) {
    s32 r = (t << 12) / n;
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E97CC);
    VECTOR p;
    VECTOR s;
    s32 a;
    s32 c;

    s.vx = s.vy = s.vz = *scale >> 3;
    if (r >= 0xC00) {
        a = 0x1000 - (r - 0xC00) * 4;
    } else {
        a = 0x1000;
    }
    p.vx = pos->vx;
    p.vy = pos->vy;
    p.vz = pos->vz;
    ex->Hi_DrawEffModel(rot, &p, (s32)&s, D_801E97D4[1]);
    if (a < 0x1000) {
        ex->Hi_ModifyEffModelAbr(D_801E97D4[1], 1);
        c = (a >> 5) - 0x80;
        ex->Hi_ModifyEffModelRGB(D_801E97D4[1], c, c, c);
    }
}

void func_801E7950(SVECTOR *pos, SVECTOR *rot, VECTOR *scale, s32 r0, s32 r1, s32 z0, s32 z1, s32 col, s32 tpage,
                   s32 clut, s32 u, s32 v, s32 ang, s32 h, s32 abr, s32 step) {
    Unk800F78A4 *env = D_801E97CC;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    CVECTOR c0;
    CVECTOR c1;
    CVECTOR c2;
    MATRIX m;
    SVECTOR vs[4];
    s32 otz;
    POLY_GT4 *g;
    POLY_FT4 *f;
    s32 n;
    s32 j;
    s32 row;
    s32 rh;
    s32 hh;
    s32 u0;
    s32 v0;
    s32 u1;
    s32 v1;
    s32 ya;
    s32 yb;
    s32 ta;
    s32 tb;
    s32 sa;
    s32 sb;
    s32 ra;
    s32 rb;
    s32 a;
    s32 ft;
    s32 k;

    gte_SetRotMatrix(env->unk14);
    gte_SetTransMatrix(env->unk14);
    gte_ldv0(pos);
    g = (POLY_GT4 *)env->unk24;
    *(s32 *)&c2 = 0;
    f = (POLY_FT4 *)(g + 2);
    env->unk24 = (u8 *)(f + 14);
    c0.r = c0.g = c0.b = col;
    c1.r = c1.g = c1.b = col;
    if (abr != 0xFF) {
        c0.cd = 0x2E;
    } else {
        c0.cd = 0x2C;
    }
    c2.cd = abr != 0xFF ? 0x3E : 0x3C;
    tpage |= (abr & 3) << 5;
    c1.cd = c2.cd;
    gte_rtv0tr_b();
    ex->func_800316D8(rot, &m);
    ex->ScaleMatrix(&m, scale);
    gte_stlvnl(m.t);
    ex->MulRotMatrix(&m);
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    z0 >>= 1;
    z1 >>= 1;
    hh = h;
    u0 = u;
    u1 = u + 0x7F;
    v0 = v;
    v1 = v + 0x3F;
    n = 0;
    for (rh = 0, row = 0; row < 1; row++, rh += h) {
        for (j = 0; j < 16; j++, n++, ang += step << 8) {
            k = (j + 1) << 8;
            ya = rh + (((j << 8) * h) >> 12);
            yb = rh + ((k * h) >> 12);
            ta = ((ya + (hh >> 1)) << 12) / hh;
            tb = ((yb + (hh >> 1)) << 12) / hh;
            sa = ex->math_sin(ta >> 1);
            sb = ex->math_sin(tb >> 1);
            ra = ex->btlfx_lerp(r0, r1, sa);
            rb = ex->btlfx_lerp(r0, r1, sb);
            vs[0].vx = vs[1].vx = (ex->math_cos(ang) * ra) >> 12;
            vs[0].vy = vs[1].vy = (ex->math_sin(ang) * ra) >> 12;
            vs[0].vz = ya - ex->btlfx_lerp(z0, z1, sa);
            vs[1].vz = ya + ex->btlfx_lerp(z0, z1, sb);
            a = ang + (step << 8);
            vs[2].vx = vs[3].vx = (ex->math_cos(a) * rb) >> 12;
            vs[2].vy = vs[3].vy = (ex->math_sin(a) * rb) >> 12;
            vs[2].vz = yb - ex->btlfx_lerp(z0, z1, sa);
            vs[3].vz = yb + ex->btlfx_lerp(z0, z1, sb);
            gte_ldv3c(vs);
            gte_rtpt();
            gte_nclip();
            if (n == 0) {
                *(s32 *)&g->r0 = *(s32 *)&g->r1 = *(s32 *)&c2;
                *(s32 *)&g->r2 = *(s32 *)&g->r3 = *(s32 *)&c1;
                ft = 0;
            } else if (n >= 15) {
                *(s32 *)&g->r2 = *(s32 *)&g->r3 = *(s32 *)&c2;
                *(s32 *)&g->r0 = *(s32 *)&g->r1 = *(s32 *)&c1;
                ft = 0;
            } else {
                ft = 1;
                *(s32 *)&f->r0 = *(s32 *)&c0;
            }
            gte_stopz(&otz);
            if (otz != 0) {
                if (ft) {
                    f->u0 = f->u1 = u0;
                    f->u2 = f->u3 = u1;
                    f->v0 = f->v2 = v0;
                    f->v1 = f->v3 = v1;
                    f->tpage = tpage;
                    f->clut = clut;
                    gte_stsxy3(&f->x0, &f->x1, &f->x2);
                } else {
                    g->u0 = g->u1 = u0;
                    g->u2 = g->u3 = u1;
                    g->v0 = g->v2 = v0;
                    g->v1 = g->v3 = v1;
                    g->tpage = tpage;
                    g->clut = clut;
                    gte_stsxy3(&g->x0, &g->x1, &g->x2);
                }
                gte_avsz3();
                gte_stszotz(&otz);
                gte_ldv0(&vs[3]);
                gte_rtps();
                if (ft) {
                    ex->btlfx_addPrim(0x09000000, otz, f);
                    gte_stsxy(&f->x3);
                    f++;
                } else {
                    ex->btlfx_addPrim(0x0C000000, otz, g);
                    gte_stsxy(&g->x3);
                    g++;
                }
            }
        }
    }
}

INCLUDE_ASM("asm/us/fx_1a8/nonmatchings/fx_1a8", fx_1a8_op80);

INCLUDE_RODATA("asm/us/fx_1a8/nonmatchings/fx_1a8", fx_1a8_headerPtr);
