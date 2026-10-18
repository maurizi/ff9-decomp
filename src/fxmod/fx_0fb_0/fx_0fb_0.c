#include "fxmod/fx_0fb_0.h"

void func_801E7720(SVECTOR *a, SVECTOR *b, u32 n, CVECTOR *ca, CVECTOR *cb, s32 p, Fx0fb0Seg *buf, s32 abr) {
    Unk800F78A4 *env = D_801EB3AC;
    CVECTOR c0;
    CVECTOR c1;
    s32 otz;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    POLY_G4 *prim;
    Fx0fb0Seg *s;
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

void func_801E7B90(SVECTOR *pos, SVECTOR *rot, VECTOR *scale, s32 col, s32 tpage, s32 clut, s32 u, s32 v, s32 amp,
                   s32 ph, s32 step, s32 abr) {
    Unk800F78A4 *env = D_801EB3AC;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    CVECTOR c0;
    CVECTOR c1;
    CVECTOR c2;
    MATRIX m;
    SVECTOR vs[4];
    s32 otz;
    s32 xo;
    s32 w;
    s32 i;
    s32 j;
    s32 x;
    s32 y0;
    s32 y1;
    s32 k;
    s32 xb;
    s32 yb;
    s32 t;
    s32 u0;
    s32 u1;
    s32 v0;
    s32 v1;
    s32 ft;
    POLY_GT4 *g;
    POLY_FT4 *f;

    gte_SetRotMatrix(env->unk14);
    gte_SetTransMatrix(env->unk14);
    gte_ldv0(pos);
    g = (POLY_GT4 *)env->unk24;
    *(s32 *)&c2 = 0;
    f = (POLY_FT4 *)(g + 64);
    env->unk24 = (u8 *)(f + 448);
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
    xb = -0x2EE0;
    yb = -0x2000;
    vs[0].vz = vs[1].vz = vs[2].vz = vs[3].vz = 0;
    for (w = 0, i = 0; i < 32; i++, w += 0x200) {
        ph += step;
        y0 = yb + w;
        y1 = y0 + 0x200;
        for (xo = 0, j = 0, k = i & 3; j < 16; j++, xo += 0x5DC) {
            x = xb + xo;
            vs[0].vx = x + ((ex->math_sin(ph) * amp) >> 12);
            vs[1].vx = vs[0].vx + 0x5DC;
            t = ex->math_sin(ph + step);
            vs[0].vy = vs[1].vy = y0;
            vs[2].vy = vs[3].vy = y1;
            x += (t * amp) >> 12;
            vs[2].vx = x;
            vs[3].vx = x + 0x5DC;
            gte_ldv3c(vs);
            gte_rtpt();
            u0 = u + (((j & 1) * 0x70) >> 1);
            u1 = u0 + 0x38;
            if (j & 1) {
                u1 = u0 + 0x37;
            }
            v0 = v + ((k * 0x70) >> 2);
            v1 = v0 + 0x1C;
            if (k == 3) {
                v1 = v0 + 0x1B;
            }
            if (j == 0) {
                *(s32 *)&g->r0 = *(s32 *)&g->r2 = *(s32 *)&c2;
                *(s32 *)&g->r1 = *(s32 *)&g->r3 = *(s32 *)&c1;
                ft = 0;
            } else if (j >= 15) {
                *(s32 *)&g->r1 = *(s32 *)&g->r3 = *(s32 *)&c2;
                *(s32 *)&g->r0 = *(s32 *)&g->r2 = *(s32 *)&c1;
                ft = 0;
            } else {
                ft = 1;
                *(s32 *)&f->r0 = *(s32 *)&c0;
            }
            gte_stopz(&otz);
            if (otz != 0) {
                if (ft) {
                    f->u0 = f->u2 = u0;
                    f->u1 = f->u3 = u1;
                    f->v0 = f->v1 = v0;
                    f->v2 = f->v3 = v1;
                    f->tpage = tpage;
                    f->clut = clut;
                    gte_stsxy3(&f->x0, &f->x1, &f->x2);
                } else {
                    g->u0 = g->u2 = u0;
                    g->u1 = g->u3 = u1;
                    g->v0 = g->v1 = v0;
                    g->v2 = g->v3 = v1;
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

void func_801E809C(SVECTOR *pos, s32 dy, s32 c, s32 tpage, s32 clut, s32 u, s32 v, s32 amp, s32 ph, s32 step,
                   s32 abr, s32 ph2) {
    Unk800F78A4 *env = D_801EB3AC;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR a;
    SVECTOR b;
    SVECTOR vs[4];
    s32 otz;
    s32 x;
    s32 t;
    s32 code;
    POLY_GT4 *p;
    s32 i;
    s32 c0;
    s32 c1;
    s32 u0;
    s32 u1;
    s32 v1;
    s32 nph;

    c1 = 0;
    gte_SetRotMatrix(env->unk14);
    gte_SetTransMatrix(env->unk14);
    p = (POLY_GT4 *)env->unk24;
    code = 0x3C;
    env->unk24 = (u8 *)(p + 44);
    if (abr != 0xFF) {
        code = 0x3E;
    }
    tpage |= (abr & 3) << 5;
    x = -0x6098;
    c0 = c >> 3;
    for (i = 0; i < 44; i++) {
        t = ((i + 1) << 12) / 44;
        if (i == 0) {
            BTLFX_COPY_SVEC(&a, pos);
            a.vx += x;
            a.vz += ex->math_sin(ph2) * 2;
            a.vz += (ex->math_sin(ph) * amp) >> 12;
            a.vx -= (ex->math_sin(ph) * amp) >> 13;
            c1 = c;
        }
        BTLFX_COPY_SVEC(&b, pos);
        b.vx += 0x464 + x;
        b.vz += ex->math_sin(t + ph2) * 2;
        nph = ph + step;
        b.vz += (ex->math_sin(nph) * amp) >> 12;
        b.vx -= (ex->math_sin(nph) * amp) >> 13;
        BTLFX_COPY_SVEC(&vs[0], &a);
        BTLFX_COPY_SVEC(&vs[2], &a);
        BTLFX_COPY_SVEC(&vs[1], &b);
        BTLFX_COPY_SVEC(&vs[3], &b);
        BTLFX_COPY_SVEC(&a, &b);
        vs[2].vy += dy;
        vs[3].vy += dy;
        gte_ldv3c(vs);
        gte_rtpt();
        u0 = u + (((i & 7) * 0x70) >> 3);
        u1 = u0 + 0xE;
        if ((i & 7) == 7) {
            u1 = u0 + 0xD;
        }
        v1 = v + 0x6F;
        gte_nclip();
        p->r1 = p->g1 = p->b1 = c0;
        p->r0 = p->g0 = p->b0 = c0;
        p->r2 = p->g2 = p->b2 = c1;
        c1 = ex->math_cos(t >> 2) >> 5;
        p->r3 = p->g3 = p->b3 = c1;
        p->code = code;
        gte_stopz(&otz);
        if (otz != 0) {
            p->u0 = p->u2 = u0;
            p->u1 = p->u3 = u1;
            p->v0 = p->v1 = v;
            p->v2 = p->v3 = v1;
            p->tpage = tpage;
            p->clut = clut;
            gte_stsxy3(&p->x0, &p->x1, &p->x2);
            gte_avsz3();
            gte_stszotz(&otz);
            gte_ldv0(&vs[3]);
            gte_rtps();
            ex->btlfx_addPrim(0x0C000000, otz, p);
            gte_stsxy(&p->x3);
            p++;
        }
        ph = nph;
        x += 0x464;
    }
}

s32 func_801E8540(s32 mode, Fx0fb0Rock *r, Unk800F78A4 *env) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR sc;

    if (mode == 1) {
        r->unk0 = 0;
        r->unk8 = 0;
        r->unk6 = ex->btlfx_randCentered(0x1800) - 0x3000;
        r->unk14.vx = 0;
        r->unk14.vy = 0;
        r->unk14.vz = 0;
        r->unk1C = ex->btlfx_rand() % 64 + 0x40;
        r->unk1E = 0x20 - ex->btlfx_rand() % 32;
    } else {
    if (r->unk4 < 0x19) {
        sc.vx = 0x1000;
        sc.vy = 0x1000;
        sc.vz = 0x1000;
    } else {
        sc.vx = 0x199;
        sc.vy = 0x199;
        sc.vz = 0x200;
    }
    pos.vx = r->unkC.vx;
    pos.vy = r->unkC.vy;
    pos.vz = r->unkC.vz;
    r->unk14.vx += r->unk1C;
    r->unk14.vy += r->unk1E;
    r->unkC.vx += ex->btlfx_randCentered(0x10);
    r->unkC.vz += ex->btlfx_randCentered(0x10);
    switch (r->unk0) {
    case 0:
        mode = (r->unk8 << 12) / 40; /* reuses the parameter */
        pos.vy += (ex->math_sin(mode >> 2) * r->unk6) >> 12;
        if (++r->unk8 >= 0x28) {
            r->unk0 = 1;
            r->unk8 = 0;
            r->unkC.vy = pos.vy;
            r->unk6 = ex->btlfx_rand();
        }
        break;
    case 1:
        pos.vy += ex->math_sin((r->unk8 << 7) + r->unk6) >> 3;
        r->unk8++;
        break;
    }
    if (D_801EB348 != 0 || (D_801EB344 != 0 && r->unk4 >= 0x19)) {
        return 1;
    }
    ex->Hi_DrawEffModel(&r->unk14, &pos, (s32)&sc, r->unk20);
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0fb_0/nonmatchings/fx_0fb_0", fx_0fb_0_headerPtr);

INCLUDE_ASM("asm/us/fx_0fb_0/nonmatchings/fx_0fb_0", fx_0fb_0_op80);

s32 fx_0fb_0_op81(s32 mode, Fx0fb0Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    s32 frame;
    s32 t;
    s32 i;

    if (mode == 0) {
        *arg4 = 0x10C;
    } else if (mode == 1) {
        D_801EB3AC = env;
        work->unk104 = env->unkC->unk24;
        work->unk0 = 0;
        for (i = 0; i < work->unk104; i++) {
            if (ex->btlfx_isEnemyUnit(i) != 0) {
                ex->btlfx_getUnitHomePos(i, (s16 *)&work->unk4[i]);
                work->unk108 = ex->btlfx_randCentered(0x400) - 0x3000;
                ex->btlfx_getUnitRot(i, (s16 *)&work->unk44[i]);
                BTLFX_COPY_SVEC(&work->unkC4[i], &work->unk44[i]);
                work->unk84[i].vx = ex->btlfx_randCentered(0x100);
                work->unk84[i].vy = ex->btlfx_randCentered(0x180);
                work->unk84[i].vz = ex->btlfx_randCentered(0x20);
            }
        }
    } else {
        frame = entry->unk0;
        if (work->unk0 == 0) {
            t = (frame << 12) / 46;
            for (i = 0; i < work->unk104; i++) {
                if (ex->btlfx_isEnemyUnit(i) != 0) {
                    BTLFX_COPY_SVEC(&v, &work->unk4[i]);
                    v.vy += (ex->math_sin(t >> 2) * work->unk108) >> 12;
                    ex->btlfx_setUnitPos(i, (s16 *)&v);
                    work->unkC4[i].vx += work->unk84[i].vx;
                    work->unkC4[i].vy += work->unk84[i].vy;
                    work->unkC4[i].vz += work->unk84[i].vz;
                    ex->btlfx_setUnitRot(i, (s16 *)&work->unkC4[i]);
                }
            }
            if (frame >= 0x2E) {
                work->unk0 = 1;
                entry->unk0 = -1;
            }
        } else {
            for (i = 0; i < work->unk104; i++) {
                if (ex->btlfx_isEnemyUnit(i) != 0) {
                    work->unkC4[i].vx += work->unk84[i].vx;
                    work->unkC4[i].vy += work->unk84[i].vy;
                    work->unkC4[i].vz += work->unk84[i].vz;
                    ex->btlfx_setUnitRot(i, (s16 *)&work->unkC4[i]);
                }
            }
            if (D_801EB348 != 0) {
                for (i = 0; i < work->unk104; i++) {
                    if (ex->btlfx_isEnemyUnit(i) != 0) {
                        ex->btlfx_setUnitRot(i, (s16 *)&work->unk44[i]);
                        ex->btlfx_getUnitHomePos(i, (s16 *)&v);
                        ex->btlfx_setUnitPos(i, (s16 *)&v);
                    }
                }
                return 1;
            }
        }
    }
    return 0;
}

s32 fx_0fb_0_op87(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    SVECTOR v;

    if (mode == 0) {
        *arg4 = 4;
    } else if (mode == 1) {
        v.vx = env->unkC->unk38;
        v.vy = env->unkC->unk3A;
        v.vz = env->unkC->unk3C;
        v.vy -= 0x230;
        env->unk2C->unk0[2].vx = v.vx;
        env->unk2C->unk0[2].vy = v.vy;
        env->unk2C->unk0[2].vz = v.vz;
        D_801EB3AC = env;
        return 1;
    } else {
        return 0;
    }
    return 0;
}
