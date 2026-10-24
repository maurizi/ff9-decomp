#include "ovl_0b.h"

s32 btlfx_align4(s32 arg0) {
    s32 v = arg0 & ~3;
    if (arg0 & 3) {
        v += 4;
    }
    return v;
}

s32 btlfx_rangeProgress(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 *arg5) {
    if (arg0 >= arg1 && arg1 + arg2 >= arg0) {
        *arg5 = (arg0 - arg1) * arg4 / arg2;
        if (arg3 < 0) {
            *arg5 = arg4 - *arg5;
        }
        return 1;
    }
    return 0;
}

void btlfx_rotateOffset(SVECTOR *arg0, s32 arg1, s32 arg2) {
    RotMatrix33 m;

    math_rotMatrixYXZ(arg0, &m);
    gte_SetRotMatrix(&m);
    gte_ldtr0();
    btlfx_transformPoint(arg1, 0, arg2);
    gte_SetRotMatrix(g_btlfxEnv->viewMatrix);
    gte_SetTransMatrix(g_btlfxEnv->viewMatrix);
}

void btlfx_pointAhead(s16 *arg0, s16 *arg1, s32 arg2, s16 *arg3) {
    MATRIX m;

    math_rotMatrixYXZ(arg1, &m);
    gte_SetRotMatrix(&m);
    m.t[0] = arg0[0];
    m.t[1] = arg0[1];
    m.t[2] = arg0[2];
    gte_SetTransMatrix(&m);
    btlfx_transformPoint(arg2, 0, (s32)arg3);
    gte_SetRotMatrix(g_btlfxEnv->viewMatrix);
    gte_SetTransMatrix(g_btlfxEnv->viewMatrix);
}

void btlfx_matrixPointAhead(MATRIX *arg0, s32 arg1, s32 arg2) {
    gte_SetRotMatrix(arg0);
    gte_SetTransMatrix(arg0);
    btlfx_transformPoint(arg1, 0, arg2);
    gte_SetRotMatrix(g_btlfxEnv->viewMatrix);
    gte_SetTransMatrix(g_btlfxEnv->viewMatrix);
}

void btlfx_matrixAngles(MATRIX *arg0, s16 *arg1) {
    s16 a[4];
    s32 b[2];

    gte_SetRotMatrix(arg0);
    gte_SetTransMatrix(arg0);
    b[1] = 0;
    b[0] = 0;
    btlfx_transformPoint(0x1000, 0, (s32)a);
    btlfx_aimAngles((s16 *)b, a, arg1);
    gte_SetRotMatrix(g_btlfxEnv->viewMatrix);
    gte_SetTransMatrix(g_btlfxEnv->viewMatrix);
}

s32 btlfx_angleDiff(s32 arg0, s32 arg1, s32 *arg2) {
    s32 s;
    s32 d;
    s32 ad;
    s32 neg;
    arg0 &= 0xFFF;
    arg1 &= 0xFFF;
    s = -1;
    if (arg1 < arg0) {
        s = 1;
    }
    d = arg0 - arg1;
    neg = d < 0;
    ad = d;
    if (neg) {
        ad = -ad;
    }
    if (ad > 0x800) {
        s = -s;
        ad = 0x1000 - ad;
    }
    if (arg2 != NULL) {
        *arg2 = ad;
    }
    return s;
}

void btlfx_turnToward(s16 *arg0, s16 *arg1, s32 arg2) {
    s32 sp10;
    s32 t;
    s32 v;
    t = btlfx_angleDiff(arg0[0], arg1[0], &sp10);
    v = (sp10 * arg2) >> 12;
    arg1[0] += v * t;
    sp10 = v;
    t = btlfx_angleDiff(arg0[1], arg1[1], &sp10);
    v = (sp10 * arg2) >> 12;
    arg1[1] += v * t;
    sp10 = v;
}

void btlfx_setWorldTrans(SVECTOR *arg0, MATRIX *arg1) {
    MATRIX m;

    gte_SetRotMatrix(g_btlfxEnv->viewMatrix);
    gte_SetTransMatrix(g_btlfxEnv->viewMatrix);
    gte_ldv0(arg0);
    gte_rtv0tr();
    if (arg1 == NULL) {
        arg1 = &m;
    }
    gte_stlvnl(arg1->t);
    gte_SetTransMatrix(arg1);
}

void btlfx_setRotScale(SVECTOR *arg0, VECTOR *arg1, MATRIX *arg2, s32 arg3) {
    MATRIX m;
    RotMatrix33 l;

    if (arg2 == NULL) {
        arg2 = &m;
    }
    math_rotMatrixYXZ(arg0, arg2);
    if (arg1 != NULL) {
        ScaleMatrix(arg2, arg1);
    }
    if (arg0->pad != 0) {
        gte_ldclmv(arg2);
        gte_rtir();
        gte_stclmv(arg2);
        gte_ldclmv((s16 *)arg2 + 1);
        gte_rtir();
        gte_stclmv((s16 *)arg2 + 1);
        gte_ldclmv((s16 *)arg2 + 2);
        gte_rtir();
        gte_stclmv((s16 *)arg2 + 2);
    }
    if (arg3 != 0) {
        gte_MulMatrix0(&l, arg2, &l);
        gte_SetLightMatrix(&l);
    }
    gte_SetRotMatrix(arg2);
}

void func_800D044C(SVECTOR *arg0, s32 arg1, s32 arg2, XY32 *arg3, s32 arg4, s32 arg5) {
    struct {
        XY32 xy;
        s16 z;
    } r;
    MATRIX m;

    r.xy = *arg3;
    r.z = arg5;
    math_rotMatrixYXZ(&r, &m);
    gte_SetRotMatrix(&m);
    m.t[0] = arg0->vx;
    m.t[1] = arg0->vy;
    m.t[2] = arg0->vz;
    gte_SetTransMatrix(&m);
    btlfx_transformPoint(arg2, arg4, arg1);
    gte_SetRotMatrix(g_btlfxEnv->viewMatrix);
    gte_SetTransMatrix(g_btlfxEnv->viewMatrix);
}

