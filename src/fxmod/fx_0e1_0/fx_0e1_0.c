#include "fxmod/fx_0e1_0.h"

INCLUDE_ASM("asm/us/fx_0e1_0/nonmatchings/fx_0e1_0", func_801E7718);

void func_801E7CB4(SVECTOR *a, SVECTOR *b, s32 n, s32 tpage, s32 clut, s32 u, s32 v, s32 w, s32 h, s32 p,
                   Fx0e10Seg *buf, s32 abr, s32 phase) {
    Unk800F78A4 *env = D_801E9E38;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    CVECTOR c;
    s32 otz;
    POLY_GT4 *prim;
    Fx0e10Seg *s;
    s32 i;
    s32 t;
    s32 k;
    s32 j;

    if (p < 0) {
        p = 0x1000;
    }
    if (n > 1000) {
        n -= 999;
        for (i = 0; i < n; i++) {
            buf[i].unk0.pad = 0;
        }
        return;
    }
    s = &buf[(u32)(n - 1)]; /* (n - 1) * 16, not n * 16 - 16 */
    for (i = 0; i < n; i++) {
        BTLFX_COPY_SVEC(&s[1].unk0, &s[0].unk0);
        BTLFX_COPY_SVEC(&s[1].unk8, &s[0].unk8);
        s--;
    }
    s = buf;
    for (i = 0; i < n; i++, s++) {
        if (!(s[0].unk0.pad & s[1].unk0.pad)) {
            break;
        }
    }
    BTLFX_COPY_SVEC(&buf->unk0, a);
    buf->unk0.pad = 1;
    BTLFX_COPY_SVEC(&buf->unk8, b);
    n = i;
    prim = (POLY_GT4 *)env->unk24;
    env->unk24 = (u8 *)(prim + n);
    c.cd = abr == 0xFF ? 0x3C : 0x3E;
    w--;
    h--;
    tpage |= (abr & 3) << 5;
    gte_SetRotMatrix(env->unk14);
    gte_SetTransMatrix(env->unk14);
    /* the last uv store (u1) is the address giv loop.c bases the others on, as in the original's s0 = &prim->u1 */
    /* i = j in the increment: i + 1 lives across the call in its own register and is copied back */
    for (i = 0; i < n; i = j, buf++, prim++, phase++) {
        gte_ldv3c(buf);
        t = (i << 12) / n;
        c.r = c.g = c.b = p;
        gte_rtpt_b();
        t = ((0x1000 - t) * p) >> 12;
        c.r = c.g = c.b = t;
        *(s32 *)&prim->r0 = *(s32 *)&prim->r1 = *(s32 *)&c;
        c.r = c.g = c.b = t;
        *(s32 *)&prim->r2 = *(s32 *)&prim->r3 = *(s32 *)&c;
        j = i + 1;
        gte_stopz(&otz);
        if (otz != 0) {
            gte_stsxy3(&prim->x0, &prim->x1, &prim->x2);
            gte_avsz3();
            gte_stszotz(&otz);
            gte_ldv0(&buf[1].unk8);
            gte_rtps();
            ex->btlfx_addPrim(0x0C000000, otz, prim);
            gte_stsxy(&prim->x3);
            prim->clut = clut;
            prim->tpage = tpage;
            prim->u0 = prim->u2 = u;
            k = phase & 0x1F;
            prim->v0 = prim->v1 = v + ((k * h) >> 5);
            prim->v2 = prim->v3 = v + (((k + 1) * h) >> 5);
            prim->u1 = prim->u3 = u + w;
        }
    }
}

