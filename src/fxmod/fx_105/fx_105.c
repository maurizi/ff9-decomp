#include "fxmod/fx_105.h"

void func_801E7704(SVECTOR *a, SVECTOR *b, u32 n, CVECTOR *ca, CVECTOR *cb, s32 p, Fx105Seg *buf, s32 abr) {
    Unk800F78A4 *env = D_801EAB18;
    CVECTOR c0;
    CVECTOR c1;
    s32 otz;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    POLY_G4 *prim;
    Fx105Seg *s;
    u32 i;
    s32 p0;
    s32 p1;

    if (p < 0) {
        p = 0x1000;
    }
    if (n >= 1000) {
        n -= 999;
        for (i = 0; i < n; i++) {
            buf[i].unk0.pad = 0;
        }
        return;
    }
    s = &buf[(s32)(n - 1)];
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
    prim = (POLY_G4 *)env->unk24;
    env->unk24 = (s32)(prim + n);
    if (ca == NULL) {
        c0.r = c0.g = c0.b = 0;
    } else {
        gte_DpqColorFar_nst(ca, p);
        gte_strgb(&c0);
    }
    if (cb == NULL) {
        c1.r = c1.g = c1.b = 0;
    } else {
        gte_DpqColorFar_nst(cb, p);
        gte_strgb(&c1);
    }
    gte_SetRotMatrix(env->unk14);
    gte_SetTransMatrix(env->unk14);
    c0.cd = abr != 0xFF ? 0x3A : 0x38;
    c1.cd = abr != 0xFF ? 0x3A : 0x38;
    for (i = 0; i < n; i++, buf++, prim++) {
        gte_ldv3c(buf);
        p0 = 0x1000 - (i << 12) / n;
        gte_rtpt_b();
        p1 = 0x1000 - ((i + 1) << 12) / n;
        gte_stopz(&otz);
        if (otz != 0) {
            gte_stsxy3(&prim->x0, &prim->x1, &prim->x2);
            gte_avsz3();
            gte_stszotz(&otz);
            gte_ldv0(&buf[1].unk8);
            gte_rtps();
            ex->btlfx_addPrimAbr(0x8000000, otz, prim, abr);
            gte_stsxy(&prim->x3);
            gte_DpqColorFar_nst(&c0, p0);
            gte_strgb(&prim->r0);
            gte_DpqColorFar_nst(&c1, p0);
            gte_strgb(&prim->r1);
            gte_DpqColorFar_nst(&c0, p1);
            gte_strgb(&prim->r2);
            gte_DpqColorFar_nst(&c1, p1);
            gte_strgb(&prim->r3);
        }
    }
}

void func_801E7B74(void) {
    s32 i;
    s32 m;

    D_801EAB1C = 0;
    m = -1;
    for (i = 15; i >= 0; i--) {
        D_801EAB28[i].unk4 = m;
    }
}

void func_801E7BA8(s32 start, u32 dst, u32 src, s16 *data) {
    s32 i;

    for (i = 0; i < 16; i++) {
        if (D_801EAB28[i].unk4 == -1) {
            D_801EAB28[i].unk4 = 0;
            D_801EAB28[i].unk10 = data + 1;
            D_801EAB28[i].unk0 = start;
            D_801EAB28[i].unk8 = dst;
            D_801EAB28[i].unkC = src;
            return;
        }
    }
}

INCLUDE_ASM("asm/us/fx_105/nonmatchings/fx_105", func_801E7BF8);

void func_801E7D9C(s32 z, s32 tpage, s32 y0, s32 y1, CVECTOR *rgb) {
    POLY_G4 *p = (POLY_G4 *)D_801EAB18->unk24;
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EAB18);

    D_801EAB18->unk24 = (u8 *)(p + 1);
    p->code = (tpage != 0xFF) ? 0x3A : 0x38;
    p->r0 = rgb[0].r;
    p->g0 = rgb[0].g;
    p->b0 = rgb[0].b;
    p->r1 = rgb[1].r;
    p->g1 = rgb[1].g;
    p->b1 = rgb[1].b;
    p->r2 = rgb[2].r;
    p->g2 = rgb[2].g;
    p->b2 = rgb[2].b;
    p->r3 = rgb[3].r;
    p->g3 = rgb[3].g;
    p->b3 = rgb[3].b;
    p->x2 = 0;
    p->x0 = 0;
    p->x3 = 0x140;
    p->x1 = 0x140;
    p->y1 = y0;
    p->y0 = y0;
    p->y3 = y1;
    p->y2 = y1;
    ex->btlfx_addPrim(0x08000000, z * 4, p);
    ex->btlfx_addPrim(0x01000000, z * 4, ex->btlfx_allocDrMode(0, tpage));
}