void func_800D0580(s16 *arg0, XY32 *arg1, s32 arg2) {
    s16 a[4];
    s32 b[2];
    struct {
        XY32 xy;
        s16 z;
    } r;
    MATRIX m;

    r.xy = *arg1;
    r.z = arg2;
    math_rotMatrixYXZ(&r, &m);
    gte_SetRotMatrix(&m);
    gte_ldtr0();
    b[1] = 0;
    b[0] = 0;
    btlfx_transformPoint(0, 0x1000, (s32)a);
    btlfx_aimAngles((s16 *)b, a, arg0);
    gte_SetRotMatrix(g_btlfxEnv->viewMatrix);
    gte_SetTransMatrix(g_btlfxEnv->viewMatrix);
}

void func_800D0674(SVECTOR *arg0, SVECTOR *arg1, SVECTOR *arg2, s32 arg3, s32 arg4, s32 *arg5) {
    SVECTOR v;
    struct {
        XY32 xy;
        s16 z;
    } r;
    RotMatrix33 m;

    v.vx = arg0->vx - arg1->vx;
    v.vy = arg0->vy - arg1->vy;
    v.vz = arg0->vz - arg1->vz;
    r.xy.w = (-arg2->vx & 0xFFFF) | (-arg2->vy << 16);
    r.z = 0;
    math_rotMatrixXYZ(&r.xy, &m);
    gte_SetRotMatrix(&m);
    gte_ldtr0();
    *arg5 = btlfx_transformPolar(&v, arg3, arg4);
    gte_SetRotMatrix(g_btlfxEnv->viewMatrix);
    gte_SetTransMatrix(g_btlfxEnv->viewMatrix);
}

/* adds prim to ot with the given code/len word; tpage != 0xFF also adds a DR_TPAGE (semi-trans mode) */
void btlfx_linkPrimAbr(u32 code, u32 *ot, void *prim, s32 tpage) {
    DR_TPAGE *p;

    addPrimLenIn(ot, prim, code);
    if (tpage != 0xFF) {
        p = (DR_TPAGE *)g_btlfxEnv->prim;
        g_btlfxEnv->prim = (u8 *)(p + 1);
        p->code[0] = ((tpage & 3) << 5) | 0xE1000200;
        addPrimLenIn(ot, p, 0x1000000);
    }
}

void btlfx_addPrim(u32 arg0, s32 arg1, void *arg2) {
    s32 z = (arg1 - g_btlfxEnv->unk28) >> 2;
    if ((u32)z < 0x1000) {
        addPrimLen(&g_btlfxEnv->ot[z], arg2, arg0);
    }
}

void btlfx_addPrimAbr(s32 arg0, s32 arg1, void *arg2, s32 arg3) {
    u32 i = (arg1 - g_btlfxEnv->unk28) >> 2;
    if (i < 0x1000) {
        ((void (*)())btlfx_linkPrimAbr)(arg0, &g_btlfxEnv->ot[i], arg2, arg3);
    }
}

DR_TPAGE *btlfx_allocDrMode(s32 arg0, s32 arg1) {
    DR_TPAGE *p = (DR_TPAGE *)g_btlfxEnv->prim;
    g_btlfxEnv->prim = (u8 *)(p + 1);
    p->code[0] = arg0 | ((arg1 & 3) << 5) | 0xE1000200;
    return p;
}

void btlfx_resetMaskBit(void) {
    s32 n;
    Unk800D0940 *p;
    u32 *ot;
    if (g_btlfxEnv->prim != NULL) {
        D_800F7278 = n = 1;
        p = (Unk800D0940 *)g_btlfxEnv->prim;
        ot = &p->unkC;
        g_btlfxEnv->prim = (u8 *)(p + 1);
        setlen(p, 2);
        p->unk4 = GPU_STP(0, 0);
        p->unk8 = 0;
        ClearOTag(ot, n);
        addPrim(ot, p);
        EnterCriticalSection();
        DrawOTag(ot);
        ExitCriticalSection();
        D_800F7278 = 0;
    }
}

void btlfx_drawScreenTint(u32 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    TILE *t;
    s32 i;
    t = (TILE *)g_btlfxEnv->prim;
    g_btlfxEnv->prim = (u8 *)(t + 8);
    for (i = 0; i < 8; i++, t++) {
        t->code = arg1 == 0xFF ? 0x60 : 0x62;
        t->w = 0x50;
        t->h = 0x6E;
        t->x0 = (i & 3) * 0x50;
        t->r0 = arg2;
        t->g0 = arg3;
        t->b0 = arg4;
        t->y0 = (i >> 2) * 0x6E;
        btlfx_linkPrimAbr(0x03000000, arg0, t, arg1);
    }
}

/* Draws a grid of gouraud quads whose rows ripple with a sine wave. */
void btlfx_drawRippleGrid(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, CVECTOR *arg6, s32 arg7, s32 arg8,
                   s32 arg9, s32 arg10) {
    POLY_G4 *p;
    CVECTOR *ca;
    s32 r;
    s32 c;
    s32 y;
    s32 ph;

    ca = arg6;
    r = 0;
    p = (POLY_G4 *)g_btlfxEnv->prim;
    g_btlfxEnv->prim = (u8 *)(p + arg2 * arg3);
    y = arg1;
    if (arg3 > 0) {
        do {
            ph = arg8 + r * arg10;
            ca[1].cd = (arg5 == 0xFF) ? 0x38 : 0x3A;
            ca[0].cd = ca[1].cd;
            for (c = 0; c < arg2; c++, p++, ph += arg9) {
                p->x0 = p->x2 = (c * 0x140) / arg2;
                p->x1 = p->x3 = ((c + 1) * 0x140) / arg2;
                if (r == 0) {
                    p->y0 = p->y1 = y;
                } else {
                    p->y0 = y + ((math_sin(ph) * arg7) >> 12);
                    p->y1 = y + ((math_sin(ph + arg9) * arg7) >> 12);
                }
                ph += arg10;
                p->y2 = (y + arg4) + ((math_sin(ph) * arg7) >> 12);
                p->y3 = (y + arg4) + ((math_sin(ph + arg9) * arg7) >> 12);
                ph -= arg10;
                *(s32 *)&p->r0 = *(s32 *)&p->r1 = *(s32 *)ca;
                *(s32 *)&p->r2 = *(s32 *)&p->r3 = *(s32 *)&ca[1];
                btlfx_linkPrimAbr(0x08000000, arg0, p, arg5);
            }
            r++;
            y += arg4;
            ca++;
        } while (r < arg3);
    }
}