void func_801E80A8(s32 ot, s32 x, s32 y, s32 nx, s32 ny, s32 dx, s32 dy, s32 abr, s32 sy, s32 tpage, s32 clut, s32 u,
                   s32 v, s32 w, s32 h, s32 c, s32 amp, s32 ph, s32 dphx, s32 dphy) {
    Unk800F78A4 *env = D_801E9E38;
    BtlfxExports *ex;
    POLY_FT4 *ft4;
    POLY_GT4 *gt4;
    s32 i;
    s32 j;
    s32 px;
    s32 yo;
    s32 u0;
    s32 v0;
    CVECTOR col;
    s32 u1;
    s32 v1;
    s32 k;
    s32 p;

    yo = 0;
    j = 0;
    tpage |= (abr & 3) << 5;
    ft4 = (POLY_FT4 *)env->unk24;
    gt4 = (POLY_GT4 *)(ft4 + nx * (ny - 1));
    ex = BTLFX_EXPORTS(env);
    env->unk24 = (u8 *)(gt4 + nx);
    col.r = col.g = col.b = c;
    for (; j < ny; j++, yo -= dy) {
        px = x;
        p = ph + j * dphy;
        for (i = 0; i < nx; i++, p += dphx, px += dx) {
            k = i & 3;
            u0 = u + ((k * w) >> 2);
            u1 = u0 + (w >> 2);
            if (k == 3) {
                u1--;
            }
            k = j & 3;
            v0 = v + ((k * h) >> 2);
            v1 = v0 + (h >> 2);
            if (k == 3) {
                v1--;
            }
            if (j < ny - 1) {
                ft4->x0 = px + ((ex->math_sin(p) * amp) >> 12);
                ft4->x2 = px + ((ex->math_sin(p + dphy) * amp) >> 12);
                ft4->x1 = (px + dx) + ((ex->math_sin(p + dphx) * amp) >> 12);
                ft4->x3 = (px + dx) + ((ex->math_sin(p + dphy + dphx) * amp) >> 12);
                if (j == 0) {
                    ft4->y1 = y;
                    ft4->y0 = y;
                } else {
                    ft4->y0 = y + (((yo + ((ex->math_sin(p) * amp) >> 12)) * sy) >> 12);
                    ft4->y1 = y + (((yo + ((ex->math_sin(p + dphx) * amp) >> 12)) * sy) >> 12);
                }
                p += dphy;
                ft4->y2 = y + ((((yo - dy) + ((ex->math_sin(p) * amp) >> 12)) * sy) >> 12);
                ft4->y3 = y + ((((yo - dy) + ((ex->math_sin(p + dphx) * amp) >> 12)) * sy) >> 12);
                p -= dphy;
                ft4->tpage = tpage;
                ft4->clut = clut;
                ft4->u0 = ft4->u2 = u0;
                ft4->u1 = ft4->u3 = u1;
                ft4->v0 = ft4->v1 = v0;
                ft4->v2 = ft4->v3 = v1;
                col.cd = abr == 0xFF ? 0x2C : 0x2E;
                *(s32 *)&ft4->r0 = *(s32 *)&col;
                ex->btlfx_addPrim(0x09000000, ot, ft4);
                ft4++;
            } else {
                gt4->x0 = px + ((ex->math_sin(p) * amp) >> 12);
                gt4->x2 = px + ((ex->math_sin(p + dphy) * amp) >> 12);
                gt4->x1 = (px + dx) + ((ex->math_sin(p + dphx) * amp) >> 12);
                gt4->x3 = (px + dx) + ((ex->math_sin(p + dphy + dphx) * amp) >> 12);
                if (j == 0) {
                    gt4->y1 = y;
                    gt4->y0 = y;
                } else {
                    gt4->y0 = y + (((yo + ((ex->math_sin(p) * amp) >> 12)) * sy) >> 12);
                    gt4->y1 = y + (((yo + ((ex->math_sin(p + dphx) * amp) >> 12)) * sy) >> 12);
                }
                p += dphy;
                gt4->y2 = y + ((((yo - dy) + ((ex->math_sin(p) * amp) >> 12)) * sy) >> 12);
                gt4->y3 = y + ((((yo - dy) + ((ex->math_sin(p + dphx) * amp) >> 12)) * sy) >> 12);
                p -= dphy;
                gt4->tpage = tpage;
                gt4->clut = clut;
                gt4->u0 = gt4->u2 = u0;
                gt4->u1 = gt4->u3 = u1;
                gt4->v0 = gt4->v1 = v0;
                gt4->v2 = gt4->v3 = v1;
                col.cd = abr == 0xFF ? 0x3C : 0x3E;
                *(s32 *)&gt4->r0 = *(s32 *)&gt4->r1 = *(s32 *)&col;
                *(s32 *)&gt4->r2 = *(s32 *)&gt4->r3 = 0;
                ex->btlfx_addPrim(0x0C000000, ot, gt4);
                gt4++;
            }
        }
    }
}

s32 func_801E8844(SVECTOR *out) {
    Unk800F78A4 *env = D_801E9E38;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 n;

    if (D_801E9DBC != 0) {
        out->pad = 1;
        n = D_801E9DB8;
        ex->Hi_GetSummonBonePos(((Fx0e10EnvB *)env->unk4)->unk60, D_801E9D84[n++], (u16 *)out);
        if (n >= 5) {
            n = 0;
        }
        D_801E9DB8 = n;
    } else {
        out->vx = 10000;
        out->vy = 10000;
        out->vz = 10000;
        out->pad = 1;
    }
    return 0;
}

void func_801E88F0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s16 *v) {
    v[0] = (v[0] * 5) >> 2;
    v[1] = (v[1] * 5) >> 2;
}

INCLUDE_RODATA("asm/us/fx_0e1_0/nonmatchings/fx_0e1_0", fx_0e1_0_headerPtr);

INCLUDE_ASM("asm/us/fx_0e1_0/nonmatchings/fx_0e1_0", fx_0e1_0_op80);