INCLUDE_ASM("asm/us/fx_105/nonmatchings/fx_105", fx_105_op80);

void func_801E9E1C(void) {
    Unk800F78A4 *env = D_801EAB18;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx105Spark *p;
    s32 t;
    s32 i;

    for (i = 0; i < 0x20; i++) {
        t = ex->btlfx_randRange(-((i + 1) * 0x180), -(i * 0x180));
        p = &D_801EC168[i];
        p->vx = t;
        p->vz = ex->btlfx_randRange(-0x1000, 0x2000);
        p->vy = p->vx >> 4;
        p->vx += env->unk2C->unk0[2].vx;
        p->vz += env->unk2C->unk0[2].vz;
        p->vy += env->unk2C->unk0[2].vy;
    }
}

void func_801E9F28(s32 c) {
    Unk800F78A4 *env = D_801EAB18;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    MATRIX m;
    SVECTOR v[4];
    SVECTOR rot;
    u8 uv[4][8];
    s32 otz;
    POLY_FT4 *p;
    s32 i;
    s32 w;
    s32 abr;
    s32 tp;
    s32 clut;

    abr = 1;
    clut = 0x3D00;
    uv[1][0] = uv[3][0] = 0x4F;
    uv[0][1] = uv[1][1] = 0xA0;
    uv[0][0] = uv[2][0] = 0;
    uv[2][1] = uv[3][1] = 0xEF;
    gte_SetRotMatrix(env->unk14);
    gte_SetTransMatrix(env->unk14);
    gte_ldv0(&env->unk2C->unk0[2]);
    p = (POLY_FT4 *)env->unk24;
    tp = 0x3A;
    env->unk24 = (u8 *)(p + 32);
    gte_rtv0tr_b();
    rot.vx = 0;
    rot.vy = 0;
    rot.vz = 0;
    ex->func_800316D8(&rot, &m);
    gte_stlvnl(m.t);
    ex->MulRotMatrix(&m);
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    for (i = 0; i < 32; i++) {
        w = i * 16 - 0x5E8;
        v[0].vx = v[2].vx = D_801EC168[i].vx - 0x400;
        v[1].vx = v[3].vx = D_801EC168[i].vx;
        v[0].vz = v[1].vz = D_801EC168[i].vz + w;
        v[2].vz = v[3].vz = D_801EC168[i].vz - w;
        v[0].vy = v[2].vy = D_801EC168[i].vy - 0x600;
        v[1].vy = v[3].vy = D_801EC168[i].vy;
        gte_ldv3c(v);
        gte_rtpt();
        if (abr == 0xFF) {
            p->code = 0x2C;
        } else {
            p->code = 0x2E;
        }
        p->r0 = p->g0 = p->b0 = (c * 0x70) >> 12;
        gte_stsxy3(&p->x0, &p->x1, &p->x2);
        gte_avsz3();
        *(u16 *)&p->u0 = *(u16 *)uv[0];
        *(u16 *)&p->u1 = *(u16 *)uv[1];
        *(u16 *)&p->u2 = *(u16 *)uv[2];
        *(u16 *)&p->u3 = *(u16 *)uv[3];
        gte_stszotz(&otz);
        gte_ldv0(&v[3]);
        gte_rtps();
        p->clut = clut;
        p->tpage = tp;
        gte_stsxy(&p->x3);
        ex->btlfx_addPrim(0x09000000, otz >> 1, p);
        p++;
        D_801EC168[i].vz -= (D_801EC168[i].vx + 0x3000) >> 6;
        if (D_801EC168[i].vz < env->unk2C->unk0[2].vz - 0x1000) {
            D_801EC168[i].vz = 0x2000;
        }
    }
}