void btlfx_drawTrail3D(s32 arg0, s32 *arg1, s32 n, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 p,
                   u32 *c1, u32 *c2, s32 tp) {
    Unk800D50A0Ctx c;
    u8 *prim;
    s32 inv;

    if (p < 0) {
        p = 0x1000;
    }
    gte_DpqColorFar_nst(c1, p);
    prim = g_btlfxEnv->prim;
    g_btlfxEnv->prim = prim + n * 0x34;
    gte_strgb(&c.unkC);
    gte_DpqColorFar_nst(c2, p);
    c.unk26 = (arg4 + arg6) << 8;
    c.unk24 = arg4 << 8;
    gte_strgb(&c.unk10);
    c.unk18 = (((tp & 3) << 5) | arg7) << 16;
    c.unk1C = arg8 << 16;
    c.unkC.b[3] = (tp == 0xFF) ? 0x3C : 0x3E;
    gte_SetRotMatrix(g_btlfxEnv->viewMatrix);
    gte_SetTransMatrix(g_btlfxEnv->viewMatrix);
    if (n != 0) {
        inv = 0x1000 / n;
        c.unk20 = inv;
        c.unk8 = inv;
        c.unk28 = arg1[0];
        inv = arg1[1];
        c.unk4 = arg0;
        c.unk14 = arg3;
        c.unk16 = arg5;
        c.unk0 = g_btlfxEnv->unk28;
        c.unk2C = inv;
        btlfx_emitTrailRibbon3D(n, &c, prim, g_btlfxEnv->ot);
    }
}

void btlfx_drawTrailFlat(s32 arg0, s32 n, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 p, u32 *c1,
                   u32 *c2, s32 tp) {
    Unk800D4D6CCtx c;
    u8 *prim;
    s32 inv;
    s32 s;

    if (p < 0) {
        p = 0x1000;
    }
    gte_DpqColorFar_nst(c1, p);
    gte_strgb(&c.unkC);
    gte_DpqColorFar_nst(c2, p);
    if (arg6 >= 0) {
        prim = g_btlfxEnv->prim;
        g_btlfxEnv->prim = prim + n * 0x34;
        c.unk26 = (arg3 + arg5) << 8;
        c.unk24 = arg3 << 8;
        s = math_sin(-g_btlfxEnv->camera->yaw + 0x400);
        c.unk28 = (s & 0xFFFF) | (math_cos(-g_btlfxEnv->camera->yaw + 0x400) << 16);
        c.unkC.b[3] = (tp != 0xFF) ? 0x3E : 0x3C;
        c.unk18 = (((tp & 3) << 5) | arg6) << 16;
        c.unk1C = arg7 << 16;
        c.unk14 = arg2;
        c.unk16 = arg4;
    } else {
        prim = g_btlfxEnv->prim;
        g_btlfxEnv->prim = prim + n * 0x1C;
        c.unk28 = (tp & 3) << 5;
        c.unkC.b[3] = (tp == 0xFF) ? 0x50 : 0x52;
    }
    gte_strgb(&c.unk10);
    gte_SetRotMatrix(g_btlfxEnv->viewMatrix);
    gte_SetTransMatrix(g_btlfxEnv->viewMatrix);
    if (n != 0) {
        inv = 0x1000 / n;
        c.unk4 = arg0;
        c.unk20 = inv;
        c.unk8 = inv;
        c.unk0 = g_btlfxEnv->unk28;
        if (arg6 >= 0) {
            btlfx_emitTrailRibbonFlat(n, &c, prim, g_btlfxEnv->ot);
        } else {
            btlfx_emitTrailLines(n, &c, prim, g_btlfxEnv->ot);
        }
    }
}

void btlfx_drawTrailScreen(s32 arg0, s32 n, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 p, u32 *c1,
                   u32 *c2, s32 tp) {
    Unk800D5C30Ctx c;
    u8 *prim;
    s32 inv;

    if (p < 0) {
        p = 0x1000;
    }
    gte_DpqColorFar_nst(c1, p);
    prim = g_btlfxEnv->prim;
    g_btlfxEnv->prim = prim + n * 0x34;
    gte_strgb(&c.unkC);
    gte_DpqColorFar_nst(c2, p);
    c.unk26 = (arg3 + arg5) << 8;
    c.unk24 = arg3 << 8;
    c.unk18 = (((tp & 3) << 5) | arg6) << 16;
    c.unk1C = arg7 << 16;
    c.unkC.b[3] = (tp == 0xFF) ? 0x3C : 0x3E;
    gte_strgb(&c.unk10);
    gte_SetRotMatrix(g_btlfxEnv->viewMatrix);
    gte_SetTransMatrix(g_btlfxEnv->viewMatrix);
    if (n != 0) {
        inv = 0x1000 / n;
        c.unk4 = arg0;
        c.unk14 = arg2;
        c.unk16 = arg4;
        c.unk20 = inv;
        c.unk8 = inv;
        c.unk0 = g_btlfxEnv->unk28;
        btlfx_emitTrailRibbonScreen(n, &c, prim, g_btlfxEnv->ot);
    }
}

void btlfx_drawTrail(u8 *p, s32 arg1, s32 *arg2, s32 frame, s32 scale, s32 arg5, s32 tp, u8 *out, s32 mode) {
    s32 n;
    s32 k;
    s32 cnt;
    s32 a;
    s32 b;
    s32 c;
    s32 x;
    s32 d;
    s32 y;
    s32 v;
    u32 *data;

    if (out == NULL) {
        return;
    }
    n = ((Unk800D1494 *)p)->unk2;
    k = ((Unk800D1494 *)p)->unkA;
    cnt = ((Unk800D1494 *)p)->unk0;
    x = ((Unk800D1494 *)p)->unk4;
    y = ((Unk800D1494 *)p)->unk6;
    c = ((Unk800D1494 *)p)->unk8 - 1;
    d = ((Unk800D1494 *)p)->unk9 - 1;
    p += sizeof(Unk800D1494);
    if (frame == -1) {
        out[1] = n;
        out[0] = n;
        out[2] = 0;
        return;
    }
    data = (u32 *)p;
    if (frame >= cnt) {
        frame = cnt - 1;
    }
    p = (u8 *)data + (k * 8 + frame * 4);
    v = p[2];
    v = (v * scale) >> 12;
    a = p[0];
    b = p[1];
    k = p[3];
    data += k * 2;
    if (arg1 != 0) {
        n = btlfx_pushTrailPoint(out, n, v, arg1);
    }
    if (mode == 0) {
        btlfx_drawTrailScreen((s32)out, n, a, b, c, d, x, y, arg5, data, data + 1, tp);
    } else if (mode == 1) {
        btlfx_drawTrail3D((s32)out, arg2, n, a, b, c, d, x, y, arg5, data, data + 1, tp);
    } else {
        btlfx_drawTrailFlat((s32)out, n, a, b, c, d, x, y, arg5, data, data + 1, tp);
    }
}

