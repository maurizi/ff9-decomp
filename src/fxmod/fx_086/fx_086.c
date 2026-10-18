#include "fxmod/fx_086.h"

void func_801E7704(SVECTOR *pos, SVECTOR *rot, VECTOR *scale, s32 r0, s32 r1, s32 w0, s32 w1, s32 c, s32 tpage,
                   s32 clut, s32 u, s32 v, s32 ang, s32 dy, s32 abr, s32 dang) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E9D6C);
    Fx086Rgbc c0;
    Fx086Rgbc c1;
    Fx086Rgbc c2;
    MATRIX m;
    SVECTOR vv[4];
    s32 z;
    POLY_GT4 *gt4;
    POLY_FT4 *ft4;
    s32 k;
    s32 j;
    s32 row;
    s32 h;
    s32 d;
    s32 y0;
    s32 y1;
    s32 s0;
    s32 s1;
    s32 ra;
    s32 rb;
    s32 a;
    s32 ft;
    s32 t;
    s32 u0;
    s32 u1;
    s32 v0;
    s32 v1;
    s32 jn;
    s32 hd;
    s32 q;

    gte_SetRotMatrix(D_801E9D6C->unk14);
    gte_SetTransMatrix(D_801E9D6C->unk14);
    gte_ldv0(pos);
    gt4 = (POLY_GT4 *)D_801E9D6C->unk24;
    *(s32 *)&c2 = 0;
    ft4 = (POLY_FT4 *)(gt4 + 2);
    D_801E9D6C->unk24 = (u8 *)(ft4 + 14);
    c0.r = c0.g = c0.b = c;
    c1.r = c1.g = c1.b = c;
    c0.code = abr != 0xFF ? 0x2E : 0x2C;
    c2.code = abr != 0xFF ? 0x3E : 0x3C;
    tpage |= (abr & 3) << 5;
    c1.code = c2.code;
    gte_rtv0tr_b();
    ex->func_800316D8(rot, &m);
    ex->ScaleMatrix(&m, scale);
    gte_stlvnl(m.t);
    ex->MulRotMatrix(&m);
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    k = 0;
    h = 0;
    w0 >>= 1;
    w1 >>= 1;
    d = dy; /* a copy: the original keeps it in its own stack slot */
    for (row = 0; row < 1; row++, h += dy) {
        for (j = 0; j < 16; j++, k++, ang += dang << 8) {
            jn = (j + 1) << 8;
            y0 = h + (((j << 8) * dy) >> 12);
            y1 = h + ((jn * dy) >> 12);
            /* one variable for d / 2 and the second quotient: the original recomputes d >> 1 every iteration */
            hd = d >> 1;
            q = ((y0 + hd) << 12) / d;
            hd = ((y1 + hd) << 12) / d;
            s0 = ex->math_sin(q >> 1);
            s1 = ex->math_sin(hd >> 1);
            ra = ex->btlfx_lerp(r0, r1, s0);
            rb = ex->btlfx_lerp(r0, r1, s1);
            vv[0].vx = vv[1].vx = (ex->math_cos(ang) * ra) >> 12;
            vv[0].vy = vv[1].vy = (ex->math_sin(ang) * ra) >> 12;
            vv[0].vz = y0 - ex->btlfx_lerp(w0, w1, s0);
            vv[1].vz = y0 + ex->btlfx_lerp(w0, w1, s1);
            a = ang + (dang << 8);
            vv[2].vx = vv[3].vx = (ex->math_cos(a) * rb) >> 12;
            vv[2].vy = vv[3].vy = (ex->math_sin(a) * rb) >> 12;
            vv[2].vz = y1 - ex->btlfx_lerp(w0, w1, s0);
            vv[3].vz = y1 + ex->btlfx_lerp(w0, w1, s1);
            gte_ldv3c(vv);
            gte_rtpt();
            t = k & 1;
            u0 = u + (t << 6);
            u1 = u0 + 0x40;
            if (t != 0) {
                u1 = u0 + 0x3F;
            }
            v0 = v;
            v1 = v0 + 0x3F;
            gte_nclip();
            if (k == 0) {
                ft = 0;
                *(s32 *)&gt4->r0 = *(s32 *)&gt4->r1 = *(s32 *)&c2;
                *(s32 *)&gt4->r2 = *(s32 *)&gt4->r3 = *(s32 *)&c1;
            } else if (k >= 15) {
                ft = 0;
                *(s32 *)&gt4->r2 = *(s32 *)&gt4->r3 = *(s32 *)&c2;
                *(s32 *)&gt4->r0 = *(s32 *)&gt4->r1 = *(s32 *)&c1;
            } else {
                ft = 1;
                *(s32 *)&ft4->r0 = *(s32 *)&c0;
            }
            gte_stopz(&z);
            if (z != 0) {
                if (ft) {
                    ft4->u0 = ft4->u1 = u0;
                    ft4->u2 = ft4->u3 = u1;
                    ft4->v0 = ft4->v2 = v0;
                    ft4->v1 = ft4->v3 = v1;
                    ft4->tpage = tpage;
                    ft4->clut = clut;
                    gte_stsxy3(&ft4->x0, &ft4->x1, &ft4->x2);
                } else {
                    gt4->u0 = gt4->u1 = u0;
                    gt4->u2 = gt4->u3 = u1;
                    gt4->v0 = gt4->v2 = v0;
                    gt4->v1 = gt4->v3 = v1;
                    gt4->tpage = tpage;
                    gt4->clut = clut;
                    gte_stsxy3(&gt4->x0, &gt4->x1, &gt4->x2);
                }
                gte_avsz3();
                gte_stszotz(&z);
                gte_ldv0(&vv[3]);
                gte_rtps();
                if (ft) {
                    ex->btlfx_addPrim(0x09000000, z, ft4);
                    gte_stsxy(&ft4->x3);
                    ft4++;
                } else {
                    ex->btlfx_addPrim(0x0C000000, z, gt4);
                    gte_stsxy(&gt4->x3);
                    gt4++;
                }
            }
        }
    }
}