void func_801EA298(s16 *data, s32 c, s32 h, s32 n) {
    Unk800F78A4 *env = D_801EAB18;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    SVECTOR rot;
    SVECTOR cen;
    SVECTOR ang;
    SVECTOR a;
    SVECTOR b;
    MATRIX m;
    POLY_FT4 *prim;
    s32 r;
    s32 tp;
    s32 i;
    s32 (*fsin)(s32);
    s32 av;

    a.vx = a.vz = b.vx = b.vz = 0;
    a.vy = -h;
    b.vy = h;
    ex->btlfx_aimAngles(&env->unk2C->unk0[0].vx, &env->unk2C->unk0[1].vx, &ang.vx);
    i = 0;
    cen.vy = env->unk2C->unk0[0].vy - ((ex->math_sin(ang.vx) * 0x800) >> 12);
    data += 2;
    r = ex->math_cos(ang.vx) * 0x800;
    r >>= 12;
    /* The do-while only raises r's reference count so global-alloc puts it ahead of data; the sin pointer and
     * argument are loaded before the abs() branch in the original. */
    do {
        av = ang.vy;
        fsin = ex->math_sin;
        if (r < 0) {
            r = -r;
        }
    } while (0);
    cen.vx = env->unk2C->unk0[0].vx + ((fsin(av) * r) >> 12);
    cen.vz = env->unk2C->unk0[0].vz + ((ex->math_cos(ang.vy) * r) >> 12);
    for (; i < 0x11; i++) {
        v.vx = -*data++ + cen.vx;
        v.vy = *data++ + cen.vy;
        v.vz = -*data++ + cen.vz;
        rot.vx = *data++ + ang.vx;
        rot.vy = *data++ + ang.vy;
        rot.vz = *data++ + ang.vz;
        gte_SetRotMatrix(env->unk14);
        gte_SetTransMatrix(env->unk14);
        gte_ldv0(&v);
        gte_rtv0tr();
        gte_stlvnl(m.t);
        ex->func_800316D8(&rot, &m);
        ex->MulRotMatrix(&m);
        gte_SetRotMatrix(&m);
        gte_SetTransMatrix(&m);
        gte_ldv0(&a);
        gte_rtps();
        gte_stsxy(&D_801EC2E8[i].xy0);
        gte_ldv0(&b);
        gte_rtps();
        gte_stsxy(&D_801EC2E8[i].xy1);
    }
    if (n != 0) {
        prim = (POLY_FT4 *)env->unk24;
        env->unk24 = (u8 *)(prim + 16);
        for (i = 0; i < n; prim++) {
            tp = 0xB8;
            prim->clut = 0x3D80;
            prim->tpage = tp;
            prim->b0 = c;
            prim->g0 = c;
            prim->r0 = c;
            prim->code = 0x2E;
            prim->x0 = D_801EC2E8[i].xy0.vx;
            prim->y0 = D_801EC2E8[i].xy0.vy;
            prim->x1 = D_801EC2E8[i].xy1.vx;
            prim->y1 = D_801EC2E8[i].xy1.vy;
            prim->x2 = D_801EC2E8[i + 1].xy0.vx;
            prim->y2 = D_801EC2E8[i + 1].xy0.vy;
            prim->x3 = D_801EC2E8[i + 1].xy1.vx;
            prim->y3 = D_801EC2E8[i + 1].xy1.vy;
            prim->u2 = 0xFF;
            prim->u0 = 0xFF;
            prim->u3 = 0xC0;
            prim->u1 = 0xC0;
            prim->v0 = prim->v1 = ex->btlfx_lerpRatio(0, 0xFF, i, n);
            i++;
            prim->v2 = prim->v3 = ex->btlfx_lerpRatio(0, 0xFF, i, n);
            ex->btlfx_addPrim(0x09000000, 0, prim);
        }
    }
}

INCLUDE_ASM("asm/us/fx_105/nonmatchings/fx_105", func_801EA6D4);

INCLUDE_RODATA("asm/us/fx_105/nonmatchings/fx_105", fx_105_headerPtr);