void btlfx_drawParticles(u8 *arg0, SVECTOR *arg1, SVECTOR *arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8) {
    VECTOR s;
    union {
        SVECTOR v;
        s32 w[2];
    } zero;
    MATRIX m;
    Unk800D1680Ctx c;
    u8 *prim;
    u8 *d;
    u8 *e;
    s32 off;
    u16 *o;
    Unk800D1BC4 *h = (Unk800D1BC4 *)arg0;
    s32 k;

    if (arg2 == NULL) {
        arg2 = &zero.v;
        zero.w[0] = 0;
        zero.w[1] = 0;
    }
    k = arg4;
    off = h->unk0 & 0x7FFF;
    if (k >= off) {
        k = off - 1;
    }
    o = &h->unk6[k];
    c.unk10 = h->unk2;
    off = *o;
    c.unk14 = *(u16 *)(arg0 + off);
    off += 2;
    if (c.unk14 & 0x8000) {
        d = arg0 + off;
        off += 4;
        c.unk0 = *(u16 *)d | (d[2] << 16);
        if (arg6 >= 0) {
            gte_DpqColorFar(&c.unk0, arg6);
        }
    } else {
        if (arg6 < 0) {
            arg6 = 0x1000;
        }
        arg6 = (arg6 >> 5) & 0xFF;
        c.unk0 = arg6 | (arg6 << 8) | (arg6 << 16);
    }
    s.vz = 0x1000;
    if (c.unk14 & 0x4000) {
        e = arg0 + off;
        s.vx = *(u16 *)e;
        s.vy = *(u16 *)(e + 2);
        if (arg3 >= 0) {
            gte_lddp(arg3);
            gte_ldir(&s);
            gte_gpf12();
            gte_stir(&s);
        }
        off += 4;
    } else {
        if (arg3 < 0) {
            arg3 = 0x1000;
        }
        s.vx = s.vy = arg3;
    }
    arg0 += off;
    c.unk14 &= 0x3FFF;
    gte_SetRotMatrix(g_btlfxEnv->viewMatrix);
    gte_SetTransMatrix(g_btlfxEnv->viewMatrix);
    gte_ldv0(arg1);
    prim = g_btlfxEnv->prim;
    g_btlfxEnv->prim = prim + c.unk14 * 40;
    gte_rtv0tr_nn();
    math_rotMatrixYXZ(arg2, &m);
    ScaleMatrix(&m, &s);
    gte_stlvnl(m.t);
    if (arg2->pad != 0) {
        MulRotMatrix(&m);
    }
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    c.unk0 |= (arg5 == 0xFF) ? 0x2C000000 : 0x2E000000;
    c.unkC = (arg5 & 3) << 5;
    c.unk4 = arg7;
    c.unk8 = arg8;
    c.unk18 = g_btlfxEnv->unk28;
    btlfx_emitParticles(arg0, &c, prim, g_btlfxEnv->ot);
}

void btlfx_drawSprite(u8 *arg0, SVECTOR *arg1, s32 arg2, s32 arg3, s32 arg4) {
    DVECTOR sxy;
    Unk800D1BC4Ctx c;
    s32 z;
    u8 *prim;
    u8 *d;
    s32 off;
    u16 *o;
    Unk800D1BC4 *h = (Unk800D1BC4 *)arg0;

    off = h->unk0 & 0x7FFF;
    if (arg2 >= off) {
        arg2 = off - 1;
    }
    o = &h->unk6[arg2];
    c.unk10 = h->unk2;
    off = *o;
    c.unk14 = *(u16 *)(arg0 + off);
    off += 2;
    if (c.unk14 & 0x8000) {
        d = arg0 + off;
        off += 4;
        c.unk0 = *(u16 *)d | (d[2] << 16);
        if (arg4 >= 0) {
            gte_DpqColorFar(&c.unk0, arg4);
        }
    } else {
        if (arg4 < 0) {
            arg4 = 0x1000;
        }
        arg4 = (arg4 >> 5) & 0xFF;
        c.unk0 = arg4 | (arg4 << 8) | (arg4 << 16);
    }
    if (c.unk14 & 0x4000) {
        off += 4;
    }
    gte_SetRotMatrix(g_btlfxEnv->viewMatrix);
    gte_SetTransMatrix(g_btlfxEnv->viewMatrix);
    gte_ldv0(arg1);
    arg0 += off;
    c.unk14 &= 0x3FFF;
    gte_rtps_nn();
    prim = g_btlfxEnv->prim;
    g_btlfxEnv->prim = prim + c.unk14 * 20;
    gte_stsxy(&sxy);
    gte_stszotz(&z);
    c.unk0 |= (arg3 == 0xFF) ? 0x64000000 : 0x66000000;
    c.unkC = (arg3 & 3) << 5;
    c.unk4 = sxy.vx;
    c.unk8 = sxy.vy;
    c.unk18 = g_btlfxEnv->prim;
    z = (z - g_btlfxEnv->unk28) >> 2;
    if ((u32)z < 0x1000) {
        g_btlfxEnv->prim = btlfx_emitScreenSprites(arg0, &c, prim, &g_btlfxEnv->ot[z]);
    }
}

