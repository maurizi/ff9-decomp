#include "ovl_0b.h"

s32 btlfx_align4(s32 arg0) {
    s32 v = arg0 & ~3;
    if (arg0 & 3) {
        v += 4;
    }
    return v;
}

s32 func_800CFC2C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 *arg5) {
    if (arg0 >= arg1 && arg1 + arg2 >= arg0) {
        *arg5 = (arg0 - arg1) * arg4 / arg2;
        if (arg3 < 0) {
            *arg5 = arg4 - *arg5;
        }
        return 1;
    }
    return 0;
}

void func_800CFCA8(SVECTOR *arg0, s32 arg1, s32 arg2) {
    RotMatrix33 m;

    func_800316D8(arg0, &m);
    gte_SetRotMatrix(&m);
    gte_ldtr0();
    btlfx_transformPoint(arg1, 0, arg2);
    gte_SetRotMatrix(g_btlfxEnv->unk14);
    gte_SetTransMatrix(g_btlfxEnv->unk14);
}

void func_800CFD78(s16 *arg0, s16 *arg1, s32 arg2, s16 *arg3) {
    MATRIX m;

    func_800316D8(arg1, &m);
    gte_SetRotMatrix(&m);
    m.t[0] = arg0[0];
    m.t[1] = arg0[1];
    m.t[2] = arg0[2];
    gte_SetTransMatrix(&m);
    btlfx_transformPoint(arg2, 0, (s32)arg3);
    gte_SetRotMatrix(g_btlfxEnv->unk14);
    gte_SetTransMatrix(g_btlfxEnv->unk14);
}

void func_800CFE88(MATRIX *arg0, s32 arg1, s32 arg2) {
    gte_SetRotMatrix(arg0);
    gte_SetTransMatrix(arg0);
    btlfx_transformPoint(arg1, 0, arg2);
    gte_SetRotMatrix(g_btlfxEnv->unk14);
    gte_SetTransMatrix(g_btlfxEnv->unk14);
}

void func_800CFF40(MATRIX *arg0, s16 *arg1) {
    s16 a[4];
    s32 b[2];

    gte_SetRotMatrix(arg0);
    gte_SetTransMatrix(arg0);
    b[1] = 0;
    b[0] = 0;
    btlfx_transformPoint(0x1000, 0, (s32)a);
    btlfx_aimAngles((s16 *)b, a, arg1);
    gte_SetRotMatrix(g_btlfxEnv->unk14);
    gte_SetTransMatrix(g_btlfxEnv->unk14);
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

void func_800D0130(SVECTOR *arg0, MATRIX *arg1) {
    MATRIX m;

    gte_SetRotMatrix(g_btlfxEnv->unk14);
    gte_SetTransMatrix(g_btlfxEnv->unk14);
    gte_ldv0(arg0);
    gte_rtv0tr();
    if (arg1 == NULL) {
        arg1 = &m;
    }
    gte_stlvnl(arg1->t);
    gte_SetTransMatrix(arg1);
}

void func_800D01D4(SVECTOR *arg0, VECTOR *arg1, MATRIX *arg2, s32 arg3) {
    MATRIX m;
    RotMatrix33 l;

    if (arg2 == NULL) {
        arg2 = &m;
    }
    func_800316D8(arg0, arg2);
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
    func_800316D8(&r, &m);
    gte_SetRotMatrix(&m);
    m.t[0] = arg0->vx;
    m.t[1] = arg0->vy;
    m.t[2] = arg0->vz;
    gte_SetTransMatrix(&m);
    btlfx_transformPoint(arg2, arg4, arg1);
    gte_SetRotMatrix(g_btlfxEnv->unk14);
    gte_SetTransMatrix(g_btlfxEnv->unk14);
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
    func_800316D8(&r, &m);
    gte_SetRotMatrix(&m);
    gte_ldtr0();
    b[1] = 0;
    b[0] = 0;
    btlfx_transformPoint(0, 0x1000, (s32)a);
    btlfx_aimAngles((s16 *)b, a, arg0);
    gte_SetRotMatrix(g_btlfxEnv->unk14);
    gte_SetTransMatrix(g_btlfxEnv->unk14);
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
    func_80030AF4(&r.xy, &m);
    gte_SetRotMatrix(&m);
    gte_ldtr0();
    *arg5 = btlfx_transformPolar(&v, arg3, arg4);
    gte_SetRotMatrix(g_btlfxEnv->unk14);
    gte_SetTransMatrix(g_btlfxEnv->unk14);
}

/* adds prim to ot with the given code/len word; tpage != 0xFF also adds a DR_TPAGE (semi-trans mode) */
void btlfx_linkPrimAbr(u32 code, u32 *ot, void *prim, s32 tpage) {
    DR_TPAGE *p;

    addPrimLenIn(ot, prim, code);
    if (tpage != 0xFF) {
        p = (DR_TPAGE *)g_btlfxEnv->unk24;
        g_btlfxEnv->unk24 = (u8 *)(p + 1);
        p->code[0] = ((tpage & 3) << 5) | 0xE1000200;
        addPrimLenIn(ot, p, 0x1000000);
    }
}

void btlfx_addPrim(u32 arg0, s32 arg1, void *arg2) {
    s32 z = (arg1 - g_btlfxEnv->unk28) >> 2;
    if ((u32)z < 0x1000) {
        addPrimLen(&g_btlfxEnv->unk20[z], arg2, arg0);
    }
}

void btlfx_addPrimAbr(s32 arg0, s32 arg1, void *arg2, s32 arg3) {
    u32 i = (arg1 - g_btlfxEnv->unk28) >> 2;
    if (i < 0x1000) {
        ((void (*)())btlfx_linkPrimAbr)(arg0, &g_btlfxEnv->unk20[i], arg2, arg3);
    }
}

DR_TPAGE *btlfx_allocDrMode(s32 arg0, s32 arg1) {
    DR_TPAGE *p = (DR_TPAGE *)g_btlfxEnv->unk24;
    g_btlfxEnv->unk24 = (u8 *)(p + 1);
    p->code[0] = arg0 | ((arg1 & 3) << 5) | 0xE1000200;
    return p;
}

void func_800D0940(void) {
    s32 n;
    Unk800D0940 *p;
    u32 *ot;
    if (g_btlfxEnv->unk24 != NULL) {
        D_800F7278 = n = 1;
        p = (Unk800D0940 *)g_btlfxEnv->unk24;
        ot = &p->unkC;
        g_btlfxEnv->unk24 = (u8 *)(p + 1);
        setlen(p, 2);
        p->unk4 = 0xE6000000;
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
    t = (TILE *)g_btlfxEnv->unk24;
    g_btlfxEnv->unk24 = (u8 *)(t + 8);
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
void func_800D0B18(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, CVECTOR *arg6, s32 arg7, s32 arg8,
                   s32 arg9, s32 arg10) {
    POLY_G4 *p;
    CVECTOR *ca;
    s32 r;
    s32 c;
    s32 y;
    s32 ph;

    ca = arg6;
    r = 0;
    p = (POLY_G4 *)g_btlfxEnv->unk24;
    g_btlfxEnv->unk24 = (u8 *)(p + arg2 * arg3);
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

void func_800D0E14(s32 arg0, s32 *arg1, s32 n, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 p,
                   u32 *c1, u32 *c2, s32 tp) {
    Unk800D50A0Ctx c;
    u8 *prim;
    s32 inv;

    if (p < 0) {
        p = 0x1000;
    }
    gte_DpqColorFar_nst(c1, p);
    prim = g_btlfxEnv->unk24;
    g_btlfxEnv->unk24 = prim + n * 0x34;
    gte_strgb(&c.unkC);
    gte_DpqColorFar_nst(c2, p);
    c.unk26 = (arg4 + arg6) << 8;
    c.unk24 = arg4 << 8;
    gte_strgb(&c.unk10);
    c.unk18 = (((tp & 3) << 5) | arg7) << 16;
    c.unk1C = arg8 << 16;
    c.unkC.b[3] = (tp == 0xFF) ? 0x3C : 0x3E;
    gte_SetRotMatrix(g_btlfxEnv->unk14);
    gte_SetTransMatrix(g_btlfxEnv->unk14);
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
        func_800D50A0(n, &c, prim, g_btlfxEnv->unk20);
    }
}

void func_800D0FFC(s32 arg0, s32 n, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 p, u32 *c1,
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
        prim = g_btlfxEnv->unk24;
        g_btlfxEnv->unk24 = prim + n * 0x34;
        c.unk26 = (arg3 + arg5) << 8;
        c.unk24 = arg3 << 8;
        s = math_sin(-g_btlfxEnv->unk2C->unk50 + 0x400);
        c.unk28 = (s & 0xFFFF) | (math_cos(-g_btlfxEnv->unk2C->unk50 + 0x400) << 16);
        c.unkC.b[3] = (tp != 0xFF) ? 0x3E : 0x3C;
        c.unk18 = (((tp & 3) << 5) | arg6) << 16;
        c.unk1C = arg7 << 16;
        c.unk14 = arg2;
        c.unk16 = arg4;
    } else {
        prim = g_btlfxEnv->unk24;
        g_btlfxEnv->unk24 = prim + n * 0x1C;
        c.unk28 = (tp & 3) << 5;
        c.unkC.b[3] = (tp == 0xFF) ? 0x50 : 0x52;
    }
    gte_strgb(&c.unk10);
    gte_SetRotMatrix(g_btlfxEnv->unk14);
    gte_SetTransMatrix(g_btlfxEnv->unk14);
    if (n != 0) {
        inv = 0x1000 / n;
        c.unk4 = arg0;
        c.unk20 = inv;
        c.unk8 = inv;
        c.unk0 = g_btlfxEnv->unk28;
        if (arg6 >= 0) {
            func_800D4D6C(n, &c, prim, g_btlfxEnv->unk20);
        } else {
            func_800D5568(n, &c, prim, g_btlfxEnv->unk20);
        }
    }
}

void func_800D12C4(s32 arg0, s32 n, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 p, u32 *c1,
                   u32 *c2, s32 tp) {
    Unk800D5C30Ctx c;
    u8 *prim;
    s32 inv;

    if (p < 0) {
        p = 0x1000;
    }
    gte_DpqColorFar_nst(c1, p);
    prim = g_btlfxEnv->unk24;
    g_btlfxEnv->unk24 = prim + n * 0x34;
    gte_strgb(&c.unkC);
    gte_DpqColorFar_nst(c2, p);
    c.unk26 = (arg3 + arg5) << 8;
    c.unk24 = arg3 << 8;
    c.unk18 = (((tp & 3) << 5) | arg6) << 16;
    c.unk1C = arg7 << 16;
    c.unkC.b[3] = (tp == 0xFF) ? 0x3C : 0x3E;
    gte_strgb(&c.unk10);
    gte_SetRotMatrix(g_btlfxEnv->unk14);
    gte_SetTransMatrix(g_btlfxEnv->unk14);
    if (n != 0) {
        inv = 0x1000 / n;
        c.unk4 = arg0;
        c.unk14 = arg2;
        c.unk16 = arg4;
        c.unk20 = inv;
        c.unk8 = inv;
        c.unk0 = g_btlfxEnv->unk28;
        func_800D5C30(n, &c, prim, g_btlfxEnv->unk20);
    }
}

void func_800D1494(u8 *p, s32 arg1, s32 *arg2, s32 frame, s32 scale, s32 arg5, s32 tp, u8 *out, s32 mode) {
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
        func_800D12C4((s32)out, n, a, b, c, d, x, y, arg5, data, data + 1, tp);
    } else if (mode == 1) {
        func_800D0E14((s32)out, arg2, n, a, b, c, d, x, y, arg5, data, data + 1, tp);
    } else {
        func_800D0FFC((s32)out, n, a, b, c, d, x, y, arg5, data, data + 1, tp);
    }
}

void func_800D1680(u8 *arg0, SVECTOR *arg1, SVECTOR *arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8) {
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
    gte_SetRotMatrix(g_btlfxEnv->unk14);
    gte_SetTransMatrix(g_btlfxEnv->unk14);
    gte_ldv0(arg1);
    prim = g_btlfxEnv->unk24;
    g_btlfxEnv->unk24 = prim + c.unk14 * 40;
    gte_rtv0tr_nn();
    func_800316D8(arg2, &m);
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
    func_800D4478(arg0, &c, prim, g_btlfxEnv->unk20);
}

void func_800D198C(u8 *arg0, SVECTOR *arg1, s32 arg2, s32 arg3, s32 arg4) {
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
    gte_SetRotMatrix(g_btlfxEnv->unk14);
    gte_SetTransMatrix(g_btlfxEnv->unk14);
    gte_ldv0(arg1);
    arg0 += off;
    c.unk14 &= 0x3FFF;
    gte_rtps_nn();
    prim = g_btlfxEnv->unk24;
    g_btlfxEnv->unk24 = prim + c.unk14 * 20;
    gte_stsxy(&sxy);
    gte_stszotz(&z);
    c.unk0 |= (arg3 == 0xFF) ? 0x64000000 : 0x66000000;
    c.unkC = (arg3 & 3) << 5;
    c.unk4 = sxy.vx;
    c.unk8 = sxy.vy;
    c.unk18 = g_btlfxEnv->unk24;
    z = (z - g_btlfxEnv->unk28) >> 2;
    if ((u32)z < 0x1000) {
        g_btlfxEnv->unk24 = func_800D45FC(arg0, &c, prim, &g_btlfxEnv->unk20[z]);
    }
}

void func_800D1BC4(u32 arg0, u8 *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
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
    prim = g_btlfxEnv->unk24;
    g_btlfxEnv->unk24 = prim + c.unk14 * 20;
    c.unk0 |= (arg5 == 0xFF) ? 0x64000000 : 0x66000000;
    c.unkC = (arg5 & 3) << 5;
    c.unk4 = arg2;
    c.unk8 = arg3;
    c.unk18 = g_btlfxEnv->unk24;
    if (arg0 < 0x1000) {
        g_btlfxEnv->unk24 = func_800D45FC(arg1, &c, prim, &g_btlfxEnv->unk20[arg0]);
    }
}

void func_800D1D74(u8 *arg0, SVECTOR *arg1, SVECTOR *arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
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
    gte_SetRotMatrix(g_btlfxEnv->unk14);
    gte_SetTransMatrix(g_btlfxEnv->unk14);
    gte_ldv0(arg1);
    prim = g_btlfxEnv->unk24;
    g_btlfxEnv->unk24 = prim + c.unk1C * 40;
    gte_rtv0tr_nn();
    func_800316D8(arg2, &m);
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
        func_800D3ED4(arg0, &c, prim, g_btlfxEnv->unk20);
    }
}

void func_800D1FFC(u8 *arg0, SVECTOR *arg1, SVECTOR *arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
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
    gte_SetRotMatrix(g_btlfxEnv->unk14);
    gte_SetTransMatrix(g_btlfxEnv->unk14);
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
    prim = g_btlfxEnv->unk24;
    g_btlfxEnv->unk24 = prim + size * c.unk1C;
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
            func_800D40CC(arg0, &c, prim, &g_btlfxEnv->unk20[z]);
        } else if (size == 0xC) {
            func_800D437C(arg0, &c, prim, &g_btlfxEnv->unk20[z]);
        } else {
            func_800D4228(arg0, &c, prim, &g_btlfxEnv->unk20[z]);
        }
        addPrimLenIn(&g_btlfxEnv->unk20[z], tp, 0x1000000);
    }
}

void func_800D23D8(u8 *arg0, SVECTOR *arg1, SVECTOR *arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
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
    gte_SetRotMatrix(g_btlfxEnv->unk14);
    gte_SetTransMatrix(g_btlfxEnv->unk14);
    gte_ldv0(arg1);
    nf = s1[1];
    f = 0;
    if (n >= 2) {
        f = arg4 * (nf - 1) / (n - 1);
    }
    gte_rtv0tr_nn();
    func_800316D8(arg2, &m);
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
        prim = g_btlfxEnv->unk24;
        g_btlfxEnv->unk24 = prim + w * cnt * 40;
        ((u8 *)&c.unk0)[3] = (arg5 != 0xFF) ? 0x2E : 0x2C;
        c.unk24 = cnt;
        c.unk1C = w;
        c.unk18 = g_btlfxEnv->unk28;
        func_800D4768(k, &c, prim, g_btlfxEnv->unk20);
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
        prim = g_btlfxEnv->unk24;
        g_btlfxEnv->unk24 = prim + w * cnt * 44;
        d.unk0 = (arg5 != 0xFF) ? 0x3A000000 : 0x38000000;
        d.unk4 = arg6;
        d.unk24 = cnt;
        d.unk28 = (arg5 & 3) << 5;
        d.unk1C = w;
        d.unk18 = g_btlfxEnv->unk28;
        func_800D4A30(m2, &d, prim, g_btlfxEnv->unk20);
        break;
    }
}

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
    gte_SetRotMatrix(g_btlfxEnv->unk14);
    gte_SetTransMatrix(g_btlfxEnv->unk14);
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
        c1.unk4 = g_btlfxEnv->unk28;
        prim = g_btlfxEnv->unk24;
        g_btlfxEnv->unk24 = prim + 0x28;
        func_800D57D0((u8 *)arg0, &c1, prim, g_btlfxEnv->unk20);
    } else {
        c2.unkC = (arg5 & 3) << 5;
        c2.unk14 = t;
        c2.unk10 = z;
        c2.unk0 = (arg5 != 0xFF) ? 0x32000000 : 0x30000000;
        c2.unk8 = arg6;
        c2.unk4 = g_btlfxEnv->unk28;
        prim = g_btlfxEnv->unk24;
        g_btlfxEnv->unk24 = prim + 0x5C;
        func_800D59D0((u8 *)arg0, &c2, prim, g_btlfxEnv->unk20);
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

    p = (POLY_FT4 *)g_btlfxEnv->unk24;
    g_btlfxEnv->unk24 = (u8 *)(p + 1);
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

void func_800D2DBC(u8 *arg0, s32 x, s32 y, s32 frame, s32 tp, s32 p) {
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
    prim = g_btlfxEnv->unk24;
    mode = (u32 *)(prim + num * 20);
    g_btlfxEnv->unk24 = (u8 *)(mode + 2);
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

void func_800D3030(BtlBgLayers *arg0, s32 arg1) {
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

    da = (DR_AREA *)g_btlfxEnv->unk24;
    n = arg0->unk0;
    w = arg0->unk2;
    h = arg0->unk3;
    x = arg0->unk4;
    y = arg0->unk6;
    g_btlfxEnv->unk24 = (u8 *)(da + 2);
    r.x = x;
    r.y = y;
    r.w = w;
    r.h = h;
    SetDrawArea(da, &r);
    ((XY32 *)&r)[0].w = ((XY32 *)&g_btlfxEnv->unk1C->clip)[0].w;
    ((XY32 *)&r)[1].w = ((XY32 *)&g_btlfxEnv->unk1C->clip)[1].w;
    SetDrawArea(&da[1], &r);
    i = 0;
    dofs = (DR_OFFSET *)g_btlfxEnv->unk24;
    g_btlfxEnv->unk24 = (u8 *)(dofs + 2);
    ofs[0] = x;
    ofs[1] = y;
    SetDrawOffset(dofs, ofs);
    SetDrawOffset(&dofs[1], g_btlfxEnv->unk1C->ofs);
    tp = (DR_TPAGE *)g_btlfxEnv->unk24;
    g_btlfxEnv->unk24 = (u8 *)(tp + 2);
    tp[0].code[0] = 0xE1000000;
    tp[1].code[0] = 0xE1000200;
    btlfx_addPrim(0x01000000, 0xFFE, &tp[1]);
    btlfx_addPrim(0x02000000, 0xFFE, &da[1]);
    btlfx_addPrim(0x02000000, 0xFFE, &dofs[1]);
    offs = arg0->unk8;
    for (; i < n; i++) {
        L = (BtlBgLayer *)((u8 *)arg0 + offs[i]);
        if (L->unkC != 0) {
            func_800E651C((u8 *)arg0 + L->unkC, &col, arg1 % L->unkA);
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
                    func_800D2DBC((u8 *)arg0 + fd->unk0 + 4, fd->unk8, fd->unkA, e, L->unk8, f);
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
    t = (TILE *)g_btlfxEnv->unk24;
    g_btlfxEnv->unk24 = (u8 *)(t + 1);
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

s32 func_800D3A6C(s32 arg0, s32 arg1, s32 arg2) {
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

void func_800D3B6C(u8 *arg0, SVECTOR *arg1, s32 arg2, s32 arg3) {
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

    gte_SetRotMatrix(g_btlfxEnv->unk14);
    gte_SetTransMatrix(g_btlfxEnv->unk14);
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
                func_800D1BC4(8, (u8 *)h + e->unk0, q[0], q[1], arg2, 1, v);
            }
        }
    } else {
        r = h->unk10 + (((h->unk12 - h->unk10) * func_800D3A6C(arg2, h->unk0, h->unk6)) >> 12);
        a = h->unk14 + (((h->unk16 - h->unk14) * func_800D3A6C(arg2, h->unk0, h->unk7)) >> 12);
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
                func_800D1BC4(8, (u8 *)h + e->unk0, q[0], q[1], arg2, 1, v);
            }
        }
    }
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D3ED4);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D40CC);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D4228);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D437C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D4478);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D45FC);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D4768);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D4A08);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D4A30);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D4D6C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D50A0);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D5568);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D57D0);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D597C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D59D0);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D5C30);

void btlfx_reset(void) {
    g_btlfxState.unk0 = -1;
    g_btlfxState.unk22 = 0;
    g_btlfxState.unk24 = 0;
}

s32 btlfx_resolveFirstUse(s32 arg0) {
    s32 id;
    s32 i;
    u8 *p;
    s32 bit;
    GameState *g;

    id = arg0;
    if (func_800E8498(0) >= 0x385) {
        for (p = D_800F727C[0]; *p != 0xFF; p += 2) {
            if (p[0] == arg0) {
                id = p[1];
                break;
            }
        }
    }
    bit = 1;
    g = g_gameState;
    for (i = 0; i < 16; i++, bit <<= 1) {
        if (g_btlfxFirstUsePairs[i][0] == id && !(g->unk1C->firstUseFxSeen & bit)) {
            id = g_btlfxFirstUsePairs[i][1];
            g->unk1C->firstUseFxSeen |= bit;
            break;
        }
        if (g_btlfxFirstUsePairs[i][1] == id) {
            if (!(g->unk1C->firstUseFxSeen & bit)) {
                g->unk1C->firstUseFxSeen |= bit;
            }
            break;
        }
    }
    return id;
}

s32 btlfx_start(s32 arg0) {
    if (g_btlfxState.unk0 != -1) {
        return -1;
    }
    g_btlfxState.unk0 = 0;
    g_btlfxState.unk2 = btlfx_resolveFirstUse(arg0);
    g_btlfxState.unkF = 0;
    g_btlfxState.unk23 = 0;
    g_btlfxState.unk4 = 0;
    g_btlfxState.unk10.w = 0;
    g_btlfxState.unk6 = 0;
    g_btlfxState.unk8 = 0;
    g_btlfxState.unk18 = 0;
    g_btlfxState.unk22 = 0;
    g_btlfxState.unk24 = 0;
    g_btlfxState.unk25 = 0;
    g_btlfxState.unk1C = 0x8000;
    func_800C30B8();
    g_btlfxEnv->unk30 = g_gameState->unk1C->unk8E4.battle->unk11C0->unkC;
    return 0;
}

void btlfx_retryOp(void) {
    g_btlfxState.unk20 = 1;
    g_btlfxState.unk18 -= 3;
}

u8 *btlfx_findScript(s32 arg0) {
    s32 i;
    u8 *p;
    s32 n;
    for (i = 0, p = (u8 *)0x801E7700; i < 0x200; i++) {
        n = p[0];
        if (p[1] & 0x80) {
            n += 0x100;
        }
        if (n == arg0) {
            break;
        }
        p += (p[1] & 0x7F) * 3 + 2;
    }
    return p + 2;
}

void btlfx_swapVramPages(s32 arg0, u32 *arg1, s32 arg2) {
    RECT r;
    s32 i;
    s32 bit = 1;
    u32 *p;
    if (arg0 != 2) {
        p = (u32 *)(g_gameState->unk1C->unk8E4.battle->unk153C[0] + 0x50000);
    } else {
        arg2 = 1 << arg1[0x1FFF];
        p = arg1;
    }
    r.w = 0x40;
    r.h = 0x80;
    func_800D0940();
    for (i = 0; i < 4; i++) {
        if (bit & arg2) {
            p -= 0x2000;
            r.x = (i >> 1) * 64 + 0x380;
            r.y = (i & 1) << 7;
            if (arg0 == 0) {
                StoreImage(&r, p);
            } else {
                LoadImage(&r, p);
            }
        }
        if (i & 1) {
            bit <<= 1;
        }
    }
}

void btlfx_callScript(s32 arg0) {
    D_800FEC38 = 1;
    D_800FEC3C = g_btlfxState.unk18;
    g_btlfxState.unk18 = btlfx_findScript(arg0);
}

void btlfx_runScript(void) {
    RECT r;
    u8 *p;
    s16 op;
    s16 a;
    s16 b;
    s32 cont;
    u16 hdr;
    s32 n;
    s32 k;
    Unk800FEC40 *d = &g_btlfxState;

    if (d->unk22 != 0 && func_800AB410(0) == 0) {
        d->unk22 = 0;
        func_800E89D0();
    }
    if (d->unk24 != 0 && func_800AB3E4(d->unk24 - 1, 0) == 0) {
        d->unk24 = 0;
        func_800E8A80(0x10);
    }
    switch (d->unk0) {
    case 0:
        r.x = 0x300;
        r.y = 0x140;
        r.w = 0x40;
        r.h = 0x10;
        func_800D0940();
        StoreImage(&r, (u32 *)0x801E7700);
        D_800FEC38 = 0;
        d->unk0 = 1;
        break;
    case 1:
        hdr = ((u16 *)0x801E7700)[d->unk2];
        d->unk1C = hdr & 0xFFFE;
        g_btlfxEnv->unkC->unk40 &= ~8;
        if (hdr & 0xC00) {
            btlfx_swapVramPages(0, NULL, (hdr & 0xC00) >> 10);
        }
        if (hdr & 0x8000) {
            r.x = 0x300;
            r.y = 0x150;
            r.w = 0x40;
            r.h = 0xB0;
            func_800D0940();
            StoreImage(&r, (u32 *)0x801E7700);
            d->unk0 = 2;
            d->unk1C &= ~8;
            d->unk1C &= ~4;
            break;
        }
        d->unk21 = 0;
        func_800E9168();
        btlcam_beginEffect();
        if (!(g_btlfxEnv->unkC->unk40 & 1)) {
            func_800E9290(d->unk1C);
        }
        if (btlfx_loadFile(d->unk2) == 0) {
            k = (u8)d->unk1C >> 4;
            if (k == 1) {
                g_btlfxEnv->unkC->unk40 |= 8;
            }
            btlfx_startCasterTask(k);
            d->unk18 = D_800FF068;
            d->unk0 = 3;
        }
        break;
    case 3:
        if (g_btlfxEnv->unk4->unkE == 3) {
            d->unk0 = 4;
            g_btlfxEnv->unk2C->unk5C |= 2;
        }
        break;
    case 2:
        d->unk18 = btlfx_findScript(d->unk2);
        d->unk0 = 4;
        func_800E8D14();
        break;
    case 4:
        /* padded: a zero word follows the first jump table */
        __asm__(".section .rodata\n\t.word 0\n\t.section .text");
        cont = 1;
        do {
            if (d->unk4 != 0) {
                if (--d->unk4 > 0) {
                    return;
                }
                d->unk4 = 0;
            }
            p = d->unk18;
            op = p[0];
            a = p[1];
            b = p[2];
            d->unk18 = p + 3;
            if (op < 0x20) {
                switch (op) {
                case 0:
                    cont = 0;
                    d->unk18 -= 3;
                    if (func_800E8CE4() == 0) {
                        func_800E8C84(1);
                    }
                    if (g_btlfxTasks.unk0 == 0) {
                        if (d->unk1C & 1) {
                            if (d->unk21 == 0) {
                                btlcam_endEffect();
                                func_800E3C58();
                                func_800E970C();
                                func_800E954C();
                                btlfx_reloadCommonData(0);
                                d->unk21 = 1;
                            } else if (btlfx_reloadCommonData(1) != 0) {
                                d->unk1C &= ~1;
                            }
                        } else {
                            d->unk0 = -1;
                            g_btlfxEnv->unkC = NULL;
                            btlcam_endEffect();
                            func_800E3C58();
                            func_800E970C();
                            func_800E954C();
                            func_80054F80();
                        }
                    }
                    break;
                case 1:
                    if (a == 0) {
                        d->unk4 = b;
                        cont = 0;
                    } else if (d->unk10.b[b] == 0) {
                        cont = 0;
                        d->unk18 -= 3;
                        d->unk10.b[b] = 0;
                    }
                    break;
                case 2:
                    d->unk10.b[a] = b;
                    break;
                case 3:
                    func_800DA5C4();
                    break;
                case 4:
                    if (g_btlfxEnv->unk4->unkE < a) {
                        cont = 0;
                        btlfx_retryOp();
                    }
                    break;
                case 5:
                    d->unk6 = a;
                    d->unk8 = btlfx_findFileSlot(a);
                    break;
                case 6:
                    switch (a) {
                    case 2:
                        if (g_btlfxEnv->unk4->unk53 == 0) {
                            cont = 0;
                            btlfx_retryOp();
                        }
                        break;
                    case 1:
                        if (g_btlfxEnv->unk4->unk52 != 2) {
                            cont = 0;
                            btlfx_retryOp();
                        }
                        break;
                    case 0:
                        if (g_btlfxEnv->unk4->unk50 != 2) {
                            cont = 0;
                            btlfx_retryOp();
                        }
                        break;
                    }
                    break;
                case 8:
                    if (btlfx_isEnemyUnit(0x10) != 0) {
                        g_btlfxState.unk23 = 1;
                        if (!(g_btlfxEnv->unkC->unk40 & 0x10) && d->unkF == 0) {
                            cont = 0;
                            d->unk18 -= 3;
                        }
                    }
                    break;
                case 9:
                    if (func_800DA6B4(a, b)) {
                        cont = 0;
                        btlfx_retryOp();
                    }
                    break;
                case 10:
                    if (func_800DA634(0)) {
                        cont = 0;
                        btlfx_retryOp();
                    }
                    break;
                case 11:
                    if (a != 0) {
                        if (d->unk22 != 0) {
                            d->unk18 -= 3;
                        }
                        cont = 0;
                    } else {
                        func_800AB410(1);
                        d->unk22 = 1;
                        cont = 0;
                    }
                    break;
                case 12:
                    if (g_btlfxEnv->unk4->unk68 != 2) {
                        cont = 0;
                        d->unk18 -= 3;
                    }
                    break;
                case 13:
                    if (a != 0) {
                        if (d->unk24 != 0) {
                            d->unk18 -= 3;
                        }
                        cont = 0;
                    } else {
                        n = func_800E7D18(b);
                        func_800AB3E4(n, 1);
                        n++;
                        d->unk24 = n;
                        cont = 0;
                    }
                    break;
                case 7:
                    switch (a) {
                    case 0:
                        btlfx_swapVramPages(1, NULL, (d->unk1C & 0xC00) >> 10);
                        break;
                    case 1:
                        btlfx_swapVramPages(2, (u32 *)btlfx_getFileData(-1, b), 0);
                        break;
                    case 2:
                        if (g_btlfxEnv->unk4->unk69 < b) {
                            cont = 0;
                            btlfx_retryOp();
                        }
                        break;
                    }
                    break;
                case 14:
                    if (D_800FEC38 != 0) {
                        D_800FEC38 = 0;
                        d->unk18 = D_800FEC3C;
                    }
                    break;
                case 15:
                    if (b == 0) {
                        func_800E86E0(D_800FE530, (s32 *)btlfx_getFileData(d->unk8, a), 0x708);
                        func_80054A28(D_800FE530);
                        func_80054E88(0, 0, 1);
                        func_80054E88(0, 1, 1);
                    } else {
                        func_80054FE0(0);
                        func_80054E88(1, 0, 1);
                        func_80054E88(1, 1, 1);
                    }
                    func_80054FB8(1);
                    break;
                case 20:
                    break;
                }
            } else {
                if (op < 0x80) {
                    btlfx_spawnTask(op - 0x20, d->unk8, a, b);
                } else {
                    btlfx_spawnTask(op, d->unk8, a, b);
                }
            }
        } while (cont);
        break;
    }
}

void btlfx_initTasks(void) {
    s32 i;
    Unk80100C78Entry *p;
    g_btlfxTasks.unk0 = 0;
    btlfx_heapInit(&D_800FF470, 0x1800);
    p = g_btlfxTasks.unk4;
    for (i = 10; i >= 0; i--) {
        p->unk4.w = 0;
        p++;
    }
}

s32 btlfx_allocTaskMem(Unk80100C78Entry *arg0, s32 arg1) {
    s32 r = 0;
    s32 size = arg1 + 4;
    s32 i = 0;
    Heap *h = g_btlfxLoader.unk3C[arg0->unk8];
    for (; i < 2; i++) {
        if (arg0->unkA != 0 && h != NULL) {
            if (btlfx_heapWrapIfFull(h, size) == 0) {
                r = btlfx_heapAlloc(h, size);
                break;
            }
            arg0->unkA = 0;
        } else {
            if (btlfx_heapWrapIfFull(&D_800FF470, size) == 0) {
                r = btlfx_heapAlloc(&D_800FF470, size);
                break;
            }
            arg0->unkA = 1;
        }
    }
    return r;
}

void btlfx_spawnTask(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    Unk80100C78Entry *e;
    s32 i;
    EffFn *tbl;
    s32 idx;
    Unk80100C78 *d = &g_btlfxTasks;
    EffInit init;
    s32 args[2];

    e = d->unk4;
    i = 0;
loop:
    if (e->unk4.h != 0) {
        i++;
        e++;
        if (i < 11) {
            goto loop;
        }
    }
    d->unk0++;
    e->unk4.h = 1;
    e->unkA = 1;
    e->unk0 = 0;
    e->unk8 = arg1;
    if (arg0 >= 0x80) {
        e->unk14 = D_80100C70[arg1][arg0 - 0x80];
    } else {
        if (arg0 < 0x30) {
            e->unk14 = g_btlfxResidentOps[arg0];
        } else {
            e->unk14 = g_btlfxUnitOps[arg0 - 0x30];
        }
    }
    init.unk4 = 0;
    init.unk0 = 0;
    e->unk10 = 0;
    e->unkC = 0;
    if (e->unk14(0, 0, 0, 0, &init) == 0) {
        if (init.unk4 != 0) {
            e->unk10 = (void *)btlfx_allocTaskMem(e, btlfx_poolSize(init.unk8, init.unk4));
            btlfx_poolInit(e->unk10, e, init.unk8, init.unk4, init.unk14);
        }
        if (init.unk0 != 0) {
            e->unkC = (void *)btlfx_allocTaskMem(e, btlfx_align4(init.unk0));
        }
    }
    args[0] = arg2;
    args[1] = arg3;
    if (e->unk14(1, e->unkC, g_btlfxEnv, e, args) != 0) {
        e->unk4.h = 0;
        d->unk0--;
        if (e->unkC != 0) {
            btlfx_heapFree(e->unkC);
        }
        if (e->unk10 != 0) {
            btlfx_heapFree(e->unk10);
        }
    }
}

s32 btlfx_updateTasks(void) {
    Unk80100C78 *d = &g_btlfxTasks;
    Unk80100C78Entry *e;
    s32 n;
    s32 i;
    s32 r;

    e = d->unk4;
    n = 0;
    for (i = 0; i < 11; i++, e++) {
        if (e->unk4.h != 0) {
            n++;
            g_btlfxEnv->unk28 = 0;
            if (e->unk14(2, e->unkC, g_btlfxEnv, e, NULL) != 0) {
                e->unk4.h = 2;
            } else {
                e->unk0++;
            }
            if (e->unk10 != NULL) {
                r = btlfx_poolUpdate(e->unk10);
            } else {
                r = 0;
            }
            if (r == 0 && e->unk4.h == 2) {
                e->unk4.h = 0;
                d->unk0--;
                if (e->unkC != NULL) {
                    btlfx_heapFree(e->unkC);
                }
                if (e->unk10 != NULL) {
                    btlfx_heapFree(e->unk10);
                }
            }
        }
    }
    return n;
}

s32 btlfx_moveUnitArc(s32 arg0, s16 *arg1, s32 arg2, s32 arg3, s32 arg4, s16 *arg5) {
    s16 sp10[4];
    s32 t;
    if (arg2 == 1) {
        btlfx_getUnitPos(arg0, arg5);
    }
    t = (arg2 << 12) / arg3;
    btlfx_lerpVec(arg5, arg1, t, sp10);
    if (arg4 != 0) {
        sp10[1] -= (math_sin(t >> 1) * arg4) >> 12;
    }
    btlfx_setUnitPos(arg0, sp10);
    return arg2 >= arg3;
}

s32 btlfx_moveUnitLinear(s32 arg0, s16 *arg1, s32 arg2, s32 arg3, s16 *arg4, s32 arg5) {
    s16 sp10[4];
    s32 t;
    if (arg2 == 0) {
        switch (arg5) {
        case 0:
            btlfx_getUnitHomePos(arg0, arg4);
            break;
        case 1:
            btlfx_getUnitPos(arg0, arg4);
            break;
        default:
            return 1;
        }
    }
    t = ((arg2 + 1) << 12) / arg3;
    if (t > 0x1000) {
        t = 0x1000;
    }
    btlfx_lerpVec(arg4, arg1, t, sp10);
    btlfx_setUnitPos(0x10, sp10);
    return arg2 >= arg3 - 1;
}

s32 btlfx_opSetParamsA(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 *arg4) {
    if (arg0 == 1) {
        D_80100D88.unk0 = arg4[0];
        D_80100D88.unk1 = arg4[1];
    }
    return 1;
}

s32 btlfx_opSetParamsB(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 *arg4) {
    if (arg0 == 1) {
        D_80100D88.unk2 = arg4[0];
        D_80100D88.unk3 = arg4[4];
    }
    return 1;
}

s32 btlfx_opSwingSound(s32 arg0, u8 *arg1, s32 arg2, s32 arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 4;
    } else if (arg0 == 1) {
        arg1[0] = arg4[0];
        arg1[1] = arg4[1];
        if (btlfx_isPartyUnit(arg1[0]) == 0 || arg1[1] == arg0) {
            return 1;
        }
        return 0;
    } else {
        return btlfx_stepSwingSound(arg1, arg2, arg3);
    }
    return 0;
}

s32 btlfx_opHitSound(s32 arg0, u8 *arg1, s32 arg2, s32 arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 4;
    } else if (arg0 == 1) {
        arg1[0] = arg4[0];
        arg1[1] = arg4[1];
        arg1[2] = D_80100D88.unk0;
        arg1[3] = D_80100D88.unk1;
        if (btlfx_isPartyUnit(arg1[0]) == 0) {
            return 1;
        }
        if (arg1[3] == arg0) {
            return 1;
        }
    } else {
        return btlfx_stepHitSound(arg1, arg2, arg3);
    }
    return 0;
}

s32 func_800D7578(s32 arg0, u8 *arg1, s32 arg2, s32 arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 4;
    } else if (arg0 != 1) {
        return func_800D8D08(arg1, arg2, arg3);
    } else {
        arg1[0] = arg4[0];
        arg1[1] = arg4[1];
    }
    return 0;
}

s32 btlfx_opPlayMotion(s32 arg0, u8 *arg1, s32 arg2, s32 arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 4;
    } else if (arg0 != 1) {
        return btlfx_stepPlayMotion(arg1, arg2, arg3);
    } else {
        arg1[0] = arg4[0];
        arg1[1] = arg4[1];
    }
    return 0;
}

s32 btlfx_opPlayMotionWait(s32 arg0, Unk800D8EA4 *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 8;
    } else if (arg0 != 1) {
        return btlfx_stepPlayMotionWait(arg1, arg2, arg3);
    } else {
        arg1->unk0 = arg4[0];
        arg1->unk1 = arg4[1];
        D_800FEC54 = 0;
    }
    return 0;
}

s32 func_800D76DC(s32 arg0, Unk800D8EA4 *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 8;
    } else if (arg0 != 1) {
        return func_800D8E10(arg1, arg2, arg3);
    } else {
        arg1->unk0 = arg4[0];
        arg1->unk1 = arg4[1];
        arg1->unk2 = D_80100D88.unk0;
        D_800FEC54 = 0;
    }
    return 0;
}

s32 func_800D7768(s32 arg0, Unk800D8EA4 *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 8;
    } else if (arg0 != 1) {
        return func_800D8EA4(arg1, arg2, arg3);
    } else {
        arg1->unk0 = arg4[0];
        arg1->unk1 = arg4[1];
        D_800FEC54 = 0;
    }
    return 0;
}

s32 func_800D77E4(s32 arg0, Unk800D8EA4 *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 8;
    } else if (arg0 != 1) {
        return func_800D8F1C(arg1, arg2, arg3);
    } else {
        arg1->unk0 = arg4[0];
        arg1->unk1 = arg4[1];
        arg1->unk2 = D_80100D88.unk0;
        D_800FEC54 = 0;
    }
    return 0;
}

s32 func_800D7870(s32 arg0, Unk800D8EA4 *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 8;
    } else if (arg0 != 1) {
        return func_800D8FAC(arg1, arg2, arg3);
    } else {
        arg1->unk0 = arg4[0];
        arg1->unk1 = arg4[1];
        D_800FEC54 = 0;
    }
    return 0;
}

s32 btlfx_opResetMotion(s32 arg0, u8 *arg1, s32 arg2, s32 arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 8;
    } else if (arg0 != 1) {
        return btlfx_stepResetMotion(arg1, arg2, arg3);
    } else {
        *arg1 = *arg4;
    }
    return 0;
}

s32 func_800D7948(s32 arg0, u8 *arg1, s32 arg2, Unk800F78A40 *arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 4;
    } else if (arg0 != 1) {
        return func_800D905C(arg1, arg2, arg3);
    } else {
        arg1[0] = arg4[0];
        arg1[1] = arg4[1];
    }
    return 0;
}

s32 func_800D79BC(s32 arg0, Unk800D8EA4 *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 8;
    } else if (arg0 != 1) {
        return func_800D908C(arg1, arg2, arg3);
    } else {
        arg1->unk0 = arg4[0];
        arg1->unk1 = arg4[1];
        D_800FEC54 = 0;
    }
    return 0;
}

s32 func_800D7A38(s32 arg0, u8 *arg1, s32 arg2, s32 arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 4;
    } else if (arg0 != 1) {
        return func_800D9110(arg1, arg2, arg3);
    } else {
        *arg1 = *arg4;
    }
    return 0;
}

s32 func_800D7A94(s32 arg0, u8 *arg1, s32 arg2, s32 arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 0x18;
    } else if (arg0 == 1) {
        arg1[0] = arg4[0];
        arg1[1] = arg4[1];
        if (btlfx_isSameUnit(arg1[0], arg1[1]) != 0) {
            D_800FEC55 = arg0;
            return 1;
        }
        D_800FEC55 = 0;
    } else {
        return func_800D9134((Unk800D9134 *)arg1, arg2, (s32 *)arg3);
    }
    return 0;
}

s32 btlfx_opRunHome(s32 arg0, Unk800D9260 *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 0x10;
    } else if (arg0 != 1) {
        return btlfx_stepRunHome(arg1, arg2, arg3);
    } else {
        arg1->unk0 = arg4[0];
        arg1->unk1 = arg4[1];
        D_800FEC55 = 0;
    }
    return 0;
}

s32 func_800D7BA8(s32 arg0, Unk800D7C9C *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 0x1C;
    } else if (arg0 == 1) {
        arg1->unk0 = 0x10;
        arg1->unk1 = arg4[0];
        arg1->unk8 = arg4[1];
        arg1->unk4 = D_80100D88.unk0 | (D_80100D88.unk1 << 8);
        arg1->unk6 = D_80100D88.unk2 | (D_80100D88.unk3 << 8);
        if (btlfx_isSameUnit(arg1->unk0, arg1->unk1) != 0) {
            D_800FEC55 = arg0;
            return 1;
        }
        if (btlfx_isPartyUnit(arg1->unk1) == 0) {
            return 1;
        }
        D_800FEC55 = 0;
    } else {
        return func_800D9350(arg1, arg2, arg3);
    }
    return 0;
}

s32 btlfx_opJumpForward(s32 arg0, Unk800D7C9C *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    s32 v;
    if (arg0 == 0) {
        *arg4 = 0x1C;
    } else if (arg0 == 1) {
        arg1->unk0 = 0x10;
        arg1->unk1 = arg4[0];
        arg1->unk2 = arg4[1];
        arg1->unk3 = 1;
        arg1->unk4 = D_80100D88.unk0 | (D_80100D88.unk1 << 8);
        if ((v = arg1->unk2) == arg0 && btlfx_isPartyUnit(arg1->unk1) != 0) {
            D_800FEC55 = v;
            return 1;
        }
        D_800FEC55 = 0;
    } else {
        return btlfx_stepJumpForward(arg1, arg2, arg3);
    }
    return 0;
}

s32 btlfx_opJumpHome(s32 arg0, Unk800D955C *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    s32 v;
    if (arg0 == 0) {
        *arg4 = 0x18;
    } else if (arg0 == 1) {
        arg1->unk0 = 0x10;
        arg1->unk1 = arg4[0];
        arg1->unk2 = arg4[1];
        arg1->unk3 = 1;
        if ((v = arg1->unk2) == arg0 && btlfx_isPartyUnit(arg1->unk1) != 0) {
            D_800FEC55 = v;
            return 1;
        }
        D_800FEC55 = 0;
    } else {
        return btlfx_stepJumpHome(arg1, arg2, arg3);
    }
    return 0;
}

s32 func_800D7E14(s32 arg0, Unk800D7E14 *arg1, s32 arg2, s32 arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 0x10;
    } else if (arg0 != 1) {
        return func_800D960C(arg1, arg2, arg3);
    } else {
        arg1->unk0 = arg4[0];
        arg1->unk1 = arg4[1];
        arg1->unk4 = D_80100D88.unk0;
        do {
            D_800FEC55 = 0;
        } while (0);
    }
    return 0;
}

s32 func_800D7EA0(s32 arg0, Unk800D7C9C *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 0x1C;
    } else if (arg0 == 1) {
        arg1->unk0 = 0x10;
        arg1->unk1 = arg4[0];
        arg1->unk8 = arg4[1];
        arg1->unk4 = D_80100D88.unk0 | (D_80100D88.unk1 << 8);
        arg1->unk2 = D_80100D88.unk2;
        if (btlfx_isSameUnit(arg1->unk0, arg1->unk1) != 0 || btlfx_isPartyUnit(arg1->unk1) != 0) {
            D_800FEC55 = arg0;
            return 1;
        }
        D_800FEC55 = 0;
    } else {
        return func_800D974C(arg1, arg2, arg3);
    }
    return 0;
}

s32 btlfx_opRunToTarget(s32 arg0, Unk800D7C9C *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 0x1C;
    } else if (arg0 == 1) {
        arg1->unk0 = 0x10;
        arg1->unk1 = arg4[0];
        arg1->unk8 = arg4[1];
        arg1->unk4 = D_80100D88.unk0 | (D_80100D88.unk1 << 8);
        if (btlfx_isSameUnit(arg1->unk0, arg1->unk1) != 0) {
            D_800FEC55 = arg0;
            return 1;
        }
        if (btlfx_isPartyUnit(arg1->unk1) != 0) {
            return 1;
        }
        D_800FEC55 = 0;
    } else {
        return btlfx_stepRunToTarget(arg1, arg2, arg3);
    }
    return 0;
}

s32 btlfx_opFaceTarget(s32 arg0, Unk800D805C *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 0x14;
    } else if (arg0 == 1) {
        arg1->unk0 = arg4[0];
        arg1->unk1 = arg4[1];
        arg1->unk4 = D_80100D88.unk0;
        arg1->unk2 = D_80100D88.unk1;
        arg1->unk3 = D_80100D88.unk2;
        arg1->unk8 = 0;
        if (btlfx_isSameUnit(arg1->unk0, arg1->unk1) != 0) {
            D_800FEC56 = arg0;
            return 1;
        }
        D_800FEC56 = 0;
    } else {
        return btlfx_stepFaceTarget(arg1, arg2, arg3);
    }
    return 0;
}

s32 btlfx_opFaceHome(s32 arg0, Unk800D8124 *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 0x10;
    } else if (arg0 == 1) {
        arg1->unk0 = arg4[0];
        arg1->unk1 = arg4[1];
        arg1->unk4 = D_80100D88.unk0;
        arg1->unk2 = D_80100D88.unk1;
        if (arg1->unk2 == 2 && btlfx_isPartyUnit(arg1->unk1) != 0) {
            D_800FEC56 = arg0;
            return 1;
        }
        D_800FEC56 = 0;
    } else {
        return btlfx_stepFaceHome(arg1, arg2, arg3);
    }
    return 0;
}

s32 func_800D81EC(s32 arg0, u8 *arg1, s32 arg2, s32 arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 4;
    } else if (arg0 != 1) {
        return func_800D9F5C(arg1, arg2, arg3);
    } else {
        arg1[0] = arg4[0];
        arg1[1] = arg4[1];
        arg1[2] = D_80100D88.unk0;
    }
    return 0;
}

s32 func_800D8270(s32 arg0, u8 *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    s32 v;
    s32 w;
    if (arg0 == 0) {
        *arg4 = 4;
    } else if (arg0 == 1) {
        arg1[0] = arg4[0];
        arg1[1] = arg4[1];
        arg1[2] = D_80100D88.unk0;
        if (func_800DA06C(arg1[0], arg1[2]) == 0) {
            D_800FEC57 = arg0;
            return 1;
        }
        if ((v = arg1[2]) == arg0 && (w = btlfx_isSameSide(arg1[0], arg1[1])) == v) {
            D_800FEC57 = w;
            return 1;
        }
        D_800FEC57 = 0;
    } else {
        return func_800D9F88(arg1, arg2, arg3);
    }
    return 0;
}

s32 func_800D8368(s32 arg0, u8 *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 4;
    } else if (arg0 == 1) {
        arg1[0] = arg4[0];
        arg1[1] = arg4[1];
        arg1[2] = D_80100D88.unk0;
        if (btlfx_isSameSide(arg1[0], arg1[1]) == 0) {
            D_800FEC57 = arg0;
            return 1;
        }
        D_800FEC57 = 0;
    } else {
        return func_800D9FD8(arg1, arg2, arg3);
    }
    return 0;
}

s32 func_800D8414(s32 arg0, u8 *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    s32 v;
    u8 a, b;
    if (arg0 == 0) {
        *arg4 = 4;
    } else if (arg0 == 1) {
        arg1[0] = arg4[0];
        arg1[1] = arg4[1];
        arg1[2] = D_80100D88.unk0;
        v = btlfx_isSameSide(arg1[0], arg1[1]);
        if (v == arg0) {
            D_800FEC57 = v;
            return 1;
        }
        D_800FEC57 = 0;
    } else {
        return func_800DA004(arg1, arg2, arg3);
    }
    return 0;
}

s32 btlfx_opCallWeaponScript(s32 arg0, u8 *arg1, s32 arg2, s32 arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 4;
    } else if (arg0 != 1) {
        return btlfx_stepCallWeaponScript(arg1, arg2, arg3);
    } else {
        *arg1 = *arg4;
    }
    return 0;
}

s32 func_800D8520(s32 arg0, u8 *arg1, s32 arg2, s32 arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 4;
    } else if (arg0 == 1) {
        arg1[0] = arg4[0];
        arg1[1] = arg4[1];
        if (btlfx_isSameSide(arg1[0], arg1[1]) == arg0) {
            return 1;
        }
    } else {
        return func_800DA064(arg1, arg2, arg3);
    }
    return 0;
}

void btlfx_playUnitMotion(s32 arg0, s32 arg1) {
    if (btlfx_isPartyUnit(arg0) != 0) {
        btlfx_setUnitMotion(arg0, btlfx_getUnitMotion(arg0, arg1));
    }
}

void func_800D85F8(Unk800F78A40 *arg0, s32 arg1, s32 arg2) {
    if (btlfx_isPartyUnit(arg1) != 0) {
        btlfx_setUnitMotion(arg1, btlfx_getFileData(arg0->unk8, arg2));
    }
}

void btlfx_resetUnitMotion(s32 arg0, u8 arg1) {
    BtlUnit *u = btl_getUnit(arg0);
    btlfx_setUnitMotion(arg0, u->unkA4[0]);
    if (arg1) {
        u->unk86[0] = u->unkA0->unk20;
        u->unk86[1] = u->unkA0->unk22;
        u->unk86[2] = u->unkA0->unk24;
    }
}

void btlfx_setUnitMotion(s32 arg0, s32 arg1) {
    if (btlfx_isPartyUnit(arg0) != 0) {
        BtlUnit *u = btl_getUnit(arg0);
        BtlUnit9C *m = u->unk9C;
        if (m != NULL) {
            m->unkC = (BtlUnit9CC *)arg1;
            u->unkA0->unk3A = 0;
        }
    }
}

s32 btlfx_getUnitMotion(s32 arg0, s32 arg1) {
    return btl_getUnit(arg0)->unkA4[arg1];
}

void btlfx_setMotionFrame(s32 arg0, s32 arg1) {
    BtlUnit *u = btl_getUnit(arg0);
    if (arg1 >= u->unk9C->unkC->unk2) {
        arg1 = u->unk9C->unkC->unk2;
    }
    u->unkA0->unk3A = arg1;
}

s32 btlfx_getMotionLength(s32 arg0) {
    return btl_getUnit(arg0)->unk9C->unkC->unk2;
}

u8 btlfx_getMotionFrame(s32 arg0) {
    return btl_getUnit(arg0)->unkA0->unk3A;
}

void btlfx_turnUnit(s32 arg0, s32 arg1) {
    BtlUnit *u = btl_getUnit(arg0);
    s16 a[4];
    s16 b[4];
    s16 c[4];
    switch (arg1) {
    case 0:
        u->unk86[1] = u->unkA0->unk22 + 0x800;
        break;
    case 1:
        btlfx_getUnitPos(arg0, a);
        b[0] = 0;
        b[1] = 0;
        b[2] = 0;
        btlfx_aimAngles(a, b, c);
        btlfx_setUnitRot(arg0, c);
        break;
    case 2:
        u->unk86[0] = u->unkA0->unk20;
        u->unk86[1] = u->unkA0->unk22;
        u->unk86[2] = u->unkA0->unk24;
        break;
    }
}

void btlfx_playSwingSound(s32 arg0) {
    BtlUnit *u = btl_getUnit(arg0);
    if (btlfx_isPartyUnit(arg0) != 0) {
        btl_soundCmd(0xD000, func_800AE47C((u8)u->unk68 >> 5, 0), 0, btl_soundPan(arg0), 0x7F);
    }
}

void btlfx_playHitSound(s32 arg0, s32 arg1, s32 arg2) {
    BtlUnit *a = btl_getUnit(arg0);
    BtlUnit *b = btl_getUnit(arg1);
    if (btlfx_isPartyUnit(arg0) != 0) {
        btl_playHitSound(a, b);
    }
}

u32 func_800D89E8(s32 arg0) {
    BtlUnit *u = btl_getUnit(arg0);
    u32 f = u->unk68;
    u->unk68 = f | 0x200000;
    return (f >> 21) & 1;
}

void func_800D8A20(s32 arg0, s32 arg1) {
    BtlUnit *u = btl_getUnit(arg0);
    u->unk68 = (u->unk68 & ~0x200000) | ((arg1 & 1) << 21);
}

s32 btlfx_isPartyUnit(s32 arg0) {
    if (arg0 == 0xFF) {
        arg0 = 0;
    }
    return btl_getUnit(arg0)->unk68 & 1;
}

s32 btlfx_isSameSide(s32 arg0, s32 arg1) {
    return btlfx_isPartyUnit(arg0) == btlfx_isPartyUnit(arg1);
}

s32 btlfx_isSameUnit(s32 arg0, s32 arg1) {
    BtlUnit *a;
    if (arg0 == 0xFF || arg1 == 0xFF) {
        return 0;
    }
    a = btl_getUnit(arg0);
    return a == btl_getUnit(arg1);
}

s32 func_800D8B20(s32 arg0) {
    s32 idx = (btl_getUnit(arg0)->unk68 >> 1) & 0xF;
    return g_gameState->unk1C->characters[idx].unk34.b.unk4;
}

s32 btlfx_getUnitWeapon(s32 arg0) {
    s32 idx = (btl_getUnit(arg0)->unk68 >> 1) & 0xF;
    return g_gameState->unk1C->characters[idx].equip[0];
}

void btlfx_setUnitVisible(s32 arg0, s32 arg1) {
    BtlUnit *u = btl_getUnit(arg0);
    if (arg1 != 0) {
        u->unk68 &= ~0x400000;
    } else {
        u->unk68 |= 0x400000;
    }
}

void btlfx_setWeaponVisible(s32 arg0, s32 arg1) {
    BtlUnit *u = btl_getUnit(arg0);
    if (arg1 != 0) {
        btl_showWeapon(u);
    } else {
        btl_hideWeapon(u);
    }
}

s32 btlfx_isUnitActive(s32 arg0) {
    return btlfx_getUnitStatus(arg0, 0x02001103) == 0;
}

u32 btlfx_getUnitStatus(s32 arg0, u32 arg1) {
    return btl_getUnit(arg0)->status & arg1;
}

s32 func_800D8C9C(void) {
    return 0;
}

s32 func_800D8CA4(void) {
    return -1;
}

void func_800D8CAC(void) {
}

s32 btlfx_stepSwingSound(u8 *arg0, s32 arg1, s32 arg2) {
    btlfx_playSwingSound(*arg0);
    return 1;
}

s32 btlfx_stepHitSound(u8 *arg0, s32 arg1, s32 arg2) {
    btlfx_playHitSound(arg0[0], arg0[1], arg0[2]);
    return 1;
}

s32 func_800D8D08(u8 *arg0, s32 arg1, s32 arg2) {
    BtlUnit *u = btl_getUnit(arg0[0]);
    switch (arg0[1]) {
    case 0:
        btl_playUnitSound(u, 0x456, 0x7F);
        break;
    case 1:
        btl_playUnitSound(u, 0xAC0, 0x7F);
        break;
    }
    return 1;
}

s32 btlfx_stepPlayMotion(u8 *arg0, s32 arg1, s32 arg2) {
    btlfx_playUnitMotion(arg0[0], arg0[1]);
    return 1;
}

s32 btlfx_stepPlayMotionWait(Unk800D8EA4 *arg0, s32 arg1, s32 *arg2) {
    s32 v = *arg2;
    if (v == 0) {
        btlfx_playUnitMotion(arg0->unk0, arg0->unk1);
        arg0->unk4 = btlfx_getMotionLength(arg0->unk0) - 1;
    }
    if (v >= arg0->unk4) {
        D_800FEC54 = 1;
        return 1;
    }
    return 0;
}

s32 func_800D8E10(Unk800D8EA4 *arg0, s32 arg1, s32 *arg2) {
    s32 v = *arg2;
    if (v == 0) {
        btlfx_playUnitMotion(arg0->unk0, arg0->unk1);
        arg0->unk4 = btlfx_getMotionLength(arg0->unk0) - arg0->unk2 - 1;
    }
    if (v >= arg0->unk4) {
        D_800FEC54 = 1;
        func_800D89E8(arg0->unk0);
        return 1;
    }
    return 0;
}

s32 func_800D8EA4(Unk800D8EA4 *arg0, s32 arg1, s32 *arg2) {
    s32 v = *arg2;
    if (v == 0) {
        btlfx_playUnitMotion(arg0->unk0, 0x17);
        arg0->unk4 = btlfx_getMotionLength(arg0->unk0) - 1;
    }
    if (v >= arg0->unk4) {
        D_800FEC54 = 1;
        return 1;
    }
    return 0;
}

s32 func_800D8F1C(Unk800D8EA4 *arg0, s32 arg1, s32 *arg2) {
    s32 v = *arg2;
    if (v == 0) {
        btlfx_playUnitMotion(arg0->unk0, 0x17);
        arg0->unk4 = btlfx_getMotionLength(arg0->unk0) - arg0->unk2 - 1;
    }
    if (v >= arg0->unk4) {
        func_800D89E8(arg0->unk0);
        D_800FEC54 = 1;
        return 1;
    }
    return 0;
}

s32 func_800D8FAC(Unk800D8EA4 *arg0, s32 arg1, s32 *arg2) {
    s32 v = *arg2;
    if (btlfx_isPartyUnit(arg0->unk1) == 0) {
        if (v == 0) {
            btlfx_playUnitMotion(arg0->unk0, 0x19);
            arg0->unk4 = btlfx_getMotionLength(arg0->unk0) - 1;
        }
        if (v < arg0->unk4) {
            return 0;
        }
    }
    D_800FEC54 = 1;
    return 1;
}

s32 btlfx_stepResetMotion(u8 *arg0, s32 arg1, s32 arg2) {
    btlfx_resetUnitMotion(*arg0, 1);
    return 1;
}

s32 func_800D905C(u8 *arg0, s32 arg1, Unk800F78A40 *arg2) {
    func_800D85F8(arg2, arg0[0], arg0[1]);
    return 1;
}

s32 func_800D908C(Unk800D8EA4 *arg0, s32 arg1, s32 *arg2) {
    s32 v = *arg2;
    if (v == 0) {
        func_800D85F8((Unk800F78A40 *)arg2, arg0->unk0, arg0->unk1);
        arg0->unk4 = btlfx_getMotionLength(arg0->unk0) - 1;
    }
    if (v >= arg0->unk4) {
        D_800FEC54 = 1;
        return 1;
    }
    return 0;
}

s32 func_800D9110(u8 *arg0, s32 arg1, s32 arg2) {
    func_800D8A20(*arg0, 0);
    return 1;
}

s32 func_800D9134(Unk800D9134 *p, s32 arg1, s32 *arg2) {
    s16 a[4];
    s16 b[4];
    s16 *pos;
    s16 c[4];
    s32 n = *arg2;
    s32 v;
    if (btlfx_isPartyUnit(p->unk1) == 0) {
        if (n == 0) {
            p->unk4 = func_800DA06C(p->unk0, 0);
            pos = p->unk10;
            btlfx_getUnitPos(p->unk1, pos);
            v = p->unk4;
            if (v == 1) {
                btlfx_setUnitPos(p->unk0, p->unk10);
                D_800FEC55 = v;
                return 1;
            }
        }
        if (btlfx_moveUnitArc(p->unk0, p->unk10, n + 1, p->unk4, 0, p->unk8) != 0) {
            D_800FEC55 = 1;
            return 1;
        }
        return 0;
    }
    btlfx_getUnitHomePos(p->unk1, b);
    btlfx_getUnitRot(p->unk1, c);
    func_800CFD78(b, c, 1000, a);
    btlfx_setUnitPos(p->unk0, a);
    D_800FEC55 = 1;
    return 1;
}

s32 btlfx_stepRunHome(Unk800D9260 *arg0, s32 arg1, s32 *arg2) {
    s16 sp18[4];
    s32 n = *arg2;
    s32 v;
    btlfx_getUnitHomePos(arg0->unk0, sp18);
    if (btlfx_isPartyUnit(arg0->unk1) == 0) {
        if (n == 0) {
            v = func_800DA06C(arg0->unk0, 3);
            arg0->unk4 = v;
            if (v == 1) {
                btlfx_setUnitPos(arg0->unk0, sp18);
                D_800FEC55 = v;
                return 1;
            }
        }
        if (btlfx_moveUnitArc(arg0->unk0, sp18, n + 1, arg0->unk4, 0, arg0->unk8) != 0) {
            D_800FEC55 = 1;
            return 1;
        }
        return 0;
    }
    btlfx_setUnitPos(arg0->unk0, sp18);
    D_800FEC55 = 1;
    return 1;
}

s32 func_800D9350(Unk800D7C9C *arg0, s32 arg1, s32 *arg2) {
    s16 a[4];
    s16 b[4];
    s16 c[4];
    s32 m;
    s32 v = *arg2;
    if (v == 0) {
        btlfx_getUnitPos(arg0->unk0, a);
        btlfx_getUnitHomePos(arg0->unk1, b);
        c[0] = 0;
        c[1] = 0;
        c[2] = 0;
        if (a[1] == 0) {
            if (b[1] != 0) {
                b[1] = 0;
            }
        } else {
            b[1] = a[1];
        }
        func_800CFD78(b, c, arg0->unk4, a);
        func_800CFD78(b, c, arg0->unk6, arg0->unk14);
        btlfx_setUnitPos(arg0->unk0, a);
        m = arg0->unk8;
        if (m == 1) {
            btlfx_setUnitPos(arg0->unk0, arg0->unk14);
            D_800FEC55 = m;
            return 1;
        }
    }
    if (btlfx_moveUnitArc(arg0->unk0, arg0->unk14, v + 1, arg0->unk8, 0, arg0->unkC) != 0) {
        D_800FEC55 = 1;
        return 1;
    }
    return 0;
}

s32 btlfx_stepJumpForward(Unk800D7C9C *arg0, s32 arg1, s32 *arg2) {
    s16 a[4];
    s16 b[4];
    s32 v = *arg2;
    if (v == 0) {
        btlfx_getUnitPos(arg0->unk0, a);
        btlfx_getUnitRot(arg0->unk0, b);
        func_800CFD78(a, b, arg0->unk4, arg0->unk14);
        if (arg0->unk3 != 0) {
            btlfx_playUnitMotion(arg0->unk0, 0x1D);
        }
        arg0->unk8 = btlfx_getMotionLength(arg0->unk0);
    }
    if (btlfx_moveUnitLinear(arg0->unk0, arg0->unk14, v, arg0->unk8, arg0->unkC, 0) != 0) {
        D_800FEC55 = 1;
        return 1;
    }
    return 0;
}

s32 btlfx_stepJumpHome(Unk800D955C *arg0, s32 arg1, s32 *arg2) {
    s32 v = *arg2;
    if (v == 0) {
        btlfx_getUnitHomePos(arg0->unk0, arg0->unk10);
        if (arg0->unk3 != 0) {
            btlfx_playUnitMotion(arg0->unk0, 0x1E);
        }
        arg0->unk4 = btlfx_getMotionLength(arg0->unk0);
    }
    if (btlfx_moveUnitLinear(arg0->unk0, arg0->unk10, v, arg0->unk4, arg0->unk8, 1) != 0) {
        D_800FEC55 = 1;
        return 1;
    }
    return 0;
}

s32 func_800D960C(Unk800D7E14 *arg0, s32 arg1, s32 arg2) {
    s16 sp18[4];
    s32 n = *(s32 *)arg2;
    s32 v;
    switch (arg0->unk1) {
    case 0:
        sp18[0] = 0;
        sp18[1] = 0;
        sp18[2] = -0x100;
        break;
    case 1:
        btlfx_getUnitHomePos(arg0->unk0, sp18);
        break;
    case 2:
        sp18[0] = ((Unk800F78A4 *)arg1)->unkC->unk38;
        sp18[1] = ((Unk800F78A4 *)arg1)->unkC->unk3A;
        sp18[2] = ((Unk800F78A4 *)arg1)->unkC->unk3C;
        break;
    default:
        sp18[0] = 0;
        sp18[1] = 0;
        sp18[2] = 0;
        break;
    }
    if (n == 0) {
        v = arg0->unk4;
        if (v == 1) {
            btlfx_setUnitPos(arg0->unk0, sp18);
            D_800FEC55 = v;
            return 1;
        }
    }
    if (btlfx_moveUnitArc(arg0->unk0, sp18, n + 1, arg0->unk4, 0, arg0->unk8) != 0) {
        D_800FEC55 = 1;
        return 1;
    }
    return 0;
}

s32 func_800D974C(Unk800D7C9C *arg0, s32 arg1, s32 *arg2) {
    s16 a[4];
    s16 b[4];
    s16 c[4];
    s32 m;
    s32 v = *arg2;
    if (v == 0) {
        btlfx_getUnitPos(arg0->unk0, a);
        btlfx_getStrikePos(arg0->unk0, arg0->unk1, b);
        if (a[1] == 0) {
            if (b[1] != 0) {
                b[1] = 0;
            }
        } else {
            b[1] = a[1];
        }
        btlfx_aimAngles(b, a, c);
        func_800CFD78(b, c, arg0->unk4, arg0->unk14);
        arg0->unk14[0] -= a[0];
        arg0->unk14[1] -= a[1];
        arg0->unk14[2] -= a[2];
        arg0->unk14[0] *= arg0->unk2;
        arg0->unk14[1] *= arg0->unk2;
        arg0->unk14[2] *= arg0->unk2;
        arg0->unk14[0] /= 10;
        arg0->unk14[1] /= 10;
        arg0->unk14[2] /= 10;
        arg0->unk14[0] += a[0];
        arg0->unk14[1] += a[1];
        arg0->unk14[2] += a[2];
        m = arg0->unk8;
        if (m == 1) {
            btlfx_setUnitPos(arg0->unk0, arg0->unk14);
            D_800FEC55 = m;
            return 1;
        }
    }
    if (btlfx_moveUnitArc(arg0->unk0, arg0->unk14, v + 1, arg0->unk8, 0, arg0->unkC) != 0) {
        D_800FEC55 = 1;
        return 1;
    }
    return 0;
}

s32 btlfx_stepRunToTarget(Unk800D7C9C *arg0, s32 arg1, s32 *arg2) {
    s16 a[4];
    s16 b[4];
    s16 c[4];
    s32 m;
    s32 v = *arg2;
    if (v == 0) {
        btlfx_getUnitPos(arg0->unk0, a);
        btlfx_getStrikePos(arg0->unk0, arg0->unk1, b);
        if (a[1] == 0) {
            if (b[1] != 0) {
                b[1] = 0;
            }
        } else {
            b[1] = a[1];
        }
        btlfx_aimAngles(b, a, c);
        func_800CFD78(b, c, arg0->unk4, arg0->unk14);
        m = arg0->unk8;
        if (m == 1) {
            btlfx_setUnitPos(arg0->unk0, arg0->unk14);
            D_800FEC55 = m;
            return 1;
        }
    }
    if (btlfx_moveUnitArc(arg0->unk0, arg0->unk14, v + 1, arg0->unk8, 0, arg0->unkC) != 0) {
        D_800FEC55 = 1;
        return 1;
    }
    return 0;
}

s32 btlfx_stepFaceTarget(Unk800D805C *arg0, s32 arg1, s32 *arg2) {
    s16 pos[4];
    s16 rot[4];
    s16 tgt[4];
    s32 v = *arg2;

    if (arg0->unk3 == 0 || btlfx_isPartyUnit(arg0->unk1) == 0) {
        btl_getUnit(arg0->unk0);
        if (v == 0) {
            btlfx_getUnitPos(arg0->unk0, pos);
            if (arg0->unk1 == 0xFF) {
                tgt[0] = ((Unk800F78A4 *)arg1)->unkC->unk38;
                tgt[1] = ((Unk800F78A4 *)arg1)->unkC->unk3A;
                tgt[2] = ((Unk800F78A4 *)arg1)->unkC->unk3C;
            } else {
                btlfx_getUnitHomePos(arg0->unk1, tgt);
            }
            btlfx_aimAngles(pos, tgt, arg0->unkC);
            arg0->unkC[0] = 0;
            if (arg0->unk2 == 1) {
                arg0->unkC[1] += 0x800;
            }
        }
        btlfx_getUnitRot(arg0->unk0, rot);
        rot[0] += arg0->unkC[0] / arg0->unk4;
        rot[1] += arg0->unkC[1] / arg0->unk4;
        rot[2] += arg0->unkC[2] / arg0->unk4;
        btlfx_setUnitRot(arg0->unk0, rot);
        if (v != arg0->unk4 - 1) {
            goto end;
        }
        D_800FEC56 = 1;
        return 1;
    }
    if (arg0->unk1 == 0xFF) {
        tgt[0] = ((Unk800F78A4 *)arg1)->unkC->unk38;
        tgt[1] = ((Unk800F78A4 *)arg1)->unkC->unk3A;
        tgt[2] = ((Unk800F78A4 *)arg1)->unkC->unk3C;
    } else {
        btlfx_getUnitHomePos(arg0->unk1, tgt);
    }
    btlfx_getUnitRot(arg0->unk0, rot);
    rot[1] = 0x800;
    btlfx_setUnitRot(arg0->unk0, rot);
    D_800FEC56 = 1;
    return 1;
end:
    return 0;
}

s32 btlfx_stepFaceHome(Unk800D8124 *arg0, s32 arg1, s32 *arg2) {
    s16 rot[4];
    BtlUnit *u;
    s32 v = *arg2;
    BtlUnitA0 *a;

    u = btl_getUnit(arg0->unk0);
    if (arg0->unk2 == 0 || btlfx_isPartyUnit(arg0->unk1) == 0) {
        if (v == 0) {
            arg0->unk8[0] = u->unkA0->unk20 - u->unk86[0];
            arg0->unk8[1] = u->unkA0->unk22 - u->unk86[1];
            arg0->unk8[2] = u->unkA0->unk24 - u->unk86[2];
        }
        btlfx_getUnitRot(arg0->unk0, rot);
        rot[0] += arg0->unk8[0] / arg0->unk4;
        rot[1] += arg0->unk8[1] / arg0->unk4;
        rot[2] += arg0->unk8[2] / arg0->unk4;
        btlfx_setUnitRot(arg0->unk0, rot);
        if (v != arg0->unk4 - 1) {
            goto end;
        }
        D_800FEC56 = 1;
        return 1;
    }
    u->unk86[0] = u->unkA0->unk20;
    a = u->unkA0;
    u->unk86[1] = a->unk22;
    *(volatile s16 *)&u->unk86[2] = a->unk24;
    return D_800FEC56 = 1;
end:
    return 0;
}

s32 func_800D9F5C(u8 *arg0, s32 arg1, s32 arg2) {
    btlfx_turnUnit(arg0[0], arg0[2]);
    return 1;
}

s32 func_800D9F88(u8 *arg0, s32 arg1, s32 *arg2) {
    s32 v = *arg2;
    if (v == func_800DA06C(arg0[0], arg0[2]) - 1) {
        D_800FEC57 = 1;
        return 1;
    }
    return 0;
}

s32 func_800D9FD8(u8 *arg0, s32 arg1, s32 *arg2) {
    if (*arg2 == arg0[2] - 1) {
        D_800FEC57 = 1;
        return 1;
    }
    return 0;
}

s32 func_800DA004(u8 *arg0, s32 arg1, s32 *arg2) {
    if (*arg2 == arg0[2] - 1) {
        D_800FEC57 = 1;
        return 1;
    }
    return 0;
}

s32 btlfx_stepCallWeaponScript(u8 *arg0, s32 arg1, s32 arg2) {
    s32 v = func_800DA0B8(*arg0);
    D_80100D8C = v;
    btlfx_callScript(v);
    return 1;
}

s32 func_800DA064(u8 *arg0, s32 arg1, s32 arg2) {
    return 1;
}

s32 func_800DA06C(s32 arg0, s32 arg1) {
    s32 i = func_800D8B20(arg0);
    return D_800F73A0[arg1][i];
}

s32 func_800DA0B8(s32 arg0) {
    s32 v = btlfx_getUnitWeapon(arg0);
    if (v >= 0x55) {
        v = 0;
    }
    return D_800F7460[v];
}

s32 func_800DA0FC(void) {
    return 0;
}

s32 btlfx_poolSize(s32 arg0, s32 arg1) {
    return (btlfx_align4(arg0) + 4) * arg1 + 0xC;
}

void btlfx_poolInit(Unk800DA1B8 *arg0, Unk80100C78Entry *arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 sz;
    s32 i;
    u8 *p;

    /* sz in two statements and the do-while around p += sz give sz and p the reference counts
       the original's register allocation needs (sz v1, p a0, i a1). */
    sz = btlfx_align4(arg2);
    sz += 4;
    p = (u8 *)arg0->unkC;
    i = 0;
    arg0->unk0 = sz;
    arg0->unk2 = arg3;
    arg0->unk8 = (Unk800DA1B88 *)arg1;
    arg0->unk4 = (void *)arg4;
    if (arg3 > 0) {
    loop:
        *(s32 *)p = 0;
        i++;
        do {
            p += sz;
        } while (0);
        if (i < arg3) goto loop;
    }
}

void btlfx_poolClear(Unk800DA1B8 *arg0) {
    s32 i;
    s32 *p = arg0->unkC;
    for (i = 0; i < arg0->unk2; i++) {
        *p = 0;
        p = (s32 *)((u8 *)p + arg0->unk0);
    }
}

void *btlfx_poolSpawn(s16 *p) {
    Unk800DA1B8 *pool;
    Unk800DA1B8 *x;
    s16 *e;
    s32 i;
    s32 stride;
    u8 *d;

    x = (Unk800DA1B8 *)p;
    pool = (Unk800DA1B8 *)p;
    p = (s16 *)((Unk800DA1B8 *)p)->unkC;
    stride = x->unk0;
    for (i = 0; i < pool->unk2; i++) {
        e = p;
        if (*p == 0) {
            break;
        }
        p = (s16 *)((u8 *)p + stride);
    }
    if (i >= pool->unk2) {
        return NULL;
    }
    d = (u8 *)(p + 2);
    e[0] = 1;
    e[1] = 0;
    pool->unk4(1, d, g_btlfxEnv, pool->unk8, 0, pool, pool->unk8->unkC);
    /* Probably macro residue: a dead store that flow deletes (no code). Its only effect is that p is
       mentioned last, which keeps p (not pool) the CSE class head so `p += 12` and the header reads stay
       on the original registers. */
    p = NULL;
    return d;
}

s32 btlfx_poolUpdate(Unk800DA1B8 *arg0) {
    Unk800DA1B8 *pool = arg0;
    s16 *p = (s16 *)pool->unkC;
    s32 i = 0;
    s32 n = i;
    s32 (*fn)(s32, void *, Unk800F78A4 *, Unk800DA1B88 *, s32, struct Unk800DA1B8 *, s32) = pool->unk4;
    s32 stride = pool->unk0;
    for (; i < pool->unk2; i++, p = (s16 *)((u8 *)p + stride)) {
        if (p[0] != 0) {
            g_btlfxEnv->unk28 = 0;
            n++;
            if (fn(2, p + 2, g_btlfxEnv, pool->unk8, p[1], pool, pool->unk8->unkC) != 0) {
                *(s32 *)p = 0;
            } else {
                p[1]++;
            }
        }
    }
    return n;
}

s32 btlfx_findFileSlot(s32 arg0) {
    s32 i;
    for (i = 0; i < 2; i++) {
        if (arg0 == g_btlfxLoader.unk2C.h[i]) {
            break;
        }
    }
    return i;
}

s32 btlfx_getFileData(s32 arg0, s32 arg1) {
    s32 *base;
    if (arg1 < 0x40) {
        base = g_btlfxEnv->unk4->unk30[arg0];
    } else if (arg1 < 0x80) {
        base = (s32 *)0x801F1700;
        arg1 -= 0x40;
    } else if (arg1 < 0xC0) {
        arg1 -= 0x80;
        base = D_80100E14->unk50[0];
        arg1 &= 0x7F;
    } else {
        base = g_gameState->unk1C->unk8E4.battle->unk11C0->unkC;
        arg1 -= 0xC0;
    }
    return (s32)base + base[arg1];
}

s32 btlfx_getFileDataSize(s32 arg0, s32 arg1) {
    s32 v = btlfx_getFileData(arg0, arg1);
    return btlfx_getFileData(arg0, arg1 + 1) - v;
}

/* takes arg0 bytes from the effect work area (g_btlfxLoader.unk58/unk5C), returns the old pointer */
s32 func_800DA4C4(s32 arg0) {
    Unk80100DC0 *d = &g_btlfxLoader;
    s32 p = d->unk58;

    d->unk5C -= arg0;
    d->unk58 = p + arg0;
    return p;
}

void func_800DA4E8(void) {
    D_80100DCC = 0;
}

s32 btlfx_loadFile(s32 arg0) {
    s32 ofs;
    s32 base;

    if (g_btlfxLoader.unkC != 0) {
        return -1;
    }
    ofs = ((FileEntry16 *)file_find(0xD, arg0))->unk0;
    base = g_gameState->unk1C->unk8E4.battle->unk1534;
    g_btlfxLoader.unk2C.w = -1;
    g_btlfxLoader.unk4 = arg0;
    g_btlfxLoader.unk8 = 0;
    g_btlfxLoader.unk1A = 0;
    g_btlfxLoader.unkC = 1;
    g_btlfxLoader.unkE = 0;
    g_btlfxLoader.unk50 = 0;
    g_btlfxLoader.unk52 = 0;
    g_btlfxLoader.unk53 = 0;
    g_btlfxLoader.unk68 = 0;
    g_btlfxLoader.unk69 = 0;
    g_btlfxLoader.unk64 = 0;
    g_btlfxLoader.unk66 = 0;
    g_btlfxLoader.unk65 = 0;
    g_btlfxLoader.unk67 = 0;
    g_btlfxLoader.unk38 = 0;
    g_btlfxLoader.unk3C[0] = 0;
    g_btlfxLoader.unk3C[1] = 0;
    g_btlfxLoader.unk0 = base + ofs;
    return 0;
}

s32 func_800DA5C4(void) {
    if (g_btlfxLoader.unkC != 0) {
        return -1;
    }
    g_btlfxLoader.unkC = 3;
    g_btlfxLoader.unkE = 0;
    g_btlfxLoader.unk50 = 0;
    g_btlfxLoader.unk68 = 0;
    g_btlfxLoader.unk64 = 0;
    g_btlfxLoader.unk65 = 0;
    g_btlfxLoader.unk67 = 0;
    g_btlfxLoader.unk1A++;
    g_btlfxLoader.unk8 = (g_btlfxLoader.unk8 + 1) & 1;
    g_btlfxLoader.unk2C.h[g_btlfxLoader.unk8] = -1;
    return 0;
}

s32 func_800DA634(s32 arg0) {
    if (arg0 == 0) {
        if (D_80100DCC != 7 && D_80100DCC != 0) {
            return 1;
        }
    }
    func_800562E0(g_gameState->unk1C->unk8E4.battle->unk11C0->unkC, 0);
    g_btlfxLoader.unk67 = 1;
    g_btlfxLoader.unk66 = 1;
    return 0;
}

s32 func_800DA6B4(s32 arg0, s32 arg1) {
    if (g_btlfxLoader.unk66 != 0) {
        if (func_80055DCC() == 0) {
            g_btlfxLoader.unk66 = 0;
            func_80056388(0x80, arg1);
            return 0;
        }
    }
    return 1;
}

void func_800DA71C(RECT *arg0) {
    if (arg0->x >= 0x300 && arg0->x + arg0->w < 0x381) {
        if (arg0->y >= 0x100 && arg0->y + arg0->h < 0x201) {
            g_btlfxState.unk1C |= 1;
        }
    }
}

void func_800DA790(RECT *arg0, u32 *arg1, Unk800DA790 *arg2) {
    s32 i;
    s32 n = arg2->unk17;
    Unk800DA790 *e;
    RECT *r;
    arg0->w = 0x40;
    arg0->h = 0x80;
    func_800D0940();
    for (i = 0; i < n; i++) {
        e = arg2;
        r = e->unk44;
        if (++e->unk48 >= 2) {
            arg0->x = r->x;
            arg0->y = r->y + 0x80;
            goto next;
        }
        arg0->x = r->x;
        arg0->y = r->y;
        if ((u16)r->h == 0x80) {
        next:
            e->unk48 = 0;
            e->unk44++;
        }
        LoadImage(arg0, arg1);
        func_800DA71C(arg0);
        arg1 += 0x1000;
    }
}

void func_800DA898(void) {
    if (D_800F750C == 2) {
        EnterCriticalSection();
        if (DrawSync(1) == 0) {
            D_800F750C = 1;
            FlushCache();
        }
        ExitCriticalSection();
    }
}

s32 func_800DA8EC(void) {
    s32 ret = 0;
    switch (D_800F750C) {
    case 0:
        D_800F750C = 2;
        break;
    case 1:
        D_800F750C = 0;
        ret = 1;
        break;
    case 2:
        break;
    }
    return ret;
}

void btlfx_updateLoader(void) {
    union {
        RECT r;
        s32 w;
    } r;
    Unk80100DC0 *d = &g_btlfxLoader;
    s32 cont;
    s16 st;
    u16 *p;
    u8 *dst;
    u32 rd;
    s32 sz;
    s32 k;
    s32 cnt2;
    s32 x;
    s32 m;
    u8 *src;
    Unk80100DC054 *e2;
    s32 i;
    s32 n;
    s32 cnt;
    s32 *hdr;
    u16 *q;
    Unk80100DC054 *e;
    u8 *base;
    s32 bit;
    Heap *h;
    s32 ni;
    u8 *fh;
    Unk800DA92CFn fn;

    if (d->unkC != 0) {
        do {
            st = d->unkC - 1;
            cont = 0;
            switch (st) {
            case 0:
                cd_readAsync(d->unk0, 0x800, D_800FEC68, NULL);
                d->unkC = 2;
                d->unk0++;
                break;
            case 1:
                if (cd_getState() == 0) {
                    p = D_800FEC68;
                    cont = 1;
                    d->unk1C = p + 1;
                    d->unkC = 3;
                    d->unk18 = p[0];
                }
                break;
            case 2:
                if (d->unk1A < d->unk18) {
p = d->unk1C; cont = 1; d->unk10 = p[0]; d->unk12 = p[1]; d->unk1C += 2; d->unkC = 4;
                } else {
                    d->unkC = 0;
                }
                break;
            case 3:
                __asm__(".section .rodata\n\t.word 0\n\t.section .text");
                p = d->unk1C;
                d->unk16 = ((u8 *)p)[0];
                d->unk17 = ((u8 *)p)[1];
                if (g_btlfxEnv->unkC->unk40 & 8) {
                    if (d->unk16 == 4 || d->unk16 == 9) {
                        if (func_800E8AD0() != 0) {
                            break;
                        }
                    }
                }
                dst = (u8 *)0x801E7700;
                if (d->unk10 == 1 && d->unk8 != 0) {
                    dst = (u8 *)0x801EC700;
                }
                rd = p[1] << 11;
                p += 2;
                switch (d->unk16) {
                case 2:
                    sz = rd;
                    d->unkE = 1;
                    if (d->unk17 != 0) {
x = *p++ << 11; rd += x; d->unk20 = (u8 *)0x801F1700 + x; dst = (u8 *)0x801F1700;
                        d->unk24 = 0xC800 - x;
                        d->unk30[d->unk8] = d->unk28 = d->unk20;
                    } else {
                        dst = d->unk30[d->unk8] = d->unk28;
                    }
                    d->unk28 += sz;
                    if (d->unk28 >= d->unk20 + d->unk24) {
                        d->unk28 = d->unk20;
                    }
                    break;
                case 10:
                    dst = d->unk28;
                    d->unk28 = dst + rd;
                    break;
                case 1:
                    dst = (u8 *)d->unk4C;
                    break;
                case 3:
                    d->unkE = 2;
                    break;
                case 4:
                    d->unk50 = 1;
                    func_800AB43C();
                    dst = (u8 *)g_gameState->unk1C->unk8E4.battle->unk153C[0];
                    d->unk54 = (Unk80100DC054 *)dst;
                    ((Unk80100DC054 *)dst)->unk4C = 0;
                    for (i = 0; i < 16; i++) {
                        d->unk54->unk50[i] = 0;
                    }
                    break;
                case 5:
                    dst = (u8 *)d->unk54 + d->unk54->unk0;
                    break;
                case 6:
                    d->unk52 = 1;
                    e2 = d->unk54;
                    d->unk58 = (s32)e2->unk44;
                    d->unk5C = e2->unk48;
                    dst = (u8 *)d->unk58;
                    e2->unk50[0] = (s32 *)dst;
                    d->unk5C -= rd;
                    d->unk58 += rd;
                    break;
                case 7:
                    d->unk64 = 1;
                    dst = (u8 *)g_gameState->unk1C->unk8E4.battle->unk11C0->unkC;
                    break;
                case 8:
                    d->unk68 = 1;
                    dst = (u8 *)g_gameState->unk1C->unk8E4.battle->unk11C0->unkC;
                    break;
                case 9:
                    if (g_btlfxEnv->unkC->unk40 & 8) {
                        func_800AB43C();
                        dst = (u8 *)g_gameState->unk1C->unk8E4.battle->unk153C[0];
                    } else {
                        dst = (u8 *)g_gameState->unk1C->unk8E4.battle->unk11C0->unkC;
                    }
                    break;
                }
                d->unk1C = p;
                cd_readAsync(d->unk0, rd, dst, NULL);
                d->unkC = 5;
                d->unk0 += rd >> 11;
                break;
            case 4:
                if (cd_getState() != 0) {
                    break;
                }
                if (d->unk16 == 9 && (d->unk17 & 0xF) && D_800FEC65 == 0) {
                    break;
                }
                if (d->unk16 == 3 && func_800DA8EC() == 0) {
                    break;
                }
                switch (d->unk16) {
                case 2:
                    if (d->unk17 != 0 && btlfx_getFileDataSize(0, 0x40) != 0) {
                        d->unk38 = btlfx_getFileData(0, 0x40);
                    }
                    m = btlfx_getFileDataSize(d->unk8, 0);
                    if (m != 0) {
                        h = (Heap *)btlfx_getFileData(d->unk8, 0);
                        d->unk3C[d->unk8] = h;
                        btlfx_heapInit(h, m);
                    } else {
                        d->unk3C[d->unk8] = NULL;
                    }
                    break;
                case 0:
                    dst = (u8 *)0x801E7700;
                    if (d->unk10 == 1 && d->unk8 != 0) {
                        dst = (u8 *)0x801EC700;
                    }
                    hdr = (s32 *)dst;
                    q = (u16 *)(dst + hdr[1]);
                    k = hdr[0];
                    cnt2 = hdr[2];
                    func_800D0940();
                    for (i = 0; i < cnt2; i++) {
                        r.r.x = q[0];
                        r.r.y = q[1];
                        r.r.w = q[2];
                        r.r.h = q[3];
                        n = q[2] * q[3];
                        q += 4;
                        LoadImage(&r.r, (u32 *)q);
                        q += n;
                    }
                    hdr = (s32 *)(dst + k);
                    q = (u16 *)(dst + hdr[0]);
                    d->unk44 = (RECT *)(hdr + 2);
                    d->unk48 = 0;
                    d->unk4C = (u32 *)q;
                    func_800DA790(&r.r, (u32 *)q, (Unk800DA790 *)d);
                    break;
                case 1:
                    func_800DA790(&r.r, d->unk4C, (Unk800DA790 *)d);
                    break;
                case 3:
                    dst = (u8 *)0x801E7700;
                    if (d->unk10 == 1 && d->unk8 != 0) {
                        dst = (u8 *)0x801EC700;
                    }
                    fh = *(u8 **)dst;
                    D_80100C70[d->unk8] = (EffFn *)(fh + 8);
                    fn = ((Unk800DA92CFn *)fh)[1];
                    if (fn != NULL) {
                        fn(d->unk38, g_btlfxEnv, g_btlfxEnv->unk10);
                    }
                    d->unkE = 3;
                    d->unk2C.h[d->unk8] = d->unk1A;
                    break;
                case 4:
                    cnt = 0;
                    e = d->unk54;
                    dst = (u8 *)e + e->unk0;
                    r.r.x = 0x100;
                    r.r.y = 0xE6;
                    r.r.w = 0x100;
                    r.r.h = e->unk6;
                    func_800D0940();
                    LoadImage(&r.r, (u32 *)(dst + e->unk8));
                    k = e->unk4;
                    for (src = dst; cnt < k; src += 0x4000, cnt++) {
                        r.r.x = (e->unk18[cnt] & 0xF) << 6;
                        r.r.y = e->unk30[cnt] + ((e->unk18[cnt] & 0x10) << 4);
                        r.r.w = 0x40;
                        r.r.h = 0x80;
                        LoadImage(&r.r, (u32 *)src);
                        ((s32 *)D_80100D90)[cnt * 2] = ((s32 *)D_80100D90)[cnt * 2 + 1] = r.w;
                    }
                    break;
                case 5:
                    e = d->unk54;
                    dst = (u8 *)e;
                    e->unk3C = dst + e->unk0;
                    e->unk40 = (BtlUnit9C50 *)(dst + e->unk14);
                    for (ni = 0; ni < e->unk2; ni++) {
                        e->unk180[ni] += (s32)dst;
                    }
                    e->unk44 = dst + e->unk10;
                    d->unk58 = (s32)e->unk44;
                    e->unk48 = 0x50000 - e->unk10;
                    d->unk5C = e->unk48;
                    btlfx_initSummonModels(&e->unk90, 1);
                    d->unk60 = Hi_RegisterSummonModel(e, e->unk180[0] != (s32)e->unk40 ? (s16 *)D_80100D90 : NULL);
                    d->unk50 = 2;
                    break;
                case 6:
                    d->unk52 = 2;
                    break;
                case 7:
                    d->unk64 = 2;
                    d->unk66 = 1;
                    if (d->unk65 == 0) {
                        func_800DA634(1);
                    }
                    break;
                case 8:
                    d->unk68 = 2;
                    break;
                case 9:
                    m = 1;
                    d->unk53 = 1;
                    if (g_btlfxEnv->unkC->unk40 & 8) {
                        func_800AB43C();
                        base = (void *)g_gameState->unk1C->unk8E4.battle->unk153C[0];
                    } else {
                        base = (void *)g_gameState->unk1C->unk8E4.battle->unk11C0->unkC;
                    }
                    r.r.w = 0x40;
                    r.r.h = 0x80;
                    func_800D0940();
                    for (i = 0; i < 8; i++) {
                        if (m & d->unk17) {
                            if (i < 4) {
                                r.r.x = ((i >> 1) << 6) + 0x300;
                            } else {
                                r.r.x = (((i - 4) >> 1) << 6) + 0x140;
                            }
                            r.r.y = ((i & 1) << 7) + 0x100;
                            LoadImage(&r.r, (u32 *)base);
                            func_800DA71C(&r.r);
                            base += 0x4000;
                        }
                        if (i >= 4 || (i & 1)) {
                            m <<= 1;
                        }
                    }
                    d->unk69++;
                    break;
                }
                if (--d->unk12 <= 0) {
                    d->unkC = 0;
                    break;
                }
                if (d->unk16 == 7) {
                    if (d->unk65 == 0) {
                        d->unkC = 6;
                    } else {
                        d->unkC = 7;
                    }
                    cont = 0;
                    d->unk65++;
                } else {
                    cont = 0;
                    if (d->unk16 < 2 || d->unk16 == 4 || d->unk16 == 9) {
                        cont = 0;
                    } else {
                        cont = 1;
                    }
                    d->unkC = 4;
                }
                break;
            case 5:
                if (func_80055DCC() == 0) {
                    d->unkC = 4;
                    cont = 1;
                }
                break;
            case 6:
                if (d->unk67 != 0) {
                    d->unkC = 6;
                    d->unk67 = 0;
                }
                break;
            }
        } while (cont);
    }
}

u16 Hi_RegisterSolidEffModel(BtlUnit9C8 *arg0) {
    BtlObjSlot *p = btlfx_allocEffModel();
    if (p == NULL) {
        sprintf(g_btlfxErrorMsg, "Hi_RegisterSolidEffModel()\nmemory not enough!");
        btlfx_error(g_btlfxErrorMsg);
    }
    if (p->unk0 == NULL) {
        sprintf(g_btlfxErrorMsg, "Hi_RegisterSolidEffModel()\nmemory not enough!");
        btlfx_error(g_btlfxErrorMsg);
    }
    p->unk1C = 1;
    p->unk1D = 0;
    p->unk20 = 0;
    p->unk22 = 0;
    p->unk24 = 0;
    p->unk0->unk8 = arg0;
    p->unk0->unkC = NULL;
    func_800C7730(p->unk0, NULL, NULL);
    return p->unk1E;
}

u16 Hi_RegisterGouEffModel(BtlUnit9C8 *arg0) {
    BtlObjSlot *p = btlfx_allocEffModel();
    if (p == NULL) {
        sprintf(g_btlfxErrorMsg, "Hi_RegisterGouEffModel()\nmemory not enough!");
        btlfx_error(g_btlfxErrorMsg);
    }
    if (p->unk0 == NULL) {
        sprintf(g_btlfxErrorMsg, "Hi_RegisterGouEffModel()\nmemory not enough!");
        btlfx_error(g_btlfxErrorMsg);
    }
    p->unk1C = 1;
    p->unk1D = 0;
    p->unk20 = 1;
    p->unk22 = 0;
    p->unk24 = 0;
    p->unk0->unk8 = arg0;
    p->unk0->unkC = NULL;
    func_800C7730(p->unk0, NULL, NULL);
    return p->unk1E;
}

u16 Hi_RegisterTexEffModel(BtlUnit9C8 *arg0, s16 arg1, s16 arg2, s32 arg3, s32 arg4) {
    s16 v[2];
    BtlObjSlot *p = btlfx_allocEffModel();
    if (p == NULL) {
        sprintf(g_btlfxErrorMsg, "Hi_RegisterTexEffModel()\nmemory not enough!");
        btlfx_error(g_btlfxErrorMsg);
    }
    if (p->unk0 == NULL) {
        sprintf(g_btlfxErrorMsg, "Hi_RegisterTexEffModel()\nmemory not enough!");
        btlfx_error(g_btlfxErrorMsg);
    }
    p->unk1C = 1;
    p->unk1D = 0;
    p->unk20 = 2;
    p->unk22 = 0;
    p->unk24 = 0;
    p->unk0->unk8 = arg0;
    p->unk0->unkC = NULL;
    p->unk4.unk0[0][0] = arg1;
    p->unk4.unk0[0][1] = arg2;
    v[0] = arg3;
    v[1] = arg4;
    func_800C7730(p->unk0, p->unk4.unk0[0], v);
    return p->unk1E;
}

u16 Hi_RegisterTexListModel(BtlUnit9C8 *arg0, BtlTexInfo *arg1, s16 *arg2, s32 arg3) {
    BtlObjSlot *p = btlfx_allocEffModel();
    if (p == NULL) {
        sprintf(g_btlfxErrorMsg, "Hi_RegisterTexListModel()\nmemory not enough!");
        btlfx_error(g_btlfxErrorMsg);
    }
    if (p->unk0 == NULL) {
        sprintf(g_btlfxErrorMsg, "Hi_RegisterTexListModel()\nmemory not enough!");
        btlfx_error(g_btlfxErrorMsg);
    }
    p->unk1C = 1;
    p->unk1D = 0;
    p->unk20 = 2;
    p->unk22 = 0;
    p->unk24 = 0;
    p->unk0->unk8 = arg0;
    p->unk0->unkC = NULL;
    p->unk4 = *arg1;
    func_800C7730(p->unk0, p->unk4.unk0[0], arg2);
    return p->unk1E;
}

u16 Hi_RegisterTexPtrModel(BtlUnit9C8 *arg0, s16 *arg1, s16 *arg2) {
    BtlObjSlot *p = btlfx_allocEffModel();
    if (p == NULL) {
        sprintf(g_btlfxErrorMsg, "Hi_RegisterTexPtrModel()\nmemory not enough!");
        btlfx_error(g_btlfxErrorMsg);
    }
    if (p->unk0 == NULL) {
        sprintf(g_btlfxErrorMsg, "Hi_RegisterTexPtrModel()\nmemory not enough!");
        btlfx_error(g_btlfxErrorMsg);
    }
    p->unk1C = 1;
    p->unk1D = 0;
    p->unk20 = 2;
    p->unk22 = 0;
    p->unk24 = 0;
    p->unk0->unk8 = arg0;
    p->unk0->unkC = NULL;
    func_800C7730(p->unk0, arg1, arg2);
    return p->unk1E;
}

s32 func_800DB8D8(void) {
    return 0;
}

s32 Hi_RegisterSummonModel(Unk80100DC054 *arg0, s16 *arg1) {
    s16 t[6][2];
    Unk8010141C *p = btlfx_allocSummonModel();
    s32 i;
    if (p == NULL) {
        sprintf(g_btlfxErrorMsg, "Hi_RegisterSummonModel()\nmemory not enough!");
        btlfx_error(g_btlfxErrorMsg);
    }
    if (p->unk0 == NULL) {
        sprintf(g_btlfxErrorMsg, "Hi_RegisterSummonModel()\nmemory not enough!");
        btlfx_error(g_btlfxErrorMsg);
    }
    p->unk4C = 1;
    p->unk50 = 0;
    p->unk0->unk8 = (BtlUnit9C8 *)arg0->unk3C;
    p->unk0->unkC = (BtlUnit9CC *)arg0->unk180[0];
    if (arg1 != NULL) {
        p->unk4D = 1;
    } else {
        p->unk4D = 0;
    }
    for (i = 0; i < arg0->unk4; i++) {
        p->unk4[i][0] = arg0->unk18[i];
        p->unk4[i][1] = arg0->unk24[i];
        t[i][0] = 0;
        t[i][1] = arg0->unk30[i];
        if (arg1 != NULL) {
            p->unk1C[i][0] = arg1[0];
            p->unk1C[i][1] = arg1[1];
            p->unk1C[i][2] = arg1[2];
            p->unk1C[i][3] = arg1[3];
            arg1 += 4;
        }
    }
    func_800C7730(p->unk0, p->unk4[0], t[0]);
    p->unk0->unk50 = arg0->unk40;
    func_800C9374((TexAnimSet *)p->unk0->unk50, (u16 (*)[4])p->unk1C);
    return (u16)p->unk4E;
}

void Hi_DrawEffModel(SVECTOR *arg0, VECTOR *arg1, s32 arg2, s32 arg3) {
    BtlObjSlot *s = btlfx_getEffModel(arg3, "Hi_DrawEffModel()\nid:%d cannot use!");
    s32 n;
    s32 i;
    s32 v;
    btl_setModelTransform(s->unk0, arg0, arg1, arg2);
    g_btlfxEnv->unk24 = btl_calcBoneMatrices(s->unk0, 0, g_btlfxEnv->unk24);
    s->unk0->unk10 = 0;
    n = s->unk0->unk8->unk3;
    for (i = 0; i < n; i++) {
        btl_projectMesh(s->unk0, i);
        if (s->unk22 != 0) {
            g_btlfxEnv->unk24 = btl_drawMeshOffset(s->unk0, i, g_btlfxEnv->unk24, s->unk22);
        } else {
            switch ((u16)s->unk20) {
            case 0:
            case 2:
                g_btlfxEnv->unk24 = btl_drawMesh(s->unk0, i, g_btlfxEnv->unk24);
                break;
            case 1:
                g_btlfxEnv->unk24 = btl_drawMeshOffset(s->unk0, i, g_btlfxEnv->unk24, 0);
                break;
            }
        }
    }
}

void Hi_DrawSliceEffModel(SVECTOR *arg0, VECTOR *arg1, s32 arg2, s32 arg3) {
    BtlObjSlot *s = btlfx_getEffModel(arg3, "Hi_DrawSliceEffModel()\nid:%d cannot use!");
    s32 n;
    s32 i;
    btl_setModelTransform(s->unk0, arg0, arg1, arg2);
    g_btlfxEnv->unk24 = btl_calcBoneMatrices(s->unk0, 0, g_btlfxEnv->unk24);
    s->unk0->unk10 = 0;
    s->unk0->unk5A = arg1->vy - s->unk24;
    n = s->unk0->unk8->unk3;
    for (i = 0; i < n; i++) {
        btlfx_projectSliceMesh(s->unk0, i);
        g_btlfxEnv->unk24 = func_800DE11C(s->unk0, i, g_btlfxEnv->unk24);
    }
}

void Hi_DrawEffModelByBone(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    BtlObjSlot *s = btlfx_getEffModel(arg1, "Hi_DrawEffModelByBone()\nid:%d cannot use!");
    s32 n;
    s32 i;
    s32 v;
    g_btlfxEnv->unk24 = btl_calcBoneMatrices(s->unk0, 0, g_btlfxEnv->unk24);
    Hi_GetSummonBoneMatrix(arg2, arg3, (BtlBone *)&s->unk0->unk24);
    if (arg0 != 0) {
        btl_scaleModelMatrix(s->unk0, (VECTOR *)arg0);
    }
    s->unk0->unk10 = 0;
    n = s->unk0->unk8->unk3;
    for (i = 0; i < n; i++) {
        btl_projectMesh(s->unk0, i);
        if (s->unk22 != 0) {
            g_btlfxEnv->unk24 = btl_drawMeshOffset(s->unk0, i, g_btlfxEnv->unk24, s->unk22);
        } else {
            switch ((u16)s->unk20) {
            case 0:
            case 2:
                g_btlfxEnv->unk24 = btl_drawMesh(s->unk0, i, g_btlfxEnv->unk24);
                break;
            case 1:
                g_btlfxEnv->unk24 = btl_drawMeshOffset(s->unk0, i, g_btlfxEnv->unk24, 0);
                break;
            }
        }
    }
}

void Hi_DrawMorphEffModel(SVECTOR *arg0, VECTOR *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    char *str = "Hi_DrawMorphEffModel()\nid:%d cannot use!";
    BtlObjSlot *s = btlfx_getEffModel(arg3, str);
    s32 n;
    s32 i;
    s->unk0->unk5C = btlfx_getEffModel(arg4, str)->unk0->unk8;
    btl_setModelTransform(s->unk0, arg0, arg1, arg2);
    g_btlfxEnv->unk24 = btl_calcBoneMatrices(s->unk0, 0, g_btlfxEnv->unk24);
    s->unk0->unk10 = 0;
    n = s->unk0->unk8->unk3;
    for (i = 0; i < n; i++) {
        btlfx_projectMorphMesh(s->unk0, i, arg5);
        if (s->unk22 != 0) {
            g_btlfxEnv->unk24 = btl_drawMeshOffset(s->unk0, i, g_btlfxEnv->unk24, s->unk22);
        } else {
            switch ((u16)s->unk20) {
            case 0:
            case 2:
                g_btlfxEnv->unk24 = btl_drawMesh(s->unk0, i, g_btlfxEnv->unk24);
                break;
            case 1:
                g_btlfxEnv->unk24 = btl_drawMeshOffset(s->unk0, i, g_btlfxEnv->unk24, 0);
                break;
            }
        }
    }
}

void Hi_DrawMorphModelByBone(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    char *str = "Hi_DrawMorphModelByBone()\nid:%d cannot use!";
    BtlObjSlot *s = btlfx_getEffModel(arg1, str);
    s32 n;
    s32 i;
    s->unk0->unk5C = btlfx_getEffModel(arg2, str)->unk0->unk8;
    g_btlfxEnv->unk24 = btl_calcBoneMatrices(s->unk0, 0, g_btlfxEnv->unk24);
    Hi_GetSummonBoneMatrix(arg4, arg5, (BtlBone *)&s->unk0->unk24);
    if (arg0 != 0) {
        btl_scaleModelMatrix(s->unk0, (VECTOR *)arg0);
    }
    s->unk0->unk10 = 0;
    n = s->unk0->unk8->unk3;
    for (i = 0; i < n; i++) {
        btlfx_projectMorphMesh(s->unk0, i, arg3);
        if (s->unk22 != 0) {
            g_btlfxEnv->unk24 = btl_drawMeshOffset(s->unk0, i, g_btlfxEnv->unk24, s->unk22);
        } else {
            switch ((u16)s->unk20) {
            case 0:
            case 2:
                g_btlfxEnv->unk24 = btl_drawMesh(s->unk0, i, g_btlfxEnv->unk24);
                break;
            case 1:
                g_btlfxEnv->unk24 = btl_drawMeshOffset(s->unk0, i, g_btlfxEnv->unk24, 0);
                break;
            }
        }
    }
}

s32 Hi_DrawSummonModel(SVECTOR *arg0, VECTOR *arg1, s32 arg2, s32 arg3, s32 arg4) {
    Unk8010141C *s = btlfx_getSummonModel(arg3, "Hi_DrawSummonModel()\nid:%d cannot use!");
    BtlUnit9CC *a;
    s32 n;
    s32 f;
    s32 c;
    s32 i;
    btl_setModelTransform(s->unk0, arg0, arg1, arg2);
    a = s->unk0->unkC;
    if (IS_OFFSET(a->unkC)) {
        a->unkC = (s32)a + a->unkC;
        if (a->unk10 != 0) {
            a->unk10 = (s32)s->unk0->unkC + a->unk10;
        }
    }
    n = s->unk0->unkC->unk2;
    if (s->unk50 >= n) {
        if (arg4 & 1) {
            s->unk50 = 0;
        } else {
            s->unk50 = n - 1;
        }
    }
    if (s->unk4D != 0) {
        func_800C9040((TexAnimSet *)s->unk0->unk50);
    }
    f = s->unk50;
    g_btlfxEnv->unk24 = btl_calcBoneMatrices(s->unk0, f, g_btlfxEnv->unk24);
    s->unk50++;
    s->unk0->unk10 = 0;
    c = s->unk0->unk8->unk3;
    for (i = 0; i < c; i++) {
        if (!(s->unk0->unk14 & (1 << i))) {
            btl_projectMesh(s->unk0, i);
            g_btlfxEnv->unk24 = btl_drawMesh(s->unk0, i, g_btlfxEnv->unk24);
        }
    }
    return f;
}

void Hi_SetSummonMotion(BtlUnit9CC *arg0, s32 arg1) {
    Unk8010141C *p = btlfx_getSummonModel(arg1, "Hi_SetSummonMotion()\nid:%d cannot use!");
    p->unk50 = 0;
    p->unk0->unkC = arg0;
}

void Hi_SetSummonMotFrame(s32 arg0, s32 arg1) {
    Unk8010141C *p = btlfx_getSummonModel(arg0, D_800A7F34);
    if (p->unk0->unkC->unk2 < arg1) {
        p->unk50 = 0;
    } else {
        p->unk50 = arg1;
    }
}

BtlObjSlot *btlfx_allocEffModel(void) {
    BtlObjSlot *p = g_btlfxEffModels;
    s32 i;
    for (i = 0; i < 32; i++, p++) {
        if (p->unk1C == 0) {
            return p;
        }
    }
    return NULL;
}

Unk8010141C *btlfx_allocSummonModel(void) {
    Unk8010141C *p = g_btlfxSummonModels;
    s32 i;
    for (i = 0; i < 1; i++, p++) {
        if (p->unk4C == 0) {
            return p;
        }
    }
    return NULL;
}

void func_800DC500(void) {
}

void func_800DC508(void) {
}

void btlfx_halt(char *arg0) {
    D_80100E30.unkE8 = &D_80100E30.unk0[1];
    D_80100E30.unk0[0].unk70 = 0;
    D_80100E30.unk0[1].unk70 = 1;
    SetDefDrawEnv(&D_80100E30.unk0[0].unk0, 0, 0, 320, 240);
    SetDefDrawEnv(&D_80100E30.unk0[1].unk0, 0, 240, 320, 240);
    SetDefDispEnv(&D_80100E30.unk0[0].unk5C, 0, 240, 320, 240);
    SetDefDispEnv(&D_80100E30.unk0[1].unk5C, 0, 0, 320, 240);
    D_80100E30.unk0[1].unk0.isbg = 1;
    D_80100E30.unk0[0].unk0.isbg = 1;
    setRGB0(&D_80100E30.unk0[0].unk0, 0, 0, 0);
    setRGB0(&D_80100E30.unk0[1].unk0, 0, 0, 0);
    EnterCriticalSection();
    VSyncCallback(NULL);
    DrawSyncCallback(NULL);
    ExitCriticalSection();
    SetDispMask(1);
    while (1) {
        D_80100E30.unkE8 = (D_80100E30.unkE8 == &D_80100E30.unk0[0]) ? &D_80100E30.unk0[1] : &D_80100E30.unk0[0];
        DrawSync(0);
        VSync(0);
        PutDispEnv(&D_80100E30.unkE8->unk5C);
        PutDrawEnv(&D_80100E30.unkE8->unk0);
    }
}

void btlfx_error(char *arg0) {
    char buf[0x100];
    sprintf(buf, D_800A7F60, arg0);
    btlfx_halt(buf);
}

void btlfx_initEffModels(BtlModel *arg0, s32 arg1) {
    BtlObjSlot *p = g_btlfxEffModels;
    s32 i;
    s32 j;
    for (i = 0; i < 32; i++, p++) {
        p->unk1C = 0;
        p->unk1E = i;
        p->unk0 = NULL;
        for (j = 0; j < 6; j++) {
            p->unk4.unk0[j][0] = 0;
            p->unk4.unk0[j][1] = 0;
        }
    }
    p = g_btlfxEffModels;
    for (i = 0; i < arg1; i++, p++, arg0++) {
        p->unk0 = &arg0->unk0;
    }
}

void Hi_FreeEffModel(s32 arg0) {
    btlfx_getEffModel(arg0, D_800A7F78)->unk1C = 0;
}

void btlfx_clearEffModels(void) {
    BtlObjSlot *p = g_btlfxEffModels;
    s32 i;
    for (i = 0; i < 32; i++, p++) {
        p->unk1C = 0;
        p->unk1E = i;
        p->unk0 = NULL;
    }
}

void btlfx_initSummonModels(BtlModel *arg0, s32 arg1) {
    Unk8010141C *p = g_btlfxSummonModels;
    s32 i;
    s32 j;
    for (i = 0; i < 1; i++, p++) {
        p->unk4C = 0;
        p->unk4D = 0;
        p->unk4E = i;
        p->unk0 = NULL;
        for (j = 0; j < 6; j++) {
            p->unk4[j][0] = 0;
            p->unk4[j][1] = 0;
            p->unk1C[j][0] = 0;
            p->unk1C[j][1] = 0;
            p->unk1C[j][2] = 0;
            p->unk1C[j][3] = 0;
        }
    }
    p = g_btlfxSummonModels;
    for (i = 0; i < arg1; i++, p++, arg0++) {
        p->unk0 = &arg0->unk0;
    }
}

void btlfx_clearSummonModels(void) {
    Unk8010141C *p = g_btlfxSummonModels;
    s32 i;
    for (i = 0; i < 1; i++, p++) {
        p->unk4C = 0;
        p->unk4D = 0;
        p->unk4E = i;
        p->unk0 = NULL;
    }
}

void func_800DC818(s32 arg0, s32 arg1, u16 *arg2) {
    func_800DC9D0(btl_getUnit(arg0)->unk9C, arg1, arg2);
}

void func_800DC858(s32 arg0, s32 arg1, u16 *arg2) {
    func_800DC9D0(btl_getUnit(arg0)->unk9C, g_btlfxEnv->unkC->unk25[arg1], arg2);
}

void func_800DC8AC(s32 arg0, s32 arg1, BtlBone *arg2) {
    func_800DCA18(btl_getUnit(arg0)->unk9C, arg1, arg2);
}

void func_800DC8EC(s32 arg0, s32 arg1, BtlBone *arg2) {
    func_800DCA18(btl_getUnit(arg0)->unk9C, g_btlfxEnv->unkC->unk25[arg1], arg2);
}

INCLUDE_RODATA("asm/us/ovl_0b/nonmatchings/ovl_0b_2", D_800A7F34);

INCLUDE_RODATA("asm/us/ovl_0b/nonmatchings/ovl_0b_2", D_800A7F60);

INCLUDE_RODATA("asm/us/ovl_0b/nonmatchings/ovl_0b_2", D_800A7F78);

void Hi_GetSummonBonePos(s32 arg0, s32 arg1, u16 *arg2) {
    func_800DC9D0(btlfx_getSummonModel(arg0, "Hi_GetSummonBonePos()\nid:%d cannot use!")->unk0, arg1, arg2);
}

void Hi_GetSummonBoneMatrix(s32 arg0, s32 arg1, BtlBone *arg2) {
    func_800DCA18(btlfx_getSummonModel(arg0, "Hi_GetSummonBoneMatrix()\nid:%d cannot use!")->unk0, arg1, arg2);
}

void func_800DC9D0(BtlUnit9C *arg0, s32 arg1, u16 *arg2) {
    arg2[0] = arg0->unk20[arg1].unk14;
    arg2[1] = arg0->unk20[arg1].unk18;
    arg2[2] = arg0->unk20[arg1].unk1C;
}

void func_800DCA18(BtlUnit9C *arg0, s32 arg1, BtlBone *arg2) {
    *arg2 = arg0->unk20[arg1];
}

void btl_setModelTransform(BtlUnit9C *arg0, SVECTOR *arg1, VECTOR *arg2, VECTOR *arg3) {
    SVECTOR r;
    VECTOR t;
    if (arg1 != NULL) {
        func_800316D8(arg1, &arg0->unk24);
    } else {
        r.vx = 0;
        r.vy = 0;
        r.vz = 0;
        func_800316D8(&r, &arg0->unk24);
    }
    if (arg2 != NULL) {
        TransMatrix(&arg0->unk24, arg2);
    } else {
        t.vx = 0;
        t.vy = 0;
        t.vz = 0;
        TransMatrix(&arg0->unk24, &t);
    }
    if (arg3 != 0) {
        btl_scaleModelMatrix(arg0, arg3);
    } else {
        arg0->unk54 = 0x1000;
        arg0->unk56 = 0x1000;
        arg0->unk58 = 0x1000;
        func_800C7B84(arg0);
    }
}

/* scales the model matrix by arg1 (MulMatrix0 with a diagonal matrix built word by word) */
void btl_scaleModelMatrix(BtlUnit9C *arg0, VECTOR *arg1) {
    MATRIX m;
    s32 *d;
    s16 x, y, z;

    d = (s32 *)&m;
    x = arg1->vx;
    d[0] = 0x1000;
    m.m[0][0] = x;
    y = arg1->vy;
    d[2] = 0x1000;
    m.m[1][1] = y;
    z = arg1->vz;
    d[1] = 0;
    d[3] = 0;
    d[4] = 0x1000;
    d[5] = 0;
    d[6] = 0;
    d[7] = 0;
    m.m[2][2] = z;
    gte_MulMatrix0(&arg0->unk24, &m, &arg0->unk24);
}

void Hi_ShowSummonModelMesh(s32 arg0, s32 arg1) {
    btlfx_getSummonModel(arg0, "Hi_ShowSummonModelMesh()\nid:%d cannot use!")->unk0->unk14 &= ~(1 << arg1);
}

void Hi_HideSummonModelMesh(s32 arg0, s32 arg1) {
    btlfx_getSummonModel(arg0, "Hi_HideSummonModelMesh()\nid:%d cannot use!")->unk0->unk14 |= 1 << arg1;
}

/* starts texture animation arg1 of summon model arg0 (arg2: also set flag 2) */
void Hi_StartSummonTexAnim(s32 arg0, s32 arg1, u8 arg2) {
    Unk8010141C *p = btlfx_getSummonModel(arg0, "Hi_StartSummonTexAnim()\nid:%d cannot use!");
    s32 off;

    if (arg2) {
        off = arg1 * 4;
        off = (off + arg1) * 4; /* arg1 * sizeof(BtlUnit9C50) */
        ((BtlUnit9C50 *)((u8 *)p->unk0->unk50 + off))->unk4 |= 3;
    } else {
        off = arg1 * 4;
        off = (off + arg1) * 4;
        ((BtlUnit9C50 *)((u8 *)p->unk0->unk50 + off))->unk4 |= 1;
    }
    ((BtlUnit9C50 *)((u8 *)p->unk0->unk50 + off))->unkC = 0;
    ((BtlUnit9C50 *)((u8 *)p->unk0->unk50 + off))->unk12 = 0x1000;
}

void Hi_StopSummonTexAnim(s32 arg0, s32 arg1) {
    BtlUnit9C50 *e = &btlfx_getSummonModel(arg0, "Hi_StopSummonTexAnim()\nid:%d cannot use!")->unk0->unk50[arg1];
    e->unk4 &= ~3;
}

void Hi_ModifyEffModelAbr(s32 arg0, s32 arg1) {
    if (arg1 != 0xFF) {
        btl_setModelAbr(btlfx_getEffModel(arg0, "Hi_ModifyEffModelAbr()\nid:%d cannot use!")->unk0, (u16)(arg1 << 5));
    }
}

void Hi_ModifyEffModelRGB(s32 arg0, s16 arg1, s16 arg2, s16 arg3) {
    btl_setModelRgb(btlfx_getEffModel(arg0, "Hi_ModifyEffModelRGB()\nid:%d cannot use!")->unk0, arg1, arg2, arg3);
}

void Hi_SetEffModelOffset(s32 arg0, s16 arg1) {
    btlfx_getEffModel(arg0, "Hi_SetEffModelOffset()\nid:%d cannot use!")->unk22 = arg1;
}

void Hi_SetEffModelSlice(s32 arg0, s16 arg1) {
    BtlObjSlot *p = btlfx_getEffModel(arg0, "Hi_SetEffModelSlice()\nid:%d cannot use!");
    p->unk1D = 1;
    p->unk24 = arg1;
}

void Hi_ModifySummonModelAbr(s32 arg0, s32 arg1) {
    if (arg1 != 0xFF) {
        btl_setModelAbr(btlfx_getSummonModel(arg0, "Hi_ModifySummonModelAbr()\nid:%d cannot use!")->unk0, (u16)(arg1 << 5));
    }
}

void Hi_ModifySummonModelRGB(s32 arg0, s16 arg1, s16 arg2, s16 arg3) {
    btl_setModelRgb(btlfx_getSummonModel(arg0, "Hi_ModifySummonModelRGB()\nid:%d cannot use!")->unk0, arg1, arg2, arg3);
}

BtlObjSlot *btlfx_getEffModel(s32 arg0, char *arg1) {
    BtlObjSlot *p = &g_btlfxEffModels[arg0];
    if (p->unk1C == 0 || p->unk0 == NULL) {
        sprintf(g_btlfxErrorMsg, arg1, arg0);
        btlfx_error(g_btlfxErrorMsg);
    }
    return p;
}

Unk8010141C *btlfx_getSummonModel(s32 arg0, char *arg1) {
    Unk8010141C *p = &g_btlfxSummonModels[arg0];
    if (p->unk4C == 0 || p->unk0 == NULL) {
        sprintf(g_btlfxErrorMsg, arg1, arg0);
        btlfx_error(g_btlfxErrorMsg);
    }
    return p;
}

void func_800DD0B0(s32 arg0) {
    func_800DD10C(btl_getUnit(arg0)->unk9C);
}

void func_800DD0DC(s32 arg0) {
    func_800DD10C(btlfx_getEffModel(arg0, D_800A81A8)->unk0);
}

u8 func_800DD10C(BtlUnit9C *arg0) {
    return arg0->unk8->unk3;
}

void func_800DD120(BtlTexInfo *arg0, BtlTexInfo *arg1, s32 arg2) {
    s32 k;
    BtlTexInfo *a;
    BtlTexInfo *b;
    u16 i;
    Unk8E0 *e;

    i = (btl_getUnit(0x10)->unk68 >> 1) & 0xF;
    e = &g_gameState->unk1C->unk8E0[i];
    k = arg2 & 0xFFFF;
    k |= 0x20000;
    k |= e->unkC->unk3B << 24;
    a = (BtlTexInfo *)func_800634A8(k);
    b = (BtlTexInfo *)func_800634D8(k);
    *arg0 = *a;
    *arg1 = *b;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", btl_drawMeshOffset);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", btlfx_projectSliceMesh);

u8 *func_800DE11C(BtlUnit9C *arg0, u8 arg1, u8 *prim) {
    s32 opz;
    s32 otz;
    BtlMesh *g;
    u8 (*uv)[2];
    u8 (*col)[4];
    u32 *ot;
    u32 *tex;
    u32 n;
    u32 i;
    s32 xy0, xy1, xy2;
    u32 a, b;
    u32 *cp;
    u32 i1, i3;
    u32 t;
    u32 hi;
    u8 *src;
    WmSrcFT4 *sFT4;
    POLY_FT4 *pFT4;
    WmSrcFT3 *sFT3;
    POLY_FT3 *pFT3;
    WmSrcGT4 *sGT4;
    POLY_GT4 *pGT4;
    WmSrcGT3 *sGT3;
    POLY_GT3 *pGT3;
    WmSrcG4 *sG4;
    POLY_G4 *pG4;
    WmSrcG3 *sG3;
    POLY_G3 *pG3;
    WmSrcF4 *sF4;
    POLY_F4 *pF4;
    WmSrcF3 *sF3;
    POLY_F3 *pF3;

    if (arg0->unk0 & 0x10) {
        return prim;
    }
    g = &arg0->unk8->unk10[arg1];
    src = (u8 *)g->unk1C;
    uv = g->unk20;
    col = g->unk24;
    if (arg0->unk10 == 0) {
        arg0->unk10 = (s32)prim;
    }
    tex = (u32 *)arg0->unk18;
    ot = g_gameState->unk1C->unk10[g_gameState->unk1C->unk8];

    n = g->unk2;
    for (i = 0; i < n; src += sizeof(WmSrcFT4)) {
        sFT4 = (WmSrcFT4 *)src;
        pFT4 = (POLY_FT4 *)prim;
        a = *(u32 *)&sFT4->unk0[0];
        b = *(u32 *)&sFT4->unk0[2];
        i1 = a >> 16;
        i3 = b >> 16;
        xy0 = g_vtxScreenXY[a & 0xFFFF];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[b & 0xFFFF];
        otz = g_vtxScreenZ[a & 0xFFFF] + g_vtxScreenZ[i1] + g_vtxScreenZ[b & 0xFFFF] + g_vtxScreenZ[i3];
        i++;
        if (otz > 0x3FFFFFFF) {
            continue;
        }
        if (!(sFT4->unk15 & 2)) {
            gte_ldsxy3(xy0, xy1, xy2);
            gte_nclip();
            gte_stopz(&opz);
            if (opz <= 0) {
                continue;
            }
        }
        *(u32 *)&pFT4->r0 = *(u32 *)sFT4->unk10;
        setlen(pFT4, 9);
        *(s32 *)&pFT4->x0 = xy0;
        *(s32 *)&pFT4->x1 = xy1;
        *(s32 *)&pFT4->x2 = xy2;
        setcode(pFT4, 0x2C);
        *(s32 *)&pFT4->x3 = g_vtxScreenXY[i3];
        a = *(u32 *)&sFT4->unk8[0];
        b = *(u32 *)&sFT4->unk8[2];
        t = tex[sFT4->unk13];
        hi = t & 0xFFFF0000;
        t <<= 16;
        *(u32 *)&pFT4->u0 = *(u16 *)uv[a & 0xFFFF] | hi;
        *(u32 *)&pFT4->u1 = *(u16 *)uv[a >> 16] | t;
        *(u16 *)&pFT4->u2 = *(u16 *)uv[b & 0xFFFF];
        *(u16 *)&pFT4->u3 = *(u16 *)uv[b >> 16];
        if (sFT4->unk15 & 1) {
            pFT4->tpage |= sFT4->unk15 & 0xE0;
            setSemiTrans(pFT4, 1);
        }
        otz &= 0xFFFFF;
        addPrim(&ot[otz >> 6], pFT4);
        prim += sizeof(POLY_FT4);
    }

    n = g->unk4;
    for (i = 0; i < n; src += sizeof(WmSrcFT3)) {
        sFT3 = (WmSrcFT3 *)src;
        pFT3 = (POLY_FT3 *)prim;
        a = *(u32 *)&sFT3->unk0[0];
        b = sFT3->unk0[2];
        i1 = a >> 16;
        xy0 = g_vtxScreenXY[a & 0xFFFF];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[b];
        otz = g_vtxScreenZ[a & 0xFFFF] + g_vtxScreenZ[i1] + g_vtxScreenZ[b & 0xFFFF];
        i++;
        if (otz > 0x2FFFFFFF) {
            continue;
        }
        if (!(sFT3->unk12 & 2)) {
            gte_ldsxy3(xy0, xy1, xy2);
            gte_nclip();
            gte_stopz(&opz);
            if (opz <= 0) {
                continue;
            }
        }
        otz &= 0xFFFFF;
        *(u32 *)&pFT3->r0 = *(u32 *)sFT3->unk8;
        setlen(pFT3, 7);
        *(s32 *)&pFT3->x0 = xy0;
        *(s32 *)&pFT3->x1 = xy1;
        *(s32 *)&pFT3->x2 = xy2;
        setcode(pFT3, 0x24);
        t = tex[sFT3->unk6];
        a = *(u32 *)&sFT3->unkC[0];
        hi = t & 0xFFFF0000;
        t <<= 16;
        *(u32 *)&pFT3->u0 = *(u16 *)uv[a & 0xFFFF] | hi;
        *(u32 *)&pFT3->u1 = *(u16 *)uv[a >> 16] | t;
        *(u16 *)&pFT3->u2 = *(u16 *)uv[sFT3->unkC[2]];
        otz /= 3;
        if (sFT3->unk12 & 1) {
            pFT3->tpage |= sFT3->unk12 & 0xE0;
            setSemiTrans(pFT3, 1);
        }
        addPrim(&ot[otz >> 4], pFT3);
        prim += sizeof(POLY_FT3);
    }

    n = g->unk6;
    for (i = 0; i < n; src += sizeof(WmSrcGT4)) {
        sGT4 = (WmSrcGT4 *)src;
        pGT4 = (POLY_GT4 *)prim;
        a = *(u32 *)&sGT4->unk0[0];
        b = *(u32 *)&sGT4->unk0[2];
        i1 = a >> 16;
        i3 = b >> 16;
        xy0 = g_vtxScreenXY[a & 0xFFFF];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[b & 0xFFFF];
        otz = g_vtxScreenZ[a & 0xFFFF] + g_vtxScreenZ[i1] + g_vtxScreenZ[b & 0xFFFF] + g_vtxScreenZ[i3];
        i++;
        if (otz > 0x3FFFFFFF) {
            continue;
        }
        if (!(sGT4->unk1C & 2)) {
            gte_ldsxy3(xy0, xy1, xy2);
            gte_nclip();
            gte_stopz(&opz);
            if (opz <= 0) {
                continue;
            }
        }
        *(s32 *)&pGT4->x0 = xy0;
        *(s32 *)&pGT4->x1 = xy1;
        *(s32 *)&pGT4->x2 = xy2;
        *(s32 *)&pGT4->x3 = g_vtxScreenXY[i3];
        a = *(u32 *)&sGT4->unk10[0];
        i1 = *(u32 *)&sGT4->unk10[2];
        t = tex[sGT4->unk1D];
        cp = (u32 *)col[a & 0xFFFF];
        *(u32 *)&pGT4->r0 = *cp;
        cp = (u32 *)col[a >> 16];
        *(u32 *)&pGT4->r1 = *cp;
        cp = (u32 *)col[i1 & 0xFFFF];
        *(u32 *)&pGT4->r2 = *cp;
        cp = (u32 *)col[i1 >> 16];
        *(u32 *)&pGT4->r3 = *cp;
        setlen(pGT4, 12);
        setcode(pGT4, 0x3C);
        a = *(u32 *)&sGT4->unk8[0];
        i1 = *(u32 *)&sGT4->unk8[2];
        hi = t & 0xFFFF0000;
        t <<= 16;
        *(u32 *)&pGT4->u0 = *(u16 *)uv[a & 0xFFFF] | hi;
        *(u32 *)&pGT4->u1 = *(u16 *)uv[a >> 16] | t;
        *(u16 *)&pGT4->u2 = *(u16 *)uv[i1 & 0xFFFF];
        *(u16 *)&pGT4->u3 = *(u16 *)uv[i1 >> 16];
        if (sGT4->unk1C & 1) {
            pGT4->tpage |= sGT4->unk1C & 0xE0;
            setSemiTrans(pGT4, 1);
        }
        otz &= 0xFFFFF;
        addPrim(&ot[otz >> 6], pGT4);
        prim += sizeof(POLY_GT4);
    }

    n = g->unk8;
    for (i = 0; i < n; src += sizeof(WmSrcGT3)) {
        sGT3 = (WmSrcGT3 *)src;
        pGT3 = (POLY_GT3 *)prim;
        a = *(u32 *)&sGT3->unk0[0];
        b = sGT3->unk0[2];
        i1 = a >> 16;
        xy0 = g_vtxScreenXY[a & 0xFFFF];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[b];
        otz = g_vtxScreenZ[a & 0xFFFF] + g_vtxScreenZ[i1] + g_vtxScreenZ[b & 0xFFFF];
        i++;
        if (otz > 0x2FFFFFFF) {
            continue;
        }
        if (!(sGT3->unkF & 2)) {
            gte_ldsxy3(xy0, xy1, xy2);
            gte_nclip();
            gte_stopz(&opz);
            if (opz <= 0) {
                continue;
            }
        }
        otz &= 0xFFFFF;
        *(s32 *)&pGT3->x0 = xy0;
        *(s32 *)&pGT3->x1 = xy1;
        *(s32 *)&pGT3->x2 = xy2;
        a = *(u32 *)&sGT3->unk10[0];
        t = tex[sGT3->unk16];
        cp = (u32 *)col[a & 0xFFFF];
        *(u32 *)&pGT3->r0 = *cp;
        cp = (u32 *)col[a >> 16];
        *(u32 *)&pGT3->r1 = *cp;
        cp = (u32 *)col[sGT3->unk10[2]];
        *(u32 *)&pGT3->r2 = *cp;
        setlen(pGT3, 9);
        setcode(pGT3, 0x34);
        hi = t & 0xFFFF0000;
        t <<= 16;
        *(u32 *)&pGT3->u0 = *(u16 *)uv[sGT3->unk6[0]] | hi;
        a = *(u32 *)&sGT3->unk6[1];
        *(u32 *)&pGT3->u1 = *(u16 *)uv[a & 0xFFFF] | t;
        *(u16 *)&pGT3->u2 = *(u16 *)uv[a >> 16];
        otz /= 3;
        if (sGT3->unkF & 1) {
            pGT3->tpage |= sGT3->unkF & 0xE0;
            setSemiTrans(pGT3, 1);
        }
        addPrim(&ot[otz >> 4], pGT3);
        prim += sizeof(POLY_GT3);
    }

    n = g->unkA;
    for (i = 0; i < n; src += sizeof(WmSrcG4)) {
        sG4 = (WmSrcG4 *)src;
        pG4 = (POLY_G4 *)prim;
        a = *(u32 *)&sG4->unk0[0];
        b = *(u32 *)&sG4->unk0[2];
        i1 = a >> 16;
        i3 = b >> 16;
        xy0 = g_vtxScreenXY[a & 0xFFFF];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[b & 0xFFFF];
        otz = g_vtxScreenZ[a & 0xFFFF] + g_vtxScreenZ[i1] + g_vtxScreenZ[b & 0xFFFF] + g_vtxScreenZ[i3];
        i++;
        if (otz > 0x3FFFFFFF) {
            continue;
        }
        if (!(sG4->unk14 & 2)) {
            gte_ldsxy3(xy0, xy1, xy2);
            gte_nclip();
            gte_stopz(&opz);
            if (opz <= 0) {
                continue;
            }
        }
        *(s32 *)&pG4->x0 = xy0;
        *(s32 *)&pG4->x1 = xy1;
        *(s32 *)&pG4->x2 = xy2;
        *(s32 *)&pG4->x3 = g_vtxScreenXY[i3];
        otz &= 0xFFFFF;
        a = *(u32 *)&sG4->unk8[0];
        i1 = *(u32 *)&sG4->unk8[2];
        cp = (u32 *)col[a & 0xFFFF];
        *(u32 *)&pG4->r0 = *cp;
        cp = (u32 *)col[a >> 16];
        *(u32 *)&pG4->r1 = *cp;
        cp = (u32 *)col[i1 & 0xFFFF];
        *(u32 *)&pG4->r2 = *cp;
        cp = (u32 *)col[i1 >> 16];
        *(u32 *)&pG4->r3 = *cp;
        setlen(pG4, 8);
        setcode(pG4, 0x38);
        addPrim(&ot[otz >> 6], pG4);
        prim += sizeof(POLY_G4);
    }

    n = g->unkC;
    for (i = 0; i < n; src += sizeof(WmSrcG3)) {
        sG3 = (WmSrcG3 *)src;
        pG3 = (POLY_G3 *)prim;
        a = *(u32 *)&sG3->unk0[0];
        b = sG3->unk0[2];
        i1 = a >> 16;
        xy0 = g_vtxScreenXY[a & 0xFFFF];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[b];
        otz = g_vtxScreenZ[a & 0xFFFF] + g_vtxScreenZ[i1] + g_vtxScreenZ[b & 0xFFFF];
        i++;
        if (otz > 0x2FFFFFFF) {
            continue;
        }
        if (!(sG3->unk13 & 2)) {
            gte_ldsxy3(xy0, xy1, xy2);
            gte_nclip();
            gte_stopz(&opz);
            if (opz <= 0) {
                continue;
            }
        }
        otz &= 0xFFFFF;
        *(s32 *)&pG3->x0 = xy0;
        *(s32 *)&pG3->x1 = xy1;
        *(s32 *)&pG3->x2 = xy2;
        a = *(u32 *)&sG3->unk8[0];
        cp = (u32 *)col[a & 0xFFFF];
        *(u32 *)&pG3->r0 = *cp;
        cp = (u32 *)col[a >> 16];
        *(u32 *)&pG3->r1 = *cp;
        cp = (u32 *)col[sG3->unk8[2]];
        *(u32 *)&pG3->r2 = *cp;
        setlen(pG3, 6);
        setcode(pG3, 0x30);
        otz /= 3;
        addPrim(&ot[otz >> 4], pG3);
        prim += sizeof(POLY_G3);
    }

    n = g->unkE;
    for (i = 0; i < n; src += sizeof(WmSrcF4)) {
        sF4 = (WmSrcF4 *)src;
        pF4 = (POLY_F4 *)prim;
        a = *(u32 *)&sF4->unk0[0];
        b = *(u32 *)&sF4->unk0[2];
        i1 = a >> 16;
        i3 = b >> 16;
        xy0 = g_vtxScreenXY[a & 0xFFFF];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[b & 0xFFFF];
        otz = g_vtxScreenZ[a & 0xFFFF] + g_vtxScreenZ[i1] + g_vtxScreenZ[b & 0xFFFF] + g_vtxScreenZ[i3];
        i++;
        if (otz > 0x3FFFFFFF) {
            continue;
        }
        if (!(sF4->unkD & 2)) {
            gte_ldsxy3(xy0, xy1, xy2);
            gte_nclip();
            gte_stopz(&opz);
            if (opz <= 0) {
                continue;
            }
        }
        *(u32 *)&pF4->r0 = *(u32 *)sF4->unk8;
        setlen(pF4, 5);
        *(s32 *)&pF4->x0 = xy0;
        *(s32 *)&pF4->x1 = xy1;
        *(s32 *)&pF4->x2 = xy2;
        setcode(pF4, 0x28);
        otz &= 0xFFFFF;
        *(s32 *)&pF4->x3 = g_vtxScreenXY[i3];
        addPrim(&ot[otz >> 6], pF4);
        prim += sizeof(POLY_F4);
    }

    n = g->unk10;
    for (i = 0; i < n; src += sizeof(WmSrcF3)) {
        sF3 = (WmSrcF3 *)src;
        pF3 = (POLY_F3 *)prim;
        a = *(u32 *)&sF3->unk0[0];
        b = sF3->unk0[2];
        i1 = a >> 16;
        xy0 = g_vtxScreenXY[a & 0xFFFF];
        xy1 = g_vtxScreenXY[i1];
        xy2 = g_vtxScreenXY[b];
        otz = g_vtxScreenZ[a & 0xFFFF] + g_vtxScreenZ[i1] + g_vtxScreenZ[b & 0xFFFF];
        i++;
        if (otz > 0x2FFFFFFF) {
            continue;
        }
        if (!(sF3->unkB & 2)) {
            gte_ldsxy3(xy0, xy1, xy2);
            gte_nclip();
            gte_stopz(&opz);
            if (opz <= 0) {
                continue;
            }
        }
        otz &= 0xFFFFF;
        *(u32 *)&pF3->r0 = *(u32 *)sF3->unk8;
        setlen(pF3, 4);
        setcode(pF3, 0x20);
        *(s32 *)&pF3->x0 = xy0;
        *(s32 *)&pF3->x1 = xy1;
        *(s32 *)&pF3->x2 = xy2;
        otz /= 3;
        addPrim(&ot[otz >> 4], pF3);
        prim += sizeof(POLY_F3);
    }
    setcode((P_TAG *)prim, 0xFF);
    return prim + sizeof(P_TAG);
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", btlfx_projectMorphMesh);

INCLUDE_RODATA("asm/us/ovl_0b/nonmatchings/ovl_0b_2", D_800A81A8);

void Hi_SplitMdlVertex(s32 arg0, s32 arg1, s32 arg2) {
    GeoSplitVertices(btlfx_getEffModel(arg0, "Hi_SplitMdlVertex()\nid:%d cannot use!")->unk0, arg1, arg2);
}

s32 Hi_GetSplitMdlVertex(s32 arg0, s32 arg1) {
    return func_800DF670(btlfx_getEffModel(arg0, "Hi_GetSplitMdlVertex()\nid:%d cannot use!")->unk0, arg1);
}

void GeoSplitVertices(BtlUnit9C *arg0, s32 arg1, s32 arg2) {
    s32 n;
    BtlMesh *m;
    s32 i;

    n = arg0->unk8->unk3;
    m = arg0->unk8->unk10;
    for (i = 0; i < n; i++) {
        if (arg1 == -1 || arg1 == i) {
            arg2 = func_800DF940(arg0, i, arg2);
            if (m->unk14[i] >= 0x322) {
                printf("warning:GeoSplitVertices() overflow vertex buffer!(%d)\n", m->unk14[i]);
                m->unk14[i] = 0x321;
            }
        }
    }
}

s32 func_800DF670(BtlUnit9C *arg0, s32 arg1) {
    BtlUnit9C8 *m = arg0->unk8;
    s32 n = m->unk3;
    s32 r = 0;
    s32 i;
    for (i = 0; i < n; i++) {
        if (arg1 == -1 || arg1 == i) {
            BtlMesh *g = &m->unk10[i];
            r += g->unk2 * 4;
            r += g->unk4 * 3;
            r += g->unk6 * 4;
            r += g->unk8 * 3;
            r += g->unkA * 4;
            r += g->unkC * 3;
            r += g->unkE * 4;
            r += g->unk10 * 3;
        }
    }
    return r * 8;
}

void Hi_GetMdlVertexPtr(s32 arg0, u16 arg1, s32 arg2, s32 *arg3, s32 arg4) {
    func_800DF7A0(btlfx_getEffModel(arg0, D_800A825C)->unk0, arg1, arg2, arg3, arg4);
}

u16 func_800DF7A0(BtlUnit9C *arg0, u16 arg1, s32 arg2, s32 *arg3, s32 arg4) {
    BtlMesh *m = &arg0->unk8->unk10[arg1];
    if (arg4 == 0) {
        *arg3 = arg2;
        return m->unk2;
    }
    arg2 += m->unk2 << 5;
    if (arg4 == 1) {
        *arg3 = arg2;
        return m->unk4;
    }
    arg2 += m->unk4 * 0x18;
    if (arg4 == 2) {
        *arg3 = arg2;
        return m->unk6;
    }
    arg2 += m->unk6 << 5;
    if (arg4 == 3) {
        *arg3 = arg2;
        return m->unk8;
    }
    arg2 += m->unk8 * 0x18;
    if (arg4 == 4) {
        *arg3 = arg2;
        return m->unkA;
    }
    arg2 += m->unkA << 5;
    if (arg4 == 5) {
        *arg3 = arg2;
        return m->unkC;
    }
    arg2 += m->unkC * 0x18;
    if (arg4 == 6) {
        *arg3 = arg2;
        return m->unkE;
    }
    arg2 += m->unkE << 5;
    if (arg4 == 7) {
        *arg3 = arg2;
        return m->unk10;
    }
    *arg3 = 0;
    return 0;
}

void func_800DF920(void) {
}

s32 func_800DF928(void) {
    return 0;
}

s32 func_800DF930(void) {
    return 0;
}

void func_800DF938(void) {
}

/* gives every vertex reference of nv-vertex primitives its own copy in arg2 */
#define SPLIT_PRIMS(cnt, stride, nv)                                \
    for (j = 0; j < (cnt); j++) {                                   \
        for (k = 0; k < (nv); k++) {                                \
            ((SVECTOR *)arg2)[n].vx = src[((u16 *)p)[k]].vx;        \
            ((SVECTOR *)arg2)[n].vy = src[((u16 *)p)[k]].vy;        \
            ((SVECTOR *)arg2)[n].vz = src[((u16 *)p)[k]].vz;        \
            ((u16 *)p)[k] = n;                                      \
            n++;                                                    \
        }                                                           \
        p += (stride);                                              \
    }

u32 func_800DF940(BtlUnit9C *arg0, u16 arg1, s32 arg2) {
    u16 n;
    BtlMesh *g;
    u8 *p;
    SVECTOR *src;
    u32 j;
    u32 k;

    n = 0;
    g = &arg0->unk8->unk10[arg1];
    p = (u8 *)g->unk1C;
    src = g->unk18;
    g->unk18 = (SVECTOR *)arg2;
    SPLIT_PRIMS(g->unk2, 0x18, 4);
    SPLIT_PRIMS(g->unk4, 0x14, 3);
    SPLIT_PRIMS(g->unk6, 0x20, 4);
    SPLIT_PRIMS(g->unk8, 0x18, 3);
    SPLIT_PRIMS(g->unkA, 0x18, 4);
    SPLIT_PRIMS(g->unkC, 0x14, 3);
    SPLIT_PRIMS(g->unkE, 0x10, 4);
    SPLIT_PRIMS(g->unk10, 0xC, 3);
    g->unk14[arg1] = n;
    return arg2 + n * 2;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800DFE4C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800DFE54);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800DFF5C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800DFF64);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E0014);

void func_800E00DC(s32 arg0, Pool30Entry *arg1, Pool30 *arg2) {
    s32 i = 0;
    arg2->unk0 = arg0 / (s32)sizeof(Pool30Entry);
    arg2->unkC = arg1;
    arg2->unk4 = 0;
    arg2->unk8 = 0;
    for (; i < arg2->unk0;) {
        i++;
        arg1->unk1F = 0;
        arg1++;
    }
}

s32 func_800E0134(Pool30Entry *arg0, Pool6CEntry *arg1) {
    s32 p = arg1->unk14;
    s32 t;
    if (p == 0) {
        return 0;
    }
    t = arg0->unk1F;
    t = (t - 1) * arg1->unkC;
    return p + t;
}

INCLUDE_RODATA("asm/us/ovl_0b/nonmatchings/ovl_0b_2", D_800A825C);

s32 func_800E0168(Pool30 *pool) {
    SVECTOR rel;
    SVECTOR pos;
    SVECTOR v;
    VECTOR d;
    s32 alpha;
    s32 cnt;
    s32 scale;
    Pool30Entry *e;
    Pool6CEntry *owner;
    EffMotion *m;
    EffElem *el;
    EffSub *sub;
    SVECTOR *vp;
    s16 *rp;
    s32 i;
    s32 frame;
    s32 frames;
    s32 last;
    s32 lvl;
    s32 t;
    s32 range;
    s32 ofs;
    s32 curve;
    s32 mode;
    s32 rs;
    s32 amp;
    s32 k;
    u32 fl;

    lvl = 0;
    range = 0;
    curve = 0;
    last = 0;
    e = pool->unkC;
    amp = 0;
    i = 0;
    D_801014B0 = g_btlfxEnv->unk28;
    pool->unk4 = 0;
    for (; i <= pool->unk8; i++, e++) {
        if (e->unk1F == 0) {
            continue;
        }
        frame = e->unk1A;
        frames = e->unk1B;
        owner = e->unk2C;
        if (frame >= frames) {
            e->unk1F = 0;
            if (e->unk12 & 0x80) {
                owner->unk34.s.y--;
            } else {
                owner->unk34.s.x--;
            }
            continue;
        }
        last = i;
        pool->unk4++;
        t = (frame << 12) / frames;
        if (e->unk12 & 0x80) {
            m = (EffMotion *)owner->unk4;
            sub = owner->unk4 - 1;
            mode = sub->unk24;
            rs = owner->unk56;
            rp = owner->unk50;
            if (mode != 0) {
                curve = sub->unk26;
                range = owner->unk58.s.x;
                if (mode < 7) {
                    amp = owner->unk58.s.y;
                } else {
                    amp = sub->unk28[1];
                }
            }
        } else {
            m = (EffMotion *)owner->unk0;
            mode = owner->unk24->unk4.unk1C;
            rs = owner->unk4A;
            rp = owner->unk44;
            if (mode != 0) {
                curve = owner->unk24->unk4.unk1E;
                range = owner->unk4C.s.x;
                if (mode < 7) {
                    amp = owner->unk4C.s.y;
                } else {
                    amp = owner->unk24->unk4.unk20[1];
                }
            }
            if (owner->unk4 != NULL) {
                lvl = (owner->unk4 - 1)->unkC;
                if (lvl >= 4) {
                    rel.vx = e->unk0.vx >> 12;
                    rel.vy = e->unk0.vy >> 12;
                    rel.vz = e->unk0.vz >> 12;
                }
            }
        }
        switch (m->unk0) {
        case 0:
            func_800DFE4C(e, rs, rp, &pos);
            if (m->unk18 != 0) {
                e->unkC += btlfx_randCentered(m->unk1A);
                e->unkE += btlfx_randCentered(m->unk1C);
                e->unk10 += btlfx_randCentered(m->unk1E);
            }
            if (m->unk12 != 0) {
                if (m->unk12 == 1) {
                    if (owner->unk5C.pad - m->unk14 < pos.vy && e->unkE > 0) {
                        e->unkE = -e->unkE;
                        e->unkE = (e->unkE * m->unk16) >> 8;
                    }
                } else if (owner->unk5C.pad - m->unk14 < pos.vy) {
                    if (e->unkE > 0) {
                        s32 x = e->unkE * (owner->unk5C.pad - pos.vy);
                        if (m->unk14 != 0) {
                            x /= m->unk14;
                        }
                        e->unkE = x;
                    }
                    if (owner->unk5C.pad - (m->unk14 >> 1) < pos.vy) {
                        e->unkE = (s16)(e->unkE - 0x40) - ((owner->unk4A * e->unk1E) >> 6);
                    }
                    if (pos.vy > owner->unk5C.pad) {
                        e->unkE = 0;
                    }
                }
            }
            break;
        case 1:
            func_800DFE54(e, rs, rp, &pos);
            e->unk24.s.x += e->unk20;
            e->unk24.s.y += e->unk22;
            v.vx = e->unk24.s.x;
            v.vy = e->unk24.s.y;
            switch (m->unk14) {
            case 0:
                rs = (math_sin(btlfx_lerp(m->unk1C, m->unk1E, t)) * e->unk28.s.x) >> 12;
                break;
            case 1:
                rs = (e->unk28.s.x * t) >> 12;
                break;
            case 2:
                rs = e->unk28.s.x - ((e->unk28.s.x * t) >> 12);
                break;
            }
            func_800CFD78((s16 *)&pos, (s16 *)&v, rs, (s16 *)&pos);
            break;
        case 2:
            func_800DFF5C(e, rs, rp, &pos);
            e->unkC += e->unkE;
            e->unk20 += e->unk22;
            e->unk24.s.x += e->unk24.s.y;
            func_800D044C(&pos, (s32)&pos, e->unk20 >> 2, &e->unk28, e->unk24.s.x >> 2, e->unkC);
            break;
        case 3:
            func_800DFF64(e, rs, rp, &pos);
            e->unkC += e->unk20;
            e->unkE += e->unk22;
            v.vx = e->unk24.s.y + math_sin(e->unkC);
            v.vy = e->unk28.s.x + math_sin(e->unkE);
            switch (m->unk14) {
            case 0:
                rs = (math_sin(btlfx_lerp(m->unk1C, m->unk1E, t)) * e->unk10) >> 12;
                break;
            case 1:
                rs = (e->unk10 * t) >> 12;
                break;
            case 2:
                rs = e->unk10 - ((e->unk10 * t) >> 12);
                break;
            }
            func_800CFD78((s16 *)&pos, (s16 *)&v, rs, (s16 *)&pos);
            break;
        case 4:
            func_800DFF5C(e, rs, rp, &pos);
            e->unk24.w += e->unk22;
            e->unkC += e->unkE;
            switch (m->unkE) {
            case 0:
                rs = (math_sin(btlfx_lerp(m->unk1C, m->unk1E, t)) * e->unk20) >> 12;
                break;
            case 1:
                rs = (e->unk20 * t) >> 12;
                break;
            case 2:
                rs = e->unk20 - ((e->unk20 * t) >> 12);
                break;
            }
            func_800D044C(&pos, (s32)&pos, e->unk24.w >> 6, &e->unk28, rs, e->unkC);
            break;
        case 5:
            func_800DFE4C(e, rs, rp, &pos);
            if (m->unk18 != 0) {
                e->unkC += btlfx_randCentered(m->unk1A);
                e->unkE += btlfx_randCentered(m->unk1C);
                e->unk10 += btlfx_randCentered(m->unk1E);
            }
            rs = btlfx_lerp(m->unk12, m->unk14, t);
            btlfx_aimAngles((s16 *)&pos, (s16 *)owner->unk2C, (s16 *)&v);
            v.vx += m->unk20;
            v.vy += m->unk22;
            func_800CFD78(&e->unkC, (s16 *)&v, rs, &e->unkC);
            break;
        case 0xFF:
            if (m->unk8(e, owner, &pos, func_800E0134(e, owner), frame, frames) != 0) {
                e->unk1F = 0;
                if (e->unk12 & 0x80) {
                    owner->unk34.s.y--;
                } else {
                    owner->unk34.s.x--;
                }
            }
            break;
        }
        ofs = 0xFFFF;
        if (mode != 0) {
            switch (mode) {
            case 1:
                /* padded: a zero word follows the first jump table */
                __asm__(".section .rodata\n\t.word 0\n\t.section .text");
                cnt = btlfx_distance(&pos, owner->unk64);
                if (cnt <= range) {
                    d.vx = pos.vx - owner->unk64->vx;
                    d.vy = pos.vy - owner->unk64->vy;
                    d.vz = pos.vz - owner->unk64->vz;
                    VectorNormalS(&d, &v);
                }
                break;
            case 2:
                cnt = btlfx_distance(&pos, owner->unk64);
                if (cnt <= range) {
                    d.vx = owner->unk64->vx - pos.vx;
                    d.vy = owner->unk64->vy - pos.vy;
                    d.vz = owner->unk64->vz - pos.vz;
                    VectorNormalS(&d, &v);
                }
                break;
            case 3:
            case 4:
                *(s32 *)&v.vx = *(s32 *)&pos.vx;
                v.vy = owner->unk64->vy;
                *(s32 *)&v.vz = *(s32 *)&pos.vz;
                cnt = btlfx_distance(&v, owner->unk64);
                ((s32 *)&v)[0] = 0;
                v.vy = -0x1000;
                break;
            case 5:
                func_800D0674(&pos, owner->unk64, owner->unk68, (s32)&alpha, (s32)&cnt, &scale);
                if (cnt <= range) {
                    func_800CFCA8(owner->unk68, 0x1000, (s32)&v);
                }
                break;
            case 6:
                func_800D0674(&pos, owner->unk64, owner->unk68, (s32)&alpha, (s32)&cnt, &scale);
                if (cnt <= range) {
                    ((s32 *)&v)[1] = 0;
                    ((s32 *)&v)[0] = 0;
                    func_800D044C(&v, (s32)&v, 0, (XY32 *)owner->unk68, 0x1000, scale);
                }
                break;
            case 7:
                cnt = btlfx_distance(&pos, owner->unk64);
                break;
            case 8:
                *(s32 *)&v.vx = *(s32 *)&pos.vx;
                v.vy = owner->unk64->vy;
                *(s32 *)&v.vz = *(s32 *)&pos.vz;
                cnt = btlfx_distance(&v, owner->unk64);
                break;
            case 9:
                func_800D0674(&pos, owner->unk64, owner->unk68, (s32)&alpha, (s32)&cnt, &scale);
                break;
            }
            if (cnt <= range) {
                scale = (cnt << 10) / range;
                d.vx = v.vx;
                d.vy = v.vy;
                d.vz = v.vz;
                switch (curve) {
                case 0:
                    break;
                case 1:
                    amp = (math_cos(scale) * amp) >> 12;
                    break;
                case 2:
                    amp = (math_sin(scale) * amp) >> 12;
                    break;
                case 3:
                    amp = ((0x1000 - math_sin(scale)) * amp) >> 12;
                    break;
                case 4:
                    amp = ((0x1000 - math_cos(scale)) * amp) >> 12;
                    break;
                case 5:
                    amp = (math_sin(scale * 2) * amp) >> 12;
                    break;
                }
                if (mode < 7) {
                    gte_lddp(amp);
                    gte_ldir(&d);
                    gte_gpf12();
                    gte_stir(&d);
                    e->unk0.vx += d.vx << 6;
                    e->unk0.vy += d.vy << 6;
                    e->unk0.vz += d.vz << 6;
                } else {
                    ofs = amp;
                }
            } else if (mode >= 7) {
                ofs = 0;
            }
        }
        g_btlfxEnv->unk28 = owner->unk22;
        if (e->unk1C != 0xFF) {
            el = &owner->unk18[e->unk1C];
            fl = el->unk3;
            vp = NULL;
            if (!(fl & 1)) {
                vp = &v;
                *(s32 *)&vp->vx = e->unk14.w;
                vp->vz = e->unk18;
                vp->vx += el->unkA;
                vp->vy += el->unkC;
                vp->vz += el->unkE;
                v.pad = ((fl >> 2) ^ 1) & 1;
                rs = e->unk13 * 4;
                if (fl & 0x10) {
                    vp->vx += rs;
                }
                if (fl & 0x20) {
                    vp->vy += rs;
                }
                if (fl & 0x40) {
                    vp->vz += rs;
                }
                e->unk14.w = *(s32 *)&vp->vx;
                e->unk18 = vp->vz;
            }
            if (fl & 8) {
                alpha = owner->unk20;
            } else {
                rs = owner->unk20;
                switch (el->unk6) {
                case 1:
                    alpha = (rs * math_cos(t >> 2)) >> 12;
                    break;
                case 2:
                    alpha = (rs * math_sin(t >> 2)) >> 12;
                    break;
                case 3:
                    alpha = (rs * math_sin(t >> 1)) >> 12;
                    break;
                }
            }
            if (ofs != 0xFFFF) {
                alpha += ofs;
                if (alpha < 0) {
                    alpha = 0;
                }
                if (alpha > 0x1000) {
                    alpha = 0x1000;
                }
            }
            if (alpha == 0x1000) {
                alpha = -1;
            }
            if (fl & 2) {
                scale = owner->unk1C;
            } else {
                rs = owner->unk1C;
                scale = (btlfx_lerp(el->unk1A << 7, el->unk1B << 7, t) * rs) >> 12;
            }
            rs = el->unk2;
            if ((rs == 0xFF || rs == 0) && (u32)alpha < 0x801) {
                rs = 1;
            }
            cnt = rs;
            for (k = 0; k < 2; k++) {
                if (k == 0) {
                    rs = el->unk0;
                    mode = el->unk1C;
                } else {
                    rs = el->unk1;
                    mode = el->unk20;
                }
                switch (rs) {
                case 9:
                    if (vp == NULL) {
                        vp = &v;
                        ((s32 *)&v)[0] = 0;
                        ((s32 *)&v)[1] = 0;
                    }
                    el->unk24(e, owner, &pos, vp, &scale, &alpha, &cnt, frame, frames);
                    break;
                case 0:
                    func_800D1680((u8 *)mode, &pos, vp, scale, e->unk1D, cnt, alpha, 0, 0);
                    break;
                case 1:
                    func_800D198C((u8 *)mode, &pos, e->unk1D, cnt, alpha);
                    break;
                case 2:
                    func_800D1D74((u8 *)mode, &pos, vp, scale, e->unk1D, cnt, alpha);
                    break;
                case 3:
                    func_800D1FFC((u8 *)mode, &pos, vp, scale, e->unk1D, cnt, alpha);
                    break;
                case 4:
                    func_800D1494((u8 *)mode, (s32)&pos, (s32 *)owner->unk28, e->unk1D, scale, alpha, cnt, (u8 *)func_800E0134(e, owner), 1);
                    break;
                case 5:
                    func_800D1494((u8 *)mode, (s32)&pos, NULL, e->unk1D, scale, alpha, cnt, (u8 *)func_800E0134(e, owner), 2);
                    break;
                case 6:
                    func_800D23D8((u8 *)mode, &pos, vp, scale, e->unk1D, cnt, alpha);
                    break;
                case 7:
                    func_800D2938((Unk800D2938 *)mode, &pos, vp, scale, e->unk1D, cnt, alpha);
                    break;
                case 8:
                    func_800D1494((u8 *)mode, (s32)&pos, NULL, e->unk1D, scale, alpha, cnt, (u8 *)func_800E0134(e, owner), 0);
                    break;
                }
                if (el->unk1 == 0xFF) {
                    break;
                }
            }
            if (el->unk4 == 1) {
                e->unk1D++;
            } else if (el->unk4 == 2) {
                rs = e->unk1D;
                rs++;
                if (rs >= el->unk8) {
                    rs = 0;
                }
                e->unk1D = rs;
            }
        }
        if (owner->unk4 != NULL && (e->unk12 & 0x7F)) {
            if (lvl >= 4) {
                btlfx_aimAngles((s16 *)&rel, (s16 *)&pos, (s16 *)&v);
            }
            func_800E2F84(pool, e, &pos, &v);
        }
        frame++;
        e->unk1A = frame;
    }
    pool->unk8 = last;
    g_btlfxEnv->unk28 = D_801014B0;
    return pool->unk4;
}

void func_800E14D8(u32 arg0, Pool6C *arg1) {
    s32 i;
    Pool6CEntry *e = arg1->unk10;
    arg1->unk0 = (arg0 - 0x10) / sizeof(Pool6CEntry);
    arg1->unkC = e;
    arg1->unk4 = 0;
    arg1->unk8 = 0;
    for (i = 0; i < arg1->unk0;) {
        i++;
        e->unk30 = 0;
        e++;
    }
}

Pool6CEntry *func_800E1534(Pool30 *arg0, Pool6C *arg1, u8 *arg2, s32 arg3) {
    Pool6CEntry *e;
    s32 i;
    u32 k;
    u8 *p;
    EffElemHdr *h;
    EffElem *el;
    s32 *q;
    s32 v;

    e = arg1->unkC;
    for (i = 0; i < arg1->unk0; i++, e++) {
        if (e->unk30 == 0) {
            if (arg1->unk8 < i) {
                arg1->unk8 = i;
            }
            p = arg2;
            for (k = 0; k < 0x6C; k += 4) {
                *(s32 *)((u8 *)e + k) = 0;
            }
            e->unk30 = 1;
            e->unkE = 0;
            e->unk10 = 0;
            e->unk32 = 0;
            e->unk34.s.x = 0;
            e->unk34.s.y = 0;
            e->unk14 = 0;
            e->unk3A = 0xFF;
            e->unk22 = 0;
            e->unk0 = (EffFileHdr *)p;
            if (*p != 0xFF) {
                p += 0x28;
            } else {
                p += 0x10;
            }
            if (((u8 *)e->unk0)[1] != 0) {
                p += 0x38;
                e->unk4 = (EffSub *)p;
                p += 0x28;
            } else {
                e->unk4 = NULL;
            }
            h = (EffElemHdr *)p;
            p += 0x14;
            el = (EffElem *)p;
            e->unk18 = el;
            i = 0;
            p += h->unk4 * 0x28;
            for (; i < h->unk4; i++, el++) {
                if (el->unk0 != 9) {
                    if (el->unk1C != -1 && !(el->unk1C & 0x80000000)) {
                        el->unk1C = (s32)arg2 + el->unk1C;
                    }
                }
                if (el->unk1 != 0xFF) {
                    if (el->unk20 != -1 && !(el->unk20 & 0x80000000)) {
                        el->unk20 = (s32)arg2 + el->unk20;
                    }
                }
            }
            /* unk24 (tail), unk28, unk2C written through one pointer */
            q = (s32 *)&e->unk24;
            q[0] = (s32)p;
            v = *(s32 *)p;
            k = (u32)p;
            if (v == -1) {
                *(s32 *)&((EffTail *)k)->unk3C = 0;
            }
            q[1] = arg3;
            q[2] = arg3;
            do {
            } while (0);
            return e;
        }
    }
    return NULL;
}

void func_800E1708(Pool30 *arg0, s32 arg1, s32 arg2) {
    Pool6CEntry *m = (Pool6CEntry *)arg1;
    SVECTOR *v = (SVECTOR *)arg2;
    VECTOR d;
    Pool30Entry *e;
    s32 i;
    d.vx = (v->vx - m->unk28->vx) << 12;
    d.vy = (v->vy - m->unk28->vy) << 12;
    d.vz = (v->vz - m->unk28->vz) << 12;
    e = arg0->unkC;
    for (i = 0; i < arg0->unk0; i++, e++) {
        if (e->unk1F != 0 && e->unk2C == m) {
            e->unk0.vx += d.vx;
            e->unk0.vy += d.vy;
            e->unk0.vz += d.vz;
        }
    }
    *(s32 *)&m->unk28->vx = *(s32 *)&v->vx;
    m->unk28->vz = v->vz;
}

void func_800E1810(Pool30 *arg0, Pool6CEntry *arg1) {
    s32 i = 0;
    Pool30Entry *p = arg0->unkC;
    if (i < arg0->unk0) {
        do {
            if (p->unk1F != 0 && p->unk2C == arg1) {
                p->unk1A = p->unk1B;
            }
            i++;
            p++;
        } while (i < arg0->unk0);
    }
    arg1->unk30 = -1;
}

s32 func_800E1874(Pool30 *arg0, Pool6C *arg1) {
    union {
        s16 h[4];
        s32 w;
    } a;
    s16 b[4];
    s32 i;
    s32 last;
    Pool6CEntry *e;
    EffTail *def;
    EffTail *d2;
    EffAnim *an;
    EffElemHdr *hdr;
    EffSub *sub;
    EffFileHdr *f;
    s16 *tbl;
    s32 frame;
    s32 t;
    s32 v;
    s32 y;
    s32 x;
    Pool6C *p = arg1;

    last = 0;
    e = p->unkC;
    v = 0;
    i = 0;
    p->unk4 = 0;
    for (; i <= p->unk8; i++, e++) {
        if (e->unk30 == 0) {
            continue;
        }
        if (e->unk30 == -1) {
            e->unk30 = 0;
            continue;
        }
        p->unk4++;
        def = e->unk24;
        frame = e->unk38;
        an = &def->unk4;
        last = i;
        y = an->unk2;
        t = (frame << 12) / y;
        e->unk32 = 0;
        if (frame >= y) {
            if (e->unk34.w == 0) {
                e->unk30 = -1;
            }
            continue;
        }
        switch (def->unk4.unk0) {
        case 0:
            v = btlfx_lerp(an->unk4, an->unk6, t);
            break;
        case 1:
            v = (math_sin(t >> 1) * an->unk4) >> 12;
            break;
        case 2:
            def = e->unk24;
            if (def->unk0 != -1) {
                tbl = def->unk50;
            } else {
                tbl = def->unk40;
            }
            v = tbl[(frame << 4) / y];
            break;
        case 3:
            v = 0x100;
            break;
        }
        x = v >> 8;
        y = e->unk3A;
        y += v & 0xFF;
        if (y >= 0x100) {
            y -= 0x100;
            x++;
        }
        e->unk3A = y;
        e->unk32 = x;
        if (frame == 0) {
            ((s32 *)&e->unk5C)[0] = ((s32 *)&an->unk28)[0];
            ((s32 *)&e->unk5C)[1] = ((s32 *)&an->unk28)[1];
        } else {
            e->unk5C.vx += an->unk30.vx;
            e->unk5C.vy += an->unk30.vy;
            e->unk5C.vz += an->unk30.vz;
        }
        hdr = (EffElemHdr *)e->unk18 - 1;
        if (hdr->unkE == 1) {
            hdr->unk10.w = 0;
            x = btlfx_lerp(0, hdr->unk0 * 4, t);
            y = (x & 3) << 5;
            v = x >> 2;
            if (y != 0) {
                x = v + 1;
                if (x >= hdr->unk0) {
                    x = v;
                }
                hdr->unk10.b[v] = -0x80 - y;
                hdr->unk10.b[x] += y;
            } else {
                hdr->unk10.b[v] = 0x80;
            }
        }
        a.h[0] = hdr->unk6;
        a.h[1] = hdr->unkA;
        a.h[2] = an->unk12;
        b[0] = hdr->unk8;
        b[1] = hdr->unkC;
        b[2] = an->unk1A;
        btlfx_lerpVec(a.h, b, t, a.h);
        gte_lddp(0x1000 - t);
        gte_ldsv(an->unkC);
        gte_gpf12();
        e->unk1C = a.h[0];
        e->unk20 = a.h[1];
        e->unk4A = a.h[2];
        gte_lddp(t);
        gte_ldsv(an->unk14);
        gte_gpl12();
        gte_stsv(e->unk44);
        if (an->unk1C != 0) {
            btlfx_lerpVec(an->unk20, an->unk24, t, a.h);
            e->unk4C.w = a.w;
        }
        if (e->unk4 != NULL) {
            sub = e->unk4 - 1;
            btlfx_lerpVec(sub->unk14, sub->unk1C, t, e->unk50);
            e->unk56 = btlfx_lerp(sub->unk1A, sub->unk22, t);
            if (sub->unk24 != 0) {
                btlfx_lerpVec(sub->unk28, sub->unk2C, t, a.h);
                e->unk58.w = a.w;
            }
        }
        d2 = e->unk24;
        switch (d2->unk0) {
        case 0:
            a.h[0] = d2->unk3C;
            a.h[1] = d2->unk40[0];
            b[0] = d2->unk3E;
            b[1] = d2->unk40[1];
            btlfx_lerpVec(a.h, b, t, a.h);
            e->unk3C = a.h[0];
            e->unk3E = a.h[1];
            break;
        case 1:
        case 2:
        case 3:
            a.h[0] = d2->unk3C;
            a.h[1] = d2->unk3E;
            b[0] = d2->unk40[0];
            b[1] = d2->unk40[1];
            btlfx_lerpVec(a.h, b, t, a.h);
            e->unk3C = a.h[0];
            e->unk3E = a.h[1];
            a.h[0] = d2->unk40[2];
            a.h[1] = d2->unk40[3];
            b[0] = d2->unk40[4];
            b[1] = d2->unk40[5];
            btlfx_lerpVec(a.h, b, t, a.h);
            e->unk40 = a.h[0];
            e->unk42 = a.h[1];
            break;
        case 4:
            a.h[0] = d2->unk3C;
            a.h[1] = d2->unk40[0];
            a.h[2] = d2->unk40[2];
            b[0] = d2->unk3E;
            b[1] = d2->unk40[1];
            b[2] = d2->unk40[3];
            btlfx_lerpVec(a.h, b, t, a.h);
            e->unk3C = a.h[0];
            e->unk3E = a.h[1];
            e->unk40 = a.h[2];
            break;
        }
        switch (e->unk0->unk0) {
        case 0:
        case 5:
            e->unk8 = btlfx_lerp(e->unk0->unk8, e->unk0->unkA, t);
            break;
        case 1:
            f = e->unk0;
            e->unk8 = btlfx_lerp(f->unk18, f->unk1A, t);
            e->unkA = btlfx_lerp(f->unk20, f->unk22, t);
            break;
        case 2:
        case 4:
            e->unk8 = btlfx_lerp(e->unk0->unk8, e->unk0->unkA, t);
            break;
        case 3:
            e->unk8 = btlfx_lerp(e->unk0->unk18, e->unk0->unk1A, t);
            break;
        }
        frame++;
        e->unk38 = frame;
    }
    p->unk8 = last;
    return p->unk4;
}

Pool30Entry *func_800E1F90(Pool30 *arg0) {
    s32 i;
    Pool30Entry *p = arg0->unkC;
    for (i = 0; i < arg0->unk0; i++, p++) {
        if (p->unk1F == 0) {
            if (arg0->unk8 < i) {
                arg0->unk8 = i;
            }
            break;
        }
    }
    if (i >= arg0->unk0) {
        p = 0;
    }
    return p;
}

void func_800E2000(Pool30 *pool, Pool6C *list) {
    SVECTOR off;
    SVECTOR rot;
    SVECTOR tmp;
    SVECTOR ang;
    CVECTOR col;
    s32 k;
    s32 r;
    s32 d58;
    Pool6C *pl;
    s32 i;
    s32 j;
    s32 dur;
    s32 t;
    Pool6CEntry *e;
    Pool30Entry *p;
    EffElemHdr *hdr;
    EffElem *el;
    EffAnim *anim;
    SVECTOR *base;
    s32 frame;
    s32 acc;
    s32 n;

    el = NULL;
    pl = list;
    e = pl->unkC;
    base = NULL;
    for (i = 0; i <= pl->unk8; i++, e++) {
        if (e->unk30 == 0) {
            continue;
        }
        if (e->unk30 == -1) {
            continue;
        }
        for (j = 0; j < e->unk32; j++) {
            if (e->unk34.s.x >= e->unk24->unk4.unk8) {
                break;
            }
            p = func_800E1F90(pool);
            if (p == NULL) {
                break;
            }
            frame = e->unk38;
            anim = &e->unk24->unk4;
            e->unk34.s.x++;
            dur = anim->unk2;
            t = (frame << 12) / dur;
            hdr = (EffElemHdr *)e->unk18 - 1;
            acc = 0;
            r = btlfx_rand() & 0x7F;
            for (k = 0; k < hdr->unk0; k++) {
                acc += hdr->unk10.b[k];
                if (r < acc) {
                    break;
                }
            }
            if (k >= hdr->unk0) {
                k = 0xFF;
            }
            p->unk1C = k;
            p->unk1D = 0;
            if (k == 0xFF) {
                acc = btlfx_lerp(((EffEmit *)e->unk0)->unk2, ((EffEmit *)e->unk0)->unk4, t);
                acc += btlfx_randCentered(((EffEmit *)e->unk0)->unk6);
                if (acc <= 0) {
                    acc = 1;
                }
            } else {
                el = &e->unk18[k];
                if (el->unk4 == 1) {
                    acc = el->unk8;
                } else {
                    acc = btlfx_lerp(((EffEmit *)e->unk0)->unk2, ((EffEmit *)e->unk0)->unk4, t);
                    acc += btlfx_randCentered(((EffEmit *)e->unk0)->unk6);
                    if (acc <= 0) {
                        acc = 1;
                    }
                }
                off.vx = (el->unk14[0] * 8) & 0xFFF;
                off.vy = (el->unk14[1] * 8) & 0xFFF;
                off.vz = (el->unk14[2] * 8) & 0xFFF;
                gte_lddp(frame);
                gte_ldsv(&off);
                gte_gpf0();
                ang.vx = (el->unk11[0] * 16 + btlfx_randCentered(el->unk17[0] * 16)) & 0xFFF;
                ang.vy = (el->unk11[1] * 16 + btlfx_randCentered(el->unk17[1] * 16)) & 0xFFF;
                ang.vz = (el->unk11[2] * 16 + btlfx_randCentered(el->unk17[2] * 16)) & 0xFFF;
                gte_lddp(1);
                gte_ldsv(&ang);
                gte_gpl0();
                p->unk13 = btlfx_randCentered(el->unk10);
                gte_stsv(&p->unk14);
            }
            p->unk1F = e->unkE % 255 + 1;
            p->unk1B = acc;
            p->unk1A = 0;
            p->unk2C = e;
            p->unk1E = btlfx_randCentered(anim->unkA) + 0x40;
            if (((EffEmit *)e->unk0)->unk1 != 0) {
                p->unk12 = 0x7F;
            } else {
                p->unk12 = 0;
            }
            e->unkE++;
            if (e->unk14 != 0) {
                e->unkE %= e->unk12;
            }
            switch (e->unk24->unk0) {
            case 0:
                btlfx_randRotation(&rot);
                acc = btlfx_randRange(e->unk3C, e->unk3E);
                if (((EffTailSpawn *)e->unk24)->unk44.cd == 1) {
                    if (rot.vx > 0x800) {
                        rot.vx -= 0x800;
                    }
                } else if (((EffTailSpawn *)e->unk24)->unk44.cd == 2) {
                    if (rot.vx < 0x800) {
                        rot.vx += 0x800;
                    }
                }
                func_800CFCA8(&rot, acc, (s32)&off);
                LoadAverageCol((u8 *)&((EffTailSpawn *)e->unk24)->unk44, (u8 *)&((EffTailSpawn *)e->unk24)->unk48.c, 0x1000 - t, t, (u8 *)&col);
                off.vx = (off.vx * col.r) >> 7;
                off.vy = (off.vy * col.g) >> 7;
                off.vz = (off.vz * col.b) >> 7;
                base = e->unk28;
                break;
            case 1:
                rot.vx = 0;
                rot.vy = btlfx_rand();
                rot.vz = 0;
                func_800CFCA8(&rot, btlfx_randRange(e->unk3C, e->unk3E), (s32)&off);
                off.vy = btlfx_randRange(e->unk40, e->unk42);
                if (((EffTailSpawn *)e->unk24)->unk4C == 1) {
                    rot.vx = 0x400;
                    rot.vy = 0;
                } else if (((EffTailSpawn *)e->unk24)->unk4C == 2) {
                    rot.vx = -0x400;
                    rot.vy = 0;
                }
                base = e->unk28;
                break;
            case 2:
                ((XY32 *)&ang)[0].w = ((XY32 *)&e->unk5C)[0].w;
                ((XY32 *)&ang)[1].w = ((XY32 *)&e->unk5C)[1].w;
                d58 = btlfx_rand();
                acc = btlfx_randRange(e->unk40, e->unk42);
                r = btlfx_randRange(e->unk3C, e->unk3E);
                *(s32 *)&off.vx = 0;
                *(s32 *)&off.vz = 0;
                func_800D044C(&off, (s32)&off, acc, (XY32 *)&ang, r, d58);
                if (((EffTailSpawn *)e->unk24)->unk4C == 1) {
                    ((XY32 *)&rot)[0].w = ((XY32 *)&ang)[0].w;
                } else if (((EffTailSpawn *)e->unk24)->unk4C == 2) {
                    ((XY32 *)&rot)[0].w = ((XY32 *)&ang)[0].w;
                    rot.vx += 0x800;
                } else {
                    func_800D0580(&rot.vx, (XY32 *)&ang, d58);
                }
                base = e->unk28;
                break;
            case 3:
                btlfx_aimAngles((s16 *)e->unk28, (s16 *)e->unk2C, &ang.vx);
                d58 = btlfx_rand();
                acc = btlfx_randRange(e->unk40, e->unk42);
                r = btlfx_randRange(e->unk3C, e->unk3E);
                btlfx_lerpVec((s16 *)e->unk28, (s16 *)e->unk2C, acc, &off.vx);
                func_800D044C(&off, (s32)&off, 0, (XY32 *)&ang, r, d58);
                if (((EffTailSpawn *)e->unk24)->unk4C == 1) {
                    ((XY32 *)&rot)[0].w = ((XY32 *)&ang)[0].w;
                } else if (((EffTailSpawn *)e->unk24)->unk4C == 2) {
                    ((XY32 *)&rot)[0].w = ((XY32 *)&ang)[0].w;
                    rot.vx += 0x800;
                } else {
                    func_800D0580(&rot.vx, (XY32 *)&ang, d58);
                }
                base = e->unk28;
                off.vx -= base->vx;
                off.vy -= base->vy;
                off.vz -= base->vz;
                break;
            case 4: {
                s32 y4;
                ((XY32 *)&ang)[0].w = ((XY32 *)&e->unk5C)[0].w;
                ((XY32 *)&ang)[1].w = ((XY32 *)&e->unk5C)[1].w;
                n = 1;
                if (e->unk40 != 0) {
                    n = e->unk40;
                }
                acc = btlfx_rand() % n;
                r = btlfx_rand() & 0xFFF;
                d58 = (e->unk3C * r) >> 12;
                y4 = (e->unk3E * r) >> 12;
                r = btlfx_lerpRatio(d58, y4, acc, n);
                n = btlfx_rand();
                *(s32 *)&off.vx = 0;
                *(s32 *)&off.vz = 0;
                func_800D044C(&off, (s32)&off, acc, (XY32 *)&ang, r, n);
                if (((EffTailSpawn *)e->unk24)->unk48.h == 1) {
                    ((XY32 *)&rot)[0].w = ((XY32 *)&ang)[0].w;
                } else if (((EffTailSpawn *)e->unk24)->unk48.h == 2) {
                    ((XY32 *)&rot)[0].w = ((XY32 *)&ang)[0].w;
                    rot.vx += 0x800;
                } else {
                    *(s32 *)&tmp.vx = 0;
                    *(s32 *)&tmp.vz = 0;
                    func_800D044C(&tmp, (s32)&tmp, 0, (XY32 *)&ang, d58, n);
                    *(s32 *)&rot.vx = 0;
                    *(s32 *)&rot.vz = 0;
                    func_800D044C(&rot, (s32)&rot, e->unk40, (XY32 *)&ang, y4, n);
                    ((XY32 *)&ang)[0].w = ((XY32 *)&rot)[0].w;
                    ((XY32 *)&ang)[1].w = ((XY32 *)&rot)[1].w;
                    btlfx_aimAngles(&tmp.vx, &ang.vx, &rot.vx);
                }
                base = e->unk28;
                break;
            }
            case -1:
                off.pad = 0;
                if (((EffTailSpawn *)e->unk24)->unk3C(&off, &rot, frame, dur) != 0) {
                    anim->unk2 = 0;
                }
                if (off.pad != 0) {
                    base = &tmp;
                    *(s32 *)&tmp.vx = 0;
                    *(s32 *)&tmp.vz = 0;
                } else {
                    base = e->unk28;
                }
                break;
            }
            p->unk0.vx = (off.vx + base->vx) << 12;
            p->unk0.vy = (off.vy + base->vy) << 12;
            p->unk0.vz = (off.vz + base->vz) << 12;
            switch (anim->unk28.pad) {
            case 1:
                rot.vx += btlfx_randCentered(anim->unk30.pad);
                rot.vy += btlfx_randCentered(anim->unk30.pad);
                rot.vx = (rot.vx + 0x800) & 0xFFF;
                break;
            case 2:
                btlfx_randRotation(&rot);
                break;
            case 3:
                ((XY32 *)&rot)[0].w = ((XY32 *)&e->unk5C)[0].w;
                ((XY32 *)&rot)[1].w = ((XY32 *)&e->unk5C)[1].w;
                rot.vx += btlfx_randCentered(anim->unk30.pad);
                rot.vy += btlfx_randCentered(anim->unk30.pad);
                break;
            case 0:
                rot.vx += btlfx_randCentered(anim->unk30.pad);
                rot.vy += btlfx_randCentered(anim->unk30.pad);
                break;
            }
            if (el != NULL && el->unk7 != 0) {
                p->unk14.s.x += rot.vx;
                p->unk14.s.y += rot.vy;
            }
            switch (((EffEmit *)e->unk0)->unk0) {
            case 1: {
                EffEmit *mot = (EffEmit *)e->unk0;
                acc = e->unkA;
                func_800CFCA8(&rot, acc + btlfx_randCentered(mot->unk24), (s32)&p->unkC);
                acc = mot->unk10 + btlfx_randCentered(mot->unk12);
                if (acc < 0) {
                    acc = 0;
                }
                p->unk28.s.y = acc;
                p->unk24.w = *(s32 *)&rot.vx;
                p->unk20 = mot->unk8 + btlfx_randCentered(mot->unkC);
                p->unk22 = mot->unkA + btlfx_randCentered(mot->unkE);
                p->unk28.s.x = e->unk8 + btlfx_randCentered(mot->unk16);
                break;
            }
            case 2: {
                EffEmit *mot = (EffEmit *)e->unk0;
                p->unk28.w = *(s32 *)&rot.vx;
                btlfx_lerpVec(&mot->unk10, &mot->unk14, t, &tmp.vx);
                p->unkE = tmp.vy + btlfx_randCentered(mot->unkE);
                p->unk24.s.y = tmp.vx + btlfx_randCentered(mot->unk18);
                p->unk22 = e->unk8 + btlfx_randCentered(mot->unkC);
                *(s32 *)&tmp.vx = 0;
                *(s32 *)&tmp.vz = 0;
                func_800D0674(&off, &tmp, &rot, (s32)&k, (s32)&r, &d58);
                p->unkC = d58;
                p->unk20 = k;
                p->unk24.s.x = r;
                p->unk0.vx = base->vx << 12;
                p->unk0.vy = base->vy << 12;
                p->unk0.vz = base->vz << 12;
                p->unk10 = 0;
                break;
            }
            case 3: {
                EffEmit *mot = (EffEmit *)e->unk0;
                *(s32 *)&p->unkC = *(s32 *)&rot.vx;
                p->unk20 = mot->unk8 + btlfx_randCentered(mot->unkC);
                p->unk22 = mot->unkA + btlfx_randCentered(mot->unkE);
                p->unk10 = e->unk8 + btlfx_randCentered(mot->unk16);
                p->unk24.s.y = rot.vx + btlfx_randCentered(mot->unk10);
                p->unk28.s.x = rot.vy + btlfx_randCentered(mot->unk12);
                p->unk24.s.x = 0;
                break;
            }
            case 4: {
                EffEmit *mot = (EffEmit *)e->unk0;
                p->unk28.w = *(s32 *)&rot.vx;
                btlfx_lerpVec(&mot->unk10, &mot->unk14, t, &tmp.vx);
                p->unkE = tmp.vy + btlfx_randCentered(mot->unk1A);
                p->unk20 = tmp.vx + btlfx_randCentered(mot->unk18);
                p->unk22 = e->unk8 + btlfx_randCentered(mot->unkC);
                p->unkC = btlfx_rand();
                p->unk24.w = 0;
                p->unk10 = 0;
                break;
            }
            case 0:
            case 5: {
                EffEmit *mot = (EffEmit *)e->unk0;
                acc = e->unk8;
                func_800CFCA8(&rot, acc + btlfx_randCentered(mot->unkC), (s32)&p->unkC);
                acc = mot->unkE + btlfx_randCentered(mot->unk10);
                if (acc < 0) {
                    acc = 0;
                }
                p->unk20 = acc;
                break;
            }
            }
            if (el != NULL && ((u32)(el->unk0 - 4) < 2 || el->unk0 == 8 || el->unk1 == 4 || el->unk1 == 5 || el->unk1 == 8)) {
                tmp.vx = p->unk0.vx >> 12;
                tmp.vy = p->unk0.vy >> 12;
                tmp.vz = p->unk0.vz >> 12;
                func_800D1494((u8 *)el->unk1C, (s32)&tmp, NULL, -1, 0, 0, 0, (u8 *)func_800E0134(p, e), 0);
            }
        }
    }
}

void func_800E2F84(Pool30 *pool, Pool30Entry *e, SVECTOR *pos, SVECTOR *rot) {
    SVECTOR off;
    SVECTOR ang;
    SVECTOR dir;
    s32 d40;
    s32 cnt;
    s32 k;
    Pool6CEntry *owner;
    EffSub *sub;
    EffElem *el;
    Pool30Entry *p;
    s32 n;
    s32 frame;
    s32 t;
    s32 r;
    s32 i;

    owner = e->unk2C;
    el = NULL;
    if (owner->unk30 == 0) {
        return;
    }
    if (owner->unk30 != -1) {
        sub = owner->unk4 - 1;
        frame = e->unk1A;
        t = (frame << 12) / e->unk1B;
        if ((!(e->unk12 & 0x80) || owner->unk38 < owner->unk24->unk4.unk2) && frame >= sub->unk11 - 1) {
            r = btlfx_lerp(sub->unk0, sub->unk2, t);
            cnt = r >> 8;
            k = (e->unk12 & 0x7F) + ((r >> 1) & 0x7F);
            if (k >= 0x80) {
                k -= 0x80;
                if (k == 0) {
                    k = 1;
                }
                cnt++;
            }
            e->unk12 = (u8)k | (e->unk12 & 0x80);
            n = cnt;
            for (i = 0; i < n; i++) {
                if (owner->unk34.s.y >= sub->unk10) {
                    return;
                }
                p = func_800E1F90(pool);
                if (p == NULL) {
                    return;
                }
                owner->unk34.s.y++;
                cnt = btlfx_rand() & 0x7F;
                k = 0;
                r = 0;
                for (; k < sub->unkD; k++) {
                    r += sub->unk8[k];
                    if (cnt < r) {
                        break;
                    }
                }
                if (k >= sub->unkD) {
                    cnt = 0xFF;
                } else {
                    cnt = sub->unk4[k];
                }
                p->unk1C = cnt;
                p->unk1D = 0;
                if (cnt == 0xFF) {
                    r = btlfx_lerp(((EffEmit *)owner->unk4)->unk2, ((EffEmit *)owner->unk4)->unk4, t);
                    r += btlfx_randCentered(((EffEmit *)owner->unk4)->unk6);
                    if (r <= 0) {
                        r = 1;
                    }
                } else {
                    el = &owner->unk18[cnt];
                    if (el->unk4 == 1) {
                        r = el->unk8;
                    } else {
                        r = btlfx_lerp(((EffEmit *)owner->unk4)->unk2, ((EffEmit *)owner->unk4)->unk4, t);
                    r += btlfx_randCentered(((EffEmit *)owner->unk4)->unk6);
                        if (r <= 0) {
                            r = 1;
                        }
                    }
                    off.vx = (el->unk14[0] * 8) & 0xFFF;
                    off.vy = (el->unk14[1] * 8) & 0xFFF;
                    off.vz = (el->unk14[2] * 8) & 0xFFF;
                    gte_lddp(frame);
                    gte_ldsv(&off);
                    gte_gpf0();
                    ang.vx = (el->unk11[0] * 16 + btlfx_randCentered(el->unk17[0] * 16)) & 0xFFF;
                    ang.vy = (el->unk11[1] * 16 + btlfx_randCentered(el->unk17[1] * 16)) & 0xFFF;
                    ang.vz = (el->unk11[2] * 16 + btlfx_randCentered(el->unk17[2] * 16)) & 0xFFF;
                    gte_lddp(1);
                    gte_ldsv(&ang);
                    gte_gpl0();
                    p->unk13 = btlfx_randCentered(el->unk10);
                    gte_stsv(&p->unk14);
                }
                if (((EffEmit *)owner->unk4)->unk1 != 0) {
                    p->unk12 = 0xFF;
                } else {
                    p->unk12 = 0x80;
                }
                p->unk1F = owner->unk10 % 255 + 1;
                p->unk1B = r;
                p->unk1A = 0;
                p->unk2C = owner;
                p->unk1E = btlfx_randCentered(sub->unk12) + 0x40;
                owner->unk10++;
                if (owner->unk14 != 0) {
                    owner->unk10 %= owner->unk12;
                }
                gte_lddp(0x1000 - t);
                gte_ldsv(sub->unk30);
                gte_gpf12();
                btlfx_randRotation(&ang);
                gte_lddp(t);
                gte_ldsv(sub->unk34);
                gte_gpl12();
                gte_stsv(&dir);
                func_800CFCA8(&ang, btlfx_randRange(dir.vx, dir.vy), (s32)&off);
                p->unk0.vx = (off.vx + pos->vx) << 12;
                p->unk0.vy = (off.vy + pos->vy) << 12;
                p->unk0.vz = (off.vz + pos->vz) << 12;
                switch (sub->unkC) {
                case 2:
                    btlfx_randRotation(&ang);
                    break;
                case 3:
                    ((XY32 *)&ang)[0].w = ((XY32 *)&owner->unk5C)[0].w;
                    ((XY32 *)&ang)[1].w = ((XY32 *)&owner->unk5C)[1].w;
                    ang.vx += btlfx_randCentered(sub->unkE);
                    ang.vy += btlfx_randCentered(sub->unkE);
                    break;
                case 4:
                    ((XY32 *)&ang)[0].w = ((XY32 *)rot)[0].w;
                    ((XY32 *)&ang)[1].w = ((XY32 *)rot)[1].w;
                    ang.vx += btlfx_randCentered(sub->unkE);
                    ang.vy += btlfx_randCentered(sub->unkE);
                    break;
                case 0:
                    ang.vx += btlfx_randCentered(sub->unkE);
                    ang.vy += btlfx_randCentered(sub->unkE);
                    break;
                case 5:
                    ((XY32 *)&ang)[0].w = ((XY32 *)rot)[0].w;
                    ((XY32 *)&ang)[1].w = ((XY32 *)rot)[1].w;
                    ang.vx += btlfx_randCentered(sub->unkE);
                    ang.vy += btlfx_randCentered(sub->unkE);
                    ang.vx = (ang.vx + 0x800) & 0xFFF;
                    break;
                case 1:
                    ang.vx += btlfx_randCentered(sub->unkE);
                    ang.vy += btlfx_randCentered(sub->unkE);
                    ang.vx = (ang.vx + 0x800) & 0xFFF;
                    break;
                }
                if (el != NULL && el->unk7 != 0) {
                    p->unk14.s.x += ang.vx;
                    p->unk14.s.y += ang.vy;
                }
                switch (((EffEmit *)owner->unk4)->unk0) {
                case 1:
                    {
                    EffEmit *mot = (EffEmit *)owner->unk4;
                    r = btlfx_lerp(mot->unk20, mot->unk22, t);
                    func_800CFCA8(&ang, r + btlfx_randCentered(mot->unk24), (s32)&p->unkC);
                    r = mot->unk10 + btlfx_randCentered(mot->unk12);
                    if (r < 0) {
                        r = 0;
                    }
                    p->unk28.s.y = r;
                    p->unk24.w = *(s32 *)&ang.vx;
                    p->unk20 = mot->unk8 + btlfx_randCentered(mot->unkC);
                    p->unk22 = mot->unkA + btlfx_randCentered(mot->unkE);
                    p->unk28.s.x = btlfx_lerp(mot->unk18, mot->unk1A, t) + btlfx_randCentered(mot->unk16);
                    break;
                    }
                case 2:
                    {
                    EffEmit *mot = (EffEmit *)owner->unk4;
                    p->unk28.w = *(s32 *)&ang.vx;
                    btlfx_lerpVec(&mot->unk10, &mot->unk14, t, (s16 *)&dir);
                    p->unkE = dir.vy + btlfx_randCentered(mot->unkE);
                    p->unk24.s.y = dir.vx + btlfx_randCentered(mot->unk18);
                    p->unk22 = btlfx_lerp(mot->unk8, mot->unkA, t) + btlfx_randCentered(mot->unkC);
                    *(s32 *)&dir.vx = 0;
                    *(s32 *)&dir.vz = 0;
                    func_800D0674(&off, &dir, &ang, (s32)&d40, (s32)&cnt, &k);
                    p->unkC = k;
                    p->unk20 = d40;
                    p->unk24.s.x = cnt;
                    p->unk0.vx = e->unk0.vx;
                p->unk0.vy = e->unk0.vy;
                p->unk0.vz = e->unk0.vz;
                    p->unk10 = 0;
                    break;
                    }
                case 3:
                    {
                    EffEmit *mot = (EffEmit *)owner->unk4;
                    *(s32 *)&p->unkC = *(s32 *)&ang.vx;
                    p->unk20 = mot->unk8 + btlfx_randCentered(mot->unkC);
                    p->unk22 = mot->unkA + btlfx_randCentered(mot->unkE);
                    p->unk10 = btlfx_lerp(mot->unk18, mot->unk1A, t) + btlfx_randCentered(mot->unk16);
                    p->unk24.s.y = ang.vx + btlfx_randCentered(mot->unk10);
                    p->unk28.s.x = ang.vy + btlfx_randCentered(mot->unk12);
                    p->unk24.s.x = 0;
                    break;
                    }
                case 4:
                    {
                    EffEmit *mot = (EffEmit *)owner->unk4;
                    p->unk28.w = *(s32 *)&ang.vx;
                    btlfx_lerpVec(&mot->unk10, &mot->unk14, t, (s16 *)&dir);
                    p->unkE = dir.vy + btlfx_randCentered(mot->unk1A);
                    p->unk20 = dir.vx + btlfx_randCentered(mot->unk18);
                    p->unk22 = btlfx_lerp(mot->unk8, mot->unkA, t) + btlfx_randCentered(mot->unkC);
                    p->unkC = btlfx_rand();
                    p->unk24.w = 0;
                    p->unk10 = 0;
                    break;
                    }
                case 0:
                case 5:
                    {
                    EffEmit *mot = (EffEmit *)owner->unk4;
                    r = btlfx_lerp(mot->unk8, mot->unkA, t);
                    func_800CFCA8(&ang, r + btlfx_randCentered(mot->unkC), (s32)&p->unkC);
                    r = mot->unkE + btlfx_randCentered(mot->unk10);
                    if (r < 0) {
                        r = 0;
                    }
                    p->unk20 = r;
                    break;
                    }
                }
                if (el != NULL && ((u32)(el->unk0 - 4) < 2 || el->unk0 == 8 || el->unk1 == 4 || el->unk1 == 5 || el->unk1 == 8)) {
                    dir.vx = p->unk0.vx >> 12;
                    dir.vy = p->unk0.vy >> 12;
                    dir.vz = p->unk0.vz >> 12;
                    func_800D1494((u8 *)el->unk1C, (s32)&dir, NULL, -1, 0, 0, 0, (u8 *)func_800E0134(p, owner), 0);
                }
            }
        }
    }
}

void btlfx_heapInit(Heap *arg0, s32 arg1) {
    s32 size = btlfx_align4(arg1) - 0xC;
    arg0->unk0 = arg0->unkC;
    arg0->unk4 = size;
    arg0->unk8 = size;
}

s32 btlfx_heapAlloc(Heap *h, s32 size) {
    s32 sz;
    u8 *p;
    s32 *q;

    sz = btlfx_align4(size);
    if (h->unk4 < sz) {
        q = (s32 *)h->unkC;
        h->unk0 = (u8 *)q;
        h->unk4 = h->unk8;
    }
    p = h->unk0;
    h->unk4 -= sz;
    h->unk0 = p + sz;
    q = (s32 *)p;
    for (size = 0; size < sz >> 2; size++) {
        *q++ = 0;
    }
    return (s32)p;
}

s32 btlfx_heapWrapIfFull(Heap *arg0, s32 arg1) {
    s32 size = btlfx_align4(arg1);
    s32 ret = 1;
    if (arg0->unk4 < size) {
        arg0->unk0 = arg0->unkC;
        arg0->unk4 = arg0->unk8;
    } else {
        ret = 0;
    }
    return ret;
}

void btlfx_heapFree(void *arg0) {
}

void func_800E3C58(void) {
    func_800E00DC(0x3000, D_801014B8, &D_801044B8);
    func_800E14D8(0x920, &D_801044C8);
}

void func_800E3C98(void) {
    func_800E2000(&D_801044B8, &D_801044C8);
    func_800E0168(&D_801044B8);
    func_800E1874(&D_801044B8, &D_801044C8);
}

Pool6CEntry *func_800E3CEC(s32 arg0, s32 arg1) {
    return func_800E1534(&D_801044B8, &D_801044C8, (u8 *)arg0, arg1);
}

void func_800E3D20(s32 arg0, s32 arg1) {
    if (arg0 != 0) {
        func_800E1708(&D_801044B8, arg0, arg1);
    }
}

void func_800E3D54(Pool6CEntry *arg0) {
    if (arg0 != NULL) {
        func_800E1810(&D_801044B8, arg0);
    }
}

void func_800E3D80(void) {
    func_800E4098();
    btlfx_resetAll();
}

void btlfx_resetAll(void) {
    btlfx_clearTargets();
    btlfx_reset();
    btlfx_initTasks();
    func_800DA4E8();
    func_800E3C58();
    func_800E954C();
    btlfx_clearEffModels();
    btlfx_clearSummonModels();
    func_80054F80();
}

void func_800E3E08(void) {
    RECT r;
    DR_MOVE *p;
    s32 i;
    r.x = g_btlfxEnv->unk18->disp.x;
    r.y = g_btlfxEnv->unk18->disp.y;
    r.w = 0x20;
    r.h = 0xDA;
    i = 0;
    p = (DR_MOVE *)g_btlfxEnv->unk24;
    g_btlfxEnv->unk24 = (u8 *)(p + 10);
    do {
        SetDrawMove(p, &r, g_btlfxEnv->unk1C->clip.x + i, g_btlfxEnv->unk1C->clip.y);
        AddPrim(g_btlfxEnv->unk20, p++);
        i += 0x20;
        r.x += 0x20;
    } while (i < 0x140);
    if (D_800F78AC == 0) {
        D_800F78AC = 1;
        func_8005562C(2);
    }
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E3EEC);

void func_800E4098(void) {
    u8 buf[0x40];
    RECT r;
    D_80104DF8 = D_800F78B4;
    D_80104DE8 = 1;
    g_btlfxEnv->unk24 = buf;
    btlfx_reloadCommonData(0);
    while (btlfx_reloadCommonData(1) == 0) {
    }
    r.x = 0;
    r.y = 0xF0;
    r.w = 0x100;
    r.h = 2;
    LoadImage(&r, (u32 *)0x801F7700);
    DrawSync(0);
    g_btlfxEnv->unk24 = 0;
}

s32 btlfx_reloadCommonData(s32 arg0) {
    RECT r;
    s32 ret = 0;
    s32 i;
    u32 *p = (u32 *)0x801E7700;
    if (arg0 == 0) {
        FileEntry *e = file_find(1, 0x2BF);
        cd_readAsync(g_gameState->unk20 + e->unk4, (e->unkC - e->unk4) << 11, (void *)0x801E7700, 0);
    } else if (cd_getState() == 0) {
        func_800D0940();
        r.w = 0x40;
        r.h = 0x80;
        for (i = 0; i < 4; i++) {
            r.x = (i >> 1) * 64 + 0x300;
            r.y = (i & 1) * 128 + 0x100;
            LoadImage(&r, p);
            p += 0x1000;
        }
        ret = 1;
    }
    return ret;
}

s32 func_800E4218(s32 arg0, s32 arg1, s32 arg2) {
    s32 a[2];
    s32 b[2];
    s32 c[2];
    s32 d[2];
    s32 n;
    s32 t;
    btlfx_getUnitHomePos(arg0, (s16 *)a);
    c[0] = a[0];
    c[1] = a[1];
    if (arg1 == 0xFF) {
        arg1 = 0;
    }
    if (btlfx_isEnemyUnit(arg1) != 0) {
        btlfx_getUnitRot(arg0, (s16 *)d);
        func_800CFD78((s16 *)c, (s16 *)d, 400, (s16 *)b);
        if (arg2 == 0) {
            btlfx_playUnitMotion(arg0, 0x1D);
        }
        n = btlfx_getMotionLength(arg0);
        if (arg2 < n) {
            btlfx_applyFloatHeight(arg0, (s16 *)a);
            btlfx_applyFloatHeight(arg0, (s16 *)b);
        }
        t = ((arg2 + 1) << 12) / n;
        if (t > 0x1000) {
            t = 0x1000;
        }
        btlfx_lerpVec((s16 *)a, (s16 *)b, t, (s16 *)c);
        btlfx_setUnitPos(arg0, (s16 *)c);
        return arg2 >= n;
    }
    return 1;
}

void func_800E436C(s32 arg0, s16 *arg1, s32 arg2, s32 arg3, s32 arg4) {
    btlfx_getMotionLength(arg0);
    func_800E78B4(arg0, arg1, (s16 *)arg2, 2, 0, arg3, 3);
}

s32 btlfx_moveUnitHome(s32 arg0, s32 arg1, s16 *arg2, s32 arg3) {
    s16 a[4];
    s16 b[4];
    s32 n;
    s32 t;
    btlfx_getUnitHomePos(arg0, a);
    if (arg1 == 0) {
        btlfx_getUnitPos(arg0, arg2);
        if (arg2[0] == a[0] && arg2[2] == a[2]) {
            return -1;
        }
        if (arg3 == 0) {
            btlfx_playUnitMotion(arg0, 0x1E);
        }
        func_800E78B4(arg0, a, 0, 0, 0, 0, 0);
    }
    if (arg3 == 0) {
        arg3 = btlfx_getMotionLength(arg0);
    } else {
        arg3 = 0x18;
    }
    n = arg3;
    if (arg1 < n) {
        btlfx_applyFloatHeight(arg0, arg2);
        btlfx_applyFloatHeight(arg0, a);
    }
    t = ((arg1 + 1) << 12) / n;
    if (t > 0x1000) {
        t = 0x1000;
    }
    btlfx_lerpVec(arg2, a, t, b);
    btlfx_setUnitPos(arg0, b);
    return arg1 >= n;
}

u16 *func_800E4530(s32 arg0) {
    s32 *p = (s32 *)0x801E9700;
    return (u16 *)(p[arg0] + (s32)p);
}

s32 func_800E454C(s32 arg0) {
    return (s32)D_80104DF8 + D_80104DF8[arg0];
}

s32 func_800E4568(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 *arg4) {
    switch (arg0) {
    case 1:
        D_80104DF0.unk0 = arg4[0];
        D_80104DF0.unk1 = arg4[4];
        break;
    }
    return 1;
}

s32 func_800E45A0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 *arg4) {
    if (arg0 == 1) {
        D_80104DF0.unk2 = arg4[0];
        D_80104DF0.unk3 = arg4[4];
    }
    return 1;
}

s32 btlfx_opSetFrameRate(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 *arg4) {
    switch (arg0) {
    case 1:
        btl_setVsyncMode(60 / *arg4);
        break;
    }
    return 1;
}

s32 func_800E4634(s32 arg0, Unk800E4634 *c, Unk800E4634Arg2 *arg2, s32 *arg3, s32 *arg4) {
    s32 t;
    s32 i;

    if (arg0 == 0) {
        *arg4 = 0x40;
        return 0;
    }
    if (arg0 == 1) {
        c->unk0 = 0;
        c->unk4 = 0;
        if (arg2->unkC->unk27 == 0) {
            c->unk4 = 0xFFFF;
            if (btlfx_isEnemyUnit(0x10) != 0) {
                c->unk0 = 0;
            } else {
                c->unk0 = 12;
            }
        }
        func_800E94C4();
        D_800FEC52 = 0;
        {
            s32 j;
            for (j = 3; j >= 0; j--) {
                c->unk30[j] = NULL;
            }
        }
        return 0;
    }
    t = *arg3;
    if (c->unk4 == 0xFFFF) {
        if (--c->unk0 < 0) {
            D_800FEC52 = 1;
            return 1;
        }
    } else if (c->unk4 != 0) {
        c->unk4++;
        if ((c->unk4 >= 5 && c->unk0 >= 0) || (c->unk4 >= 15 && c->unk0 < 0)) {
            c->unk4 = 0;
            if (c->unk0 < 0) {
                g_btlfxState.unk10.b[2] = 1;
                g_btlfxState.unk25 = 1;
                return 1;
            }
        }
        for (i = 0; i < 4; i++) {
            if (c->unk30[i] != NULL) {
                c->unk30[i]->unk5C.vx = c->unk28[0];
                c->unk30[i]->unk5C.vy = c->unk28[1];
                c->unk30[i]->unk5C.vz = c->unk28[2];
                if (c->unk30[i]->unk30 == -1) {
                    c->unk30[i] = NULL;
                }
            }
        }
    } else if (func_800E774C(c->unk0, (s16 *)&c->unk8[c->unk0], c->unk28) == 0) {
        c->unk30[c->unk0] = func_800E3CEC(func_800E454C(3), (s32)&c->unk8[c->unk0]);
        if (c->unk30[c->unk0] != NULL) {
            c->unk30[c->unk0]->unk22 = 0x73;
            if (c->unk30[c->unk0] != NULL) {
                c->unk30[c->unk0]->unk18[0].unk1C = func_800E454C(7);
                if (c->unk30[c->unk0] != NULL) {
                    c->unk30[c->unk0]->unk18[1].unk1C = func_800E454C(6);
                }
            }
        }
        c->unk28[0] = 0x400;
        c->unk4 = 1;
        c->unk28[1] += 0x800;
        func_800E49D0(3, c->unk0 + 8);
        c->unk0++;
    } else {
        c->unk0 = -1;
        c->unk4 = 1;
    }
    *arg3 = t;
    return 0;
}

void btlfx_startCasterTask(s32 arg0) {
    s32 a = 0xFF;
    s32 b = arg0 * 16;
    if (g_btlfxEnv->unkC->unk24 == 1) {
        a = 0;
    }
    btlfx_spawnTask(7, -1, a, b);
}

void func_800E49D0(s32 arg0, s32 arg1) {
    s32 i;
    for (i = 0; i < 3; i++) {
        func_800E9A00(D_800F8CF4[arg0][i], arg1);
    }
}

Pool6CEntry *func_800E4A3C_def(s32 arg0, s32 arg1, Unk800E4A3CArgs arg2) {
    Pool6CEntry *e;
    Unk800E4A3CArgs *p;

    e = NULL;
    arg2.b = arg2.a;
    /* Read back through a pointer: CSE then keeps the load, which reload_cse turns into the original's
       `move v0,a2` for the 0x30 store (a direct `arg2.c = arg2.b` stores a2 twice). */
    p = &arg2;
    arg2.c = p->b;
    switch (arg1) {
    case 0:
    case 2:
        e = func_800E3CEC(func_800E454C(5), (s32)arg2.a);
        if (arg1 == 0) {
            if (e != NULL) {
                e->unk18[0].unk1C = func_800E454C(7);
                goto common;
            }
        } else {
            if (e != NULL) {
                e->unk18[0].unk1C = func_800E454C(8);
            common:
                if (e != NULL) {
                    e->unk18[1].unk1C = func_800E454C(6);
                    e->unk22 = 8;
                }
            }
        }
        if (func_800E865C(0x10, 0) == 0) {
            goto zero;
        }
        func_800E49D0(1, arg0);
        break;
    case 1:
    case 7:
        arg2.b[1] -= 0x40;
        e = func_800E3CEC(func_800E454C(1), (s32)arg2.a);
        if (e != NULL) {
            e->unk18[0].unk1C = func_800E454C(0x14);
            e->unk18[1].unk1C = func_800E454C(6);
            e->unk18[2].unk1C = func_800E454C(2);
            e->unk2C = *(s32 *)&arg2.b;
            e->unk22 = 0x20;
        }
        func_800E49D0(2, arg0);
        break;
    case 4:
        e = func_800E3CEC(func_800E454C(4), (s32)arg2.a);
        if (e != NULL) {
            e->unk18[0].unk1C = func_800E454C(7);
            e->unk18[1].unk1C = func_800E454C(6);
            e->unk22 = 0x80;
        }
    zero:
        func_800E49D0(0, arg0);
        break;
    }
    return e;
}

void func_800E4C04(s32 arg0, s32 arg1, s32 arg2) {
    if (btlfx_isEnemyUnit(arg0) == 0) {
        func_800E8B28(arg0, arg1, arg2);
    }
}

void func_800E4C54(void) {
    if (g_btlfxEnv->unkC->unk27 == 0) {
        D_800FEC65 = 1;
    }
}

s32 btlfx_opCast(s32 arg0, Unk800E4C88 *arg1, Unk800F78A4 *arg2, s32 *arg3, s32 *arg4) {
    s16 v[4];
    s32 t;
    s32 a;
    s32 b;
    s32 *q;
    s32 k;
    s32 c;

    if (arg0 == 0) {
        *arg4 = 0x28;
    } else if (arg0 == 1) {
        q = arg4;
        arg1->unk1 = 0;
        arg1->unk6 = 0;
        arg1->unk2 = (q[1] & 0xF) + 0x10;
        arg1->unk3 = q[0];
        arg1->unk24 = NULL;
        if (arg2->unk0->unk1C & 8) {
            func_800E8C84(0);
        }
        if (arg2->unkC->unk27 != 0 && arg1->unk3 != 0xFF) {
            if (arg2->unkC->unk27 == 1) {
                arg1->unk3 += 8;
            } else {
                arg1->unk3 = 0xFF;
            }
        }
        if (arg2->unkC->unk40 & 4) {
            arg1->unk2 = 0x11;
            arg1->unk3 = 0x10;
        }
        arg1->unk0 = (u8)q[1] >> 4;
        arg1->unk4 = ((u32)g_btlfxState.unk1C & 0x3000) >> 12;
        g_btlfxState.unk10.b[2] = 0;
        g_btlfxState.unk10.b[3] = 0;
        a = arg2->unkC->unk40 & 1;
        c = btlfx_isEnemyUnit(arg1->unk2);
        arg1->unk8 = 100;
        func_800E4C04(arg1->unk2, 1, 0);
        if ((a != 0 || arg1->unk0 == 0xF) && c == 0) {
            arg1->unk5 = 2;
            arg1->unk1 = 2;
        } else if (c != 0) {
            arg1->unk5 = 1;
            arg1->unk1 = 2;
            if (a == 0 && arg1->unk0 != 0xF) {
                btlfx_getUnitBodyPos(arg1->unk2, 0, arg1->unkC);
                arg1->unk24 = func_800E4A3C(arg1->unk2, 4, arg1->unkC, arg1->unk14);
            } else {
                arg1->unk24 = NULL;
            }
        } else if (arg2->unkC->unk40 & 2) {
            arg1->unk5 = 3;
            arg1->unk1 = 2;
        } else {
            arg1->unk5 = 0;
        }
    } else {
    t = *arg3;
    switch (arg1->unk1) {
    case 0:
        if (func_800E4218(arg1->unk2, arg1->unk3, t) != 0) {
            b = arg1->unk5;
            if (b != 3) {
                btlfx_playUnitMotion(arg1->unk2, 0x1A);
                arg1->unk1 = 1;
                goto set_t;
            }
            btlfx_playUnitMotion(arg1->unk2, 0x1F);
            arg1->unk1 = b;
            func_800E8D14();
            goto set_t;
        }
        break;
    case 1:
        if (arg1->unk3 != 0xFF && (s16)D_800FEC42 != 0xED) {
            btlfx_getUnitPos(arg1->unk3, v);
            func_800E436C(arg1->unk2, v, (s32)arg1->unk1C, t, 0);
        }
        if (t >= btlfx_getMotionLength(arg1->unk2) - 1) {
            btlfx_playUnitMotion(arg1->unk2, 0x1B);
            t = -1;
            arg1->unk1 = 2;
            btlfx_getUnitPos(arg1->unk2, arg1->unkC);
            arg1->unk24 = func_800E4A3C(arg1->unk2, arg1->unk0, arg1->unkC, arg1->unk14);
            func_800E4C04(arg1->unk2, 0, 0);
            func_800E8D14();
        }
        break;
    case 2:
        if (arg1->unk24 != NULL && arg1->unk24->unk30 == -1) {
            arg1->unk24 = NULL;
        }
        if (t == 0) {
            k = ((u32)D_800FEC5C & 0x300) >> 8;
            if (k != 3) {
                func_800E60A4(k, arg1->unk0 == 1 ? 0xC : 0x14);
            }
        }
        b = t - 9;
        if (arg1->unk0 == 1) {
            if (b >= 0) {
                if (b < 16) {
                    a = (b * 255) >> 4;
                } else {
                    a = 255;
                }
                if (b == 0) {
                    arg1->unk8 = btlfx_getMotionLength(arg1->unk2);
                    if (arg2->unkC->unk40 & 1) {
                        func_800E8D14();
                    }
                }
                switch (arg1->unk4) {
                case 0:
                    btlfx_drawScreenTint((u32 *)(arg2->unk20 + 1), 1, a, a, a);
                    if (b == 16) {
                        btlfx_fadeUnitGroup(2, 0);
                    }
                    break;
                case 1:
                    btlfx_drawScreenTint((u32 *)(arg2->unk20 + 1), 2, a, a, a);
                    if (b == 16) {
                        btlfx_fadeUnitGroup(2, 0);
                    }
                    break;
                case 2:
                    btlfx_fadeUnitGroup(2, 0x80 - (a >> 1));
                    break;
                case 3:
                    btlfx_fadeUnitGroup(0, 0x80 - (a >> 1));
                    break;
                }
            }
            if (t % arg1->unk8 == 0 && t >= 25 && arg1->unk6 >= 27 && g_btlfxState.unk10.b[2] != 0) {
                func_800E3D54(arg1->unk24);
                g_btlfxState.unk10.b[3] = 1;
                g_btlfxState.unk25 = 1;
                arg2->unk2C->unk52 = 0;
                func_800E4C04(arg1->unk2, 2, 0);
                return 1;
            }
        } else if (arg1->unk5 == 0) {
            if (t % btlfx_getMotionLength(arg1->unk2) == 0 && t > 0 && arg1->unk6 >= 27 && D_800FEC52 != 0) {
                btlfx_playUnitMotion(arg1->unk2, 0x1C);
                t = -1;
                arg1->unk1 = 3;
            }
        } else if (arg1->unk5 == 1) {
            if (D_800FEC52 != 0 && arg1->unk6 >= 27) {
                arg1->unk1 = 3;
            set_t:
                t = -1;
            }
        } else if (arg1->unk5 == 3) {
            if (D_800FEC52 != 0) {
                arg1->unk1 = 0;
                goto set_t;
            }
        } else if (g_btlfxState.unk10.b[2] != 0) {
            g_btlfxState.unk10.b[3] = 1;
            func_800E4C54();
            arg2->unk2C->unk52 = 0;
            func_800E4C04(arg1->unk2, 2, 0);
            func_800E8D14();
            return 1;
        }
        break;
    case 3:
        if (t == 0 && arg1->unk5 != 1) {
            arg2->unk2C->unk52 = 0;
            func_800E4C04(arg1->unk2, 0, 1);
        }
        if (arg1->unk5 != 3) {
            if (arg1->unk24 != NULL) {
                if (arg1->unk24->unk30 == -1) {
                    arg1->unk24 = NULL;
                }
                if (arg1->unk24 != NULL && t < 5) {
                    k = (t << 12) >> 2;
                    arg1->unk24->unk20 = 0x1000 - k;
                    if (arg1->unk24 != NULL) {
                        arg1->unk24->unk1C = k * 2 + 0x1000;
                    }
                    if (t != 4) {
                        goto skip;
                    }
                    func_800E3D54(arg1->unk24);
                }
            }
            D_800FEC53 = 1;
            func_800E4C54();
        } else {
            D_800FEC53 = 1;
            if (arg1->unk3 != 0xFF) {
                btlfx_getUnitPos(arg1->unk3, v);
                func_800E436C(arg1->unk2, v, (s32)arg1->unk1C, t, 0);
            }
        }
    skip:
        if (arg1->unk5 == 1) {
            if (t >= 5) {
                return 1;
            }
            break;
        }
        if (t >= btlfx_getMotionLength(arg1->unk2) - 1) {
            if (arg1->unk5 == 0 || arg1->unk5 == 3) {
                btlfx_playUnitMotion(arg1->unk2, 9);
            }
            func_800E4C04(arg1->unk2, 2, 0);
            D_800FEC53 = 1;
            func_800E4C54();
            return 1;
        }
        break;
    }
    *arg3 = t;
    arg1->unk6++;
    }
    return 0;
}

s32 btlfx_opReturnHome(s32 arg0, Unk800E5538 *arg1, Unk800F78A4 *arg2, s32 *arg3, s32 *arg4) {
    s32 t;

    if (arg0 == 0) {
        *arg4 = 0x28;
        return 0;
    }
    if (arg0 == 1) {
        arg1->unk1 = 0;
        arg1->unk2 = *arg4;
        if (arg2->unkC->unk40 & 4) {
            arg1->unk2 = 0x11;
        }
        if (arg2->unkC->unk40 & 1) {
            return 1;
        }
        if (btlfx_isEnemyUnit(arg1->unk2) != 0) {
            return 1;
        }
        arg1->unk3 = btl_hasStatus(btl_getUnit(arg1->unk2), 0x1000);
        return 0;
    }
    t = *arg3;
    if (arg1->unk1 == 0) {
        if (btlfx_moveUnitHome(arg1->unk2, t, arg1->unkC, arg1->unk3) != 0) {
            t = -1;
            arg1->unk1 = 1;
            if (func_800E84F0(arg1->unk2) != 0) {
                btlfx_playUnitMotion(arg1->unk2, 9);
                return 1;
            }
            if (arg1->unk3 == 0) {
                btlfx_playUnitMotion(arg1->unk2, 0x20);
            }
        }
    } else {
        func_800E436C(arg1->unk2, NULL, (s32)arg1->unk1C, t, 1);
        if (t >= btlfx_getMotionLength(arg1->unk2) - 1) {
            btlfx_resetUnitMotion(arg1->unk2, 1);
            return 1;
        }
    }
    *arg3 = t;
    return 0;
}

s32 func_800E56DC(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 *arg4) {
    switch (arg0) {
    case 1:
        D_80104DF4.unk0 = arg4[0];
        D_80104DF4.unk1 = arg4[4];
        break;
    }
    return 1;
}

s32 func_800E5714(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 *arg4) {
    s32 idx;
    s32 k;
    u16 *p;
    if (arg0 == 1) {
        idx = arg4[1] & 0x1F;
        k = (arg4[1] & 0xE0) >> 5;
        p = func_800E4530(arg4[0]);
        if (k >= 0) {
            if (k < 2) {
                p[1] = D_800F8CB4[idx];
            } else if (k < 4) {
                p[2] = D_800F8CB4[idx];
            }
        }
    }
    return 1;
}

s32 func_800E57B0(s32 arg0, Unk800E57B0 *arg1, Unk800F78A4 *arg2, s32 *arg3, s32 *arg4) {
    SVECTOR rot;
    s16 v[4];
    s32 a;
    s32 b;
    Pool6CEntry *e;
    u16 *p;

    if (arg0 == 0) {
        *arg4 = 0x2C;
    } else if (arg0 == 1) {
        a = arg4[0];
        b = arg4[1];
        arg1->unk0 = D_80104DF0.unk0 >> 6;
        arg1->unk4 = D_800F8D04[D_80104DF0.unk0 & 0xF];
        arg1->unk1 = D_80104DF4.unk0;
        arg1->unk2 = D_80104DF4.unk1 * 8;
        arg1->unk14 = 0;
        if (arg1->unk1 != 0xFF) {
            btlfx_getUnitBodyPos(arg1->unk1, 0, &arg1->unk1C.vx);
        } else {
            *(s32 *)&arg1->unk1C.vx = *(s32 *)&arg2->unkC->unk38;
            *(s32 *)&arg1->unk1C.vz = *(s32 *)&arg2->unkC->unk3C;
            arg1->unk1C.vy -= 0xC0;
        }
        switch (arg1->unk0) {
        case 0:
            arg1->unk8 = D_80104DF0.unk1;
            arg1->unkC = a;
            arg1->unk6 = (s8)b << 8;
            arg1->unk10 = func_800E4530(arg1->unk8);
            ((u16 *)arg1->unk10)[1] = D_800F8CB4[arg1->unkC];
            arg1->unkE = *(u16 *)arg1->unk10 & 0x7FFF;
            arg1->unk18 = (s8)D_80104DF0.unk2 * 4;
            arg1->unk1A = (s8)D_80104DF0.unk3 * 4;
            break;
        case 1:
            arg1->unk8 = D_80104DF0.unk1;
            arg1->unkC = a;
            arg1->unk6 = (s8)b << 8;
            arg1->unk10 = func_800E4530(arg1->unk8);
            ((u16 *)arg1->unk10)[2] = D_800F8CB4[arg1->unkC];
            arg1->unkE = *(u16 *)arg1->unk10 & 0x7FFF;
            break;
        case 2:
            arg1->unkA = D_80104DF0.unk1;
            arg1->unk8 = a;
            arg1->unkE = D_80104DF0.unk0 & 0xF;
            arg1->unk4 = b;
            break;
        case 3:
            arg1->unk8 = a;
            arg1->unkE = D_80104DF0.unk1;
            arg1->unk10 = func_800E4530(arg1->unk8);
            arg1->unk6 = 0;
            break;
        default:
            return 0;
        }
    } else {
        if (arg1->unk1 != 0xFF) {
            arg2->unk28 = func_800E7CC4(arg1->unk1, arg1->unk2);
            rot.vy = func_800E7C54(0x10, arg1->unk1) << 11;
            rot.pad = 0;
            rot.vx = 0;
            rot.vz = arg1->unk6;
        }
        switch (arg1->unk0) {
        case 0:
            func_800D1680(arg1->unk10, &arg1->unk1C, &rot, arg1->unk4, arg1->unk14, 1, 0x1000, arg1->unk18, arg1->unk1A);
            break;
        case 1:
            func_800D1D74(arg1->unk10, &arg1->unk1C, &rot, arg1->unk4, arg1->unk14, 1, 0x1000);
            break;
        case 2:
            if (arg1->unk14 == arg1->unkE) {
                if (arg1->unk1 != 0xFF) {
                    if (arg1->unk1 == 0x10) {
                        func_800E7D3C(arg1->unk1, 1, &arg1->unk1C.vx);
                        btlfx_getUnitBodyPos(0, 0, v);
                        btlfx_aimAngles(&arg1->unk1C.vx, v, arg1->unk24);
                        if (btl_getUnit(0) == btl_getUnit(arg1->unk1) && arg1->unkA == 0x36) {
                            return 1;
                        }
                    } else {
                        btlfx_getUnitBodyPos(arg1->unk1, 0, &arg1->unk1C.vx);
                    }
                }
                e = func_800E3CEC((s32)func_800E4530(arg1->unkA), (s32)&arg1->unk1C);
                if (e != NULL) {
                    e->unk22 = arg1->unk2;
                }
                p = func_800E4530(arg1->unk8);
                if (e != NULL) {
                    e->unk18->unk1C = (s32)p;
                    e->unk18->unk0 = arg1->unk4;
                }
                e->unk18->unk8 = *p & 0x7FFF; /* outside the NULL check in the original */
                arg1->unk10 = e;
            }
            if (arg1->unkE < arg1->unk14) {
                e = arg1->unk10;
                if (arg1->unk1 == 0x10 && e != NULL) {
                    e->unk5C.vx = arg1->unk24[0];
                    e->unk5C.vy = arg1->unk24[1];
                    e->unk5C.vz = arg1->unk24[2];
                }
                if (e->unk30 == -1) {
                    return 1;
                }
            }
            goto next;
        case 3:
            func_800D23D8(arg1->unk10, &arg1->unk1C, &rot, arg1->unk4, arg1->unk14, 1, math_cos((arg1->unk14 << 10) / arg1->unkE));
            break;
        default:
            arg2->unk28 = 0;
            goto inc;
        }
        if (arg1->unk14 >= arg1->unkE - 1) {
            return 1;
        }
    next:
        arg2->unk28 = 0;
    inc:
        arg1->unk14++;
    }
    return 0;
}

void func_800E5D48(s8 arg0, s8 arg1, s8 arg2, s32 arg3, s32 arg4, s32 arg5) {
    D_80104DF4.unk0 = arg0;
    D_80104DF4.unk1 = arg1;
    D_80104DF0.unk1 = arg2;
    D_80104DF0.unk0 = arg5 | 0x80;
    btlfx_spawnTask(1, g_btlfxEnv->unk0->unk8, arg3, arg4);
}

s32 btlfx_opApplyResult(s32 arg0, void *arg1, Unk800F78A4 *arg2, void *arg3, s32 *arg4) {
    s32 id;
    s32 *a;

    if (arg0 == 1) {
        a = arg4;
        id = a[0];
        if (id == 0xFF) {
            id = -1;
        } else if (id == 0xFE) {
            id = btlfx_rand() % g_btlfxEnv->unkC->unk24;
        }
        if (a[1] == 0) {
            btlfx_execCmd(id);
            return 1;
        }
        if (a[1] == 1) {
            if (D_80100D8C == 0x16D || (u32)(D_800FEC42 - 0x6B) >= 2 || id != 0x10) {
                btlfx_showPopups(id);
            }
            return 1;
        }
    }
    return 1;
}

s32 btlfx_opAddCameraChoice(s32 arg0, s32 arg1, s32 arg2, Unk800F78A40 *arg3, s32 *arg4) {
    s32 v;
    Unk800FEC40 *p;
    s16 *h;

    if (arg0 == 1) {
        v = arg4[0];
        if (v == 0xFF) {
            h = g_btlfxState.unkA.h;
            h[0] = 0;
            h[1] = 0;
        } else {
            p = &g_btlfxState;
            if (p->unkA.b.unk3 < 3) {
                p->unkA.b.unk0[p->unkA.b.unk3] = v;
                p->unkA.b.unk3++;
            }
        }
    }
    return 1;
}

s32 btlfx_opPlayCamera(s32 arg0, s32 arg1, s32 arg2, Unk800F78A40 *arg3, s32 *arg4) {
    s32 r;
    s32 k;
    if (arg0 == 1) {
        r = 0;
        switch (arg4[1]) {
        case 0:
            r = btlfx_getFileData(arg3->unk8, arg4[0]);
            break;
        case 1:
            r = btlfx_getFileData(arg3->unk8, g_btlfxState.unkA.b.unk0[g_btlfxState.unkE]);
            break;
        case 2:
            if (g_btlfxState.unkA.b.unk3 != 0) {
                g_btlfxState.unkE = btlfx_rand() % g_btlfxState.unkA.b.unk3;
            }
            r = btlfx_getFileData(arg3->unk8, g_btlfxState.unkA.b.unk0[g_btlfxState.unkE]);
            break;
        case 3:
            k = g_btlfxEnv->unk2C->unk53;
            if (k >= g_btlfxState.unkA.b.unk3) {
                k = g_btlfxState.unkA.b.unk3 - 1;
            }
            r = btlfx_getFileData(arg3->unk8, g_btlfxState.unkA.b.unk0[k]);
            break;
        }
        if (D_800F8D74 != 0) {
            btlcam_play(r);
        }
    }
    return 1;
}

void func_800E60A4(s32 arg0, s32 arg1) {
    s32 v;
    if (arg0 < 0) {
        v = 0x80;
    } else {
        v = D_800F8D0C[arg0];
    }
    btlfx_spawnTask(0xA, -1, v, arg1);
}

s32 btlfx_opFadeBackground(s32 arg0, Unk800E6270 *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    s32 n;
    s32 v;
    if (arg0 == 0) {
        *arg4 = 8;
        return 0;
    }
    if (arg0 == 1) {
        arg1->unk4 = btlbg_getBrightness() & 0xFF;
        arg1->unk6 = arg4[0];
        if (arg1->unk6 == 0xFF) {
            arg1->unk6 = 0x80;
        }
        arg1->unk6 -= arg1->unk4;
        if ((arg1->unk0 = arg4[1]) == 0 || arg1->unk6 == 0) {
            btlbg_setBrightness(arg1->unk4 + arg1->unk6);
            return 1;
        }
        return 0;
    }
    n = *arg3;
    v = arg1->unk4 + n * arg1->unk6 / arg1->unk0;
    if (v < 0x10) {
        v = 0;
    }
    btlbg_setBrightness(v);
    if (n < arg1->unk0) {
        *arg3 = n;
        return 0;
    }
    return 1;
}

void func_800E6244(s32 arg0, s32 arg1) {
    btlfx_spawnTask(4, -1, arg0 & 0xFF, arg1 & 0xFF);
}

s32 btlfx_opFadeUnits(s32 arg0, Unk800E6270 *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    s16 v;
    s32 n;
    if (arg0 == 0) {
        *arg4 = 0xC;
        return 0;
    }
    if (arg0 == 1) {
        if (arg4[1] & 4) {
            arg1->unk4 = 0x80;
            v = -0x80;
        } else {
            arg1->unk4 = 0;
            v = 0x80;
        }
        arg1->unk6 = v;
        arg1->unk8 = arg4[1] & 0xB;
        if ((arg1->unk0 = arg4[0]) != 0) {
            return 0;
        }
        btlfx_fadeUnitGroup(arg1->unk8, arg1->unk4 + arg1->unk6);
        return 1;
    }
    n = *arg3;
    btlfx_fadeUnitGroup(arg1->unk8, arg1->unk4 + n * arg1->unk6 / arg1->unk0);
    if (n < arg1->unk0) {
        *arg3 = n;
        return 0;
    }
    return 1;
}

s32 func_800E639C(s32 arg0, s32 arg1, s32 arg2, Unk800F78A40 *arg3, s32 *arg4) {
    if (arg0 == 1) {
        s32 v = -1;
        if (arg4[1] != 0xFF) {
            v = arg4[1];
        }
        func_800E9930(arg4[0], arg3->unk8, v);
    }
    return 1;
}

void func_800E63E4(s32 arg0, s32 arg1) {
    btlfx_spawnTask(0xE, -1, arg0, arg1);
}

s32 func_800E6410(s32 arg0, Unk800E6410 *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    s32 n;
    s32 *p;
    if (arg0 == 0) {
        *arg4 = 0xC;
        return 0;
    }
    if (arg0 == 1) {
        p = arg4;
        if (p[0] == 2) {
            return 1;
        }
        if (p[0] == 1) {
            arg1->unk4 = 0;
        } else if (p[0] == 0) {
            arg1->unk4 = 0xFF;
        }
        if ((arg1->unk0 = p[1]) != 0) {
            return 0;
        }
        btlfx_drawScreenTint(g_btlfxEnv->unk20 + 1, 0xFF, arg1->unk4, arg1->unk4, arg1->unk4);
        return 1;
    }
    n = *arg3;
    btlfx_drawScreenTint(g_btlfxEnv->unk20 + 1, 0xFF, arg1->unk4, arg1->unk4, arg1->unk4);
    if (n < arg1->unk0) {
        *arg3 = n;
        return 0;
    }
    return 1;
}

void func_800E651C(u8 *p, CVECTOR *out, s32 x) {
    Grad *g = (Grad *)p;
    s32 n = g->count;
    s32 i;
    s32 c;

    p += 8;
    if (g->total == 0) {
        i = n = 0; /* i: running start */
        p += 4;
    loop:
        c = p[-1];
        if (c != 0) {
            ((u16 *)p)[1] = i;
            i += c;
            n++;
            ((u16 *)p)[0] = c;
            p += 8;
            goto loop;
        }
        p = (u8 *)g + 8;
        g->total = i;
        g->count = n;
    }
    if (g->total < x) {
        x = g->total;
    }
    p += n * 8 - 8;
    for (i = 0; i < n; i++, p -= 8) {
        if (x >= ((u16 *)p)[3]) {
            break;
        }
    }
    c = x - ((u16 *)p)[3];
    c = (c << 12) / ((u16 *)p)[2];
    LoadAverageCol(p, p + 8, 0x1000 - c, c, (u8 *)out);
}

void func_800E6624(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    RECT b;
    RECT a;
    DR_MOVE *p;
    s32 m = arg0 % arg4;
    s32 h;
    p = (DR_MOVE *)g_btlfxEnv->unk24;
    g_btlfxEnv->unk24 = (u8 *)(p + 2);
    h = arg4 - m;
    if (m != 0) {
        a.x = arg5;
        a.y = arg6;
        a.w = arg3;
        a.h = m;
        SetDrawMove(p, &a, arg1, arg2 + h);
        AddPrim(&g_btlfxEnv->unk20[0xFFF], p);
    }
    p++;
    if (h != 0) {
        b.x = arg5;
        b.y = arg6 + m;
        b.w = arg3;
        b.h = h;
        SetDrawMove(p, &b, arg1, arg2);
        AddPrim(&g_btlfxEnv->unk20[0xFFF], p);
    }
}

void func_800E6764(u32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8,
                   SVECTOR *arg9, VECTOR *arg10, CVECTOR *arg11, s32 arg12, void (*arg13)(SVECTOR *)) {
    XY32 buf[2][18];
    SVECTOR v[4];
    SVECTOR uv[4];
    MATRIX m;
    long ox;
    long oy;
    s32 cx;
    s32 cy;
    s32 tp0;
    s32 tp1;
    POLY_FT4 *p;
    s32 x;
    s32 y;
    s32 r;
    s32 c;
    s32 tp;
    s32 fl;
    s32 t;
    s32 sx;
    s32 sy;
    s32 ty;
    u32 len;

    ReadGeomOffset(&ox, &oy);
    p = (POLY_FT4 *)g_btlfxEnv->unk24;
    g_btlfxEnv->unk24 = (u8 *)(p + arg5 * arg6);
    if (arg12 != 0xFF) {
        arg11->cd = 0x2E;
    } else {
        arg11->cd = 0x2C;
    }
    if (arg12 == 0xFF) {
        arg12 = 0;
    }
    tp0 = GetTPage(2, arg12, g_btlfxEnv->unk18->disp.x, g_btlfxEnv->unk18->disp.y);
    tp1 = GetTPage(2, arg12, g_btlfxEnv->unk18->disp.x + 0x80, g_btlfxEnv->unk18->disp.y) << 16;
    arg7 *= arg3;
    tp0 <<= 16;
    arg8 *= arg4;
    cx = arg1;
    cy = arg2;
    SetGeomOffset(arg1, arg2);
    func_800316D8(arg9, &m);
    arg10->vz = 0x1000;
    ScaleMatrix(&m, arg10);
    m.t[1] = 0;
    m.t[0] = 0;
    m.t[2] = ReadGeomScreen();
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    gte_ldvz012_0();
    for (y = 0, r = 0; r <= arg6; r++, y += arg4) {
        for (x = 0, c = 0; c <= arg5; c++, x += arg3) {
            ((XY32 *)&v[0])->w = (*buf)[c].w;
            sx = x - arg7;
            t = buf[1][c].w;
            uv[1].vx = (v[1].vx = sx) + cx;
            uv[1].vy = (v[1].vy = y - arg8) + cy;
            *(s32 *)&uv[0] = t;
            buf[1][c].w = *(s32 *)&uv[1];
            if (arg13 != NULL) {
                arg13(&v[1]);
            }
            buf[0][c].w = *(s32 *)&v[1];
            if (c != 0 && r != 0) {
                gte_ldxy3(&v[1], &v[0], &v[3]);
                *(u32 *)&p->r0 = *(u32 *)arg11;
                gte_rtpt_b();
                if (uv[0].vx < 0x100 && uv[1].vx < 0x100 && uv[2].vx < 0x100 && uv[3].vx < 0x100) {
                    fl = 0;
                    tp = tp0;
                } else {
                    fl = 1;
                    tp = tp1;
                    uv[0].vx -= 0x80;
                    uv[1].vx -= 0x80;
                    uv[2].vx -= 0x80;
                    uv[3].vx -= 0x80;
                }
                gte_stsxy3(&p->x0, &p->x1, &p->x2);
                gte_ldxy0(&v[2]);
                if (arg0 < 0x1000) {
                    len = 0x09000000;
                    addPrimLen(&g_btlfxEnv->unk20[arg0], p, len);
                }
                gte_rtps_b();
                *(u32 *)&p->u0 = uv[1].vx | (uv[1].vy << 8);
                *(u32 *)&p->u1 = uv[0].vx | (uv[0].vy << 8) | tp;
                *(u32 *)&p->u2 = uv[3].vx | (uv[3].vy << 8);
                *(u32 *)&p->u3 = uv[2].vx | (uv[2].vy << 8);
                gte_stsxy(&p->x3);
                if (fl) {
                    uv[0].vx += 0x80;
                    uv[1].vx += 0x80;
                    uv[2].vx += 0x80;
                    uv[3].vx += 0x80;
                }
                p++;
            }
            *(s32 *)&v[2] = *(s32 *)&v[0];
            *(s32 *)&v[3] = *(s32 *)&v[1];
            *(s32 *)&uv[2] = *(s32 *)&uv[0];
            *(s32 *)&uv[3] = *(s32 *)&uv[1];
        }
    }
    SetGeomOffset(ox, oy);
}

void func_800E6C58(SVECTOR *arg0) {
    MATRIX m;
    SVECTOR v;
    memset((s32)&v, 0, 8);
    if (arg0 == 0) {
        arg0 = &v;
    }
    func_800316D8(arg0, &m);
    m.t[1] = 0;
    m.t[0] = 0;
    m.t[2] = ReadGeomScreen();
    D_80104E10 = *g_btlfxEnv->unk14;
    *g_btlfxEnv->unk14 = m;
}

void func_800E6D58(void) {
    *g_btlfxEnv->unk14 = D_80104E10;
}

void func_800E6DB8(s32 arg0) {
    s32 *p = &D_80104E00;
    *p = ReadGeomScreen();
    SetGeomScreen(arg0);
}

void func_800E6DF4(void) {
    SetGeomScreen(D_80104E00);
}

void func_800E6E1C(s32 x, s32 y, s32 w, s32 h, u32 *ot, s32 arg5, s32 arg6, s32 arg7) {
    long ox;
    long oy;
    s32 save;
    s32 mode;
    DrawEnvPair *e;

    save = D_800F7274;
    e = (DrawEnvPair *)g_btlfxEnv->unk24;
    D_800F7274 = 0;
    g_btlfxEnv->unk24 = (u8 *)(e + 1);
    SetDefDrawEnv(&e->unk0, x, y, w, h);
    e->unk0.isbg = 1;
    e->unk0.dtd = 0;
    e->unk0.dfe = 0;
    e->unk0.r0 = 7;
    e->unk0.g0 = 7;
    e->unk0.b0 = 7;
    e->unk5C = *(DRAWENV *)&g_btlfxEnv->unk1C;
    ClearOTag(ot, 0x100);
    mode = ReadGeomScreen();
    ReadGeomOffset(&ox, &oy);
    SetGeomScreen(0x200);
    SetGeomOffset(w >> 1, h >> 1);
    D_800F8D50.t[0] = -arg5;
    D_800F8D50.t[1] = arg6;
    D_800F8D50.t[2] = arg7 + 0x2900;
    func_800CEEC8(2);
    g_btlfxEnv->unk24 = btlbg_draw(&D_800F8D50, g_gameState->unk1C->unk8E4.battle->unk11C0->unk1C, (s32)g_btlfxEnv->unk24, NULL, (s32)ot);
    func_800CEEC8(0xF);
    EnterCriticalSection();
    DrawOTagEnv(ot, &e->unk0);
    PutDrawEnv(&e->unk5C);
    ExitCriticalSection();
    D_800F7274 = save;
    SetGeomScreen(mode);
    SetGeomOffset(ox, oy);
}

void btlfx_setTargets(BtlUnitList *arg0) {
    g_btlfxEnv->unkC = arg0;
}

void func_800E702C(void) {
    GameData *gd;
    g_btlfxEnv->unk28 = 0;
    gd = g_gameState->unk1C;
    g_btlfxEnv->unk20 = (s32 *)gd->unk10[gd->unk8];
    g_btlfxEnv->unk18 = &g_gameState->unkC[g_gameState->unk1C->unk8];
    g_btlfxEnv->unk1C = &g_gameState->unk10[g_gameState->unk1C->unk8];
    g_btlfxEnv->unk14 = &g_gameState->unk1C->unk18;
}

void btlfx_beginPrims(u8 *arg0, s32 *arg1) {
    Unk800F78A4 *p = g_btlfxEnv;
    GameState *g = g_gameState;
    p->unk28 = 0;
    p->unk20 = arg1;
    p->unk18 = &g->unkC[g->unk1C->unk8];
    p->unk1C = &g->unk10[g->unk1C->unk8];
    p->unk14 = &g->unk1C->unk18;
    p->unk24 = arg0;
}

u8 *btlfx_endPrims(void) {
    return g_btlfxEnv->unk24;
}

BtlUnit *btl_getUnit(s32 arg0) {
    if (arg0 == 0x10) {
        return g_btlfxEnv->unkC->unk0;
    }
    if (arg0 == 0x11) {
        return g_btlfxEnv->unkC->unk44;
    }
    if (arg0 < 8) {
        if (arg0 < 0 || arg0 >= g_btlfxEnv->unkC->unk24) {
            SystemError(0x53, 0x136);
        }
        return g_btlfxEnv->unkC->unk4[arg0];
    }
    if (arg0 - 8 < 0 || arg0 - 8 >= g_btlfxEnv->unkC->unk27) {
        SystemError(0x53, 0x136);
    }
    return g_btlfxEnv->unkC->unk28[arg0 - 8];
}

s32 func_800E7298(BtlUnit *arg0) {
    return (arg0->unk68 & 0x800001) == 0x800000;
}

void btlfx_clearTargets(void) {
    g_btlfxEnv->unkC = NULL;
}

s32 btlfx_getUnitRadius(s32 arg0) {
    s32 flag;
    BtlUnit *u;
    u32 f;
    if (arg0 & 0x100) {
        arg0 &= 0xFF;
        flag = 1;
    } else {
        flag = 0;
    }
    u = btl_getUnit(arg0);
    f = u->unk68;
    if ((f & 1) || flag != 0) {
        return u->unk9C->unk8->unkA;
    }
    return g_gameState->unk1C->unk8E4.battle->unk488[(f >> 1) & 0xF].unk0->unk24;
}

s32 func_800E7370(BtlUnit *arg0) {
    return arg0->unk9C->unk8->unk8;
}

void func_800E738C(s32 arg0) {
    func_800E7370(btl_getUnit(arg0));
}

void func_800E73B4(BtlUnit *arg0, u16 *arg1) {
    BtlBone *b = &arg0->unk9C->unk20[arg0->unk7C];
    arg1[0] = b->unk14;
    arg1[1] = b->unk18;
    arg1[2] = b->unk1C;
}

void btlfx_getUnitHomePos(s32 arg0, s16 *arg1) {
    BtlUnit *u = btl_getUnit(arg0);
    if (func_800E7298(u) != 0) {
        arg1[0] = u->unk8C[0] >> 16;
        arg1[1] = u->unk8C[1] >> 16;
        arg1[2] = u->unk8C[2] >> 16;
    } else {
        arg1[0] = u->unk98->unk0[0] >> 16;
        arg1[1] = u->unk98->unk0[1] >> 16;
        arg1[2] = u->unk98->unk0[2] >> 16;
    }
}

void func_800E748C(BtlUnit *arg0, s16 *arg1) {
    if (func_800E7298(arg0) != 0 || !(arg0->unk68 & 1)) {
        func_800E73B4(arg0, (u16 *)arg1);
        arg1[1] = 0;
    } else {
        arg1[0] = arg0->unk8C[0] >> 16;
        arg1[1] = arg0->unk8C[1] >> 16;
        arg1[2] = arg0->unk8C[2] >> 16;
    }
}

void btlfx_getUnitPos(s32 arg0, s16 *arg1) {
    func_800E748C(btl_getUnit(arg0), arg1);
}

void func_800E753C(BtlUnit *arg0, s32 arg1, s16 *arg2) {
    s32 h;
    if (func_800E7298(arg0) != 0) {
        func_800E73B4(arg0, (u16 *)arg2);
        return;
    }
    func_800E748C(arg0, arg2);
    if (arg0->unk68 & 1) {
        h = func_800E7370(arg0);
        if (arg1 == 0) {
            h >>= 1;
        }
        if ((u8)btl_hasStatus(arg0, 0x200000)) {
            h -= 0x80;
        }
        arg2[1] -= h;
    } else {
        func_800E73B4(arg0, (u16 *)arg2);
        if (arg1 == 1) {
            arg2[1] = arg0->unk9C->unk20[g_gameState->unk1C->unk8E4.battle->unk488[(arg0->unk68 >> 1) & 0xF].unk0->unk29[2]].unk18;
        }
    }
}

void btlfx_applyFloatHeight(s32 arg0, s16 *arg1) {
    if (btl_hasStatus(btl_getUnit(arg0), 0x200000) & 0xFF) {
        arg1[1] -= 0x80;
    }
}

void btlfx_getUnitBodyPos(s32 arg0, s32 arg1, s16 *arg2) {
    func_800E753C(btl_getUnit(arg0), arg1, arg2);
}

void btlfx_setUnitPos(s32 arg0, s16 *arg1) {
    BtlUnit *u = btl_getUnit(arg0);
    if (func_800E7298(u) == 0) {
        u->unk8C[0] = arg1[0] << 16;
        u->unk8C[1] = arg1[1] << 16;
        u->unk8C[2] = arg1[2] << 16;
    }
}

s32 func_800E774C(s32 arg0, s16 *arg1, s16 *arg2) {
    BtlUnitList *l = g_btlfxEnv->unkC;
    BtlUnit *u = l->unk28[arg0];
    if (arg0 >= l->unk27) {
        return 1;
    }
    func_800E753C(u, 0, arg1);
    arg2[0] = u->unk86[0];
    arg2[1] = u->unk86[1];
    arg2[2] = u->unk86[2];
    return 0;
}

void btlfx_getUnitRot(s32 arg0, s16 *arg1) {
    BtlUnit *u = btl_getUnit(arg0);
    s16 *p = u->unk86;
    arg1[0] = p[0];
    arg1[1] = p[1] + 0x800;
    arg1[2] = p[2];
}

void btlfx_setUnitRot(s32 arg0, s16 *arg1) {
    BtlUnit *u = btl_getUnit(arg0);
    if (func_800E7298(u) == 0) {
        s16 *p = u->unk86;
        p[0] = arg1[0];
        p[1] = arg1[1] - 0x800;
        p[2] = arg1[2];
    }
}

s32 btlfx_isEnemyUnit(s32 arg0) {
    return (btl_getUnit(arg0)->unk68 & 1) ^ 1;
}

void func_800E78B4(s32 arg0, s16 *arg1, s16 *arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    SVec4 rot;
    SVec4 tmp;
    SVec4 cur;
    s32 t;

    if (arg6 < arg5) {
        return;
    }
    btl_getUnit(arg0);
    if (arg6 == 0) {
        arg6 = 1;
    }
    t = (arg5 << 12) / arg6;
    if (arg5 == 0 || arg2 == NULL) {
        if (arg2 == NULL) {
            arg2 = tmp.h;
            t = 0x1000;
        }
        btlfx_getUnitRot(arg0, arg2);
    }
    if (arg1 != NULL) {
        btlfx_getUnitPos(arg0, cur.h);
        if (cur.w[0] != ((SVec4 *)arg1)->w[0] || cur.h[2] != arg1[2]) {
            btlfx_aimAngles(cur.h, arg1, rot.h);
            if (!(arg3 & 1)) {
                rot.h[0] = 0;
            }
            if (!(arg3 & 2)) {
                rot.h[1] += 0x800;
            }
        } else {
            rot.w[0] = ((SVec4 *)arg2)->w[0];
            rot.w[1] = ((SVec4 *)arg2)->w[1];
        }
    } else {
        rot.h[0] = 0;
        rot.h[1] = 0;
    }
    rot.h[2] = arg4;
    btlfx_turnToward(arg2, rot.h, 0x1000 - t);
    btlfx_setUnitRot(arg0, rot.h);
}

void btlfx_getUnitScale(s32 arg0, VECTOR *arg1) {
    BtlUnit *u = btl_getUnit(arg0);
    BtlUnit9C *m = u->unk9C;
    if (!(m->unk0 & 2)) {
        arg1->vz = 0x1000;
        arg1->vy = 0x1000;
        arg1->vx = 0x1000;
    } else {
        arg1->vx = m->unk54;
        arg1->vy = u->unk9C->unk56;
        arg1->vz = u->unk9C->unk58;
    }
}

void func_800E7ACC(s32 arg0, s32 *arg1) {
    BtlUnit *u = btl_getUnit(arg0);
    if (func_800E7298(u) == 0) {
        if (arg1[0] == 0x1000 && arg1[1] == arg1[0] && arg1[2] == arg1[1]) {
            u->unk9C->unk0 &= ~2;
        } else {
            u->unk9C->unk0 |= 2;
            u->unk9C->unk54 = arg1[0];
            u->unk9C->unk56 = arg1[1];
            u->unk9C->unk58 = arg1[2];
        }
    }
}

void btlfx_getStrikePos(s32 arg0, s32 arg1, s16 *arg2) {
    s16 sp10[4];
    s16 sp18[4];
    s16 sp20[4];
    if (btl_getUnit(arg1) != btl_getUnit(arg0)) {
        btlfx_getUnitPos(arg0, sp10);
        btlfx_getUnitPos(arg1, sp18);
        btlfx_aimAngles(sp18, sp10, sp20);
        func_800CFD78(sp18, sp20, btlfx_getUnitRadius(arg1), arg2);
    } else {
        btlfx_getUnitPos(arg0, arg2);
    }
}

s32 func_800E7C54(s32 arg0, s32 arg1) {
    s16 a[4];
    s16 b[4];
    s32 r;
    btlfx_getUnitPos(arg0, a);
    btlfx_getUnitPos(arg1, b);
    r = a[2] <= b[2];
    if (((g_btlfxEnv->unk2C->unk50 - 0x400) & 0xFFF) > 0x800) {
        r = -r;
    }
    return r;
}

s32 func_800E7CC4(s32 arg0, s32 arg1) {
    return (u16)(btl_getUnit(arg0)->unk9C->unk8->unkA / 6) + arg1;
}

u32 func_800E7D18(s32 arg0) {
    return (u8)btl_getUnit(arg0)->unk68 >> 5;
}

void func_800E7D3C(s32 arg0, s32 arg1, s16 *arg2) {
    BtlUnit *u;
    s32 bone;
    MATRIX m;

    SVECTOR v;

    u = btl_getUnit(arg0);
    bone = (g_gameState->unk1C->characters + ((u->unk68 >> 1) & 0xF))->unk21;
    if (arg1 == 0) {
        func_800DC818(arg0, bone, (u16 *)arg2);
    } else {
        func_800DC8AC(arg0, bone, (BtlBone *)&m);
        gte_SetRotMatrix(&m);
        gte_SetTransMatrix(&m);
        func_800BC1E0((Unk800BC1E0 *)u->unk74, (s16 *)&v);
        gte_ldv0(&v);
        gte_rtv0tr();
        gte_stlvnl(m.t);
        arg2[0] = m.t[0];
        arg2[1] = m.t[1];
        arg2[2] = m.t[2];
    }
    gte_SetRotMatrix(g_btlfxEnv->unk14);
    gte_SetTransMatrix(g_btlfxEnv->unk14);
}

void btlfx_setUnitBrightness(BtlUnit *arg0, s32 arg1) {
    s32 f;
    s16 v;
    if (arg1 >= 0x100) {
        f = 0;
        arg1 -= 0x100;
    } else {
        f = 1;
    }
    if ((arg0->unk68 & 1) && arg0->unkA8 != NULL && f && (*(u32 *)arg0->unkA8 & 0x30) == 0x20) {
        v = arg1 - 0x80;
        btl_setModelRgb(arg0->unkA8, v, v, v);
        if (arg1 < 0x46) {
            btl_setModelAbr(arg0->unkA8, 0x20);
        }
    }
    if ((*(u32 *)arg0->unk9C & 0x30) == 0x20) {
        v = arg1 - 0x80;
        btl_setModelRgb(arg0->unk9C, v, v, v);
        if (arg1 < 0x46) {
            btl_setModelAbr(arg0->unk9C, 0x20);
        }
    }
}

void btlfx_fadeUnit(BtlUnit *arg0, s32 arg1) {
    if (func_800E7298(arg0) == 0) {
        if (arg1 < 8) {
            if (!((arg0->unk68 >> 22) & 1)) {
                arg0->unk68 |= 0x400000;
                btlfx_setUnitBrightness(arg0, arg1);
            }
        } else if ((arg0->unk68 >> 22) & 1) {
            if (!(arg0->status & 0x40000000)) {
                arg0->unk68 &= ~0x400000;
            }
        } else {
            btlfx_setUnitBrightness(arg0, arg1);
        }
    }
}

void func_800E806C(s32 arg0, s32 arg1) {
    BtlUnit *u = btl_getUnit(arg0);
    if (arg1 < 0) {
        if (arg1 == -1) {
            btl_hideMeshes(u, 0xFFFF);
        } else {
            btl_showMeshes(u, 0xFFFF);
        }
    } else {
        btlfx_fadeUnit(u, arg1);
    }
}

void func_800E80CC(s32 arg0, s32 arg1) {
    BtlUnit *u = btl_getUnit(arg0);
    if (func_800E7298(u) == 0) {
        BtlUnit9C *m = u->unk9C;
        s16 v = arg1 - 0x80;
        if ((*(u32 *)m & 0x30) == 0x20) {
            btl_setModelRgb(m, v, v, v);
        }
    }
}

s32 btlfx_fadeUnitGroup(u32 type, s32 arg1) {
    BtlUnit *u;
    s32 n = 0;
    for (u = g_gameState->unk1C->unk8E4.battle->unk6F8; u != NULL; u = u->unk0) {
        switch (type) {
        case 0:
            if (u->unk68 & 1) {
                btlfx_fadeUnit(u, arg1);
                n++;
            }
            break;
        case 1:
            if (!(u->unk68 & 1)) {
                btlfx_fadeUnit(u, arg1);
                n++;
            }
            break;
        case 9:
            if (u == btl_getUnit(0x10)) {
                btlfx_fadeUnit(u, arg1);
                n++;
            }
            break;
        case 2:
            btlfx_fadeUnit(u, arg1);
            n++;
            break;
        case 8:
            if (btlfx_isEnemyUnit(0x10)) {
                if (!(u->unk68 & 1)) {
                    btlfx_fadeUnit(u, arg1);
                    n++;
                }
            } else if (u != btl_getUnit(0x10) && (u->unk68 & 1)) {
                btlfx_fadeUnit(u, arg1);
                n++;
            }
            break;
        }
    }
    return n;
}

void func_800E8274(s32 arg0) {
    if (btlfx_isEnemyUnit(arg0) != 0) {
        btl_removeStatuses(btl_getUnit(arg0), -0x1381);
    } else {
        btl_clearUnitStatus(btl_getUnit(arg0));
    }
}

s32 func_800E82CC(s32 id, s16 *arg1, BtlUnit **iter, s32 *out) {
    BtlUnit *u;
    s32 r = 0;

    if (*iter == (BtlUnit *)-1) {
        u = g_gameState->unk1C->unk8E4.battle->unk6F8;
    } else {
        if (*iter == NULL) {
            return -2;
        }
        u = *iter;
    }
    func_800E753C(u, 0, arg1);
    if (((u8)btl_hasStatus(u, 0x100) && u != btl_getUnit(id)) || !((u->unk68 >> 28) & 1)) {
        *iter = u->unk0;
        return -1;
    }
    if (u == btl_getUnit(id)) {
        r = 1;
    }
    *out = u->unk68 & 1;
    *iter = u->unk0;
    return r;
}

void func_800E83EC(void) {
    g_evtGlobalVars.b[0xC7] |= 0x10;
}

void btlfx_reapplyMini(BtlUnit *arg0) {
    if (arg0->status & 0x10000000) {
        arg0->unk9C->unk0 |= 2;
        arg0->unk9C->unk54 = 0x800;
        arg0->unk9C->unk56 = 0x800;
        arg0->unk9C->unk58 = 0x800;
    }
}

void btlfx_reapplyVanish(BtlUnit *arg0) {
    if (arg0->status & 0x04000000) {
        btl_hideMeshes(arg0, arg0->unkAE);
    }
}

s32 func_800E8498(s32 arg0) {
    VECTOR v;
    btlfx_getUnitScale(arg0, &v);
    return btl_getUnit(arg0)->unk9C->unk8->unkA * v.vx >> 12;
}

s32 func_800E84F0(s32 arg0) {
    s32 ret = 0;
    if (g_gameState->unk1C->unk8E4.battle->curCmd->cmdNo == 0x33) {
        ret = btl_isCmdPending(btl_getUnit(arg0)->unk4) != 0;
    }
    return ret;
}

s32 func_800E8554(s32 arg0, s32 arg1) {
    s32 r = 0;
    BtlUnit *u = btl_getUnit(arg1);
    if (arg0 == 0) {
        if ((u8)btl_hasStatus(u, 0x100)) {
            u->unk9C->unkC = (BtlUnit9CC *)u->unkA4[4];
        } else if ((u->unk68 >> 19) & 1) {
            u->unk9C->unkC = (BtlUnit9CC *)u->unkA4[9];
        } else {
            u->unk9C->unkC = (BtlUnit9CC *)u->unkA4[(u->unk68 >> 18) & 1];
        }
    } else if ((u8)btl_hasStatus(u, 0x100) || ((u->unk68 >> 1) & 0xF) == 4) {
        func_800E8554(0, 0x10);
        r = 1;
    } else {
        btlfx_playUnitMotion(0x10, 0x1C);
    }
    return r;
}

s32 func_800E865C(s32 arg0, s32 arg1) {
    u32 f = btl_getUnit(arg0)->unk68;
    u8 k = D_800F8D70[arg1];
    return ((f & 1) ? g_gameState->unk1C->characters[(f >> 1) & 0xF].unk34.b.unk4 : 0x13) == k;
}

void func_800E86E0(s32 *arg0, s32 *arg1, u32 arg2) {
    u32 i;
    u32 t = arg2 & 3;
    arg2 >>= 2;
    if (t) {
        arg2++;
    }
    for (i = 0; i < arg2;) {
        i++;
        *arg0++ = *arg1++;
    }
}

s32 *func_800E871C(s32 arg0, s32 *arg1) {
    s32 id = (arg0 & 0xFFFF) | 0x20000;
    u32 size = (u32)func_8001C7FC(5, id, 0);
    s32 *p = func_8001C7FC(4, id, 0);
    if (*arg1 == 0x1234) {
        func_800E86E0(p, arg1 + 1, size);
    } else {
        func_800E86E0(arg1 + 1, p, size);
        *arg1 = 0x1234;
    }
    return p;
}

void func_800E87C4(s32 arg0) {
    func_800AB4FC();
    func_800E86E0(g_gameState->unk1C->unk8E4.battle->unk11C0->unk44, g_btlfxLoader.unk54->unk50[0], arg0 << 10);
    g_btlfxLoader.unk54->unk50[0] = g_gameState->unk1C->unk8E4.battle->unk11C0->unk44;
}

void func_800E8854(BtlUnit9C8 *arg0, void (*arg1)(SVECTOR *, s32)) {
    s32 i;
    s32 j;
    s32 k;
    s32 n;
    BtlMesh *m;
    BtlUnit9C8 *g = arg0;
    for (i = 0; i < g->unk3; i++) {
        m = &g->unk10[i];
        n = 0;
        for (j = 0; j < g->unk2; j++) {
            for (k = 0; k < m->unk14[j]; k++) {
                arg1(&m->unk18[n + k], n + k);
            }
            n += m->unk14[j];
        }
    }
}

void func_800E8980(s32 arg0) {
    g_btlfxEnv->unkC->unk44 = btl_getUnitById(g_btlfxEnv->unkC->unk25[arg0]);
}

void func_800E89D0(void) {
    BtlUnit *u = g_gameState->unk1C->unk8E4.battle->unk6F8;
    while (u != NULL) {
        if (u->unk68 & 1) {
            func_800C0D24(u);
            btlfx_reapplyMini(u);
            btlfx_reapplyVanish(u);
        }
        u = u->unk0;
    }
    if (g_btlfxEnv->unk0->unk2 == 0x1BF) {
        btl_setIdleMotion(btl_getUnit(0x10));
    }
}

void func_800E8A80(s32 arg0) {
    BtlUnit *u = btl_getUnit(arg0);
    if (u->unk68 & 1) {
        func_800C0D24(u);
        btlfx_reapplyMini(u);
        btlfx_reapplyVanish(u);
    }
}

s32 func_800E8AD0(void) {
    BtlUnit *u = g_gameState->unk1C->unk8E4.battle->unk6F8;
    s32 n = 0;
    while (u != NULL) {
        n += (u->unk68 & 0x400001) == 1;
        u = u->unk0;
    }
    return n;
}

/* weapon texture animation: arg1 0 = start anim (arg2 ? 1 : 0), 1 = stop anim 2, 2 = start anim 2 */
void func_800E8B28(s32 arg0, s32 arg1, s32 arg2) {
    BtlUnit *u = btl_getUnit(arg0);

    switch (arg1) {
    case 0:
        ((BtlUnit9C50Arr *)u->unk9C->unk50)->e[arg2 ? 1 : 0].unk4 |= 3;
        ((BtlUnit9C50Arr *)u->unk9C->unk50)->e[arg2 ? 1 : 0].unkC = 0;
        ((BtlUnit9C50Arr *)u->unk9C->unk50)->e[arg2 ? 1 : 0].unk12 = 0x1000;
        break;
    case 1:
        ((BtlUnit9C50Arr *)u->unk9C->unk50)->e[2].unk4 &= ~3;
        break;
    case 2:
        ((BtlUnit9C50Arr *)u->unk9C->unk50)->e[2].unk4 |= 1;
        ((BtlUnit9C50Arr *)u->unk9C->unk50)->e[2].unkC = 0;
        ((BtlUnit9C50Arr *)u->unk9C->unk50)->e[2].unk12 = 0x1000;
        break;
    }
}

void func_800E8C84(s32 arg0) {
    if (arg0 != 0) {
        g_gameState->unk1C->unk8E4.battle->unk11B8 |= 2;
    } else {
        g_gameState->unk1C->unk8E4.battle->unk11B8 &= ~2;
    }
}

s32 func_800E8CE4(void) {
    return (g_gameState->unk1C->unk8E4.battle->unk11B8 >> 1) & 1;
}

void func_800E8D14(void) {
    if (btlfx_isEnemyUnit(0x10) == 0) {
        btl_showCmdName(g_gameState->unk1C->unk8E4.battle->curCmd);
    }
}

s32 func_800E8D60(void) {
    s32 r = 0;
    if (btlfx_isEnemyUnit(0x10) != 0) {
        if (g_btlfxEnv->unkC->unk25[0] == 0) {
            return r;
        }
        r = (u8)(g_btlfxEnv->unk0->unk1C >> 8) >> 7;
    }
    return r;
}

void btlfx_execCmd(s32 arg0) {
    s32 i;
    if (func_800E8D60() == 0) {
        if (arg0 == -1) {
            for (i = 0; i < g_btlfxEnv->unkC->unk24; i++) {
                btl_execCmd(g_btlfxEnv->unkC->unk4[i]);
            }
        } else {
            btl_execCmd(btl_getUnit(arg0));
        }
    }
}

void btlfx_showPopups(s32 arg0) {
    s32 i;
    if (func_800E8D60() == 0) {
        if (arg0 == -1) {
            for (i = 0; i < g_btlfxEnv->unkC->unk24; i++) {
                btl_spawnPopups(g_btlfxEnv->unkC->unk4[i]);
            }
        } else {
            btl_spawnPopups(btl_getUnit(arg0));
        }
    }
}

void btl_setVsyncMode(s32 arg0) {
    g_gameState->vsyncMode = arg0;
}

void func_800E8F50(Unk800E8F50 *arg0, s32 arg1) {
    s32 i;
    s32 n;
    Unk800E8F50 *h;
    Unk800E8F50Entry *e;
    void *p;
    h = arg0;
    if (arg0->unk0 != 0x6F73) {
        SystemError(0x53, 0x190);
    }
    p = (u8 *)arg0 + h->unk4;
    if (h->unk2 != 0) {
        n = (arg0->unk4 - 8) >> 3;
        e = h->unk8;
        for (i = 0; i < n; i++) {
            e[i].unk0 |= (arg1 & 3) << 5;
        }
        Hi_RegisterTexListModel(p, (BtlTexInfo *)e, (s16 *)((u8 *)h + h->unk6), n);
    } else {
        Hi_RegisterGouEffModel(p);
    }
}

void func_800E9000(Unk800E9000 *arg0, s32 *src, s32 n, Unk80100DC054 *e) {
    Unk800E9000 *h = arg0;
    s32 *s;
    s32 i;
    s32 k;
    s32 k2;
    Unk800E9000Ent *a;
    Unk800E9000Ent *b;

    if (h->unk0 != 0x6F73) {
        SystemError(0x53, 0x190);
    }
    k = (h->unk4 - 8) >> 3;
    e->unk2 = n;
    e->unk40 = NULL;
    e->unk4 = k;
    e->unk3C = (u8 *)h + h->unk4;
    s = src;
    for (i = 0; i < n; i++, s++) {
        e->unk180[i] = *s;
    }
    b = (Unk800E9000Ent *)((u8 *)h + h->unk6);
    a = h->unk8;
    for (i = 0; i < k; i++) {
        e->unk18[i] = a[i].unk0;
        e->unk24[i] = a[i].unk2;
        e->unk30[i] = b[i].unk2;
    }
    btlfx_initSummonModels(&e->unk90, 1);
    g_btlfxLoader.unk54 = e;
    g_btlfxLoader.unk60 = Hi_RegisterSummonModel(e, NULL);
}

BtlUnit *func_800E9124(void) {
    if (g_btlfxEnv->unkC->unk27 == 0) {
        return btl_getUnit(0);
    }
    return btl_getUnit(8);
}

void func_800E9168(void) {
    s32 r;
    BtlUnit *a;
    BtlUnit *b;
    s32 f;
    u32 v;
    if (g_btlfxEnv->unk0->unk1C & 8) {
        r = 1;
    } else {
        a = btl_getUnit(0x10);
        b = btl_getUnit(0);
        f = (g_gameState->unk24 >> 4) & 1;
        if (f == 1 || (u8)btl_hasStatus(a, 0x10000000) || (u8)btl_hasStatus(a, 0x04000000)) {
            r = 0;
        } else {
            if (b->unk68 & 1) {
                v = g_gameState->unk1C->unk8E4.battle->unk132C.unk0 >> 11;
            } else {
                v = g_gameState->unk1C->unk8E4.battle->unk132C.unk0 >> 12;
            }
            v &= 1;
            r = v == 0;
        }
    }
    D_800F8D74 = r;
}

void func_800E9290(s32 arg0) {
    u32 hi;
    s32 msg;
    BtlUnit *u;
    BtlUnit *t;

    if (arg0 == 0xFFFF) {
        return;
    }
    hi = arg0 & 0xF0;
    if (D_800F8D74 == 0 || hi == 0xF0 || (g_btlfxEnv->unkC->unk40 & 2)) {
        return;
    }
    u = btl_getUnit(0x10);
    if ((u8)btl_hasStatus(u, 0x10000000) != 0 || (u8)btl_hasStatus(u, 0x04000000) != 0) {
        return;
    }
    t = func_800E9124();
    if (g_btlfxEnv->unkC->unk40 & 4) {
        msg = func_800E454C(D_800F8D78[4]);
    } else if (u->unk68 & 1) {
        if ((u8)btl_hasStatus(u, 0x4000) == 0) {
            if ((hi >> 4) == 1 || (hi >> 4) == 7) {
                if (((u->unk68 >> 1) & 0xF) == 2) {
                    msg = func_800E454C(D_800F8D78[2]);
                } else {
                    msg = func_800E454C(D_800F8D78[3]);
                }
            } else if (t->unk68 & 1) {
                msg = func_800CA2B0(3);
            } else {
                msg = func_800CA2B0(4);
            }
        } else if (t->unk68 & 1) {
            msg = func_800E454C(D_800F8D78[0]);
        } else {
            msg = func_800E454C(D_800F8D78[1]);
        }
    } else {
        msg = g_gameState->unk1C->unk8E4.battle->unk488[(u->unk68 >> 1) & 0xF].unk0->unk2C;
    }
    if (msg != 0) {
        btlcam_play(msg);
    }
}

void func_800E94C4(void) {
    u8 v;
    if (D_800F8D74 != 0 && g_btlfxEnv->unkC->unk27 != 0) {
        if (btl_getUnit(8)->unk68 & 1) {
            v = D_800F8D80;
        } else {
            v = D_800F8D81;
        }
        btlcam_play(func_800E454C(v));
    }
}

void func_800E954C(void) {
    s32 i;
    D_80104E30.unk0 = 0;
    for (i = 5; i >= 0; i--) {
        D_80104E30.unk4[i].unk0 = 0;
    }
}

void func_800E9578(void) {
    s32 n = 0;
    s32 i;
    Unk80104E30 *p = &D_80104E30;
    Unk80104E30Entry *e = p->unk4;
    for (i = 0; i < 6; i++, e++) {
        if (e->unk0 != 0) {
            if (func_8005553C(e->unk0) == 0) {
                e->unk0 = 0;
            } else {
                n++;
                func_80055850(0, e->unk0, btl_soundPan(e->unk4));
                e->unk2++;
            }
        }
    }
    p->unk0 = n;
}

s32 func_800E9638(s32 arg0) {
    Unk80104E30Entry *best = NULL;
    s32 bestbit = 0;
    s32 max = -1;
    s32 bit = 1;
    Unk80104E30 *p = &D_80104E30;
    Unk80104E30Entry *e = p->unk4;
    s32 i;
    for (i = 0; i < 6; i++, e++, bit <<= 1) {
        if (e->unk0 == 0) {
            break;
        }
        if (max < e->unk2) {
            max = e->unk2;
            best = e;
            bestbit = bit;
        }
    }
    if (i >= 6) {
        bit = bestbit;
        e = best;
    }
    if (func_8005553C(bit) != 0) {
        func_8005546C(0, bit);
    }
    if (e != NULL) {
        e->unk0 = bit;
        e->unk4 = arg0;
        e->unk2 = 0;
    }
    return bit;
}

void func_800E970C(void) {
    func_8005546C(0, 0x3F);
}

Unk80104E34 *func_800E9730(s32 arg0) {
    Unk80104E34 *p;
    s32 i;
    if (arg0 != 0) {
        p = D_80104E34;
        for (i = 0; i < 6; i++, p++) {
            if (p->unk0 == arg0) {
                return p;
            }
        }
    }
    return NULL;
}

s32 btl_soundPan(s32 arg0) {
    SVECTOR pos;
    SVECTOR *v;
    DVECTOR sxy;
    s32 x;
    s32 sign;

    if (arg0 == -1) {
        return 0x80;
    }
    if (arg0 & 0x90000000) {
        v = (SVECTOR *)arg0;
    } else {
        v = &pos;
        btlfx_getUnitPos(arg0, (s16 *)v);
    }
    gte_SetRotMatrix(g_btlfxEnv->unk14);
    gte_SetTransMatrix(g_btlfxEnv->unk14);
    gte_ldv0(v);
    gte_rtps();
    gte_stsxy(&sxy);
    x = sxy.vx - 160;
    sign = (x < 0) ? -1 : 1;
    x = (x >= 0) ? x : -x;
    if (x > 170) {
        x = 170;
    }
    return sign * (x * 63 / 170) + 0x80;
}

void func_800E9884(s32 arg0) {
    Unk80104E34 *p = func_800E9730(arg0);
    if (p != NULL) {
        func_8005546C(0, arg0);
        p->unk0 = 0;
    }
}

void func_800E98C8(s32 arg0, s32 arg1) {
    Unk80104E34 *p = func_800E9730(arg0);
    if (p != NULL) {
        p->unk4 = arg1;
    }
}

void func_800E98F8(s32 arg0) {
    Unk80104E34 *p = D_80104E34;
    s32 i;
    for (i = 0; i < 6; i++, p++) {
        if (p->unk0 != 0) {
            p->unk4 = arg0;
        }
    }
}

s32 func_800E9930(s32 arg0, s32 arg1, s32 arg2) {
    s32 v = func_800E9638(arg2);
    if (v != 0) {
        func_800553EC(btlfx_getFileData(arg1, arg0), v, btl_soundPan(arg2), 0x7F);
    }
    return v;
}

void func_800E99B0(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    func_800E9884(arg3);
    func_800E9930(arg0, arg1, arg2);
}

s32 func_800E9A00(s32 arg0, s32 arg1) {
    s32 v = func_800E9638(arg1);
    if (v != 0) {
        func_800553EC(func_800E454C(arg0), v, btl_soundPan(arg1), 0x7F);
    }
    return v;
}

void btlui_vsync(void) {
    GameData *g;
    D_80104E68.unk1 = 1;
    func_800EA5C4();
    func_800EA548();
    btlui_readPad();
    if (D_80104E68.unkF != 0) {
        func_800E9BBC();
    }
    g = g_gameState->unk1C;
    if (!(g->unk0 & 0x100) && !(g->unk8E4.battle->unk0 & 0x20)) {
        if (D_80104E68.unkC == 0 && (u8)func_800E9FF4()) {
            func_800EB578(0xBA);
            btlui_beginFrame();
            func_800EA4D0();
            func_800EA378();
            func_800EA400();
            btlui_drawWindows();
            func_800EA498();
            btlui_submitOt();
            func_800EB578(0xBB);
        }
    }
    D_80104E68.unk1C++;
    g_drawBufs.unk8++;
    D_80104E68.unk18++;
    D_80104E68.unkF = 0;
    D_80104E68.unk1 = 0;
}

void func_800E9BBC(void) {
    func_800EA5C4();
    g_drawBufs.unk0 = g_gameState->unk1C->unk8 ^ 1;
    if (D_80104E68.unk0 == 0) {
        func_800EA550();
    }
    if (!(g_gameState->unk1C->unk0 & 0x100)) {
        if (!(g_gameState->unk1C->unk8E4.battle->unk0 & 0x20)) {
            if (D_80104E68.unk0 != 0) {
                D_80104E68.unk8 = (g_padHeld >> 19) & 1;
                D_80104E68.unkD = D_80104E68.unk2;
                func_800EB238();
                func_800EBC30();
                func_800F3E60();
                func_800F28F0();
                func_800F1484();
                win_reapClosed();
            }
        }
        if (!(g_gameState->unk1C->unk0 & 0x100) && !(g_gameState->unk1C->unk8E4.battle->unk0 & 0x20) && D_80104E68.unkC != 0) {
            D_80104E68.unkC--;
        }
    }
}

void btlui_init(void) {
    func_800EB05C();
    func_800F3C08();
    func_800F4884();
    D_80104E68.unk6 = 1;
    D_80104E68.unk15 = 0;
    VSyncCallback(btlui_vsync);
}

void func_800E9D84(void) {
    void *p = func_8001C7FC(4, 0x14, 0);
    if (p != NULL) {
        func_8002C158(p);
    }
}

void func_800E9DBC(void) {
    void *p = func_8001C7FC(4, 0x1E, 0);
    if (p != NULL) {
        func_80028B20(p);
        func_80033D1C(0);
    }
}

void func_800E9DFC(void) {
    D_80104E68.unk1C = 0;
}

void func_800E9E10(void) {
    memset(&D_80104E68, 0, 0x11C);
    func_800F2A0C();
    func_800F48DC();
    func_800F48E4();
    func_800F4630();
    func_800EBB34();
    func_800EC008();
    text_setBank(g_battleText);
    VSyncCallback(NULL);
}

void func_800E9E80(void) {
    func_800E9F3C(0);
    VSyncCallback(NULL);
    func_800F4700();
    func_800F4D20();
    D_80104E68.unkE = 0;
}

void func_800E9EC4(void) {
    DrawSync(0);
    func_800DA898();
    if (D_80104E68.unk1C < (u32)(g_gameState->vsyncMode - 1)) {
        VSync(g_gameState->vsyncMode - 1);
    }
    func_800F4384();
    D_80104E68.unkF = 1;
    VSync(0);
}

void func_800E9F3C(u8 arg0) {
    if (arg0) {
        D_80104E68.unk12 = 1;
    } else {
        D_80104E68.unk0 = 0;
    }
}

void func_800E9F68(u8 arg0) {
    if (D_80104E68.unk6 != 0) {
        if (D_80104E68.unk2 != arg0) {
            D_80104E68.unk2 = arg0;
            if (arg0 == 0) {
                func_800EB410();
                func_800EBD58();
            }
            func_800EB2FC();
        }
    }
}

BtlUnit *btlui_getActiveUnit(void) {
    BtlUnit *u = NULL;
    if (g_btlUi->menuState != 0) {
        u = g_btlUi->cmdUnit;
    }
    return u;
}

s32 func_800E9FF4(void) {
    return D_80104E68.unk0 != 0;
}

s32 func_800EA004(void) {
    return g_btlUi->menuState == 3;
}

u8 func_800EA024(void) {
    return D_80104E68.unk4;
}

u8 btl_isAtbRunning(void) {
    s32 f;
    if (D_80104E68.unk2 == 0) {
        return 0;
    }
    f = (g_gameState->unk24 >> 3) & 1;
    if (f != 1) {
        return 1;
    }
    return g_btlUi->menuState < 2;
}

s32 func_800EA094(void) {
    return func_800F1360();
}

u8 *btl_getAbilityName(s32 arg0) {
    if (arg0 >= 0xC0) {
        return NULL;
    }
    return g_gameState->unk1C->unk8E4.battle->abilities[arg0].nameOfs + D_800F909C;
}

void func_800EA0F8(s32 arg0) {
    func_800EC170(arg0);
    func_800EBEFC(arg0);
}

void func_800EA124(s32 arg0) {
    func_800EC170(arg0);
}

void func_800EA144(s32 arg0) {
    func_800EC14C(arg0);
}

void func_800EA164(s32 arg0) {
    func_800EBEFC(arg0);
    if (btlui_getItemReserved(arg0) != 0 && item_getCount(arg0) == 0) {
        func_800EC170(arg0);
    }
}

void func_800EA1B0(s32 arg0) {
    func_800EC070(arg0);
}

void func_800EA1D0(s8 arg0) {
    if (D_80104E68.unk9 == 0) {
        D_80104E68.unk9 = 1;
        D_80104E68.unk14 = arg0;
    }
}

void func_800EA1F8(u32 *arg0, DRAWENV *arg1) {
    D_80104E68.unk10 = 1;
    DrawOTagEnv(arg0, arg1);
    D_80104E68.unk10 = 0;
}

void func_800EA230(u32 *arg0) {
    D_80104E68.unk10 = 1;
    DrawOTag(arg0);
    D_80104E68.unk10 = 0;
}

u8 func_800EA268(void) {
    return D_80104E68.unk15;
}

u8 func_800EA27C(void) {
    return D_80104E68.unkE;
}

void func_800EA290(void) {
    if (D_80104E68.unk6 == 0) {
        GameData *gd = g_gameState->unk1C;
        if (!(gd->unk8E4.battle->unk0 & 0x100)) {
            PutDispEnv(&g_gameState->unkC[gd->unk8]);
        }
    }
}

u8 func_800EA30C(void) {
    return D_80104E68.unk6 ? D_80104E68.unkB : 0;
}

void func_800EA338(s8 arg0) {
    D_80104E68.unkB = arg0;
}

void func_800EA34C(void) {
    u8 v = g_gameState->unk1C->unk8;
    D_80104E68.unkC = 2;
    g_drawBufs.unk0 = v;
}

void func_800EA378(void) {
    GameState *g = g_gameState;
    s32 r = 0;
    volatile Unk80104E68 *p = &D_80104E68;
    s32 f;
    if (((g->unk1C->unk8E4.battle->unk11B8 >> 1) ^ 1) & 1) {
        r = 1;
    } else {
        do {
            f = (g->unk24 >> 19) & 1;
            if (f == 1) {
                r = 1;
            }
        } while (0);
    }
    p->unk3 = r;
    D_80104E68.unk4 = func_800F13CC() >= 2;
}

void func_800EA400(void) {
    if (D_80104E68.unk2 != 0) {
        func_800EBB78();
        btlui_menuInput();
    }
}

void btlui_drawWindows(void) {
    if (D_80104E68.unk8 == 0) {
        btlui_drawHelpWindow();
        btlui_drawPartyPanel();
        win_drawAll();
        func_800EA518();
    }
    btlui_drawTargetCursors();
}

void func_800EA498(void) {
    btlui_updateMenu();
    func_800F3D0C();
    win_updateAll();
    func_800EBBB4();
}

void func_800EA4D0(void) {
    if (D_80104E68.unkF != 0) {
        func_800F386C();
        func_800ECDB8();
        func_800F2374();
    }
}

void func_800EA518(void) {
    func_800F3A8C();
    func_800ECF0C();
    func_800F2560();
}

void func_800EA548(void) {
}

void func_800EA550(void) {
    if (D_80104E68.unk12 != 0) {
        D_80104E68.unk12 = 0;
        D_80104E68.unk0 = 1;
        if (D_80104E68.unk6 != 0 && D_80104E68.unk2 != 1) {
            D_80104E68.unk2 = 1;
            func_800EB2FC();
        }
        func_800F2AAC();
        func_800EC048();
    }
}

void func_800EA5C4(void) {
    if (D_80104E68.unkF != 0 && D_80104E68.unk6 != 0) {
        GameData *gd = g_gameState->unk1C;
        if (!(gd->unk8E4.battle->unk0 & 0x100)) {
            PutDispEnv(&g_gameState->unkC[gd->unk8]);
        }
    }
}

void func_800EA650(void) {
    BtlUnit *u = g_gameState->unk1C->unk8E4.battle->unk6F8;
    Unk80104F88Entry *e = g_btlUi->unk0;
    s8 n = 0;
    s8 f;
    while (u != NULL && (u->unk68 & 1)) {
        u->unkB4 = e;
        u->unkCF = 0;
        f = (u8)u->unk68 >> 5;
        e->unk10 = u;
        e->unk18 = f;
        e->unk14 = &g_gameState->unk1C->characters[(u->unk68 >> 1) & 0xF];
        e->unk19 = func_80034A30(e->unk14);
        e->unk1A = func_800339F4(e->unk14->unk34.b.id, (e->unk14->unkA >> 4) & 1);
        if ((g_gameState->unk24 >> 5) & 1) {
            f = ((s32)(g_gameState->unk24 >> 20) >> e->unk1A) & 1;
        } else {
            f = 0;
        }
        e->unk1B = f;
        func_800EB600(e);
        func_800EB54C(&e->unk49C);
        n++;
        e++;
        u = u->unk0;
    }
    g_btlUi->partyCount = n;
}

void func_800EA7B8(Unk80104F88Entry *arg0) {
    s32 k = arg0->unk14->unk34.b.row;
    u8 *s = D_800FDDF8[k];
    u8 *t = D_800FDDD0[k];
    Unk800FD56C *a;
    Unk800FD56C *b;
    s32 i;
    s32 j;
    u8 x;
    u8 y;
    if ((func_80034A30(arg0->unk14) & 0xFF) && (s[2] == 1 || s[2] == 2)) {
        i = 0;
        j = s[2] - 1;
        a = &D_800FD56C[t[j]];
        b = &D_800FD56C[s[j]];
        for (; i < a->unk7; i++) {
            x = D_800FDD10[a->unk8 + i];
            y = D_800FDD10[b->unk8 + i];
            if (x != y) {
                arg0->unk1C[y] = arg0->unk1C[x];
                arg0->unk31C[y] = arg0->unk31C[x];
                arg0->unk3DC[y] = arg0->unk3DC[x];
            }
        }
    }
}

/* character 3: also list the abilities of character record 1 and its equipment */
void func_800EA908(Unk80104F88Entry *arg0) {
    Unk800FD6E0 *m = &D_800FD6E0;
    Character *c = &g_gameState->unk1C->characters[1];
    Unk8006CB88 *ab = D_8006CB88;
    s32 i;
    s32 j;
    s32 k;
    s32 slot;
    s32 v;
    s32 id;
    s32 e;
    Unk80069018 *it;

    if (arg0->unk14->unk34.b.id == 3) {
        for (i = 0; i < m->unk7; i++) {
            slot = D_800FDD10[m->unk8 + i];
            v = func_80034430(1, D_800F8D84[i]);
            if (v >= 0) {
                arg0->unk1C[slot] = &c->unk58[v];
                arg0->unk31C[slot] = ab[v].unk1;
            }
        }
        for (i = 0; i < 5; i++) {
            e = c->equip[i];
            if (e != 0xFF) {
                it = &D_80069018[e];
                for (j = 0; j < 3; j++) {
                    id = it->unkC[j];
                    if (id != 0 && id < 0xC0) {
                        for (k = 0; k < m->unk7; k++) {
                            if (id == D_800F8D84[k]) {
                                slot = D_800FDD10[m->unk8 + k];
                                arg0->unk3DC[slot] = 1;
                            }
                        }
                    }
                }
            }
        }
    }
}

void btl_updateAtb(void) {
    BtlUnit *u;
    BtlUnit28 *s;
    BtlUnit24 *m;

    u = g_gameState->unk1C->unk8E4.battle->unk6F8;
    if (btl_isAtbRunning() == 0) {
        return;
    }
    for (; u != NULL; u = u->unk0) {
        if (u->unkB3 != 0 || u->unkCF != 0 || u->hp == 0) {
            continue;
        }
        if (!((u->unk68 >> 16) & 1)) {
            continue;
        }
        if ((u->unk68 & 1) == g_btlUi->unk12B6) {
            continue;
        }
        s = (BtlUnit28 *)&u->hp;
        m = u->max;
        s->unk4 += s->unk6;
        if (s->unk4 < m->maxAtb) {
            continue;
        }
        s->unk4 = m->maxAtb;
        if ((u8)btl_hasStatus(u, 0x02020002) != 0) {
            continue;
        }
        if (u->unk68 & 1) {
            if ((u8)btl_hasStatus(u, 0xC00) != 0) {
                u->unkB3 = 1;
                btl_autoAttack(u);
            } else {
                func_800EB358(u);
            }
        } else if (evt_battleTrigger(0x2F, u->unkB0, 0, 0) != 0) {
            u->unkB3 = 1;
        }
    }
}

void func_800EAC88(void) {
    BtlUnit *u;
    s32 slot;

    u = g_gameState->unk1C->unk8E4.battle->unk6F8;
    g_btlUi->unk12B7 = 0;
    g_btlUi->unk12B8.b[0] = 0;
    g_btlUi->unk12B8.b[1] = 0;
    for (; u != NULL; u = u->unk0) {
        if (!(u->unk68 & 1)) {
            break;
        }
        slot = (u->unk68 >> 1) & 0xF;
        if (slot == 1) {
            g_btlUi->unk12B7 = 1;
            if (u->hp == 0 || (u8)btl_hasStatus(u, 0x13021D0B) != 0) {
                g_btlUi->unk12B8.b[0] = 1;
            }
        } else if (slot == 3 && (u8)btl_hasStatus(u, 0x10000000) != 0) {
            g_btlUi->unk12B8.b[1] = 1;
        }
    }
}

BtlUnit *func_800EAD94(void) {
    BtlUnit *u;
    u8 ok;
    for (;;) {
        if (g_btlUi->unk12C4 == 0 || g_btlUi->menuState != 0) {
            return NULL;
        }
        u = g_btlUi->unk12C8[g_btlUi->unk12C5];
        g_btlUi->unk12C4--;
        g_btlUi->unk12C5 = (g_btlUi->unk12C5 + 1) & 3;
        if (u == NULL || u->hp == 0 || (btl_hasStatus(u, 0x42021D03) & 0xFF) || !((g_btlRewards.unkA >> ((u8)u->unk68 >> 5)) & 1)) {
            ok = 0;
        } else {
            ok = 1;
        }
        if (ok) {
            return u;
        }
        u->unkCF = 0;
    }
}

void func_800EAE8C(void) {
    BtlUnit *list[4];
    u8 seen[4];
    s32 n = 0;
    s32 head = g_btlUi->unk12C5;
    s32 cnt = g_btlUi->unk12C4;
    BtlUnit *u;
    s32 i;
    Unk80104F88 *q;
    BtlUnit *v;
    Unk80104F88 *r;
    memset(seen, 0, 4);
    while (cnt != 0) {
        u = g_btlUi->unk12C8[head];
        if (BtlUnitCanAct(u) && u->unkB3 != 1) {
            list[n++] = u;
        } else {
            u->unkCF = 0;
        }
        cnt--;
        head = (head + 1) & 3;
    }
    q = g_btlUi;
    if (n != q->unk12C4) {
        q->unk12C6 = 0;
        q->unk12C5 = 0;
        q->unk12C4 = 0;
        for (i = 0; i < n; i++) {
            if (seen[(u8)list[i]->unk68 >> 5] == 0) {
                seen[(u8)list[i]->unk68 >> 5] = 1;
                v = list[i];
                r = g_btlUi;
                v->unkCF = 1;
                g_btlUi->unk12C8[(r->unk12C5 + r->unk12C4) & 3] = v;
                r->unk12C4++;
            }
        }
    }
}

void func_800EB05C(void) {
    s32 i;
    Unk80104F88 *p = func_800F2A60(0x12D8);
    g_btlUi = p;
    p->menuState = 0;
    p->cmdUnit = NULL;
    g_btlUi->unk12C0 = 0;
    g_btlUi->unk12BC = g_gameState->unk1C->unk8E4.battle->unk132C.unk0 & 3;
    g_btlUi->unk12B6 = func_800EB9E0();
    func_800EB5AC();
    func_800EA650();
    func_800EB2E0();
    for (i = 0; i < 4; i++) {
        g_btlUi->unk12A4[i] = win_get(i);
    }
}

void btlui_menuInput(void) {
    u8 st;
    func_800EAE8C();
    st = g_btlUi->menuState;
    if (st == 4) {
        return;
    }
    if (st == 0) {
        BtlUnit *u = func_800EAD94();
        g_btlUi->cmdUnit = u;
        if (u != NULL) {
            func_800EC4EC();
        }
    } else if (!(func_800EB7B0(g_btlUi->cmdUnit) & 0xFF)) {
        func_800EB410();
    } else {
        D_800F8D94[g_btlUi->menuState]();
    }
}

void btlui_updateMenu(void) {
    void (*fn)(void);
    func_800EB81C();
    func_800EB924();
    btl_updateAtb();
    func_800EAC88();
    fn = D_800F8DA8[g_btlUi->menuState];
    if (fn != NULL) {
        fn();
    }
}

void func_800EB238(void) {
    if (g_btlUi->unk12C0 < 75) {
        g_btlUi->unk12B6 = func_800EB9E0();
    } else {
        g_btlUi->unk12B6 = 2;
    }
    g_btlUi->unk12C0++;
}

s32 func_800EB2A4(void) {
    return g_btlUi->menuState;
}

s32 btlui_setMenuState(s32 arg0) {
    s32 old = g_btlUi->menuState;
    if (arg0 != 5) {
        g_btlUi->menuState = arg0;
    }
    return old;
}

void func_800EB2E0(void) {
    Unk80104F88 *p = g_btlUi;
    p->unk12C6 = 0;
    p->unk12C5 = 0;
    p->unk12C4 = 0;
}

void func_800EB2FC(void) {
    Unk80104F88 *p;
    s32 n = g_btlUi->unk12C4;
    s32 i = g_btlUi->unk12C5;
    while (n != 0) {
        n--;
        g_btlUi->unk12C8[i]->unkCF = 0;
        i = (i + 1) & 3;
    }
    p = g_btlUi;
    p->unk12C6 = 0;
    p->unk12C5 = 0;
    p->unk12C4 = 0;
}

void func_800EB358(BtlUnit *arg0) {
    Unk80104F88 *p = g_btlUi;
    arg0->unkCF = 1;
    g_btlUi->unk12C8[(p->unk12C5 + p->unk12C4) & 3] = arg0;
    p->unk12C4++;
}

void func_800EB3A0(void) {
    BtlUnit *u = g_gameState->unk1C->unk8E4.battle->unk6F8;
    func_800EB2E0();
    while (u != NULL) {
        if (u->unk68 & 1) {
            u->unkCF = 0;
        }
        u = u->unk0;
    }
}

void func_800EB410(void) {
    BtlUnit *u = g_btlUi->cmdUnit;
    if (u != NULL) {
        u->unkCF = 0;
    }
    if (g_btlUi->menuState != 0) {
        g_btlUi->menuState = 4;
        func_800EB46C();
    }
    func_800F142C();
}

void func_800EB46C(void) {
    s32 i;
    if (g_btlUi->menuState != 0) {
        if (!func_800F29A4()) {
            win_setOnClosed(g_btlUi->unk12A4[3], func_800EBA10);
            win_closeAll();
        } else {
            win_closeAll();
            func_800F2890(0);
        }
        for (i = 0; i < 2; i++) {
            func_8002C740(i, 2, 0);
            func_8002C740(i, 3, 0);
        }
    }
}

void func_800EB518(Unk800EB518 *arg0, s32 arg1, s32 arg2, s32 arg3) {
    if (arg1 == 0) {
        arg0->unk0 = arg2;
        return;
    }
    arg0->unk8[arg0->unk0] = arg2;
    arg0->unk4[arg0->unk0] = arg3;
}

void func_800EB54C(Unk800EB518 *arg0) {
    s32 i;
    arg0->unk0 = 0;
    for (i = 0; i < 4; i++) {
        arg0->unk8[i] = 0;
        arg0->unk4[i] = 0;
    }
}

void func_800EB578(s32 arg0) {
}

u8 *func_800EB580(void) {
    return &g_btlUi->unk12C4;
}

BtlUnit *func_800EB590(s32 arg0) {
    return g_btlUi->unk12C8[arg0];
}

void func_800EB5AC(void) {
    s32 i, j;
    for (i = 0; i < 4; i++) {
        Unk80104F88Entry *e = &g_btlUi->unk0[i];
        for (j = 0; j < 0xC0; j++) {
            e->unk1C[j] = 0;
            e->unk31C[j] = 0;
            e->unk3DC[j] = 0;
        }
    }
}

void func_800EB600(Unk80104F88Entry *arg0) {
    func_800EB63C(arg0);
    func_800EB6FC(arg0);
    func_800EA7B8(arg0);
    func_800EA908(arg0);
}

void func_800EB63C(Unk80104F88Entry *arg0) {
    Character *c;
    s32 i;
    AbilityEntry *row;
    s32 v;
    c = arg0->unk14;
    if (arg0->unk19 != 0) {
        i = 0;
        row = D_8006CB28[c->unk34.b.row];
        for (; i < 0xC0; i++) {
            v = func_80034430(c->unk34.b.id, i);
            if (v >= 0) {
                arg0->unk1C[i] = &c->unk58[v];
                arg0->unk31C[i] = row[v].unk1;
            }
        }
    }
}

void func_800EB6FC(Unk80104F88Entry *arg0) {
    Character *c = arg0->unk14;
    s32 i;
    s32 j;
    memset((s32)arg0->unk3DC, 0, 0xC0);
    for (i = 0; i < 5; i++) {
        u8 item = c->equip[i];
        if (item != 0xFF) {
            Unk80069018 *e = &D_80069018[item];
            for (j = 0; j < 3; j++) {
                s32 a = e->unkC[j];
                if (a != 0 && a < 0xC0) {
                    arg0->unk3DC[a] = 1;
                }
            }
        }
    }
}

s32 func_800EB7B0(BtlUnit *arg0) {
    if (arg0 == NULL) {
        return 0;
    }
    if (arg0->hp != 0 && !(btl_hasStatus(arg0, 0x42021D03) & 0xFF)) {
        return (D_800FDF3A >> ((u8)arg0->unk68 >> 5)) & 1;
    }
    return 0;
}

void func_800EB81C(void) {
    BtlUnit *u;
    u8 c;
    for (u = g_gameState->unk1C->unk8E4.battle->unk6F8; u != 0; u = u->unk0) {
        c = u->unkB2;
        if (c >= 2) {
            if (c == 2) {
                u->unkB2 = 0;
                u->unk68 &= ~0x10000;
                u->unk68 &= ~0x10000000;
                if (g_btlUi->menuState == 3) {
                    func_800F11A4(u);
                }
            } else if (c == 3) {
                u->unkB2 = 1;
                u->unk68 |= 0x10000;
                u->unk68 |= 0x10000000;
                if (g_btlUi->menuState == 3) {
                    func_800F1150(u);
                }
            }
        }
    }
}

void func_800EB924(void) {
    Unk80104E68 *p = (Unk80104E68 *)&D_80104E68;
    if (p->unk2 != 0) {
        if ((g_padHeld & (BTN_L1 | BTN_R1)) == 0x300000) {
            g_gameState->unk1C->unk8E4.battle->unkD83 = 1;
            if (p->unk13++ >= 0x3D) {
                p->unk13 = 0;
                btl_tryEscape(1);
            } else {
                btl_tryEscape(0);
            }
        } else {
            g_gameState->unk1C->unk8E4.battle->unkD83 = 0;
        }
    }
}

s32 func_800EB9E0(void) {
    s32 v = g_btlUi->unk12BC;
    if (v != 0) {
        return (v != 1) * 2;
    }
    return 1;
}

void func_800EBA10(Window *arg0) {
    g_btlUi->menuState = 0;
}

void btlui_drawHelpWindow(void) {
    RECT r;
    s32 m;
    if (D_80104F90.unk0 != 0 && D_80104E75 != 0) {
        btlui_getHelpRect(&D_800F8DBC, &r);
        win_setClip(NULL, 2, 4);
        if (D_80104F90.unk1 == 0) {
            ui_drawSprite(r.x + 8, r.y, 2, 0xB7, 0);
        }
        btlui_updateHelpText();
        if (D_80104F90.unk4 != 0) {
            win_setClip(NULL, 2, 3);
            m = func_8002BE88();
            text_drawBlock(r.x + 6, r.y + 10, 2, D_80104F90.unk4, 8);
            func_8002BE98(m);
            win_setClip(&r, 2, 3);
        }
        win_drawHelpFrame(&r, 2, 4, 0);
        win_setClip(&r, 2, 4);
    }
}

void func_800EBB34(void) {
    D_80104F90.unk0 = 0;
    D_80104F90.unk1 = 0;
    D_80104F90.unk2 = 0;
    D_80104F90.unk3 = 0;
    D_80104F90.unk4 = 0;
    D_80104F90.unkC = 0;
    D_80104FA0 = func_800F2A60(0x100);
}

void func_800EBB78(void) {
    if (g_padRepeat & 0x08000000) {
        func_800EBCC4(D_80104F90.unk0 == 0);
    }
}

void func_800EBBB4(void) {
    if (D_80104F90.unk0 != 0 && D_80104E75 != 0) {
        D_80104F90.unkC++;
        if (D_80104F90.unk1 != 0 && D_80104F90.unkC >= 7) {
            D_80104F90.unk1 = 0;
            func_800F3ECC(0);
        }
    }
}

void func_800EBC30(void) {
    if (func_800F3EA4()->unk5C == 0) {
        if (D_80104F90.unk0 != 0) {
            D_80104F90.unk3 = 1;
        }
    }
    if (D_80104F90.unk3 != 0 && D_80104F90.unk0 != 0) {
        D_80104F90.unk0 = 0;
        D_80104F90.unk3 = 0;
        func_800F3ECC(1);
    }
}

u8 func_800EBCB4(void) {
    return D_80104F90.unk0;
}

void func_800EBCC4(u8 arg0) {
    if (arg0) {
        if (D_80104F90.unk0 == 0) {
            snd_playSe(0x2AA);
            D_80104F90.unk0 = 1;
            D_80104F90.unk1 = 1;
            D_80104F90.unkC = 0;
        }
    } else if (D_80104F90.unk0 != 0 && D_80104F90.unk3 == 0) {
        snd_playSe(0x65);
        D_80104F90.unk3 = 1;
    }
}

void func_800EBD58(void) {
    if (D_80104F90.unk0 != 0 && D_80104F90.unk3 == 0) {
        D_80104F90.unk3 = 1;
    }
}

void btlui_updateHelpText(void) {
    s32 (*fn)(void);
    D_80104F90.unk4 = 0;
    if (g_gameState->unk1C->unk8E4.battle->unkD83 != 0) {
        D_80104F90.unk4 = (s32)text_get(4);
    } else {
        fn = D_800F8DC4[g_btlUi->menuState];
        if (fn != NULL) {
            D_80104F90.unk4 = fn();
        }
    }
}

void btlui_getHelpRect(RECT *arg0, RECT *arg1) {
    Window *w;
    s16 x;
    s32 d;
    *arg1 = *arg0;
    w = func_800F1370();
    if (w != NULL && w->unk1C != 0) {
        if (arg1->x < w->unk8.x + w->unk8.w - 4) {
            x = w->unk8.x + w->unk8.w - 4;
            arg1->x = x;
            arg1->w = 0x137 - x;
        }
    }
    if (D_80104F90.unk1 != 0) {
        d = (7 - D_80104F90.unkC) * 24;
        arg1->x += d;
        arg1->w -= d;
    }
}

void func_800EBEFC(s32 arg0) {
    s32 i;
    if (item_remove(arg0, 1) != 0) {
        if (arg0 >= 0xE0) {
            i = func_800EC44C(arg0);
            if (i != 0xFF && D_80104FA8->unk4[i]->unk1 == 0) {
                D_80104FA8->unk4[i] = 0;
                D_80104FA8->unk0--;
            }
        }
        if (arg0 < 0x58 && (D_8006E0E8[arg0].unk0 & 4)) {
            i = func_800EC49C(arg0);
            if (i != 0xFF && D_80104FA8->unk80[i]->unk1 == 0) {
                D_80104FA8->unk80[i] = 0;
                D_80104FA8->unk2--;
            }
        }
    }
}

void func_800EC008(void) {
    Unk80104FA8 *p = func_800F2A60(0x2E0);
    D_80104FA8 = p;
    memset((s32)p->unk1E0, 0, 0x100);
    func_800EC048();
}

void func_800EC048(void) {
    func_800EC1B0();
    func_800EC258();
}

void func_800EC070(s32 arg0) {
    if (func_80027D84(arg0, 1) == 0) {
        return;
    }
    if (arg0 >= 0xE0 && func_800EC44C(arg0) == 0xFF) {
        func_800EC334(arg0);
    }
    if (arg0 < 0x58 && (D_8006E0E8[arg0].unk0 & 4) && func_800EC49C(arg0) == 0xFF) {
        func_800EC3C0(arg0);
    }
}

UnkPair *btlui_getItemListEntry(s32 arg0) {
    return D_80104FA8->unk4[arg0];
}

UnkPair *func_800EC130(s32 arg0) {
    return D_80104FA8->unk80[arg0];
}

void func_800EC14C(s32 arg0) {
    D_80104FA8->unk1E0[arg0]++;
}

void func_800EC170(s32 arg0) {
    D_80104FA8->unk1E0[arg0]--;
}

s32 btlui_getItemReserved(s32 arg0) {
    return D_80104FA8->unk1E0[arg0];
}

void func_800EC1B0(void) {
    s32 n = 0;
    s32 i;
    UnkPair *p = g_gameState->unk1C->items.slots;
    Unk80104FA8 *f = D_80104FA8;
    for (i = 0; i < 0x100; i++, p++) {
        if (p->unk1 != 0 && p->unk0 != 0xFF && p->unk0 >= 0xE0) {
            f->unk4[n] = p;
            n++;
        }
    }
    for (i = n; i < 31; i++) {
        D_80104FA8->unk4[i] = NULL;
    }
    D_80104FA8->unk0 = n;
}

void func_800EC258(void) {
    s32 n = 0;
    s32 i = 0;
    UnkPair *e;
    Unk80104FA8 *p = D_80104FA8;
    e = g_gameState->unk1C->items.slots;
    for (; i < 0x100; i++, e++) {
        if (e->unk1 != 0 && e->unk0 != 0xFF && e->unk0 < 0x58 && (D_8006E0E8[e->unk0].unk0 & 4)) {
            p->unk80[n] = e;
            n++;
        }
    }
    for (i = n; i < 0x58; i++) {
        D_80104FA8->unk80[i] = NULL;
    }
    D_80104FA8->unk2 = n;
}

void func_800EC334(s32 arg0) {
    s32 i;
    for (i = 0; i < 31; i++) {
        if (D_80104FA8->unk4[i] == NULL) {
            D_80104FA8->unk4[i] = item_findSlot(arg0);
            break;
        }
    }
    D_80104FA8->unk0++;
}

void func_800EC3C0(s32 arg0) {
    s32 i;
    for (i = 0; i < 0x58; i++) {
        if (D_80104FA8->unk80[i] == NULL) {
            D_80104FA8->unk80[i] = item_findSlot(arg0);
            break;
        }
    }
    D_80104FA8->unk2++;
}

s32 func_800EC44C(s32 arg0) {
    s32 i;
    Unk80104FA8 *p = D_80104FA8;
    for (i = 0; i < 31; i++) {
        if (p->unk4[i] != NULL && arg0 == p->unk4[i]->unk0) {
            return i;
        }
    }
    return 0xFF;
}

s32 func_800EC49C(s32 arg0) {
    s32 i;
    Unk80104FA8 *p = D_80104FA8;
    for (i = 0; i < 0x58; i++) {
        if (p->unk80[i] != NULL && arg0 == p->unk80[i]->unk0) {
            return i;
        }
    }
    return 0xFF;
}

void func_800EC4EC(void) {
    Unk80104F88Entry *e;
    Window *w;
    Unk80104FC0 *p;

    p = &g_btlCmdMenu.unk10;
    e = g_btlUi->cmdUnit->unkB4;
    if (!((g_gameState->unk24 >> 2) & 1)) {
        g_btlCmdMenu.cursor = 0;
    } else {
        g_btlCmdMenu.cursor = e->unk49C.unk0;
    }
    g_btlCmdMenu.unk1 = (u8)btl_hasStatus(g_btlUi->cmdUnit, 0x4000) != 0;
    g_btlCmdMenu.charIndex = e->unk14->unk34.b.row;
    g_btlCmdMenu.unit = g_btlUi->cmdUnit;
    p->unk1 = 0;
    p->unk2 = 0;
    btlui_setMenuState(4);
    w = win_open(&D_800F8DDC, 0xF, 2, 3);
    g_btlUi->unk12A4[3] = w;
    win_setDrawContent(w, btlui_drawCmdList);
    win_setDrawOverlay(g_btlUi->unk12A4[3], btlui_drawCmdOverlay);
    win_setOnOpened(g_btlUi->unk12A4[3], func_800EDC3C);
    func_800ECB5C();
    snd_playSe(0x414);
}

void btlui_drawCmdList(Window *arg0) {
    s32 y = arg0->unk2 + 6;
    s32 x = arg0->unk0 + 0xA;
    s32 i;
    u8 *t;
    if (g_btlCmdMenu.unk10.unk1 != 0) {
        func_800EC928(REG_PASSTHROUGH(arg0));
    }
    for (i = 0; i < 4; i++) {
        t = D_800FD7AC + D_800FD56C[g_btlCmdMenu.cmdIds[i]].unk0;
        if (g_btlCmdMenu.unk8[i] == 0) {
            text_draw(x, y, arg0->unk18, t, 2);
        } else {
            text_draw(x, y, arg0->unk18, t, 0);
        }
        do {
            y += 15;
        } while (0);
    }
}

INCLUDE_RODATA("asm/us/ovl_0b/nonmatchings/ovl_0b_2", g_battleText);

/* TRANCE label */
INCLUDE_RODATA("asm/us/ovl_0b/nonmatchings/ovl_0b_2", D_800A857C);

void btlui_drawCmdOverlay(Window *arg0) {
    u8 st = arg0->unk1C;
    RECT *r = &arg0->unk8;
    s32 dy = 10;
    RECT rc;
    if (arg0->unk1C == 3) {
        if (func_800EB2A4() == 1 || (func_800EB2A4() == st && func_800F13BC() == 1 && !(func_800F1360() & 0xFF) && (D_80104E80 & 1))) {
            ui_drawHand(arg0->unk8.x - 0xE, r->y + (g_btlCmdMenu.cursor * 15 + dy), arg0->unk18 - 1);
        }
    }
    if (arg0->unk1C != 3) {
        win_setClip(NULL, arg0->unk18, 4);
    }
    if (g_btlCmdMenu.unit != NULL && !(btl_hasStatus(g_btlCmdMenu.unit, 0x4000) & 0xFF)) {
        ui_drawLabel(arg0->unk0 + 6, r->y - 3, arg0->unk18, D_800703FC.unk30, arg0->unk1E ? 0 : 2);
    } else if (arg0->unk1E != 0) {
        func_800ED3F8(arg0->unk0 + 6, r->y - 3, arg0->unk18, D_800A857C);
    } else {
        ui_drawLabel(arg0->unk0 + 6, r->y - 3, arg0->unk18, (s32)D_800A857C, 2);
    }
    if (arg0->unk1C != 3) {
        rc.x = r->x;
        rc.y = r->y - 3;
        rc.w = r->w;
        rc.h = 8;
        win_setClip(&rc, arg0->unk18, 4);
    }
}

void func_800EC928(Window *arg0) {
    s32 dx2;
    RECT r;
    s32 off = 0;
    s32 dx;
    s16 y;
    s32 x;
    Unk80104FC0 *p = &D_80104FC0;
    Window *w = arg0;

    dx2 = 6;
    if (p->unk2 == 1) {
        off = (5 - p->unk3) * 3;
    } else if (p->unk2 == 2) {
        off = p->unk3 * 5;
    }
    dx = 10;
    if (p->unk1 == 2) {
        dx = dx2;
        x = off + 4;
        r.x = w->unk0 + x;
    } else {
        r.x = w->unk0 - off;
    }
    y = w->unk2 + g_btlCmdMenu.cursor * 15;
    r.w = 0x3E;
    r.h = 0x11;
    r.y = y + 4;
    text_draw(r.x + dx, r.y + 2, arg0->unk18, text_get(D_800F8DE4[D_80104FC0.unk1]), 0);
    func_800ED02C(&r, arg0->unk18);
}

void func_800ECA50(void) {
    Unk800FD56C *e = &D_800FD56C[g_btlCmdMenu.cmdIds[g_btlCmdMenu.cursor]];
    void (*f)(Unk800FD56C *);
    if (g_btlCmdMenu.unk10.unk1 == 0) {
        if (g_btlCmdMenu.unk8[g_btlCmdMenu.cursor] != 0) {
            do {
                snd_playSe(0x67);
                func_800EB518(&g_btlCmdMenu.unit->unkB4->unk49C, 0, g_btlCmdMenu.cursor, 0);
                f = D_800F8DF0[e->unk6];
            } while (0);
        } else {
            snd_playSe(0x66);
            return;
        }
    } else {
        if (g_btlCmdMenu.unk10.unk0 == 0) {
            return;
        }
        snd_playSe(0x67);
        func_800EB518(&g_btlCmdMenu.unit->unkB4->unk49C, 0, g_btlCmdMenu.cursor, 0);
        f = D_800F8E00;
    }
    f(e);
}

void func_800ECB5C(void) {
    s32 k = g_btlCmdMenu.charIndex;
    s8 f;
    g_btlCmdMenu.unk1 = f = (u8)btl_hasStatus(g_btlCmdMenu.unit, 0x4000) != 0;
    g_btlCmdMenu.cmdIds[0] = 1;
    g_btlCmdMenu.unk8[0] = 1;
    g_btlCmdMenu.cmdIds[3] = 0xE;
    g_btlCmdMenu.unk8[3] = 1;
    g_btlCmdMenu.unkC[3] = 0;
    g_btlCmdMenu.unkC[2] = 0;
    g_btlCmdMenu.unkC[1] = 0;
    g_btlCmdMenu.unkC[0] = 0;
    if (f == 0) {
        g_btlCmdMenu.cmdIds[1] = D_800FDDD0[k][0];
        g_btlCmdMenu.cmdIds[2] = D_800FDDD0[k][1];
    } else {
        g_btlCmdMenu.cmdIds[1] = D_800FDDF8[k][0];
        g_btlCmdMenu.cmdIds[2] = D_800FDDF8[k][1];
        if (D_800FDDF8[k][2] != 0xFF) {
            g_btlCmdMenu.unkC[D_800FDDF8[k][2]] = 1;
        }
    }
    g_btlCmdMenu.unk8[1] = 1;
    g_btlCmdMenu.unk8[2] = 1;
    if (g_btlCmdMenu.cmdIds[2] == 0x1F) {
        if (g_btlUi->unk12B7 == 0) {
            g_btlCmdMenu.cmdIds[2] = 0;
            if (g_btlCmdMenu.cursor == 2) {
                g_btlCmdMenu.cursor = 0;
            }
        } else if (g_btlUi->unk12B8.h != 0) {
            g_btlCmdMenu.unk8[2] = 0;
        }
    }
}

void func_800ECCA8(s32 arg0) {
    s32 i;
    s32 u = 0x3C0;
    s32 y = 0x9A;

    for (i = 0; i < 4; i++) {
        setlen(&D_801050D8->unk0[i][arg0], 4);
        setcode(&D_801050D8->unk0[i][arg0], 0x64);
        setRGB0(&D_801050D8->unk0[i][arg0], 0x80, 0x80, 0x80);
        D_801050D8->unk0[i][arg0].x0 = 0;
        D_801050D8->unk0[i][arg0].y0 = y;
        D_801050D8->unk0[i][arg0].u0 = u & 0x3F;
        D_801050D8->unk0[i][arg0].v0 = 0xE0;
        D_801050D8->unk0[i][arg0].w = 0xE;
        D_801050D8->unk0[i][arg0].h = 0x10;
        D_801050D8->unk0[i][arg0].clut = 0;
        u += 0x10;
        y += 0x10;
    }
    setlen(&D_801050D8->unkA0[arg0], 1);
    *D_801050D8->unkA0[arg0].code = 0xE100071F;
}

void func_800ECDB8(void) {
    RECT r = D_800A8584;
    s32 x = 0x3C0;
    s32 y = 0x1E0;
    s32 i = 0;
    r.x += D_801051E8[g_gameState->unk1C->unk8 & 1][0];
    r.y += D_801051E8[g_gameState->unk1C->unk8 & 1][1];
    do {
        SetDrawMove((DR_MOVE *)g_primPtr, &r, x, y);
        x += 0x10;
        i++;
        addPrim(&g_ot[19], g_primPtr);
        g_primPtr += sizeof(DR_MOVE);
        r.y += 0x10;
    } while (i < 4);
}

void func_800ECF0C(void) {
    s32 i;
    if (g_btlUi->unk12A4[3]->unk1C == 2 || g_btlUi->unk12A4[3]->unk1C == 3) {
        for (i = 0; i < 4; i++) {
            addPrim(&g_ot[18], &D_801050D8->unk0[i][g_drawBufs.unk0]);
        }
        addPrim(&g_ot[18], &D_801050D8->unkA0[g_drawBufs.unk0]);
    }
}

void func_800ED02C(RECT *r, s32 otz) {
    u32 *ot;
    u32 clut;
    Unk800689B0 *d2;
    Unk800689B0 *d1;
    RECT tw;
    RECT r2;
    u32 w;

    ot = &g_ot[otz];
    clut = func_80027480(D_80068E78[0].unk2) << 16;
    d2 = &D_80068E78[2];
    setlen(g_primPtr, 4);
    d1 = &D_80068E78[1];
    ((SprtWords *)g_primPtr)->rgbc = 0x64808080;
    ((SprtWords *)g_primPtr)->xy = r->x | (r->y << 16);
    w = D_80068E78[0].unk0;
    w |= D_80068E78[0].unk1 << 8;
    ((SprtWords *)g_primPtr)->uvclut = w | clut;
    ((SprtWords *)g_primPtr)->wh = D_80068E78[0].unk4 | (D_80068E78[0].unk5 << 16);
    addPrim(ot, g_primPtr);
    g_primPtr += sizeof(SprtWords);

    setlen(g_primPtr, 4);
    ((SprtWords *)g_primPtr)->rgbc = 0x64808080;
    ((SprtWords *)g_primPtr)->xy = (r->x + r->w - d2->unk4) | (r->y << 16);
    ((SprtWords *)g_primPtr)->uvclut = D_80068E78[2].unk0 | (d2->unk1 << 8) | clut;
    ((SprtWords *)g_primPtr)->wh = d2->unk4 | (d2->unk5 << 16);
    addPrim(ot, g_primPtr);
    g_primPtr += sizeof(SprtWords);

    tw.y = 0;
    tw.h = 0xFF;
    tw.x = D_80068E78[1].unk0;
    tw.w = d1->unk4;
    SetTexWindow(g_primPtr, &D_800F8E10);
    addPrim(ot, g_primPtr);
    g_primPtr += 0xC;

    setlen(g_primPtr, 4);
    ((SprtWords *)g_primPtr)->rgbc = 0x64808080;
    ((SprtWords *)g_primPtr)->xy = (r->x + D_80068E78[0].unk4) | (r->y << 16);
    ((SprtWords *)g_primPtr)->uvclut = (d1->unk1 << 8) | clut;
    ((SprtWords *)g_primPtr)->wh = (r->w - (D_80068E78[0].unk4 + d2->unk4)) | (d1->unk5 << 16);
    addPrim(ot, g_primPtr);
    g_primPtr += sizeof(SprtWords);

    setlen(g_primPtr, 2);
    ((DrMode2Words *)g_primPtr)->code[0] = get_mode(1, 1, d1->unk6);
    ((DrMode2Words *)g_primPtr)->code[1] = get_tw(&tw);
    addPrim(ot, g_primPtr);
    g_primPtr += sizeof(DrMode2Words);

    r2.x = r->x + 1;
    r2.y = r->y + 1;
    r2.w = r->w - 3;
    r2.h = r->h - 3;
    win_drawFill(&r2, otz, 0, 0, (g_gameState->unk24 >> 18) & 1);
}

void func_800ED3F8(s32 x, s32 y, s32 otz, char *str) {
    s32 w;
    u32 wh;
    u32 xy;
    u32 tp;
    u32 stp;

    w = func_8002AE7C(D_800F8E18);
    wh = w | 0x70000;
    setlen(g_primPtr, 3);
    ((TileWords *)g_primPtr)->rgbc = 0x62000000;
    xy = x | (y << 16);
    ((TileWords *)g_primPtr)->xy = xy;
    ((TileWords *)g_primPtr)->wh = wh;
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += sizeof(TileWords);
    setlen(g_primPtr, 1);
    tp = 0xE1000620;
    stp = 0xE6000001;
    ((DrTpageWords *)g_primPtr)->code = tp;
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += sizeof(DrTpageWords);
    setlen(g_primPtr, 2);
    ((DrMode2Words *)g_primPtr)->code[0] = stp;
    ((DrMode2Words *)g_primPtr)->code[1] = 0;
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += sizeof(DrMode2Words);
    func_800ED7C0(x, y, otz, w, 7, D_800F8E20);
    setlen(g_primPtr, 2);
    ((DrMode2Words *)g_primPtr)->code[0] = 0xE6000002;
    ((DrMode2Words *)g_primPtr)->code[1] = 0;
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += sizeof(DrMode2Words);
    ui_drawLabel(x, y, otz, (s32)str, 1);
    setlen(g_primPtr, 2);
    ((DrMode2Words *)g_primPtr)->code[0] = 0xE6000000;
    ((DrMode2Words *)g_primPtr)->code[1] = 0;
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += sizeof(DrMode2Words);
    setlen(g_primPtr, 3);
    ((TileWords *)g_primPtr)->rgbc = 0x62000000;
    ((TileWords *)g_primPtr)->xy = xy;
    ((TileWords *)g_primPtr)->wh = wh;
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += sizeof(TileWords);
    setlen(g_primPtr, 1);
    ((DrTpageWords *)g_primPtr)->code = tp;
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += sizeof(DrTpageWords);
    setlen(g_primPtr, 2);
    ((DrMode2Words *)g_primPtr)->code[0] = stp;
    ((DrMode2Words *)g_primPtr)->code[1] = 0;
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += sizeof(DrMode2Words);
    ui_drawLabel(x, y, otz, (s32)str, 0);
}

void func_800ED7C0(s32 x, s32 y, s32 ot, s32 w, s32 h, CVECTOR *c) {
    CVECTOR c0;
    CVECTOR c1;
    CVECTOR *a;
    CVECTOR *b;
    CVECTOR *t;
    CVECTOR *u;
    s32 k;
    POLY_G4 *p;

    k = D_80077150 & 0x1F;
    if (k < 16) {
        if (k >= 8) {
            k = 15 - k;
        }
        a = &c[2];
        b = &c[3];
        u = t = &c[0];
    } else {
        k -= 16;
        if (k >= 8) {
            k = 15 - k;
        }
        a = &c[2];
        b = &c[3];
        u = t = &c[1];
    }
    c0.r = a->r + ((u->r - a->r) * k) / 7;
    c0.g = a->g + ((u->g - a->g) * k) / 7;
    c0.b = a->b + ((u->b - a->b) * k) / 7;
    c1.r = b->r + ((t->r - b->r) * k) / 7;
    c1.g = b->g + ((t->g - b->g) * k) / 7;
    c1.b = b->b + ((t->b - b->b) * k) / 7;
    p = (POLY_G4 *)g_primPtr;
    setPolyG4(p);
    setRGB0(p, c0.r, c0.g, c0.b);
    setRGB2(p, c0.r, c0.g, c0.b);
    setRGB1(p, c1.r, c1.g, c1.b);
    setRGB3(p, c1.r, c1.g, c1.b);
    setXYWH(p, x, y, w, h);
    addPrim(&g_ot[ot], p);
    g_primPtr += sizeof(POLY_G4);
}

void func_800EDAD4(void) {
    if (g_padRepeat & 0x01000000) {
        func_800EDDF8();
    } else if (g_padRepeat & BTN_CROSS) {
        func_800ECA50();
    } else if (g_padRepeat & PAD_DOWN) {
        func_800EDD1C();
    } else if (g_padRepeat & PAD_UP) {
        func_800EDCA0();
    } else {
        func_800EDD9C();
    }
}

void func_800EDB68(void) {
    func_800ECB5C();
}

void *btlui_getCmdHelpText(void) {
    s32 i;
    Unk800FD56C *e;
    if (g_btlCmdMenu.unk10.unk1 != 0 && g_btlCmdMenu.unk10.unk0 != 0) {
        i = 7;
        if (g_btlCmdMenu.unk10.unk1 == 1) {
            i = 4;
        }
    } else {
        i = g_btlCmdMenu.cmdIds[g_btlCmdMenu.cursor];
    }
    e = &D_800FD56C[i];
    func_8002BD54(D_80104FA0, e->unk2 + D_800FD8CC);
    text_append(D_80104FA0, text_get(0x19));
    return D_80104FA0;
}

void func_800EDC3C(void) {
    s32 v = g_btlCmdMenu.unit->unkB4->unk1B;
    btlui_setMenuState(1);
    if (v != D_80104E68.unk15) {
        func_8002C584(v);
    }
    D_80104E68.unk15 = v;
}

void func_800EDCA0(void) {
    if (g_btlCmdMenu.unk10.unk1 == 0) {
        do {
            snd_playSe(0x67);
            g_btlCmdMenu.cursor = g_btlCmdMenu.cursor != 0 ? g_btlCmdMenu.cursor - 1 : 3;
        } while (g_btlCmdMenu.cmdIds[g_btlCmdMenu.cursor] == 0);
    }
}

void func_800EDD1C(void) {
    if (g_btlCmdMenu.unk10.unk1 == 0) {
        do {
            snd_playSe(0x67);
            g_btlCmdMenu.cursor = g_btlCmdMenu.cursor < 3 ? g_btlCmdMenu.cursor + 1 : 0;
        } while (g_btlCmdMenu.cmdIds[g_btlCmdMenu.cursor] == 0);
    }
}

void func_800EDD9C(void) {
    if (g_padHeld & PAD_LEFT) {
        func_800EE154();
    } else if (g_padHeld & PAD_RIGHT) {
        func_800EE24C();
    } else {
        func_800EE31C();
    }
}

void func_800EDDF8(void) {
    func_800EB518(&g_btlCmdMenu.unit->unkB4->unk49C, 0, g_btlCmdMenu.cursor, 0);
    btlui_setMenuState(4);
    win_setOnClosed(g_btlUi->unk12A4[3], func_800EE120);
    win_close(g_btlUi->unk12A4[3]);
}

void func_800EDE6C(Unk800EDE6C *arg0) {
    u32 hidden;
    Unk800EF80C s;
    Battle16CC *e;
    u8 b = g_btlCmdMenu.cmdIds[g_btlCmdMenu.cursor];
    e = &g_gameState->unk1C->unk8E4.battle->abilities[arg0->unk8];
    s.unk6 = arg0->unk8;
    s.unk0 = 1;
    s.unk1 = b;
    s.unk9 = g_btlCmdMenu.unk1;
    s.unk2 = e->unk0_0;
    s.unk3 = e->unk0_4;
    s.unk4 = e->unk0_31;
    s.unk7 = e->unk0_5;
    hidden = e->unk0_29;
    s.unk8 = 0;
    s.unk5 = hidden;
    func_800EF80C(&s);
}

void func_800EDF34(s32 arg0) {
    Unk800EE37C v;
    v.unk0 = g_btlCmdMenu.cmdIds[g_btlCmdMenu.cursor];
    v.unk4 = arg0;
    v.unk2 = g_btlCmdMenu.unk1;
    v.unk8 = D_800703FC.unk4;
    v.unkC = D_800703FC.unk7C;
    func_800EE37C(&v);
}

void func_800EDF90(s32 arg0) {
    Unk800EE37C v;
    v.unk0 = g_btlCmdMenu.cmdIds[g_btlCmdMenu.cursor];
    v.unk4 = arg0;
    v.unk2 = g_btlCmdMenu.unk1;
    v.unk8 = D_800703FC.unk68;
    v.unkC = D_800703FC.unk94;
    func_800EE37C(&v);
}

void func_800EDFEC(s32 arg0) {
    Unk800EE37C v;
    v.unk0 = g_btlCmdMenu.cmdIds[g_btlCmdMenu.cursor];
    v.unk4 = arg0;
    v.unk2 = g_btlCmdMenu.unk1;
    v.unk8 = D_800703FC.unkC0;
    v.unkC = D_800703FC.unk94;
    func_800EE37C(&v);
}

void func_800EE048(void) {
    s32 v = D_800F8E04[g_btlCmdMenu.unk10.unk1];
    g_btlCmdMenu.unit->unkCF = 0;
    g_btlCmdMenu.unit->unkB3 = 1;
    btl_setCmd((s32)g_btlCmdMenu.unit->unk4, v, D_800FD56C[v].unk8, g_btlCmdMenu.unit->unkB0, 0);
    btlui_setMenuState(4);
    win_setOnClosed(g_btlUi->unk12A4[3], func_800EE0FC);
    win_close(g_btlUi->unk12A4[3]);
}

void func_800EE0FC(Window *arg0) {
    btlui_setMenuState(0);
    g_btlCmdMenu.unk10.unk0 = 0;
}

void func_800EE120(Window *arg0) {
    btlui_setMenuState(0);
    g_btlCmdMenu.unk10.unk0 = 0;
    func_800EB358(g_btlCmdMenu.unit);
}

void func_800EE154(void) {
    if (D_80104FC0.unk1 == 2) {
        if (D_80104FC0.unk2 == 1) {
            if (++D_80104FC0.unk3 >= 5) {
                D_80104FC0.unk0 = 1;
                D_80104FC0.unk2 = 0;
            }
        } else if (D_80104FC0.unk2 == 2 && (g_padRepeat & PAD_LEFT)) {
            snd_playSe(0x417);
            D_80104FC0.unk2 = 1;
            D_80104FC0.unk1 = 2;
            D_80104FC0.unk0 = 0;
            D_80104FC0.unk3 = 0;
        }
    } else if (D_80104FC0.unk1 == 0) {
        if (g_padRepeat & PAD_LEFT) {
            snd_playSe(0x417);
            D_80104FC0.unk2 = 1;
            D_80104FC0.unk1 = 2;
            D_80104FC0.unk0 = 0;
            D_80104FC0.unk3 = 0;
        }
    } else {
        func_800EE31C();
    }
}

void func_800EE24C(void) {
    if (D_80104FC0.unk1 == 1) {
        if (D_80104FC0.unk2 == 1) {
            if (++D_80104FC0.unk3 >= 5) {
                D_80104FC0.unk0 = 1;
                D_80104FC0.unk2 = 0;
            }
        } else if (D_80104FC0.unk2 == 2 && (g_padRepeat & PAD_RIGHT)) {
            snd_playSe(0x417);
            D_80104FC0.unk2 = 1;
            D_80104FC0.unk1 = 1;
            D_80104FC0.unk0 = 0;
            D_80104FC0.unk3 = 0;
        }
    } else if (D_80104FC0.unk1 == 0) {
        if (g_padRepeat & PAD_RIGHT) {
            snd_playSe(0x417);
            D_80104FC0.unk2 = 1;
            D_80104FC0.unk1 = 1;
            D_80104FC0.unk0 = 0;
            D_80104FC0.unk3 = 0;
        }
    } else {
        func_800EE31C();
    }
}

void func_800EE31C(void) {
    Unk80104FC0 *p = &g_btlCmdMenu.unk10;
    if (p->unk1 != 0) {
        if (p->unk2 != 2) {
            p->unk2 = 2;
            p->unk3 = 0;
        } else if (p->unk3++ >= 3) {
            p->unk1 = 0;
            p->unk3 = 0;
        }
        p->unk0 = 0;
    }
}

void func_800EE37C(Unk800EE37C *arg0) {
    MenuList l;
    Unk80104F88Entry *e;
    u8 n;
    u8 f;
    Unk800EB518 *c;

    btlui_setMenuState(4);
    D_80104FD0.unk14 = g_btlUi->cmdUnit;
    D_80104FD0.unk0 = arg0->unk0;
    D_80104FD0.unk4 = (struct Unk80104FD4 *)arg0->unk4;
    D_80104FD0.unk8 = arg0->unk8;
    D_80104FD0.unkC = arg0->unkC;
    D_80104FD0.unk19 = arg0->unk2;
    D_80104FD0.unk10 = 0;
    D_80104FD0.unk12 = (arg0->unkC != 0) ? func_8002AE7C((u8 *)arg0->unkC) : 0;
    D_80104FD0.unk18 = func_800349BC((D_80104FD0.unk14->unk68 >> 1) & 0xF, 0xE2);
    n = D_80104FD0.unk4->unk7;
    func_8002EDFC(&l, n, (n < 5) ? n : 4, 1, 0x44);
    f = (g_gameState->unk24 >> 2) & 1;
    if (f == 1) {
        e = D_80104FD0.unk14->unkB4;
        c = &e->unk49C;
        l.unk5 = c->unk8[e->unk49C.unk0];
        l.cursor = c->unk4[e->unk49C.unk0];
    }
    g_btlUi->unk12A4[2] = win_open(&D_800F8E30, 8, 1, 2);
    func_8002E3A8(g_btlUi->unk12A4[2], &l);
    win_setDrawContent(g_btlUi->unk12A4[2], btlui_drawListContent);
    win_setDrawOverlay(g_btlUi->unk12A4[2], btlui_drawListOverlay);
    win_setOnOpened(g_btlUi->unk12A4[2], (void (*)(Window *))func_800EF00C);
}

void btlui_listInput(void) {
    MenuList *l = (MenuList *)&g_btlUi->unk12A4[2]->unk30;
    Window *w;
    if (l->unk8 == 0) {
        if (g_padRepeat & BTN_CIRCLE) {
            snd_playSe(0x65);
            w = g_btlUi->unk12A4[2];
            func_800EB518(&D_80104FD0.unk14->unkB4->unk49C, 1, w->unk30.unk5, w->unk30.cursor);
            if (D_80104FD0.unk10 == 0) {
                func_800EF044();
            } else {
                D_80104FD0.unk10--;
            }
        } else if (g_padRepeat & BTN_CROSS) {
            func_800EEE3C();
        } else {
            menuList_up(l, g_padRepeat);
            menuList_down(l, g_padRepeat);
            if (g_gameState->unk1C->unk8E4.battle->unkD83 == 0) {
                menuList_pageUp(l, g_padRepeat);
                menuList_pageDown(l, g_padRepeat);
            }
        }
    }
}

s32 func_800EE680(void) {
    Unk800EF80C s;
    MenuList *l = &g_btlUi->unk12A4[2]->unk30;
    s32 n = l->cursor + l->unk5;
    u8 k = D_800FDD10[D_80104FD0.unk4->unk8 + n];
    s32 r = btlui_getAbilityState(k);
    if (r != 2) {
        return 0;
    }
    {
        s.unk0 = r;
        s.unk1 = D_80104FD0.unk0;
        s.unk6 = func_800EF4D8(k);
        s.unk2 = g_gameState->unk1C->unk8E4.battle->abilities[k].unk0_0;
        s.unk3 = g_gameState->unk1C->unk8E4.battle->abilities[k].unk0_4;
        s.unk4 = g_gameState->unk1C->unk8E4.battle->abilities[k].unk0_31;
        s.unk7 = g_gameState->unk1C->unk8E4.battle->abilities[k].unk0_5;
        s.unk5 = g_gameState->unk1C->unk8E4.battle->abilities[k].unk0_29;
        s.unk8 = D_80104FD0.unk10;
        s.unk9 = D_80104FD0.unk19;
        func_800EF80C(&s);
        return 1;
    }
}

s32 func_800EE7F8(void) {
    Unk800EF80C s;
    MenuList *l = &g_btlUi->unk12A4[2]->unk30;
    UnkPair *p = btlui_getItemListEntry(l->cursor + l->unk5);
    u8 r = 0;
    Unk8006A774 *e = &D_8006A774[p->unk0];
    s32 n;

    if (p == NULL) {
        return 0;
    }
    if ((n = p->unk1) - btlui_getItemReserved(p->unk0) >= 0 && n != btlui_getItemReserved(p->unk0)) {
        s.unk0 = 2;
        s.unk1 = D_80104FD0.unk0;
        s.unk6 = p->unk0;
        s.unk2 = e->unk0_0;
        s.unk3 = e->unk0_4;
        s.unk4 = e->unk0_31;
        s.unk7 = e->unk0_5;
        s.unk5 = e->unk0_29;
        s.unk8 = D_80104FD0.unk10;
        s.unk9 = D_80104FD0.unk19;
        func_800EF80C(&s);
        r = 1;
    }
    return r;
}

s32 func_800EE938(void) {
    Unk800EF80C s;
    MenuList *l = &g_btlUi->unk12A4[2]->unk30;
    u8 r = 0;
    UnkPair *p = func_800EC130(l->cursor + l->unk5);
    Battle16CC *e = &g_gameState->unk1C->unk8E4.battle->abilities[D_80104FD0.unk4->unk8];
    s32 n;

    if (p == NULL) {
        return 0;
    }
    if ((n = p->unk1) - btlui_getItemReserved(p->unk0) >= 0 && n != btlui_getItemReserved(p->unk0)) {
        s.unk0 = 2;
        s.unk1 = D_80104FD0.unk0;
        s.unk6 = p->unk0;
        s.unk2 = e->unk0_0;
        s.unk3 = e->unk0_4;
        s.unk4 = e->unk0_31;
        s.unk7 = e->unk0_5;
        s.unk5 = e->unk0_29;
        s.unk8 = D_80104FD0.unk10;
        s.unk9 = D_80104FD0.unk19;
        func_800EF80C(&s);
        r = 1;
    }
    return r;
}

s32 btlui_getAbilityState(s32 arg0) {
    Unk80104F88Entry *e = D_80104FE4->unkB4;
    Battle16CC *a = &g_gameState->unk1C->unk8E4.battle->abilities[arg0];

    if (e->unk19 != 0 && e->unk3DC[arg0] == 0 && (s16)*e->unk1C[arg0] < (s16)e->unk31C[arg0]) {
        return 0;
    }
    if (a->unk8 & 2) {
        if ((u8)btl_hasStatus(D_80104FE4, 8)) {
            return 1;
        }
        if ((g_gameState->unk1C->unk8E4.battle->unk132C.unk0 >> 9) & 1) {
            return 1;
        }
    }
    if (btl_getMpCost((BtlCmd8 *)a) > D_80104FE4->mp) {
        return 1;
    }
    return 2;
}

void btlui_drawListOverlay(Window *arg0) {
    RECT *r = &arg0->unk8;
    s32 x;
    s32 y;
    s32 dy = 10;
    s32 dx = 0x74;
    if (D_80104FD0.unk8 != 0) {
        ui_drawLabel(arg0->unk8.x + 6, r->y - 4, arg0->unk18, D_80104FD0.unk8, 0);
    }
    if (D_80104FD0.unkC != 0) {
        ui_drawLabel(arg0->unk8.x - (D_80104FD0.unk12 - dx), r->y - 4, arg0->unk18, D_80104FD0.unkC, 0);
    }
    if (func_800EB2A4() == 2 || (func_800EB2A4() == 3 && func_800F13BC() == 2 && !(func_800F1360() & 0xFF) && (D_80104E80 & 1))) {
        x = arg0->unk8.x - 0x12;
        y = r->y + (arg0->unk30.cursor * 15 + dy);
        if (D_80104FD0.unk0 == 0x17 || D_80104FD0.unk0 == 0x15) {
            ui_drawSprite(x + 7, y + 2, arg0->unk18, D_80104FD0.unk10 + 0x5A, 0);
        }
        ui_drawHand(x, y, arg0->unk18);
    }
}

void func_800EED30(void) {
    if (D_80104FD0.unk0 == 0x1F) {
        if (g_btlUi->unk12B7 == 0 || g_btlUi->unk12B8.h != 0) {
            func_800EF044();
            snd_playSe(0x65);
        }
    }
    if (D_80104FD0.unk19 == 0 && (u8)btl_hasStatus(D_80104FD0.unk14, 0x4000)) {
        func_800EF044();
        snd_playSe(0x65);
    }
}

void func_800EEDD4(void) {
    D_80104FD0.unk10++;
}

s32 func_800EEDF0(void) {
    s32 r = 0;
    s32 (*f)(void) = D_800F8E58[D_80104FD4->unk6];
    if (f != NULL) {
        r = f();
    }
    return r;
}

void func_800EEE3C(void) {
    Window *w;
    if ((u8)D_800F8E38[D_80104FD0.unk4->unk6]()) {
        snd_playSe(0x67);
        w = g_btlUi->unk12A4[2];
        func_800EB518(&D_80104FD0.unk14->unkB4->unk49C, 1, w->unk30.unk5, w->unk30.cursor);
    } else {
        snd_playSe(0x66);
    }
}

void btlui_drawListContent(Window *arg0) {
    RECT r;
    MenuView v;
    RECT *rc = &arg0->unk8;
    s32 i;
    menuList_getView((MenuList *)&arg0->unk30, &v);
    win_setClip(0, 7, 1);
    for (i = 0; i < v.unk0; i++) {
        func_800EF094(0, i, v.unk1 + i, &v);
    }
    if (((MenuList *)&arg0->unk30)->unk8 & 4) {
        func_8002F40C((MenuList *)&arg0->unk30, rc, &r);
        win_setClip(&r, 7, 1);
        func_800EEFA8(arg0);
    }
    win_setClip(rc, 7, 1);
}

void func_800EEFA8(Window *arg0) {
    MenuView v;
    s32 i;
    func_8002F344((MenuList *)&arg0->unk30, &v);
    for (i = 0; i < v.unk0; i++) {
        func_800EF094(0, i, v.unk1 + i, &v);
    }
}

void func_800EF00C(void) {
    btlui_setMenuState(2);
    g_btlUi->unk12A4[3]->unk1E = 0;
}

void func_800EF044(void) {
    btlui_setMenuState(4);
    win_setOnClosed(g_btlUi->unk12A4[2], func_800EF7D4);
    win_close(g_btlUi->unk12A4[2]);
}

void func_800EF094(s32 arg0, s32 arg1, s32 arg2, MenuView *arg3) {
    D_800F8E48[D_80104FD4->unk6](arg0, arg1, arg2, arg3);
}

void btlui_drawAbilityRow(s32 arg0, s32 arg1, s32 arg2, MenuView *arg3) {
    u8 buf[8];
    u8 k = D_800FDD10[D_80104FD4->unk8 + arg2];
    Window *w = g_btlUi->unk12A4[2];
    RECT *r = &w->unk8;
    s32 dy = w->unk6 - r->h;
    s32 res = btlui_getAbilityState(k);
    s16 x;
    s32 y;
    Battle16CC *a;
    s32 c;

    if (res != 0) {
        x = r->x;
        a = &g_gameState->unk1C->unk8E4.battle->abilities[k];
        y = r->y + arg1 * 15 + arg3->unk2 - dy;
        c = (res != 2) * 2;
        text_draw(x + 6, y, 7, D_800F909C + a->nameOfs, c);
        if (a->mpCost != 0) {
            text_formatNumber(btl_getMpCost((BtlCmd8 *)a), buf, 3, 1);
            ui_drawSmallDigits(x + 0x60, y + 4, 7, buf, c);
        }
    }
}

void btlui_drawItemRow(s32 arg0, s32 arg1, s32 arg2, Window *arg3) {
    u8 buf[8];
    s32 x5;
    UnkPair *e = btlui_getItemListEntry(arg2);
    Window *w = g_btlUi->unk12A4[2];
    RECT *r = &w->unk8;
    s32 dy = w->unk6 - r->h;
    s32 n;
    u8 id;
    s16 x;
    s32 y;
    Unk80069018 *it;
    if (e != NULL) {
        if (e->unk1 - btlui_getItemReserved(e->unk0) >= 0) {
            n = e->unk1 - btlui_getItemReserved(e->unk0);
        } else {
            n = 0;
        }
        if (n != 0) {
            id = e->unk0;
            x = r->x;
            it = &D_80069018[id];
            x5 = x + 5;
            y = r->y + arg1 * 15 + arg3->unk2 - dy;
            ui_drawItemIcon(x5, y, 7, id, 1);
            text_draw(x + 0x14, y, 7, D_8006A218 + it->unk0, 0);
            text_formatNumber(n, buf, 2, 1);
            ui_drawSmallDigits(x + 0x67, y + 4, 7, buf, 0);
        }
    }
}

void func_800EF390(s32 arg0, s32 arg1, s32 arg2, Window *arg3) {
    u8 buf[8];
    UnkPair *e = func_800EC130(arg2);
    Window *w = g_btlUi->unk12A4[2];
    RECT *r = &w->unk8;
    s32 dy = w->unk6 - r->h;
    s32 n;
    s32 x;
    s32 y;
    Unk80069018 *it;
    if (e != NULL) {
        if (e->unk1 - btlui_getItemReserved(e->unk0) >= 0) {
            n = e->unk1 - btlui_getItemReserved(e->unk0);
        } else {
            n = 0;
        }
        if (n != 0) {
            x = r->x;
            it = &D_80069018[e->unk0];
            y = r->y + arg1 * 15 + arg3->unk2 - dy;
            text_formatNumber(n, buf, 2, 1);
            text_draw(x + 5, y, 7, D_8006A218 + it->unk0, 0);
            ui_drawSmallDigits(x + 0x67, y + 4, 7, buf, 0);
        }
    }
}

s32 func_800EF4D8(s32 arg0) {
    s32 t;
    if (arg0 == 0x44) {
        switch (D_80104FE4->unkB4->unk14->equip[4]) {
        case 0xE4:
            arg0 = 0x45;
            break;
        case 0xE5:
            arg0 = 0x46;
            break;
        case 0xE3:
            arg0 = 0x47;
            break;
        }
    } else if (arg0 == 0x42) {
        t = D_80104FE4->unkB4->unk14->equip[4] == 0xDE;
        arg0 = t + 0x42;
    }
    return arg0;
}

s32 btl_getMpCost(BtlCmd8 *arg0) {
    s32 cost = arg0->mpCost;
    if (arg0->unkB & 4) {
        if (D_8007AF02 != 0) {
            cost *= 4;
        }
    }
    if (D_80104FE8 != 0) {
        cost >>= 1;
    }
    return cost;
}

char *func_800EF5E0(void) {
    MenuList *l = &g_btlUi->unk12A4[2]->unk30;
    s32 n = l->cursor + l->unk5;
    u8 k = D_800FDD10[D_80104FD4->unk8 + n];
    if (btlui_getAbilityState(k) == 0) {
        return NULL;
    }
    return D_800F96E4 + D_800FB1F0[k].unk0;
}

u8 *func_800EF674(void) {
    Unk16 *w = &g_btlUi->unk12A4[2]->unk30;
    UnkPair *e = btlui_getItemListEntry(w->cursor + w->unk5);
    if (e != NULL) {
        if (e->unk1 - btlui_getItemReserved(e->unk0) >= 0) {
            if (e->unk1 != btlui_getItemReserved(e->unk0)) {
                return D_800FB4F0 + D_800FD36C[e->unk0];
            }
            return NULL;
        }
    }
    return NULL;
}

u8 *func_800EF724(void) {
    Unk16 *w = &g_btlUi->unk12A4[2]->unk30;
    UnkPair *e = func_800EC130(w->cursor + w->unk5);
    if (e != NULL) {
        if (e->unk1 - btlui_getItemReserved(e->unk0) >= 0) {
            if (e->unk1 != btlui_getItemReserved(e->unk0)) {
                return D_800FB4F0 + D_800FD36C[e->unk0];
            }
            return NULL;
        }
    }
    return NULL;
}

void func_800EF7D4(Window *arg0) {
    btlui_setMenuState(1);
    g_btlUi->unk12A4[3]->unk1E = 1;
}

void func_800EF80C(Unk800EF80C *arg0) {
    s32 i;

    g_btlTarget.unk20 = g_btlUi->cmdUnit;
    g_btlTarget.unk0 = arg0->unk0;
    g_btlTarget.unk1 = arg0->unk1;
    g_btlTarget.unk2 = arg0->unk2;
    g_btlTarget.unk3 = arg0->unk3;
    g_btlTarget.unk4 = arg0->unk4;
    g_btlTarget.unkA = arg0->unk3;
    g_btlTarget.unk1A = arg0->unk6;
    g_btlTarget.unkF = arg0->unk7;
    g_btlTarget.unkB = arg0->unk5;
    g_btlTarget.unk10 = arg0->unk8;
    g_btlTarget.unk6 = D_800F8E84[g_btlTarget.unk2];
    g_btlTarget.unkC = 0;
    g_btlTarget.unkD = func_800349BC((g_btlTarget.unk20->unk68 >> 1) & 0xF, 0xE2);
    g_btlTarget.unkE = arg0->unk9;
    g_btlTarget.unk7 = (u32)(arg0->unk2 - 6) < 3;
    g_btlTarget.unk8 = arg0->unk2 == 0 || arg0->unk2 == 3 || arg0->unk2 == 9 || arg0->unk2 == 6;
    g_btlTarget.unk9 = (u32)(arg0->unk2 - 3) < 3;
    D_80105020.unk2 = 0;
    D_80105020.unk0 = 0;
    func_800F142C();
    func_800EFA10(1);
    if (g_btlTarget.unkF == 0) {
        if (D_80104E6B == 0) {
            btlui_setMenuState(3);
        } else {
            func_800F1970(3);
        }
    } else {
        if (D_80104E6B != 0) {
            func_800F1970(5);
        }
        func_800F27CC(g_btlTarget.unkF);
    }
    for (i = 0; i < 2; i++) {
        func_8002C740(i, 2, 1);
        func_8002C740(i, 3, 1);
    }
}

void func_800EFA10(s32 arg0) {
    BtlUnit *u;
    if (g_btlTarget.unk2 == 0xC) {
        for (u = g_gameState->unk1C->unk8E4.battle->unk6F8; u != 0; u = u->unk0) {
            if (((u->unk68 >> 28) & 1) && (!(u->status & STATUS_KO) || g_btlTarget.unkB != 0)) {
                u->unk68 |= 0x20000000;
            }
        }
    } else if (g_btlTarget.unk2 == 0xD) {
        g_btlTarget.unk20->unk68 |= 0x20000000;
    } else if (arg0 & 0xFF) {
        func_800F1790(D_800F8E68[g_btlTarget.unk2][0], D_800F8E68[g_btlTarget.unk2][1]);
    } else {
        func_800F16DC(D_800F8E68[g_btlTarget.unk2][0], D_800F8E68[g_btlTarget.unk2][1]);
    }
}

s32 func_800EFB3C(s32 arg0) {
    BtlUnit *list[4];
    BtlUnit *p;
    BtlUnit *u;
    s32 cur = 0;
    s32 n;
    s32 k;
    if (g_btlTarget.unk6 != 0) {
        return 0;
    }
    if (g_btlTarget.unk2 == 0xD) {
        return 0;
    }
    u = BtlFindTargetCursor();
    if (u == NULL) {
        return 0;
    }
    memset((s32)list, 0, 0x10);
    n = 0;
    for (p = g_gameState->unk1C->unk8E4.battle->unk6F8; p != NULL; p = p->unk0) {
        if (((p->unk68 >> 28) & 1) && (p->unk68 & 1) == g_btlTarget.unkA && (!(p->status & STATUS_KO) || g_btlTarget.unkB != 0)) {
            list[n] = p;
            if (p == u) {
                cur = n;
            }
            n++;
        }
    }
    if (n >= 2) {
        u->unk68 &= ~0x20000000;
        if (!(arg0 & 0xFF)) {
            k = cur - 1;
            if (cur == 0) {
                k = n - 1;
            }
        } else {
            k = 0;
            if (cur < n - 1) {
                k = cur + 1;
            }
        }
        list[k]->unk68 |= 0x20000000;
        return 1;
    }
    return 0;
}

s32 func_800EFD1C(u8 arg0) {
    BtlUnit *cur;
    BtlUnit *u;
    BtlUnit *t;
    BtlUnit *list[4];
    s32 n;
    s32 i;
    u8 side;
    s32 k;

    for (u = g_gameState->unk1C->unk8E4.battle->unk6F8; u != NULL; u = u->unk0) {
        if ((u->unk68 & 0x30000000) == 0x30000000) {
            cur = u;
            goto found;
        }
    }
    cur = NULL;
found:
    if (g_btlTarget.unk8 != 0) {
        if (arg0 == (k = cur->unk68 & 1)) {
            return 0;
        }
        side = !k;
        g_btlTarget.unkA = side;
        memset(list, 0, sizeof(list));
        n = 0;
        for (t = g_gameState->unk1C->unk8E4.battle->unk6F8; t != NULL; t = t->unk0) {
            if (((t->unk68 >> 28) & 1) && (t->unk68 & 1) == side) {
                if (!(t->status & STATUS_KO) || g_btlTarget.unkB) {
                    list[n++] = t;
                }
            }
        }
        if (n != 0) {
            cur->unk68 &= ~0x20000000;
            if (g_btlTarget.unk6 == 0) {
                list[0]->unk68 |= 0x20000000;
            } else {
                func_800F142C();
                for (i = 0; i < n; i++) {
                    list[i]->unk68 |= 0x20000000;
                }
            }
            return 1;
        }
    }
    return 0;
}

void func_800EFF14(void) {
    u8 old = g_btlTarget.unk6;
    BtlUnit *u;
    BtlUnit *p;
    s32 n;
    if (g_btlTarget.unk9 != 0) {
        if (old != 0) {
            if ((g_btlTarget.unk24->unk68 >> 29) & 1) {
                u = g_btlTarget.unk24;
            } else {
                u = BtlFindTargetCursor();
            }
            func_800F142C();
            u->unk68 |= 0x20000000;
            g_btlTarget.unk6 = 0;
        } else {
            p = g_gameState->unk1C->unk8E4.battle->unk6F8;
            u = BtlFindTargetCursor();
            n = 0;
            g_btlTarget.unk24 = u;
            func_800F142C();
            for (; p != NULL; p = p->unk0) {
                if (((p->unk68 >> 28) & 1) && (p->unk68 & 1) == (u->unk68 & 1) && (!(p->status & STATUS_KO) || g_btlTarget.unkB != 0)) {
                    p->unk68 |= 0x20000000;
                    n++;
                }
            }
            if (n != 0) {
                g_btlTarget.unk6 = 1;
            }
        }
        if (old != g_btlTarget.unk6) {
            snd_playSe(0x67);
        }
    }
}

s32 btlui_isCmdUsable(void) {
    Unk800FD56C *e = &D_800FD56C[g_btlTarget.unk1];
    Battle16CC *b;
    u8 cost;
    if (g_btlTarget.unk1 == 0x1F && (g_btlUi->unk12B7 == 0 || g_btlUi->unk12B8.h != 0)) {
        return 0;
    }
    if (g_btlTarget.unkE == 0 && (btl_hasStatus(g_btlTarget.unk20, 0x4000) & 0xFF)) {
        return 0;
    }
    if (e->unk6 == 1) {
        b = &g_gameState->unk1C->unk8E4.battle->abilities[g_btlTarget.unk1A];
        cost = b->mpCost;
        if (g_btlTarget.unkD != 0) {
            cost >>= 1;
        }
        if (g_btlTarget.unk20->mp < cost) {
            return 0;
        }
        if ((b->unk8 & 2) && (btl_hasStatus(g_btlTarget.unk20, 8) & 0xFF)) {
            return 0;
        }
    }
    if ((u32)(e->unk6 - 2) < 2 && item_getCount(g_btlTarget.unk1A) - btlui_getItemReserved(g_btlTarget.unk1A) <= 0) {
        return 0;
    }
    return 1;
}

s32 func_800F028C(void) {
    BtlUnit *u = BtlFindTargetCursor();
    BtlUnit *p;
    s32 n;
    if (u == NULL) {
        func_800EFA10(0);
        return 1;
    }
    if (g_btlTarget.unk6 == 0) {
        if ((u->status & STATUS_KO) && g_btlTarget.unkB == 0) {
            u->unk68 &= ~0x20000000;
            func_800EFA10(0);
            return 1;
        }
        return 1;
    }
    if (g_btlTarget.unk2 != 0xC) {
        p = g_gameState->unk1C->unk8E4.battle->unk6F8;
        n = 0;
        func_800F142C();
        while (p != NULL) {
            if (((p->unk68 >> 28) & 1) && (p->unk68 & 1) == (u->unk68 & 1) && (!(p->status & STATUS_KO) || g_btlTarget.unkB != 0)) {
                p->unk68 |= 0x20000000;
                n++;
            }
            p = p->unk0;
        }
        if (n != 0) {
            g_btlTarget.unk6 = 1;
        }
    }
    return 1;
}

void btlui_confirmTarget(void) {
    s32 mask = 0;
    u8 ok = 1;
    BtlUnit *u = g_gameState->unk1C->unk8E4.battle->unk6F8;
    Unk800FD56C *e = &D_800FD56C[g_btlTarget.unk1];
    Unk80104FF0 *d;
    for (; u != NULL; u = u->unk0) {
        if ((u->unk68 & 0x30000000) == 0x30000000) {
            mask |= u->unkB0;
        }
    }
    d = &g_btlTarget;
    btlui_setMenuState(4);
    if (d->unk1 == 0x17 || d->unk1 == 0x15) {
        ok = func_800F0620(mask);
    } else {
        d->unk20->unkB3 = 1;
        d->unk20->unkCF = 0;
        btl_setCmd((s32)g_btlTarget.unk20->unk4, g_btlTarget.unk1, g_btlTarget.unk1A, mask & 0xFFFF, func_800F1B78_inline());
    }
    func_800F142C();
    if (ok) {
        if ((u32)(e->unk6 - 2) < 2) {
            func_800EC14C(g_btlTarget.unk1A);
        }
        func_800EB46C();
    }
    snd_playSe(0x67);
}

s32 func_800F0620(s32 arg0) {
    BtlUnit *u;
    s32 n;
    s8 v;
    s32 r = 0;
    if (g_btlTarget.unk10 == 0) {
        g_btlTarget.unk1C = arg0;
        g_btlTarget.unk13 = g_btlTarget.unk1;
        g_btlTarget.unk18 = g_btlTarget.unk1A;
        if ((u32)(g_btlTarget.unk2 - 9) < 3) {
            v = 2;
        } else {
            n = 0;
            for (u = g_gameState->unk1C->unk8E4.battle->unk6F8; u != NULL; u = u->unk0) {
                if ((u->unk68 & 0x30000000) == 0x30000000) {
                    n++;
                }
            }
            v = n >= 2;
        }
        g_btlTarget.unk14 = v;
        btlui_cancelTarget();
        func_800EEDD4();
    } else {
        g_btlTarget.unk20->unkCF = 0;
        g_btlTarget.unk20->unkB3 = 1;
        btl_setCmd((s32)g_btlTarget.unk20->unk10, g_btlTarget.unk13, g_btlTarget.unk18, (u16)g_btlTarget.unk1C, g_btlTarget.unk14);
        if ((u32)(g_btlTarget.unk2 - 9) < 3) {
            v = 2;
        } else {
            n = 0;
            for (u = g_gameState->unk1C->unk8E4.battle->unk6F8; u != NULL; u = u->unk0) {
                if ((u->unk68 & 0x30000000) == 0x30000000) {
                    n++;
                }
            }
            v = n >= 2;
        }
        btl_setCmd((s32)g_btlTarget.unk20->unk4, g_btlTarget.unk1, g_btlTarget.unk1A, arg0 & 0xFFFF, v);
        r = 1;
    }
    return r;
}

void btlui_cancelTarget(void) {
    BtlUnit *u;
    s32 i;
    if (g_btlTarget.unkF == 0) {
        if (g_btlTarget.unkC != 0) {
            func_800F1A20(g_btlTarget.unk0);
        } else {
            btlui_setMenuState(g_btlTarget.unk0);
        }
    } else {
        if (g_btlTarget.unkC != 0) {
            func_800F1A20(5);
        }
        func_800F2890(g_btlTarget.unk0);
    }
    for (u = g_gameState->unk1C->unk8E4.battle->unk6F8; u != 0; u = u->unk0) {
        u->unk68 &= ~0x20000000;
    }
    for (i = 0; i < 2; i++) {
        func_8002C740(i, 2, 0);
        func_8002C740(i, 3, 0);
    }
}

void btlui_updateTargetWindow(void) {
    if ((g_btlTarget.unkC == 0 || (u32)(g_btlUi->unk12A4[0]->unk1C - 1) >= 2) && g_btlTarget.unkC != D_80104E6B) {
        if (D_80104E6B != 0) {
            if (g_btlTarget.unkC == 0) {
                btlui_setMenuState(4);
                g_btlUi->unk12A4[0] = win_open(&D_800F8EF4, 5, 2, 0);
                win_setOpenAnim(g_btlUi->unk12A4[0], 4);
                win_setOnOpened(g_btlUi->unk12A4[0], func_800F1A8C);
                win_setDrawContent(g_btlUi->unk12A4[0], func_800F0A54);
                win_setDrawOverlay(g_btlUi->unk12A4[0], func_800F0BE0);
                g_btlTarget.unk11 = 3;
                g_btlTarget.unkC = 1;
            }
        } else if (g_btlTarget.unkC != 0) {
            g_btlTarget.unk12 = 3;
            btlui_setMenuState(4);
            win_setOnClosed(g_btlUi->unk12A4[0], func_800F1AF8);
            win_close(g_btlUi->unk12A4[0]);
        }
    }
}

void func_800F0A54(Window *arg0) {
    s32 y1 = arg0->unk2 + 8;
    s32 y2 = y1;
    BtlUnit *u = g_gameState->unk1C->unk8E4.battle->unk6F8;
    s32 m;
    u32 f;
    u8 *name;
    s32 c;
    Window *w = arg0;
    func_800269F4(arg0->unk0 + 0x6E, arg0->unk2 + 4, arg0->unk18, 9 - arg0->unk6);
    m = func_8002BE88();
    for (; u != NULL; u = u->unk0) {
        name = btl_getUnitName(u);
        f = u->unk68;
        if ((f >> 28) & 1) {
            if (f & 1) {
                if (u->hp != 0) {
                    c = ((u16)(u->max->maxHp / 6) >= u->hp) * 4;
                } else {
                    c = 3;
                }
                text_draw(w->unk0 + 0x7A, y2, arg0->unk18, name, c);
                y2 += 15;
            } else {
                c = ((u8)btl_hasStatus(u, 0x100) != 0) * 2;
                text_drawBlock(w->unk0 + 0xC, y1, arg0->unk18, name, c);
                y1 += 15;
            }
        }
    }
    func_8002BE98(m);
}

void func_800F0BE0(Window *arg0) {
    Window *w = arg0;
    s32 n = 0;
    BtlUnit *u = g_gameState->unk1C->unk8E4.battle->unk6F8;
    RECT *r = &arg0->unk8;
    BtlUnit *p;
    RECT rc;
    s32 yl;
    s32 yr;
    for (p = u; p != NULL; p = p->unk0) {
        if ((p->unk68 >> 29) & 1) {
            n++;
        }
    }
    if (arg0->unk1C != 3) {
        win_setClip(NULL, arg0->unk18, 4);
    }
    ui_drawLabel(w->unk0 + 8, r->y - 3, arg0->unk18, D_800703FC.unk3C, 0);
    ui_drawLabel(w->unk0 + 0x76, r->y - 3, arg0->unk18, D_800703FC.unkA0, 0);
    if (arg0->unk1C != 3) {
        rc.x = r->x;
        rc.y = r->y - 3;
        rc.w = r->w;
        rc.h = 8;
        win_setClip(&rc, arg0->unk18, 4);
    }
    if (arg0->unk1C == 3 && (n < 2 || !(D_80104E80 & 1))) {
        yl = r->y + 0xC;
        yr = yl;
        for (; u != NULL; u = u->unk0) {
            if ((u->unk68 >> 28) & 1) {
                if ((u->unk68 >> 29) & 1) {
                    if (u->unk68 & 1) {
                        ui_drawHand(r->x + 0x62, yr, arg0->unk18 - 1);
                    } else {
                        ui_drawHand(r->x - 0xC, yl, arg0->unk18 - 1);
                    }
                }
                if (u->unk68 & 1) {
                    yr += 15;
                } else {
                    yl += 15;
                }
            }
        }
    }
}

void btlui_drawTargetCursors(void) {
    u32 c = 0x64808080;
    Unk80105020 *b = &D_80105020;
    u32 *clut = &D_800686D8;
    s32 i;
    SprtWords *p;
    DR_TPAGE *t;
    if (b->unk0 >= 2 && !(D_80104E80 & 3)) {
        c = 0x64404040;
    }
    for (i = 0; i < b->unk0; i++) {
        setlen(g_primPtr, 4);
        p = (SprtWords *)g_primPtr;
        p->rgbc = c;
        p->xy = (u16)b->unk14[i][0] | (b->unk14[i][1] << 16);
        p->uvclut = *clut;
        p->wh = 0xE0018;
        addPrim(&g_ot[17], p);
        g_primPtr = (u8 *)(p + 1);
    }
    setlen(g_primPtr, 1);
    t = (DR_TPAGE *)g_primPtr;
    t->code[0] = 0xE100061F;
    addPrim(&g_ot[17], t);
    g_primPtr = (u8 *)(t + 1);
}

void *func_800F0F60(void) {
    BtlUnit *u;
    BtlUnit *t;
    s32 n;
    s32 k;

    if (g_btlTarget.unk2 < 3) {
        t = find_target();
        if (t == NULL) {
            return NULL;
        }
        func_8002BD54(D_80104FA0, text_get(D_80104FF2 + 5));
        text_append(D_80104FA0, btl_getUnitName(t));
    } else if ((u8)(g_btlTarget.unk2 - 3) < 3) {
        n = 0;
        k = g_btlTarget.unk2 - 3;
        for (u = g_gameState->unk1C->unk8E4.battle->unk6F8; u != NULL; u = u->unk0) {
            if ((u->unk68 >> 29) & 1) {
                n++;
            }
        }
        if (n < 2) {
            t = find_target();
            if (t == NULL) {
                return NULL;
            }
            func_8002BD54(D_80104FA0, text_get(k + 0x13));
            text_append(D_80104FA0, btl_getUnitName(t));
        } else {
            func_8002BD54(D_80104FA0, text_get(k + 0x16));
        }
    } else {
        func_8002BD54(D_80104FA0, text_get(g_btlTarget.unk2 + 5));
    }
    text_append(D_80104FA0, text_get(0x1A));
    return D_80104FA0;
}

void func_800F1150(BtlUnit *arg0) {
    if (g_btlTarget.unk6 != 0 && D_800F8EE4[g_btlTarget.unk2] != 0) {
        func_800EFA10(0);
    }
}

void func_800F11A4(BtlUnit *arg0) {
    if ((arg0->unk68 >> 29) & 1) {
        arg0->unk68 &= ~0x20000000;
        if (g_btlTarget.unk6 == 0) {
            func_800EFA10(0);
        }
    }
}

void btlui_targetInput(void) {
    u32 v;
    if (g_padRepeat & BTN_CIRCLE) {
        snd_playSe(0x65);
        btlui_cancelTarget();
        return;
    }
    if (g_padRepeat & BTN_CROSS) {
        btlui_confirmTarget();
        return;
    }
    if (g_gameState->unk1C->unk8E4.battle->unkD83 == 0 && (g_padRepeat & (BTN_L1 | BTN_R1))) {
        func_800EFF14();
        return;
    }
    if (g_padRepeat & BTN_L2) {
        snd_playSe(0x67);
        v = g_gameState->unk24;
        g_gameState->unk24 = (v & ~0x80000) | ((((v >> 19) & 1) ^ 1) << 19);
        return;
    }
    btlui_targetMove();
}

void func_800F1300(void) {
    if (!(btlui_isCmdUsable() & 0xFF)) {
        snd_playSe(0x65);
        btlui_cancelTarget();
    } else if (func_800F028C() & 0xFF) {
        btlui_updateTargetWindow();
    }
}

u8 func_800F1360(void) {
    return D_80104FFC;
}

Window *func_800F1370(void) {
    return g_btlUi->unk12A4[0];
}

s32 func_800F1388(void) {
    s32 ret = 0;
    if (g_btlUi->menuState == 3) {
        ret = D_80104FFF != 0;
    }
    return ret;
}

s32 func_800F13BC(void) {
    return g_btlTarget.unk0;
}

s32 func_800F13CC(void) {
    BtlUnit *u = g_gameState->unk1C->unk8E4.battle->unk6F8;
    s32 n = 0;
    while (u != NULL) {
        if ((u->unk68 >> 29) & 1) {
            n++;
        }
        u = u->unk0;
    }
    return n;
}

void func_800F142C(void) {
    BtlUnit *u = g_gameState->unk1C->unk8E4.battle->unk6F8;
    while (u != NULL) {
        u->unk68 &= ~0x20000000;
        u = u->unk0;
    }
}

void func_800F1484(void) {
    s32 n;
    s32 m;
    BtlUnit *u;
    Unk80105020 *p;
    m = n = 0;
    u = g_gameState->unk1C->unk8E4.battle->unk6F8;
    p = &D_80105020;
    if (D_80104E6A != 0) {
        for (; u != NULL; u = u->unk0) {
            if (((u->unk68 >> 28) & 1) && u->unkBC != 0) {
                p->unk14[n][0] = u->unkB8 - 0x18;
                p->unk14[n][1] = u->unkBA - 7;
                n++;
                if (u->unk68 & 1) {
                    p->unk4[m] = u;
                    m++;
                }
            }
        }
    }
    p->unk0 = n;
    p->unk2 = m;
}

Unk80105020 *func_800F1548(void) {
    return &D_80105020;
}

void btlui_targetMove(void) {
    s32 i;
    s32 m;
    if (D_80104FFC == 0) {
        m = func_800C2DF4();
    } else {
        m = 0;
    }
    for (i = 0; i < 4; i++) {
        if (g_padRepeat & D_800F8EA4[m][i]) {
            D_800F8E94[i]();
        }
    }
}

void func_800F160C(void) {
    if (func_800EFB3C(0) & 0xFF) {
        snd_playSe(0x67);
    }
}

void func_800F1640(void) {
    if (func_800EFB3C(1) & 0xFF) {
        snd_playSe(0x67);
    }
}

void func_800F1674(void) {
    if (func_800EFD1C(0) & 0xFF) {
        snd_playSe(0x67);
    }
}

void func_800F16A8(void) {
    if (func_800EFD1C(1) & 0xFF) {
        snd_playSe(0x67);
    }
}

void func_800F16DC(u8 arg0, u8 arg1) {
    BtlUnit *u = g_gameState->unk1C->unk8E4.battle->unk6F8;
    if (arg0 == 0xFF) {
        arg0 = D_80104FFA;
    }
    for (; u != NULL; u = u->unk0) {
        if (((u->unk68 >> 28) & 1) && (u->unk68 & 1) == arg0) {
            if (!(u->status & STATUS_KO) || g_btlTarget.unkB != 0) {
                u->unk68 |= 0x20000000;
                if (arg1 == 0) {
                    return;
                }
            }
        }
    }
}

void func_800F1790(u8 arg0, u8 arg1) {
    BtlUnit *u = g_gameState->unk1C->unk8E4.battle->unk6F8;
    u8 found = 0;
    BtlUnit *t;
    u32 f;
    u32 side;

    if (arg0 == 0xFF) {
        arg0 = D_80104FFA;
    }
    if (g_btlTarget.unk4 != 0) {
        t = func_800F18F0(arg0);
        if (t != NULL) {
            t->unk68 |= 0x20000000;
            return;
        }
    }
    for (; u != NULL; u = u->unk0) {
        f = u->unk68;
        side = f & 1;
        if (!((f >> 28) & 1) || side != arg0) {
            continue;
        }
        if ((u->status & STATUS_KO) && g_btlTarget.unkB == 0) {
            continue;
        }
        if (arg1 == 0 && side != 0) {
            if (u == g_btlTarget.unk20) {
                u->unk68 = f | 0x20000000;
                found = 1;
            }
        } else {
            found = 1;
            u->unk68 |= 0x20000000;
        }
        if (arg1 == 0 && found) {
            return;
        }
    }
}

BtlUnit *func_800F18F0(u8 arg0) {
    BtlUnit *u;
    for (u = g_gameState->unk1C->unk8E4.battle->unk6F8; u != 0; u = u->unk0) {
        if (((u->unk68 >> 28) & 1) && (u->unk68 & 1) == arg0 && (u->status & STATUS_KO)) {
            return u;
        }
    }
    return 0;
}

void func_800F1970(s8 arg0) {
    Window *w;
    btlui_setMenuState(4);
    w = win_open(&D_800F8EF4, 5, 2, 0);
    g_btlUi->unk12A4[0] = w;
    win_setOpenAnim(w, 4);
    win_setOnOpened(g_btlUi->unk12A4[0], func_800F1A8C);
    win_setDrawContent(g_btlUi->unk12A4[0], func_800F0A54);
    win_setDrawOverlay(g_btlUi->unk12A4[0], func_800F0BE0);
    g_btlTarget.unk11 = arg0;
    g_btlTarget.unkC = 1;
}

void func_800F1A20(s32 arg0) {
    if (g_btlTarget.unkC != 0) {
        g_btlTarget.unk12 = arg0;
        btlui_setMenuState(4);
        win_setOnClosed(g_btlUi->unk12A4[0], func_800F1AF8);
        win_close(g_btlUi->unk12A4[0]);
    }
}

void func_800F1A8C(Window *arg0) {
    if (g_btlTarget.unk0 == 2) {
        func_8002E32C(g_btlUi->unk12A4[2], 0);
    }
    func_8002E32C(g_btlUi->unk12A4[3], 0);
    btlui_setMenuState(g_btlTarget.unk11);
}

void func_800F1AF8(Window *arg0) {
    if (g_btlTarget.unk0 == 2) {
        func_8002E32C(g_btlUi->unk12A4[2], 1);
    }
    func_8002E32C(g_btlUi->unk12A4[3], 1);
    g_btlTarget.unkC = 0;
    g_btlUi->unk12A4[0] = 0;
    btlui_setMenuState((u8)g_btlTarget.unk12);
}

s32 func_800F1B78(void) {
    BtlUnit *u;
    s32 n;
    if (g_btlTarget.unk2 >= 9 && g_btlTarget.unk2 <= 11) {
        return 2;
    }
    n = 0;
    for (u = g_gameState->unk1C->unk8E4.battle->unk6F8; u != 0; u = u->unk0) {
        if ((u->unk68 & 0x30000000) == 0x30000000) {
            n++;
        }
    }
    return n >= 2;
}

void func_800F1BFC(Window *arg0) {
    Window *w = arg0;
    s32 n = 0;
    BtlUnit *u = g_gameState->unk1C->unk8E4.battle->unk6F8;
    Unk80105020 *b = func_800F1548();
    Unk80105078 *m = func_800F3EA4();
    u32 c;
    s32 y;
    s32 x;
    s32 k;
    s32 yy;
    Unk80068EE8 *d;
    SprtWords *p;
    DR_TPAGE *t;

    if (b->unk2 == 0 || arg0->unk1C != 3 || m->unk0 == 0) {
        return;
    }
    c = 0x64808080;
    if (b->unk2 >= 2 && !(D_80104E80 & 3)) {
        c = 0x64404040;
    }
    y = w->unk2 + 5;
    x = w->unk0 - 0xE;
    for (; u != NULL; u = u->unk0) {
        if (!(u->unk68 & 1)) {
            continue;
        }
        if (u == b->unk4[n]) {
            for (k = 0, yy = y; k < 2; k++, yy += 6) {
                d = &D_80068EE8[k];
                setlen(g_primPtr, 4);
                p = (SprtWords *)g_primPtr;
                p->rgbc = c;
                p->xy = x | (yy << 16);
                p->uvclut = d->unk0;
                p->wh = d->unk4 | (d->unk5 << 16);
                addPrim(&g_ot[arg0->unk18 - 1], p);
                g_primPtr = (u8 *)(p + 1);
            }
            n++;
            if (n >= b->unk2) {
                break;
            }
        }
        y += 0xC;
    }
    setlen(g_primPtr, 1);
    t = (DR_TPAGE *)g_primPtr;
    t->code[0] = (D_80068EEE & 0x9FF) | 0xE1000600;
    addPrim(&g_ot[arg0->unk18], t);
    g_primPtr = (u8 *)(t + 1);
}

void func_800F1E84(Window *arg0) {
    u8 a[8];
    u8 b[8];
    BtlUnit *u;
    Window *w = arg0;
    s32 y = arg0->unk2 + 6;
    s32 dy = arg0->unk6 - arg0->unk8.h;
    s32 c;
    for (u = g_gameState->unk1C->unk8E4.battle->unk6F8; u != 0 && (u->unk68 & 1); u = u->unk0) {
        c = 3;
        if (u->hp != 0) {
            c = ((u->status & STATUS_LOW_HP) != 0) * 4;
        }
        b[0] = 0x2F;
        text_formatNumber(u->hp, a, 4, 1);
        text_formatNumber(u->max->maxHp, b + 1, -1, 1);
        ui_drawDigits(w->unk0 + 4, y, arg0->unk18, a, c);
        ui_drawDigits(w->unk0 + 0x20, y - dy, arg0->unk18, b, c);
        y += 12;
    }
}

void func_800F1FBC(Window *arg0) {
    u8 hp[8];
    u8 max[8];
    Window *w = arg0;
    s32 y = arg0->unk2 + 6;
    BtlUnit *u = g_gameState->unk1C->unk8E4.battle->unk6F8;
    s32 dy = arg0->unk6 - arg0->unk8.h;
    s32 yy;
    s32 c;
    while (u != NULL) {
        if (u->unk68 & 1) {
            yy = y - dy;
            c = (u->mp < (s16)(u->max->maxMp / 6)) * 4;
            max[0] = 0x2F;
            text_formatNumber(u->mp, hp, 3, 1);
            y += 0xC;
            text_formatNumber((s16)u->max->maxMp, &max[1], -1, 1);
            ui_drawDigits(w->unk0 + 4, yy, arg0->unk18, hp, c);
            ui_drawDigits(w->unk0 + 0x19, yy, arg0->unk18, max, c);
        }
        u = u->unk0;
    }
}

void func_800F2118(Window *arg0) {
    BtlUnit *u;
    s32 i;
    s32 n;
    s32 x;
    Window *w = arg0;
    s32 d = arg0->unk6 - arg0->unk8.h;
    s32 y;
    u32 st;
    d -= 6;
    y = arg0->unk2 - d;
    for (u = g_gameState->unk1C->unk8E4.battle->unk6F8; u != 0; u = u->unk0) {
        if (u->unk68 & 1) {
            n = 0;
            st = u->status | u->unk38;
            x = w->unk0 + 4;
            for (i = 0; i < 16; i++) {
                if (st & D_800F8F18[i].unk0) {
                    ui_drawSprite(x, y, arg0->unk18, D_800F8F18[i].unk4, 0);
                    x += 12;
                    if (++n >= 9) {
                        break;
                    }
                }
            }
            y += 12;
        }
    }
}

void func_800F224C(Window *arg0) {
    BtlUnit *u;
    s32 i;
    s32 x;
    Window *w = arg0;
    s32 d = arg0->unk6 - arg0->unk8.h;
    s32 y;
    u32 st;
    d -= 6;
    y = arg0->unk2 - d;
    for (u = g_gameState->unk1C->unk8E4.battle->unk6F8; u != 0; u = u->unk0) {
        if (u->unk68 & 1) {
            st = u->status | u->unk38;
            x = w->unk0 + 4;
            for (i = 0; i < 8; i++) {
                if (st & D_800F8F98[i].unk0) {
                    ui_drawSprite(x, y, arg0->unk18, D_800F8F98[i].unk4, 0);
                }
                x += D_800F8F98[i].unk6;
            }
            y += 12;
        }
    }
}

void func_800F2374(void) {
    Unk80105078 *p;
    RECT r;

    p = func_800F3EA4();
    setRECT(&r, 0xD0, p->unk60 - 4, 0x40, 4);
    r.x += D_801051E8[g_gameState->unk1C->unk8 & 1][0];
    r.y += D_801051E8[g_gameState->unk1C->unk8 & 1][1];
    SetDrawMove((DR_MOVE *)g_primPtr, &r, 0x3C0, 0x1F8);
    addPrim(&g_ot[19], g_primPtr);
    g_primPtr += sizeof(DR_MOVE);
    setRECT(&r, 0x110, p->unk60 - 4, 0x30, 4);
    r.x += D_801051E8[g_gameState->unk1C->unk8 & 1][0];
    r.y += D_801051E8[g_gameState->unk1C->unk8 & 1][1];
    SetDrawMove((DR_MOVE *)g_primPtr, &r, 0x3C0, 0x1FC);
    addPrim(&g_ot[19], g_primPtr);
    g_primPtr += sizeof(DR_MOVE);
}

void func_800F2560(void) {
    SPRT *a = &D_801050D8->unkB0[g_drawBufs.unk0];
    SPRT *b = &D_801050D8->unkD8[g_drawBufs.unk0];
    a->y0 = b->y0 = func_800F3EA4()->unk60 - 4;
    addPrim(&g_ot[18], a);
    addPrim(&g_ot[18], b);
    addPrim(&g_ot[18], &D_801050D8->unk100[g_drawBufs.unk0]);
}

void func_800F2684(s32 arg0) {
    setlen(&D_801050D8->unkB0[arg0], 4);
    setcode(&D_801050D8->unkB0[arg0], 0x64);
    setRGB0(&D_801050D8->unkB0[arg0], 0x80, 0x80, 0x80);
    D_801050D8->unkB0[arg0].x0 = 0xD0;
    D_801050D8->unkB0[arg0].u0 = 0;
    D_801050D8->unkB0[arg0].v0 = 0xF8;
    D_801050D8->unkB0[arg0].clut = 0;
    D_801050D8->unkB0[arg0].w = 0x40;
    D_801050D8->unkB0[arg0].h = 4;
    setlen(&D_801050D8->unkD8[arg0], 4);
    setcode(&D_801050D8->unkD8[arg0], 0x64);
    setRGB0(&D_801050D8->unkD8[arg0], 0x80, 0x80, 0x80);
    D_801050D8->unkD8[arg0].x0 = 0x110;
    D_801050D8->unkD8[arg0].u0 = 0;
    D_801050D8->unkD8[arg0].v0 = 0xFC;
    D_801050D8->unkD8[arg0].clut = 0;
    D_801050D8->unkD8[arg0].w = 0x30;
    D_801050D8->unkD8[arg0].h = 4;
    setlen(&D_801050D8->unk100[arg0], 1);
    *D_801050D8->unk100[arg0].code = 0xE100071F;
}

void func_800F27CC(s32 arg0) {
    Unk80105078 *p = func_800F3EA4();
    Window *w;
    D_800F8EFC.y = p->unk60;
    D_800F8EFC.h = p->unk5C * 12 + 8;
    w = win_open(&D_800F8EFC, 0xA, 0, 1);
    g_btlUi->unk12A4[1] = w;
    win_setDrawContent(w, func_800F29B4);
    win_setDrawOverlay(g_btlUi->unk12A4[1], func_800F1BFC);
    func_8002E334(g_btlUi->unk12A4[1], 7);
    func_800F3D3C(arg0);
    D_80105058.unk1 = arg0;
    D_80105058.unk0 = 1;
}

void func_800F2890(s32 arg0) {
    func_8002E380(g_btlUi->unk12A4[1], 7);
    win_close(g_btlUi->unk12A4[1]);
    func_800F3DE0(arg0);
    D_80105058.unk0 = 0;
}

/* resizes the battle list window to the current entry count */
void func_800F28F0(void) {
    Unk80105078 *p = func_800F3EA4();
    Window *w;
    RECT *r;
    s16 h;

    if (D_80105058.unk0 != 0 && p->unk5C != 0) {
        w = g_btlUi->unk12A4[1];
        r = &w->unk8;
        r->y = w->unk2 = p->unk60;
        h = p->unk5C * 12 + 8;
        w->unk6 = h;
        if (w->unk1C == 2 || w->unk1C == 3) {
            r->h = h;
        } else if (w->unk1C == 1) {
            if (h < r->h) {
                r->h = h;
            }
        }
    }
}

u8 func_800F29A4(void) {
    return D_80105058.unk0;
}

void func_800F29B4(Window *arg0) {
    if (func_800F3EA4()->unk0 != 0) {
        D_800F8F04[D_80105059](arg0);
    }
}

void func_800F2A0C(void) {
    D_80105060.unk4 = g_gameState->unk1C->unk7E0;
    D_80105060.unk0 = D_80105060.unk4;
    D_80105060.unk10 = 0x8000;
    D_80105060.unk8 = D_80105060.unk10;
    D_80105060.unkC = 0;
}

void func_800F2A58(void) {
}

void *func_800F2A60(s32 arg0) {
    s32 size = (arg0 + 3) >> 2 << 2;
    s32 p = D_80105060.unk4;
    D_80105060.unk4 += size;
    D_80105060.unkC += size;
    D_80105060.unk10 -= size;
    return (void *)p;
}

void func_800F2AAC(void) {
    BtlUnit *u = g_gameState->unk1C->unk8E4.battle->unk6F8;
    s32 n = 0;
    for (; u != NULL; u = u->unk0) {
        if (u->unk68 & 1) {
            D_80105078->unk68[n] = u;
            D_80105078->unk7C[n] = (btl_hasStatus(u, 0x42021D03) & 0xFF) != 0;
            D_80105078->unk80[n] = (u8)u->unk68 >> 5;
            if (btl_hasStatus(u, 0x101000) & 0xFF) {
                D_80105078->unk84[n] = 2;
            } else if (btl_hasStatus(u, 0x80000) & 0xFF) {
                D_80105078->unk84[n] = 3;
            } else {
                D_80105078->unk84[n] = 0;
            }
            if ((u->unk68 >> 30) & 1) {
                D_80105078->unk88[n] = (btl_hasStatus(u, 0x4000) & 0xFF) ? 2 : 1;
            } else {
                D_80105078->unk88[n] = 0;
            }
            n++;
        }
    }
    D_80105078->unk64 = g_btlUi->cmdUnit;
    D_80105078->unk5C = n;
    D_80105078->unk60 = D_800F9028[n - 1];
    D_80105078->unk1 = 0;
    D_80105078->unk2 = 0xFF;
}

void func_800F2C74(s32 arg0) {
    setPolyG4(&D_801050D8->unk310[arg0]);
    setSemiTrans(&D_801050D8->unk310[arg0], 1);
    setRGB0(&D_801050D8->unk310[arg0], 0x40, 0x40, 0);
    setRGB1(&D_801050D8->unk310[arg0], 0x10, 0x10, 0);
    setRGB2(&D_801050D8->unk310[arg0], 0x40, 0x40, 0);
    setRGB3(&D_801050D8->unk310[arg0], 0x10, 0x10, 0);
    D_801050D8->unk310[arg0].x0 = 0x8E;
    D_801050D8->unk310[arg0].x1 = 0x137;
    D_801050D8->unk310[arg0].x3 = 0x137;
    D_801050D8->unk310[arg0].x2 = 0x8E;
    setlen(&D_801050D8->unk358[arg0], 1);
    *D_801050D8->unk358[arg0].code = 0xE1000620;
}

void func_800F2DC0(s32 arg0) {
    s32 i;
    SPRT *s;
    for (i = 0; i < 4; i++) {
        setlen(&D_801050D8->unk110[i][arg0], 4);
        setcode(&D_801050D8->unk110[i][arg0], 0x64);
        setRGB0(&D_801050D8->unk110[i][arg0], 0x80, 0x80, 0x80);
        D_801050D8->unk110[i][arg0].x0 = 0x92;
        D_801050D8->unk110[i][arg0].h = 0xC;
        D_801050D8->unk110[i][arg0].w = 0x54;
        if (i & 1) {
            D_801050D8->unk110[i][arg0].u0 = 0x54;
        } else {
            D_801050D8->unk110[i][arg0].u0 = 0;
        }
        D_801050D8->unk110[i][arg0].v0 = 0xF0;
    }
    setlen(&D_801050D8->unk2F0[arg0], 1);
    *D_801050D8->unk2F0[arg0].code = 0xE100061E;
}

void func_800F2ED0(s32 arg0) {
    s32 i;
    SPRT *s;
    for (i = 0; i < 4; i++) {
        setlen(&D_801050D8->unk1B0[i][arg0], 4);
        setcode(&D_801050D8->unk1B0[i][arg0], 0x64);
        setRGB0(&D_801050D8->unk1B0[i][arg0], 0x80, 0x80, 0x80);
        D_801050D8->unk1B0[i][arg0].x0 = 0xD0;
        D_801050D8->unk1B0[i][arg0].u0 = 0;
        D_801050D8->unk1B0[i][arg0].w = 0x1D;
        D_801050D8->unk1B0[i][arg0].h = 0xA;
        D_801050D8->unk1B0[i][arg0].clut = 0;
        setlen(&D_801050D8->unk250[i][arg0], 4);
        setcode(&D_801050D8->unk250[i][arg0], 0x64);
        setRGB0(&D_801050D8->unk250[i][arg0], 0x80, 0x80, 0x80);
        D_801050D8->unk250[i][arg0].x0 = 0xEF;
        D_801050D8->unk250[i][arg0].u0 = 0x20;
        D_801050D8->unk250[i][arg0].w = 0x16;
        D_801050D8->unk250[i][arg0].h = 0xA;
        D_801050D8->unk250[i][arg0].clut = 0;
    }
    setlen(&D_801050D8->unk300[arg0], 1);
    *D_801050D8->unk300[arg0].code = 0xE100071F;
}

void btlui_drawPartyPanel(void) {
    u32 i;
    s32 j;
    if (D_80105078->unk0 != 0 && D_80105078->unk5C != 0) {
        btlui_drawPanelHighlight();
        btlui_drawPanelHeader();
        for (i = 0; i < D_80105078->unk5C; i++) {
            for (j = 0; j < 4; j++) {
                btlui_updatePanelRowColor(i);
                if (D_80105078->unk4[j].unk4 != 0 && D_800F9038[j] != 0) {
                    D_800F9038[j](D_80105078->unk68[i], i);
                }
            }
        }
    }
}

void func_800F316C(void) {
    BtlUnit *u;
    Unk80104F88Entry *e;
    Unk80104F88Gauge *g;
    Unk80104F88Gauge *mp;
    s32 i;
    s32 cur;
    s32 max;
    s32 d;
    s32 a;
    s32 t;

    for (u = g_gameState->unk1C->unk8E4.battle->unk6F8; u != NULL; u = u->unk0) {
        if (!(u->unk68 & 1)) {
            continue;
        }
        e = u->unkB4;
        mp = &e->unk0[1];
        for (i = 0; i < 2; i++) {
            g = mp;
            if (i == 0) {
                g = &e->unk0[0];
            }
            if (g->unk6 != 0) {
                if (g->unk0 >= 0) {
                    if (g->unk2 + g->unk0 >= g->unk4) {
                        g->unk2 = g->unk4;
                        g->unk6 = 0;
                    } else {
                        g->unk2 += g->unk0;
                        g->unk6--;
                    }
                } else {
                    if (g->unk2 + g->unk0 <= g->unk4) {
                        g->unk2 = g->unk4;
                        g->unk6 = 0;
                    } else {
                        g->unk2 += g->unk0;
                        g->unk6--;
                    }
                }
            } else {
                cur = (i == 0) ? u->hp : u->mp;
                max = (i == 0) ? u->max->maxHp : u->max->maxMp;
                d = cur - g->unk2;
                if (d != 0) {
                    a = (d >= 0) ? d : -d;
                    g->unk4 = cur;
                    if (a < 0x10) {
                        g->unk6 = a;
                    } else {
                        g->unk6 = (a << 6) / max;
                        if (g->unk6 < 0x10) {
                            a = 0x10;
                            g->unk6 = a;
                        }
                    }
                    if (d >= 0) {
                        t = d - 1;
                        g->unk0 = (t + g->unk6) / g->unk6;
                    } else {
                        t = d + 1;
                        g->unk0 = (t - g->unk6) / g->unk6;
                    }
                }
            }
        }
    }
}

void btlui_drawPanelHeader(void) {
    RECT r;
    s32 i;
    Unk800F412C *e;

    ui_drawRule(0x8E, D_80105078->unk60, 8, 1, 0xA9, (g_gameState->unk24 >> 18) & 1);
    r.x = 0x8E;
    r.y = D_80105078->unk60 - 3;
    r.w = g_gameState->unkC->disp.w - 0x8E;
    r.h = 7;
    win_setClip(NULL, 7, 4);
    e = D_80105078->unk4;
    for (i = 0; i < 5; e++, i++) {
        if (e->unk4 != 0) {
            ui_drawLabel((s16)e->unk8, D_80105078->unk60 + (s16)e->unkA, 7,
                          ((s32 *)&D_800703FC)[e->unk0], 0);
        }
    }
    win_setClip(&r, 7, 4);
}

void btlui_drawPanelHighlight(void) {
    u8 c = D_80105078->unk2;
    POLY_G4 *p;
    s32 y;
    s32 d = 4;
    if (c != 0xFF && D_80104E77 != 0) {
        y = D_80105078->unk60 + (c * 12 + d);
        p = &D_801050D8->unk310[g_drawBufs.unk0];
        p->y0 = p->y1 = y;
        p->y2 = p->y3 = y + 13;
        addPrim(&g_ot[20], p);
        addPrim(&g_ot[20], &D_801050D8->unk358[g_drawBufs.unk0]);
    }
}

void btlui_drawPanelName(s32 arg0, u32 arg1) {
    s32 k = D_80105078->unk80[arg1];
    SPRT *s = &D_801050D8->unk110[k][g_drawBufs.unk0];
    s->y0 = D_80105078->unk60 + arg1 * 12 + 3;
    s->clut = D_800F9048[(u8)D_80105078->unk78[arg1]] + ((k >> 1) << 6);
    addPrim(&g_ot[16], s);
    if (arg1 >= D_80105078->unk5C - 1) {
        addPrim(&g_ot[16], &D_801050D8->unk2F0[g_drawBufs.unk0]);
    }
}

void btlui_drawPanelGauges(BtlUnit *arg0, s32 arg1) {
    s32 m = D_80105078->unk88[arg1];
    u8 f = 0;
    u32 y = D_80105078->unk60 + arg1 * 12;
    if (arg0->unkB3 == 0) {
        f = D_80105078->unk7C[arg1] == 0;
    }
    if (m == 2) {
        ui_drawGauge(0x10C, y + 10, 14, arg0->unk78, 0xFF, 1, f);
        ui_drawGauge(0x108, y + 6, 14, arg0->atb, arg0->max->maxAtb, D_80105078->unk84[arg1], f);
    } else if (m == 1) {
        ui_drawGauge(0x108, y + 6, 14, arg0->atb, arg0->max->maxAtb, D_80105078->unk84[arg1], f);
        ui_drawGauge(0x10C, y + 10, 14, arg0->unk78, 0xFF, m, f);
    } else {
        ui_drawGauge(0x108, y + 8, 14, arg0->atb, arg0->max->maxAtb, D_80105078->unk84[arg1], f);
    }
}

void func_800F386C(void) {
    RECT a = D_800A858C;
    RECT b = D_800A8594;
    s16 h;
    if (D_80105078->unk0 != 0 && D_80105078->unk5C != 0) {
        a.x += D_801051E8[g_gameState->unk1C->unk8 & 1][0];
        b.x += D_801051E8[g_gameState->unk1C->unk8 & 1][0];
        a.y += D_801051E8[g_gameState->unk1C->unk8 & 1][1] + D_80105078->unk60;
        b.y += D_801051E8[g_gameState->unk1C->unk8 & 1][1] + D_80105078->unk60;
        h = D_80105078->unk5C * 12;
        b.h = h;
        a.h = h;
        SetDrawMove((DR_MOVE *)g_primPtr, &a, 0x3C0, 0x1B0);
        addPrim(&g_ot[19], g_primPtr);
        g_primPtr += sizeof(DR_MOVE);
        SetDrawMove((DR_MOVE *)g_primPtr, &b, 0x3E0, 0x1B0);
        addPrim(&g_ot[19], g_primPtr);
        g_primPtr += sizeof(DR_MOVE);
    }
}

void func_800F3A8C(void) {
    s32 y = D_80105078->unk60 + 6;
    u8 v = 0xB0;
    u32 i;
    SPRT *a;
    SPRT *b;
    if (D_80105078->unk0 != 0 && D_80105078->unk5C != 0) {
        for (i = 0; i < D_80105078->unk5C; i++) {
            b = &D_801050D8->unk1B0[i][g_drawBufs.unk0];
            a = &D_801050D8->unk250[i][g_drawBufs.unk0];
            a->y0 = y;
            b->y0 = y;
            y += 12;
            a->v0 = v;
            b->v0 = v;
            v += 12;
            addPrim(&g_ot[18], b);
            addPrim(&g_ot[18], a);
        }
        addPrim(&g_ot[18], &D_801050D8->unk300[g_drawBufs.unk0]);
    }
}

void func_800F3C08(void) {
    s32 i;
    D_80105078 = func_800F2A60(0x8C);
    func_800F2AAC();
    for (i = 0; i < 5; i++) {
        D_80105078->unk4[i] = D_800F8FD8[i];
        func_800F40D0(&D_80105078->unk4[i]);
    }
    for (i = 0; i < D_80105078->unk5C; i++) {
        func_800F4010(i);
    }
    D_80105078->unk54 = 0;
    D_80105078->unk0 = 1;
    D_80105078->unk64 = 0;
}

void func_800F3D0C(void) {
    func_800F3F10();
    func_800F316C();
    func_800F3FDC();
}

void func_800F3D3C(s32 arg0) {
    func_800F412C(&D_80105078->unk4[1], &D_800F9058);
    func_800F412C(&D_80105078->unk4[2], &D_800F9058);
    func_800F412C(&D_80105078->unk4[3], &D_800F9058);
    func_800F412C(&D_80105078->unk4[4], &D_800F9064);
    D_80105078->unk4[4].unk0 = D_800F9070[arg0].unk0;
    btlui_setMenuState(4);
}

void func_800F3DE0(s32 arg0) {
    D_80105078->unk58 = arg0;
    func_800F412C(&D_80105078->unk4[1], &D_800F9084);
    func_800F412C(&D_80105078->unk4[2], &D_800F9084);
    func_800F412C(&D_80105078->unk4[3], &D_800F9084);
    func_800F412C(&D_80105078->unk4[4], &D_800F9090);
    btlui_setMenuState(4);
}

void func_800F3E60(void) {
    func_800F2AAC();
    if (D_80105078->unk64 != 0) {
        func_800F42F0();
    }
}

Unk80105078 *func_800F3EA4(void) {
    return D_80105078;
}

u8 func_800F3EB4(void) {
    return D_80105078->unk0;
}

void func_800F3ECC(u8 arg0) {
    D_80105078->unk0 = arg0;
}

void func_800F3EDC(s32 arg0) {
    func_800F2C74(arg0);
    func_800F2DC0(arg0);
    func_800F2ED0(arg0);
}

void func_800F3F10(void) {
    s32 i;
    Unk800F412C *e = D_80105078->unk4;
    for (i = 0; i < 5; i++) {
        if (e->unkC != 0) {
            e->unk8 += e->unkC->unk0;
            e->unkA += e->unkC->unk2;
            if (++e->unk6 >= e->unkC->unk4) {
                e->unk6 = 0;
                if (e->unkC->unk8 != 0) {
                    e->unkC->unk8(e);
                }
                e->unkC = 0;
            }
        }
        e++;
    }
}

void func_800F3FDC(void) {
    Unk80105078 *p = D_80105078;
    u32 v = D_80104E80 & 0x3F;
    p->unk54 = v;
    if (v >= 0x20) {
        p->unk54 = 0x3F - v;
    }
}

void func_800F4010(s32 arg0) {
    BtlUnit *u = D_80105078->unk68[arg0];
    Unk80104F88Entry *e = u->unkB4;
    Unk80104F88Gauge *g;
    s16 v;
    e->unk0[0].unk4 = e->unk0[0].unk2 = u->hp;
    v = u->mp;
    g = &e->unk0[1];
    g->unk6 = 0;
    g->unk4 = g->unk2 = v;
    e->unk0[0].unk6 = 0;
    e->unk0[1].unk0 = 0;
    e->unk0[0].unk0 = 0;
}

void btlui_updatePanelRowColor(s32 arg0) {
    BtlUnit *u = D_80105078->unk68[arg0];
    s32 hp = u->hp;
    if (hp == 0) {
        D_80105078->unk78[arg0] = 3;
        return;
    }
    D_80105078->unk78[arg0] = ((s32)(u16)(u->max->maxHp / 6) >= hp) << 2;
}

void func_800F40D0(Unk800F412C *arg0) {
    if (arg0->unk2 == 1) {
        arg0->unk8 -= func_8002AE7C(((u8 **)&D_800703FC)[arg0->unk0]);
    }
}

void func_800F412C(Unk800F412C *arg0, Unk800F3F10 *arg1) {
    arg0->unkC = arg1;
    arg0->unk4 = 1;
    arg0->unk6 = 0;
}

void func_800F4140(void) {
    D_80105078->unk4[1].unk4 = 0;
    D_80105078->unk4[2].unk4 = 0;
    D_80105078->unk4[3].unk4 = 0;
    btlui_setMenuState(3);
}

void func_800F4180(void) {
    D_80105078->unk4[4].unk4 = 0;
    btlui_setMenuState(D_80105078->unk58);
}

void btlui_drawPanelHp(BtlUnit *arg0, s32 arg1) {
    u8 buf[8];
    s32 dy = 6;
    text_formatNumber(arg0->unkB4->unk0[0].unk2, buf, 4, 1);
    ui_drawDigits(0xD0, D_80105078->unk60 + (arg1 * 12 + dy), 0xE, buf, (u8)D_80105078->unk78[arg1]);
}

void btlui_drawPanelMp(BtlUnit *arg0, s32 arg1) {
    u8 buf[8];
    s32 mp;
    s32 c;
    s32 dy = 6;
    mp = arg0->unkB4->unk0[1].unk2 < 0 ? 0 : arg0->unkB4->unk0[1].unk2;
    c = ((s16)(arg0->max->maxMp / 6) >= mp) << 2;
    text_formatNumber(mp, buf, 3, 1);
    ui_drawDigits(0xEF, D_80105078->unk60 + (arg1 * 12 + dy), 0xE, buf, c);
}

void func_800F42F0(void) {
    BtlUnit *u = g_gameState->unk1C->unk8E4.battle->unk6F8;
    s32 i = 0;
    if (D_80105078->unk5C != 0 && D_80104E70 == 0) {
        for (; u != 0; u = u->unk0) {
            if (u->unk68 & 1) {
                if (D_80105078->unk64 == u) {
                    D_80105078->unk2 = i;
                    return;
                }
                i++;
            }
        }
    }
}

void func_800F4384(void) {
    GameData *g = g_gameState->unk1C;
    Unk80068EE8 *s;
    s32 f;

    if (!(g->unk0 & 0x100) && !(g->unk8E4.battle->unk0 & 0x20) && D_80104E68.unk0 != 0 &&
        (f = (g_gameState->unk24 >> 5) & 1) == 1) {
        ClearOTag(&D_801050D4, 1);
        if (D_80104E68.unk18 & 0x20) {
            if (D_80104E68.unk15 == 0) {
                s = &D_80068ED8;
            } else {
                s = &D_80068EE0;
            }
            setlen(&D_801050C0, 4);
            D_801050C0.rgbc = 0x64808080;
            D_801050C0.xy = 0x6012B;
            D_801050C0.uvclut = s->unk0;
            D_801050C0.wh = s->unk4 | (s->unk5 << 16);
            addPrim(&D_801050D4, &D_801050C0);
        }
        if (D_80104E7D == 0) {
            s = &D_80068E90;
        } else {
            s = &D_80068ED0;
        }
        setlen(&D_801050A0, 4);
        D_801050A0.rgbc = 0x64808080;
        D_801050A0.xy = 0x60124;
        D_801050A0.uvclut = s->unk0;
        D_801050A0.wh = s->unk4 | (s->unk5 << 16);
        addPrim(&D_801050D4, &D_801050A0);
        setlen(&D_80105080, 1);
        D_80105080.code[0] = 0xE1000600 | (s->unk6 & 0x9FF);
        addPrim(&D_801050D4, &D_80105080);
        setlen(&D_80105090, 2);
        D_80105090.code[0] = 0xE6000001;
        D_80105090.code[1] = 0;
        addPrim(&D_801050D4, &D_80105090);
        DrawOTag(&D_801050D4);
        DrawSync(0);
    }
}

void func_800F4630(void) {
    s32 i;
    s32 j;
    u8 *row;
    func_8002C4B0(1);
    i = 0;
    row = D_80104E94[0];
    for (; i < 2; i++, row += 0x18) {
        for (j = 0; j < 0x17; j++) {
            row[j] = func_8002C700(i, j);
        }
        func_8002C740(i, 2, 0);
        func_8002C740(i, 3, 0);
        func_8002C8E8(i, 0xE, 0xF, 0xA);
        func_8002C740(i, 0xE, 1);
        func_8002C64C(i);
    }
    D_80104E76 = func_800F47D8();
}

void func_800F4700(void) {
    s32 i, j;
    u8 *p;
    for (i = 0, p = D_80104E94[0]; i < 2; i++) {
        for (j = 0; j < 0x17; j++) {
            func_8002C740(i, j, p[j]);
        }
        p += 0x18;
    }
    func_8002C4B0(0);
}

void btlui_readPad(void) {
    pad_update();
    func_8002C308(D_80104E68.unk15);
    if ((g_padRepeat & 8) && D_80104E68.unkB == 0) {
        D_80104E68.unkB = 1;
    }
}

u8 func_800F47D8(void) {
    s32 i;
    s32 f = (g_gameState->unk24 >> 5) & 1;
    if (f != 1) {
        return 0;
    }
    for (i = 0; i < 4; i++) {
        Character *c = g_gameState->unk1C->party[i];
        if (c == 0) {
            continue;
        }
        if (((s32)(g_gameState->unk24 >> 20) >> chr_getDefaultSet(c)) & 1) {
            return 1;
        }
    }
    return 0;
}

void func_800F4884(void) {
    s32 i;
    D_801050D8 = func_800F2A60(0x368);
    for (i = 0; i < 2; i++) {
        func_800ECCA8(i);
        func_800F2684(i);
        func_800F3EDC(i);
    }
}

void func_800F48DC(void) {
}

void func_800F48E4(void) {
    u32 *a = func_800F2A60(0xC0);
    s32 b = (s32)func_800F2A60(0x4000);
    DRAWENV *e;
    func_8002BEEC(a, a + 0x18, b, b + 0x2000);
    func_8002C064(2);
    func_8002C138(g_gameState->unk1C->unk8);
    D_801051E0 = 0;
    g_ot = g_drawBufs.unk14[0];
    g_primPtr = g_drawBufs.unkC[0];
    e = g_gameState->unk10;
    e[1].dfe = 1;
    e[0].dfe = 1;
    D_801051E8[0][0] = g_gameState->unkC[0].disp.x;
    D_801051E8[0][1] = g_gameState->unkC[0].disp.y;
    D_801051E8[1][0] = g_gameState->unkC[1].disp.x;
    D_801051E8[1][1] = g_gameState->unkC[1].disp.y;
    ClearOTagR(g_drawBufs.unk14[0], 0x18);
    ClearOTagR(g_drawBufs.unk14[1], 0x18);
    DrawSync(0);
}

void btlui_submitOt(void) {
    RECT *tw;
    Unk80104E68 *p;

    setlen(&D_80105140[D_801051E0], 2);
    D_80105140[D_801051E0].code[0] = 0xE6000001;
    D_80105140[D_801051E0].code[1] = 0;
    setlen(&D_80105120[D_801051E0], 2);
    ((u32 *)&D_80105120[D_801051E0])[1] = 0xE1000600;
    tw = &D_800FDE34;
    ((u32 *)&D_80105120[D_801051E0])[2] = get_tw(tw);
    SetDrawOffset(&D_801050E0[D_801051E0], D_801051E8[g_drawBufs.unk0 ^ 1]);
    SetDrawArea(&D_80105100[D_801051E0], &g_gameState->unkC[g_drawBufs.unk0 ^ 1].disp);
    addPrim(&g_ot[0x17], &D_80105140[D_801051E0]);
    addPrim(&g_ot[0x17], &D_80105120[D_801051E0]);
    addPrim(&g_ot[0x17], &D_801050E0[D_801051E0]);
    addPrim(&g_ot[0x17], &D_80105100[D_801051E0]);
    addPrim(&g_ot[1], &D_80105160[D_801051E0]);
    p = (Unk80104E68 *)&D_80104E68;
    if (p->unk10 == 0) {
        s32 r = BreakDraw();
        if (r == -1) {
            p->unk24++;
        } else {
            while (IsIdleGPU(1) != 0) {
            }
            GetDrawEnv2(&D_80105160[D_801051E0]);
            ContinueDraw(&g_ot[0x17], r);
        }
    }
}

void func_800F4D20(void) {
    DRAWENV *env = g_gameState->unk10;
    env[1].dfe = 0;
    env[0].dfe = 0;
}

void btlui_beginFrame(void) {
    D_801051E0 ^= 1;
    gfx_selectBuffer(D_801051E0);
    func_800F4D80(g_ot, 0x18);
}

void func_800F4D80(u32 *arg0, u32 arg1) {
    u32 i;
    ClearOTag(arg0, 1);
    for (i = 1; i < arg1; i++) {
        arg0[i] = (u32)&arg0[i - 1] & 0xFFFFFF;
    }
}

void func_800F4DF4(void) {
}

void func_800F4DFC(void) {
    RECT *r;
    s32 v;
    g_btlBanner.unk90 = 0;
    g_btlBanner.unk8C = 0;
    g_btlBanner.unk80 = 0;
    g_btlBanner.unk0 = 0xFF;
    v = D_800FDE3C[(g_gameState->unk24 >> 11) & 7];
    r = &g_btlBanner.unkA4;
    r->x = 0x60;
    r->y = 8;
    r->w = 0x80;
    r->h = 0x15;
    g_btlBanner.unk84 = v;
}

void func_800F4E68(void) {
}

u8 *btl_drawBanner(u8 *arg0) {
    if (g_btlBanner.unk8C != 0) {
        g_btlBanner.unk98 = (u32 *)func_800F4FB4(g_gameState->unk1C->unk8);
        arg0 = btl_buildBanner(arg0);
        if ((u32)++g_btlBanner.unk80 >= (u32)g_btlBanner.unk84) {
            g_btlBanner.unk8C = 0;
            g_btlBanner.unk90 = 0;
        }
    }
    return arg0;
}

void btl_setBanner(s32 arg0, s32 arg1) {
    if (arg1 >= g_btlBanner.unk90) {
        g_btlBanner.unk90 = arg1;
        g_btlBanner.unk8D = 0;
        func_800F4FE8(arg0);
    }
}

void func_800F4F44(s32 arg0, s32 arg1) {
    if (arg1 >= g_btlBanner.unk90) {
        g_btlBanner.unk90 = arg1;
        g_btlBanner.unk8D = 1;
        func_800F4FE8(arg0);
    }
}

void func_800F4F84(RECT *arg0) {
    g_btlBanner.unkA4 = *arg0;
}

s32 *func_800F4FB4(s32 arg0) {
    return &D_8010528C[arg0];
}

s32 func_800F4FC8(void) {
    return D_80105274;
}

u8 func_800F4FD8(void) {
    return D_8010527C;
}

void func_800F4FE8(s32 arg0) {
    Unk801051F088 v;
    if (arg0 == 0) {
        g_btlBanner.unk0 = 0xFF;
        g_btlBanner.unk8C = 0;
        g_btlBanner.unk90 = 0;
    } else {
        func_8002BD54(&g_btlBanner.unk0, (u8 *)arg0);
        g_btlBanner.unk8C = 1;
        g_btlBanner.unk80 = 0;
        func_8002AF70(&v.unk0, (s32)&g_btlBanner);
        g_btlBanner.unk88 = v;
    }
}

/* draws the battle help text (g_btlBanner starts with the text buffer) centred in its window */
u8 *btl_buildBanner(u8 *arg0) {
    RECT *r;
    RECT *t;

    if (g_btlBanner.unk8C != 0 && g_btlBanner.unk0 != 0xFF) {
        if (g_btlBanner.unk8D != 0) {
            r = &g_btlBanner.unkA4;
        } else {
            t = &D_800FDE58;
            t->h = g_btlBanner.unk88.unk2 + 6;
            r = t;
        }
        arg0 = btl_buildBannerFrame(func_8002920C(r->x + ((r->w - g_btlBanner.unk88.unk0) >> 1), r->y + 3,
                                           g_btlBanner.unk98, &g_btlBanner.unk0, 0, arg0),
                             r);
    }
    return arg0;
}

u8 *btl_buildBannerFrame(u8 *arg0, RECT *arg1) {
    RECT r;
    u8 *v = win_buildBorder((s32)arg1, g_btlBanner.unk98, 1, 0, 1, (g_gameState->unk24 >> 18) & 1, arg0);
    r.x = arg1->x + 2;
    r.y = arg1->y + 2;
    r.w = arg1->w - 3;
    r.h = arg1->h - 3;
    return win_buildFill((s32)&r, g_btlBanner.unk98, 0, 0, (g_gameState->unk24 >> 18) & 1, v);
}

INCLUDE_RODATA("asm/us/ovl_0b/nonmatchings/ovl_0b_2", D_800A8584);

INCLUDE_RODATA("asm/us/ovl_0b/nonmatchings/ovl_0b_2", D_800A858C);

INCLUDE_RODATA("asm/us/ovl_0b/nonmatchings/ovl_0b_2", D_800A8594);