void func_801E7E18(s32 n, s32 r, SVECTOR *pts) {
    SVECTOR *p;
    s32 *prev;
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E9D6C);
    s32 i;
    s32 rad;
    s32 a;

    p = pts;
    prev = (s32 *)&p[n];
    for (i = 0; i < n; i++, prev++, p++) {
        *prev = 0x7F7F7F7F;
        rad = ex->btlfx_randRange(r >> 2, r >> 1);
        a = ex->btlfx_rand() & 0xFFF;
        p->vx = (ex->math_cos(a) * rad) >> 12;
        p->vy = (ex->math_sin(a) * rad) >> 12;
        p->vz = ex->btlfx_rand() % 1280 - 0x100;
    }
}

void func_801E7F6C(u32 *ot, s32 n, s32 h, s32 dz, void *src, SVECTOR *pt, CVECTOR *col, s32 p, s32 abr) {
    Unk800F78A4 *env = D_801E9D6C;
    CVECTOR c;
    MATRIX m;
    s32 hsave;
    u32 otz;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    LINE_G2 *prim;
    s32 *prev;
    SVECTOR *pts;
    s32 live;
    s32 i;

    if (p < 0) {
        p = 0x1000;
    }
    prim = (LINE_G2 *)env->unk24;
    env->unk24 = (u8 *)(prim + n);
    if (col == NULL) {
        c.r = c.g = c.b = 0;
    } else {
        gte_DpqColorFar_nst(col, p);
        gte_strgb(&c);
    }
    gte_SetRotMatrix(env->unk14);
    gte_SetTransMatrix(env->unk14);
    gte_ReadGeomScreen(&hsave);
    gte_SetGeomScreen(h);
    ex->func_800316D8(src, &m);
    m.t[1] = 0;
    m.t[0] = 0;
    m.t[2] = h;
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    pts = pt;
    prev = (s32 *)&pts[n];
    for (i = 0; i < n; i++, prev++, pts++, prim++) {
        live = *prev != 0x7F7F7F7F;
        gte_ldv0(pts);
        gte_rtps();
        pts->vz -= dz;
        if (dz > 0 && pts->vz < -0x100) {
            pts->vz = 0x400;
            *prev = 0x7F7F7F7F;
        } else if (dz < 0 && pts->vz > 0x300) {
            pts->vz = -0x200;
            *prev = 0x7F7F7F7F;
        } else {
            gte_stsxy(&prim->x0);
            gte_stszotz(&otz);
            if (otz < 0x200 && live) {
                gte_DpqColorFar_nst(&c, ((0x200 - otz) << 12) >> 9);
                gte_strgb(&prim->r0);
                *(s32 *)&prim->x1 = *prev;
                *(s32 *)&prim->r1 = 0;
                prim->code = abr != 0xFF ? 0x52 : 0x50;
                ex->btlfx_linkPrimAbr(0x4000000, ot, prim, abr);
            }
            *prev = *(s32 *)&prim->x0;
        }
    }
    gte_SetGeomScreen(hsave);
}

INCLUDE_ASM("asm/us/fx_086/nonmatchings/fx_086", fx_086_op80);

INCLUDE_RODATA("asm/us/fx_086/nonmatchings/fx_086", fx_086_headerPtr);