void btlfx_drawSpriteAtOt(u32 arg0, u8 *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    Unk800D1BC4Ctx c;
    u8 *prim;
    u8 *d;
    s32 off;
    s32 n;
    u16 *o;
    Unk800D1BC4 *h = (Unk800D1BC4 *)arg1;

    s32 k = arg4;

    n = h->unk0 & 0x7FFF;
    if (k >= n) {
        k = n - 1;
    }
    o = &h->unk6[k];
    c.unk10 = h->unk2;
    off = *o;
    c.unk14 = *(u16 *)(arg1 + off);
    off += 2;
    if (c.unk14 & 0x8000) {
        d = arg1 + off;
        off += 4;
        c.unk0 = *(u16 *)d | (d[2] << 16);
        if (arg6 >= 0) {
            gte_DpqColorFar(&c.unk0, arg6);
        }
    } else {
        if (arg6 < 0) {
            arg6 = 0x1000;
        }
        arg6 = (arg6 >> 5) & 0xFF;
        c.unk0 = arg6 | (arg6 << 8) | (arg6 << 16);
    }
    if (c.unk14 & 0x4000) {
        off += 4;
    }
    arg1 += off;
    c.unk14 &= 0x3FFF;
    prim = g_btlfxEnv->prim;
    g_btlfxEnv->prim = prim + c.unk14 * 20;
    c.unk0 |= (arg5 == 0xFF) ? 0x64000000 : 0x66000000;
    c.unkC = (arg5 & 3) << 5;
    c.unk4 = arg2;
    c.unk8 = arg3;
    c.unk18 = g_btlfxEnv->prim;
    if (arg0 < 0x1000) {
        g_btlfxEnv->prim = btlfx_emitScreenSprites(arg1, &c, prim, &g_btlfxEnv->ot[arg0]);
    }
}

void btlfx_drawAnimQuads(u8 *arg0, SVECTOR *arg1, SVECTOR *arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    Unk800D1D74Hdr *h = (Unk800D1D74Hdr *)arg0;
    VECTOR s;
    SVECTOR r;
    MATRIX m;
    Unk800D3ED4Ctx c;
    s32 n;
    u16 *p;
    u8 *prim;
    u8 *pal;
    s32 i;
    s32 off;

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
    off = i * 2;
    pal = arg0 + n;
    p = (u16 *)pal;
    i = *p++;
    c.unk0 = p;
    c.unk4 = p + i + 1;
    c.unk10 = h->unk2 | ((arg5 & 3) << 5);
    c.unk14 = h->unk4;
    c.unkC = (h->unk6 - 1) * 2;
    c.unk8 = (h->unk7 - 1) * 2;
    n = *(u16 *)((u8 *)h->unk8 + off);
    c.unk1C = arg0[n++];
    arg0 += n;
    gte_SetRotMatrix(g_btlfxEnv->viewMatrix);
    gte_SetTransMatrix(g_btlfxEnv->viewMatrix);
    gte_ldv0(arg1);
    prim = g_btlfxEnv->prim;
    g_btlfxEnv->prim = prim + c.unk1C * 40;
    gte_rtv0tr_nn();
    math_rotMatrixYXZ(arg2, &m);
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
    c.unk20 = g_btlfxEnv->unk28;
    if (c.unk1C != 0) {
        btlfx_emitAnimQuads(arg0, &c, prim, g_btlfxEnv->ot);
    }
}

void btlfx_drawSprites(u8 *arg0, SVECTOR *arg1, SVECTOR *arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    Unk800D1D74Hdr *h = (Unk800D1D74Hdr *)arg0;
    VECTOR s;
    SVECTOR r;
    MATRIX m;
    Unk800D3ED4Ctx c;
    s32 z;
    s32 n;
    u16 *p;
    s32 off;
    u8 *prim;
    u8 *pal;
    s32 i;
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
    off = i * 2;
    pal = arg0 + n;
    p = (u16 *)pal;
    i = *p++;
    c.unk0 = p;
    c.unk4 = p + i + 1;
    mode = h->unk2;
    c.unk14 = h->unk4;
    c.unkC = h->unk6 - 1;
    c.unk8 = h->unk7 - 1;
    z = *(u16 *)&h->unk6;
    n = *(u16 *)((u8 *)h->unk8 + off);
    c.unk1C = arg0[n++];
    arg0 += n;
    gte_SetRotMatrix(g_btlfxEnv->viewMatrix);
    gte_SetTransMatrix(g_btlfxEnv->viewMatrix);
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
    prim = g_btlfxEnv->prim;
    g_btlfxEnv->prim = prim + size * c.unk1C;
    gte_rtv0tr_nn();
    math_rotMatrixYXZ(arg2, &m);
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
    tp = btlfx_allocDrMode(mode, arg5);
    gte_stszotz(&z);
    if (arg2->pad != 0) {
        MulRotMatrix(&m);
    }
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    c.unk18 = arg6;
    z = (z - g_btlfxEnv->unk28) >> 2;
    if ((u32)z < 0x1000 && c.unk1C != 0) {
        if (size == 0x14) {
            btlfx_emitSprites(arg0, &c, prim, &g_btlfxEnv->ot[z]);
        } else if (size == 0xC) {
            btlfx_emitDots(arg0, &c, prim, &g_btlfxEnv->ot[z]);
        } else {
            btlfx_emitFixedSprites(arg0, &c, prim, &g_btlfxEnv->ot[z]);
        }
        addPrimLenIn(&g_btlfxEnv->ot[z], tp, 0x1000000);
    }
}

void btlfx_drawRings(u8 *arg0, SVECTOR *arg1, SVECTOR *arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    VECTOR s;
    union {
        SVECTOR v;
        s32 w[2];
    } zero;
    MATRIX m;
    Unk800D4768Ctx c;
    Unk800D4A30Ctx d;
    Unk800D23D8Hdr *h = (Unk800D23D8Hdr *)arg0;
    Unk800D23D8Scale *e;
    Unk800D23D8Scale *sc;
    u16 *s1;
    s32 f;
    s32 type;
    s32 cnt;
    s32 w;
    s32 nf;
    s32 n;
    u16 *q;
    u16 *t;
    u8 *prim;
    s32 k;
    u32 *col;
    s32 m2;

    if (arg2 == NULL) {
        arg2 = &zero.v;
        zero.w[0] = 0;
        zero.w[1] = 0;
    }
    n = h->unk0;
    if (arg4 >= n) {
        arg4 %= n;
    }
    type = h->unk5;
    cnt = h->unk6;
    w = h->unk7;
    e = (Unk800D23D8Scale *)(h + 1);
    s.vz = 0x1000;
    if (((Unk800D23D8Hdr *)e)[-1].unk4 & 2) {
        sc = &e[arg4];
        s.vx = sc->unk0;
        s.vy = sc->unk2;
        if (arg3 >= 0) {
            gte_lddp(arg3);
            gte_ldir(&s);
            gte_gpf12();
            gte_stir(&s);
        }
        s1 = (u16 *)&e[n];
    } else {
        s1 = (u16 *)e;
        if (arg3 < 0) {
            arg3 = 0x1000;
        }
        s.vx = s.vy = arg3;
    }
    gte_SetRotMatrix(g_btlfxEnv->viewMatrix);
    gte_SetTransMatrix(g_btlfxEnv->viewMatrix);
    gte_ldv0(arg1);
    nf = s1[1];
    f = 0;
    if (n >= 2) {
        f = arg4 * (nf - 1) / (n - 1);
    }
    gte_rtv0tr_nn();
    math_rotMatrixYXZ(arg2, &m);
    ScaleMatrix(&m, &s);
    gte_stlvnl(m.t);
    if (arg2->pad != 0) {
        MulRotMatrix(&m);
    }
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    gte_zero_vz012();
    switch (type) {
    case 1:
        c.unk14 = (u16 *)((u32 *)s1 + (nf + 1));
        col = (u32 *)s1 + (f + 1);
        if (arg6 >= 0) {
            gte_DpqColorFar_nst(col, arg6);
        } else {
            gte_DpqColorFar_nst(col, 0x1000);
        }
        gte_strgb(&c.unk0);
        q = c.unk14;
        k = q[0];
        c.unk28 = (q[2] | ((arg5 & 3) << 5)) << 16;
        c.unk4 = q[3] << 16;
        c.unk14 = q = q + 4;
        t = q + (cnt + 1) * (k + 2);
        c.unk8 = t + n;
        c.unkC = c.unk8 + 2;
        c.unk10 = c.unkC + cnt + 1;
        c.unk20 = t[arg4];
        prim = g_btlfxEnv->prim;
        g_btlfxEnv->prim = prim + w * cnt * 40;
        ((u8 *)&c.unk0)[3] = (arg5 != 0xFF) ? 0x2E : 0x2C;
        c.unk24 = cnt;
        c.unk1C = w;
        c.unk18 = g_btlfxEnv->unk28;
        btlfx_emitRingQuads(k, &c, prim, g_btlfxEnv->ot);
        break;
    case 2:
        m2 = s1[0] ? 2 : 1;
        d.unk20 = (u32 *)s1 + (f * m2 * (cnt + 1) + 1);
        if (s1[0]) {
            d.unk14 = d.unk20 + 1;
        } else {
            d.unk14 = d.unk20;
        }
        if (arg6 < 0) {
            arg6 = 0x1000;
        }
        d.unk8 = (u32 *)s1 + (nf * m2 * (cnt + 1) + 1);
        d.unkC = d.unk8 + 1;
        d.unk10 = (u16 *)d.unkC + cnt + 1;
        prim = g_btlfxEnv->prim;
        g_btlfxEnv->prim = prim + w * cnt * 44;
        d.unk0 = (arg5 != 0xFF) ? 0x3A000000 : 0x38000000;
        d.unk4 = arg6;
        d.unk24 = cnt;
        d.unk28 = (arg5 & 3) << 5;
        d.unk1C = w;
        d.unk18 = g_btlfxEnv->unk28;
        btlfx_emitRingGouraud(m2, &d, prim, g_btlfxEnv->ot);
        break;
    }
}

void btlfx_drawFan(Unk800D2938 *arg0, SVECTOR *arg1, SVECTOR *arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
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
    k = arg4 * 4;
    e = (u16 *)((u8 *)base + k);
    s.vy = 0x1000;
    s.vz = e[0];
    s.vx = e[1];
    if (arg3 >= 0) {
        gte_lddp(arg3);
        gte_ldir(&s);
        gte_gpf12();
        gte_stir(&s);
    }
    gte_SetRotMatrix(g_btlfxEnv->viewMatrix);
    gte_SetTransMatrix(g_btlfxEnv->viewMatrix);
    gte_ldv0(arg1);
    gte_rtv0tr();
    math_rotMatrixYXZ(arg2, &m);
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
        c1.unk4 = g_btlfxEnv->unk28;
        prim = g_btlfxEnv->prim;
        g_btlfxEnv->prim = prim + 0x28;
        btlfx_emitFrameQuad((u8 *)arg0, &c1, prim, g_btlfxEnv->ot);
    } else {
        c2.unkC = (arg5 & 3) << 5;
        c2.unk14 = t;
        c2.unk10 = z;
        c2.unk0 = (arg5 != 0xFF) ? 0x32000000 : 0x30000000;
        c2.unk8 = arg6;
        c2.unk4 = g_btlfxEnv->unk28;
        prim = g_btlfxEnv->prim;
        g_btlfxEnv->prim = prim + 0x5C;
        btlfx_emitFrameFan((u8 *)arg0, &c2, prim, g_btlfxEnv->ot);
    }
}

/* old-style definition: callers pass the coordinates unpromoted (no prototype) */
void btlfx_addSpriteQuad(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11)
SprInfo *arg0;
s16 arg1;
s16 arg2;
s16 arg3;
s32 arg4, arg5, arg6, arg7, arg8;
CVECTOR *arg9;
s32 arg10, arg11;
{
    POLY_FT4 *p;
    s32 c;
    u16 tp;

    p = (POLY_FT4 *)g_btlfxEnv->prim;
    g_btlfxEnv->prim = (u8 *)(p + 1);
    p->code = (arg0->unk8 != 0xFF) ? 0x2E : 0x2C;
    p->clut = arg0->unk4;
    if (arg0->unk8 != 0xFF) {
        arg0->unk2 |= (arg0->unk8 & 3) << 5;
    }
    tp = arg0->unk2;
    p->x0 = arg1;
    c = arg5 + arg7 + arg10;
    p->x1 = arg1 + arg7 + arg10;
    p->x3 = arg2 + arg7 + arg10;
    p->y2 = arg3 + arg8 + arg11;
    p->y3 = arg4 + arg8 + arg11;
    p->x2 = arg2;
    p->y0 = arg3;
    p->y1 = arg4;
    p->u2 = arg5;
    p->u0 = arg5;
    p->tpage = tp;
    if (c >= 0x100) {
        c--;
    }
    p->u3 = c;
    p->u1 = c;
    c = arg6 + arg8 - arg11;
    p->v1 = arg6;
    p->v0 = arg6;
    if (c >= 0x100) {
        c--;
    }
    p->v3 = c;
    p->v2 = c;
    *(u16 *)&p->r0 = *(u16 *)&arg9->r;
    p->b0 = arg9->b;
    btlfx_addPrim(0x09000000, 0xFFE, p);
}

void btlfx_drawBackdropSprite(u8 *arg0, s32 x, s32 y, s32 frame, s32 tp, s32 p) {
    struct {
        u16 rg;
        u8 b;
        u8 code;
        s32 pad4;
    } col;
    u32 *mode;
    u8 *ctab;
    Unk800D1D74Hdr *h = (Unk800D1D74Hdr *)arg0;
    s32 n;
    u16 *q;
    u16 *pal;
    s32 off;
    s32 wv;
    s32 hv;
    s32 num;
    u8 *prim;
    s32 i;
    u8 *c;
    s32 b;
    s32 tpage;
    s32 f;
    s32 clut;

    n = h->unk0 & 0x7FFF;
    f = frame;
    if (f >= n) {
        f %= n;
    }
    n = n * 2 + 8;
    off = f * 2;
    q = (u16 *)(arg0 + n);
    n = *(u16 *)((u8 *)h->unk8 + off);
    pal = q + 1;
    f = *q;
    tpage = h->unk2;
    clut = h->unk4 << 16;
    hv = h->unk6 - 1;
    wv = h->unk7 - 1;
    num = arg0[n++];
    arg0 += n;
    prim = g_btlfxEnv->prim;
    mode = (u32 *)(prim + num * 20);
    g_btlfxEnv->prim = (u8 *)(mode + 2);
    mode[1] = tpage | 0xE1000000 | ((tp & 3) << 5);
    ctab = (u8 *)(pal + f + 1);
    col.code = (tp != 0xFF) ? 0x66 : 0x64;
    for (i = 0; i < num; i++, prim += 20) {
        ((SPRT *)prim)->x0 = (s8)arg0[0] + x - (wv >> 1);
        ((SPRT *)prim)->y0 = (s8)arg0[1] + y - (hv >> 1);
        b = arg0[2];
        f = (b & 0xF) * 2;
        *(u32 *)&((SPRT *)prim)->u0 = *(u16 *)((u8 *)pal + f) | clut;
        c = ctab + ((b >> 2) & 0x3C);
        arg0 += 3;
        col.rg = *(u16 *)c;
        col.b = c[2];
        gte_DpqColorFar_nst(&col, p);
        ((SPRT *)prim)->w = wv;
        ((SPRT *)prim)->h = hv;
        gte_strgb(&((SPRT *)prim)->r0);
        btlfx_addPrim(0x04000000, 0xFFE, prim);
    }
    btlfx_addPrim(0x01000000, 0xFFE, mode);
}

void btlfx_drawBackdrop(BtlBgLayers *arg0, s32 arg1) {
    RECT r;
    s16 ofs[2];
    CVECTOR col;
    DR_TPAGE *tp;
    DR_AREA *da;
    DR_OFFSET *dofs;
    s32 *offs;
    s32 i;
    s32 n;
    s32 w;
    s32 h;
    s32 step;
    s32 x, y;
    BtlBgLayer *L;
    s32 u, v;
    s32 a;
    s32 k;
    s32 j;
    s32 tw;
    s32 cnt;
    s32 wt;
    TILE *t;
    struct BtlBgScrollP *sc;
    struct BtlBgWaveP *wv;
    struct BtlBgWaveP *wv3;
    struct BtlBgFadeP *fd;

    da = (DR_AREA *)g_btlfxEnv->prim;
    n = arg0->unk0;
    w = arg0->unk2;
    h = arg0->unk3;
    x = arg0->unk4;
    y = arg0->unk6;
    g_btlfxEnv->prim = (u8 *)(da + 2);
    r.x = x;
    r.y = y;
    r.w = w;
    r.h = h;
    SetDrawArea(da, &r);
    ((XY32 *)&r)[0].w = ((XY32 *)&g_btlfxEnv->drawEnv->clip)[0].w;
    ((XY32 *)&r)[1].w = ((XY32 *)&g_btlfxEnv->drawEnv->clip)[1].w;
    SetDrawArea(&da[1], &r);
    i = 0;
    dofs = (DR_OFFSET *)g_btlfxEnv->prim;
    g_btlfxEnv->prim = (u8 *)(dofs + 2);
    ofs[0] = x;
    ofs[1] = y;
    SetDrawOffset(dofs, ofs);
    SetDrawOffset(&dofs[1], g_btlfxEnv->drawEnv->ofs);
    tp = (DR_TPAGE *)g_btlfxEnv->prim;
    g_btlfxEnv->prim = (u8 *)(tp + 2);
    tp[0].code[0] = 0xE1000000;
    tp[1].code[0] = 0xE1000200;
    btlfx_addPrim(0x01000000, 0xFFE, &tp[1]);
    btlfx_addPrim(0x02000000, 0xFFE, &da[1]);
    btlfx_addPrim(0x02000000, 0xFFE, &dofs[1]);
    offs = arg0->unk8;
    for (; i < n; i++) {
        L = (BtlBgLayer *)((u8 *)arg0 + offs[i]);
        if (L->unkC != 0) {
            btlfx_sampleGradient((u8 *)arg0 + L->unkC, &col, arg1 % L->unkA);
        } else {
            *(s32 *)&col = 0x808080;
        }
        switch (L->unk0) {
        case 0:
            sc = &L->unk10.scroll;
            if (sc->unk0 == 0) {
                wt = u = arg1 * sc->unk4;
            } else {
                wt = u = (math_sin(sc->unk4 * arg1) * sc->unk8) >> 12;
            }
            BG_WRAP(u, w);
            if (sc->unk1 == 0) {
                wt = v = arg1 * sc->unk6;
            } else {
                wt = v = (math_sin(sc->unk6 * arg1) * sc->unkA) >> 12;
            }
            BG_WRAP(v, h);
            btlfx_addSpriteQuad((SprInfo *)L, u, u, v, v, L->unk6, L->unk7, w, h, &col, 1, 1);
            if (u != 0) {
                btlfx_addSpriteQuad((SprInfo *)L, u - w, u - w, v, v, L->unk6, L->unk7, w, h, &col, 0, 1);
            }
            if (v != 0) {
                btlfx_addSpriteQuad((SprInfo *)L, u, u, v - h, v - h, L->unk6, L->unk7, w, h, &col, 1, 0);
            }
            if (u != 0 && v != 0) {
                btlfx_addSpriteQuad((SprInfo *)L, u - w, u - w, v - h, v - h, L->unk6, L->unk7, w, h, &col, 0, 0);
            }
            break;
        case 1:
            wv = &L->unk10.wave;
            wt = v = arg1 * wv->unkA;
            BG_WRAP(v, h);
            cnt = 1;
            if (v != 0) {
                cnt = 2;
            }
            step = wv->unk4;
            tw = wv->unk0;
            a = arg1 * wv->unk6 + v * step / tw;
            do {
                k = 0;
                for (j = 0; j < wv->unk2; j++) {
                    u = ((math_sin(a) * wv->unk8) >> 12) - (w >> 1);
                    a += step;
                    btlfx_addSpriteQuad((SprInfo *)L, u, ((math_sin(a) * wv->unk8) >> 12) - (w >> 1), v, v, L->unk6, L->unk7 + k, w * 2, tw, &col, 0, 0);
                    v += tw;
                    k += tw;
                }
                cnt--;
                v -= h * 2;
                a -= step * wv->unk2 * 2;
            } while (cnt != 0);
            break;
        case 2:
            fd = &L->unk10.fade;
            if (arg1 >= fd->unk5) {
                s32 e, f;
                e = arg1 - fd->unk5;
                if (e < fd->unk6) {
                    f = (e << 12) / fd->unk6;
                } else {
                    f = 0x1000;
                }
                if (fd->unk0 != 0) {
                    btlfx_drawBackdropSprite((u8 *)arg0 + fd->unk0 + 4, fd->unk8, fd->unkA, e, L->unk8, f);
                }
            }
            break;
        case 3:
            wv3 = &L->unk10.wave;
            wt = v = arg1 * wv3->unkA;
            BG_WRAP(v, w);
            cnt = 1;
            if (v != 0) {
                cnt = 2;
            }
            step = wv3->unk4;
            tw = wv3->unk0;
            a = arg1 * wv3->unk6 + v * step / tw;
            do {
                k = 0;
                for (j = 0; j < wv3->unk2; j++) {
                    u = ((math_sin(a) * wv3->unk8) >> 12) - (h >> 1);
                    a += step;
                    btlfx_addSpriteQuad((SprInfo *)L, v, v, u, ((math_sin(a) * wv3->unk8) >> 12) - (h >> 1), L->unk6 + k, L->unk7, tw, h * 2, &col, 0, 0);
                    v += tw;
                    k += tw;
                }
                cnt--;
                v -= w * 2;
                a -= step * wv3->unk2 * 2;
            } while (cnt != 0);
            break;
        }
    }
    t = (TILE *)g_btlfxEnv->prim;
    g_btlfxEnv->prim = (u8 *)(t + 1);
    *(s32 *)&t->r0 = 0;
    t->code = 0x60;
    t->y0 = 0;
    t->x0 = 0;
    t->w = w;
    t->h = h;
    btlfx_addPrim(0x03000000, 0xFFE, t);
    btlfx_addPrim(0x01000000, 0xFFE, tp);
    btlfx_addPrim(0x02000000, 0xFFE, da);
    btlfx_addPrim(0x02000000, 0xFFE, dofs);
}

s32 btlfx_ease(s32 arg0, s32 arg1, s32 arg2) {
    s32 r = 0;
    switch (arg2) {
    case 0:
        r = (arg0 << 12) / arg1;
        break;
    case 1:
        r = 0x1000 - math_cos((arg0 << 11) / arg1);
        break;
    case 2:
        r = math_sin((arg0 << 11) / arg1);
        break;
    }
    return r;
}

void btlfx_drawSpriteLine(u8 *arg0, SVECTOR *arg1, s32 arg2, s32 arg3) {
    s16 p[2];
    s16 q[2];
    s16 sxy[2];
    s32 t;
    s32 i;
    s32 d;
    s32 v;
    s32 r;
    s32 a;
    Unk800D3B6CEntry *e;
    Unk800D3B6C *h;

    gte_SetRotMatrix(g_btlfxEnv->viewMatrix);
    gte_SetTransMatrix(g_btlfxEnv->viewMatrix);
    gte_ldv0(arg1);
    h = (Unk800D3B6C *)arg0;
    e = h->unk18;
    gte_rtps_b();
    t = (arg2 << 12) / h->unk0;
    gte_stsxy(sxy);
    if (h->unk4 == 0) {
        btlfx_lerpVec(h->unk8, h->unkC, t, p);
        for (i = 0; i < h->unk2; i++, e++) {
            btlfx_lerpVec(sxy, p, e->unk6 * 2, q);
            if (arg2 >= e->unk8) {
                d = arg2 - e->unk8;
                if (d < e->unkA) {
                    v = d * arg3 / e->unkA;
                } else {
                    v = arg3;
                }
                btlfx_drawSpriteAtOt(8, (u8 *)h + e->unk0, q[0], q[1], arg2, 1, v);
            }
        }
    } else {
        r = h->unk10 + (((h->unk12 - h->unk10) * btlfx_ease(arg2, h->unk0, h->unk6)) >> 12);
        a = h->unk14 + (((h->unk16 - h->unk14) * btlfx_ease(arg2, h->unk0, h->unk7)) >> 12);
        p[0] = sxy[0] + ((math_cos(r) * a) >> 12);
        p[1] = sxy[1] + ((math_sin(r) * a) >> 12);
        for (i = 0; i < h->unk2; i++, e++) {
            btlfx_lerpVec(sxy, p, e->unk6 * 2, q);
            if (arg2 >= e->unk8) {
                d = arg2 - e->unk8;
                if (d < e->unkA) {
                    v = d * arg3 / e->unkA;
                } else {
                    v = arg3;
                }
                btlfx_drawSpriteAtOt(8, (u8 *)h + e->unk0, q[0], q[1], arg2, 1, v);
            }
        }
    }
}
