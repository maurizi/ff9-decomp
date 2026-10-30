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
    prim = (POLY_G4 *)env->prim;
    env->prim = (s32)(prim + n);
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
    gte_SetRotMatrix(env->viewMatrix);
    gte_SetTransMatrix(env->viewMatrix);
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

    gte_SetRotMatrix(env->viewMatrix);
    gte_SetTransMatrix(env->viewMatrix);
    gte_ldv0(pos);
    g = (POLY_GT4 *)env->prim;
    *(s32 *)&c2 = 0;
    f = (POLY_FT4 *)(g + 64);
    env->prim = (u8 *)(f + 448);
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
    ex->math_rotMatrixYXZ(rot, &m);
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
    gte_SetRotMatrix(env->viewMatrix);
    gte_SetTransMatrix(env->viewMatrix);
    p = (POLY_GT4 *)env->prim;
    code = 0x3C;
    env->prim = (u8 *)(p + 44);
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

/* Case 2's unk64 block: xs (s16) and ys as block locals computed right after the sin call (xs -= cc in place
 * makes post-reload cse turn the env->camera reload into the original's move), then empty do-while barriers around
 * `cc = (c_1 * 3) / 2;` that split the rest into the original's two scheduling groups. Other levers are listed in
 * the history of docs/wip/fx_0fb_0_op80.c. */
s32 fx_0fb_0_op80(s32 mode, Fx0fb0Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx0fb0Rock *r;
    Pool6CEntry *p;
    s16 *s;
    s16 *s_0;
    s16 *s_1;
    SVECTOR *rp;
    s32 frame;
    s32 i;
    s32 i0;
    s32 i3;
    s32 i11;
    s32 i2;
    s32 i1;
    s32 j;
    s32 k;
    s32 u;
    s32 u_0;
    s32 u_1;
    s32 u_2;
    s32 u_4;
    s32 u_3;
    s32 v;
    s32 v_0;
    s32 v_1;
    s32 vb;
    s32 vb_0;
    s32 vb_1;
    s32 c;
    s32 c_0;
    s32 c_1;
    s32 c_4;
    s32 c_5;
    s32 c_6;
    s32 c_7;
    s32 w;
    s32 q2;
    s32 x;
    s32 x_x0;
    s32 y;
    s32 y_0;
    s32 y_1;
    s32 big;
    s32 idx;
    s32 a8;
    s32 ac;
    s32 b0;
    s32 b4;
    s32 b8;
    s32 bc;
    s32 t;

    if (mode == 0) {
        init->workSize = 0x128;
        init->elemSize = 0x28;
        init->elemFn = (s32)func_801E8540;
        init->poolCount = 0x32;
    } else if (mode == 1) {
        work->unk0 = 0;
        work->unk4 = 0;
        D_801EB3AC = env;
        ex->btlfx_initEffModels(SUMMON0FB(env)->unk38, 0x1F);
        work->unkC.vy = 0;
        work->unkC.vx = 0;
        work->unkC.vz = 0;
        work->unkF0.vy = -0x400;
        work->unkE4.vx = 0x1800;
        work->unkE4.vy = 0x1800;
        work->unkF0.vx = 0;
        work->unkF0.vz = 0;
        work->unkE4.vz = 0x2000;
        work->unkD8.vx = work->unkC.vx;
        work->unkD8.vy = work->unkC.vy;
        work->unkD8.vz = work->unkC.vz;
        work->unkD8.vy -= 0x5000;
        env->camera->points[3].vx = work->unkC.vx;
        env->camera->points[3].vy = work->unkC.vy;
        env->camera->points[3].vz = work->unkC.vz;
        FX0FB_W(work->unk5C, 0) = FX0FB_W(work->unkC, 0);
        FX0FB_W(work->unk5C, 1) = FX0FB_W(work->unkC, 1);
    } else {
        a8 = 0;
        ac = 0;
        b0 = 0;
        b4 = 0;
        b8 = 0;
        bc = 0;
        frame = entry->frame;
        switch (work->unk0) {
        case 0:
            a8 = (frame << 12) / 30;
            work->unk5C.vy = work->unkC.vy - ((a8 << 9) >> 12);
            env->camera->points[3].vx = work->unk5C.vx;
            env->camera->points[3].vy = work->unk5C.vy;
            env->camera->points[3].vz = work->unk5C.vz;
            env->camera->points[2].vx = env->camera->eye.vx;
            env->camera->points[2].vy = env->camera->eye.vy;
            env->camera->points[2].vz = env->camera->eye.vz;
            work->unk64.vx = env->camera->eye.vx;
            work->unk64.vy = env->camera->eye.vy;
            work->unk64.vz = env->camera->eye.vz;
            if (frame >= 0xE) {
                ex->btlfx_drawRippleGrid((s32)(env->ot + 2), 0xF0 - (((frame - 0xE) * 0xE600) >> 12), 8, 2, 0x78, 1,
                                  D_801EB34C, 0x20, work->unk4 << 7, 0x100, 0x400);
            }
            if (frame == 0x18) {
                work->unk24.vx = D_801EB248.vx;
                work->unk24.vy = D_801EB248.vy;
                work->unk24.vz = D_801EB248.vz;
                work->unk24.vy += 0x640;
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->unk24);
                if (p != NULL) {
                    p->unk22 = 0x20;
                }
            }
            if (frame >= 0x1E) {
                work->unk0 = 1;
                entry->frame = -1;
                for (i0 = 0; i0 < 27; i0++) {
                    work->unk6C[i0] = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, D_801EB274[i0]),
                                                                0x9A, 0x3E00, 0, 0);
                }
                work->unk8 = 0;
                work->unk118 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 0x82), 0x96, 0x3D40, 0, 0);
                work->unk11C = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 0x87), 0x9A, 0x3E00, 0, 0);
                work->unk120 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 0x84), 0x7C, 0x3D02, 0, 0);
                work->unk124 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 0x94), 0xBD, 0x3E40, 0, 0);
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xB), (s32)&work->unk1C);
                if (p != NULL) {
                    p->unk22 = 0x180;
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xC), (s32)&work->unk14);
                if (p != NULL) {
                    p->unk22 = 0x200;
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->unk1C);
                if (p != NULL) {
                    p->unk22 = 0x100;
                }
                frame = 0;
            }
            break;
        case 1:
            t = (frame << 12) / 55;
            if (frame < 8) {
                a8 = ex->math_cos((frame << 10) >> 3);
            }
            ac = ex->math_sin(t >> 2);
            b0 = 0x1000;
            b4 = 0x78;
            if (frame < 0x19) {
                ex->btlfx_drawRippleGrid((s32)(env->ot + 2), ((((frame << 12) / 24) * 0x118) >> 12) + 0xA, 8, 2, 0x8C, 1,
                                  D_801EB34C, 0x20, work->unk4 << 7, 0x100, 0x400);
            }
            if (frame == 0) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0, 0x23, 0);
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, 0xFF, 0xFF, 0);
            } else if (frame < 0xC) {
                c_0 = 0xFF - ((frame - 1) * 0xFF) / 11;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, c_0 >> 1, c_0, c_0);
            }
            work->unk5C.vy = work->unkC.vy + (-((0x1000 - ex->math_cos(t >> 1)) * 0x3000) >> 13) - 0x180;
            env->camera->points[3].vx = work->unk5C.vx;
            env->camera->points[3].vy = work->unk5C.vy;
            env->camera->points[3].vz = work->unk5C.vz;
            work->unk64.vy += frame * 10;
            work->unk64.vz -= frame * 9;
            env->camera->points[2].vx = work->unk64.vx;
            env->camera->points[2].vy = work->unk64.vy;
            env->camera->points[2].vz = work->unk64.vz;
            work->unk14.vy = work->unkC.vy + (-(t * 0x1800) >> 12) - 0x100;
            work->unk1C.vy = work->unkC.vy + (-(t << 13) >> 12) - 0x100;
            if (frame < 0x1E) {
                for (k = 0; frame < 0xD ? k < 2 : k < 1; k++) {
                    if (work->unk8 < 0x2F) {
                        r = ex->btlfx_poolSpawn((s16 *)entry->pool);
                        if (r != NULL) {
                            big = work->unk8 >= 25;
                            idx = D_801EB2AC[work->unk8 % 25];
                            r->unk4 = idx + big * 25;
                            if (big) {
                                r->unk20 = work->unk6C[(idx & 1) + 25];
                            } else {
                                r->unk20 = work->unk6C[idx];
                            }
                            r->unkC.vx = D_801EB248.vx;
                            r->unkC.vy = D_801EB248.vy;
                            r->unkC.vz = D_801EB248.vz;
                            r->unkC.vx -= ((D_801EB2E0[idx * 2] << 2) * (big + 3)) >> 1;
                            if (!big) {
                                r->unkC.vy += 0x300;
                            } else {
                                r->unkC.vy = work->unk64.vy + 0x300;
                            }
                            r->unkC.vz -= ((D_801EB2E0[idx * 2 + 1] << 2) * (big + 3)) >> 1;
                            work->unk8++;
                        }
                    }
                }
            }
            if (frame >= 0x37) {
                work->unk0 = 2;
                entry->frame = -1;
                D_801EB344 = 1;
                ex->Hi_SetSummonMotion(SUMMON0FB(env)->unk54->unk180[0], SUMMON0FB(env)->unk60);
                ex->Hi_SetSummonMotFrame(SUMMON0FB(env)->unk60, 0);
                work->unk44.vy = 0xA70;
                work->unk38.vx = 0x1000;
                work->unk38.vy = 0x1000;
                work->unk38.vz = 0x1000;
                work->unk2C.vx = work->unkC.vx - 0x5780;
                work->unk2C.vz = work->unkC.vz - 0x3880;
                work->unk2C.vy = work->unkC.vy - 0x16DD;
                work->unk44.vx = 0;
                work->unk44.vz = 0;
                work->unk14.vx = work->unkC.vx;
                work->unk14.vz = work->unkC.vz;
                work->unk14.vy = work->unkC.vy - 0x2800;
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->unk14);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
                work->unk104.vx = 0x2000;
                work->unk104.vy = 0x555;
                work->unk104.vz = 0x2000;
                work->unk110.vx = 0;
                work->unk110.vy = 0;
                work->unk110.vz = 0;
                work->unkF8.vx = work->unkD8.vx + 0x2000;
                work->unkF8.vz = work->unkD8.vz + 0x2000;
                work->unkF8.vy = work->unkC.vy - 0x1500;
            }
            break;
        case 2:
            ac = 0x1000;
            b0 = 0x1000;
            b4 = 0x78;
            t = (frame << 12) / 40;
            bc = ex->math_sin(t >> 2);
            if (frame >= 0x20) {
                ex->Hi_DrawEffModel(&work->unk44, (VECTOR *)&work->unk2C, (s32)&work->unk38, work->unk11C);
                b8 = ((frame - 0x20) << 12) >> 3;
            }
            u_0 = t >> 1;
            work->unk5C.vx = work->unkC.vx;
            work->unk5C.vz = work->unkC.vz;
            work->unk5C.vy = work->unkC.vy - 0x2800;
            q2 = (b0 - ex->math_cos(u_0)) >> 1;
            {
            s16 xs; s32 ys; s32 q4; s32 cc;
            c_1 = ex->math_sin(u_0);
            q4 = q2 * 4;
            xs = work->unkC.vx - (q4 + q2);
            ys = q2 - 0x2800;
            do { } while (0);
            cc = (c_1 * 3) / 2;
            do { } while (0);
            work->unk64.vz = work->unkC.vz - q4 + 0x800;
            cc += 0x800;
            xs -= cc;
            work->unk64.vx = xs;
            work->unk64.vy = work->unkC.vy + ys;
            }
            env->camera->points[2].vx = work->unk64.vx;
            env->camera->points[2].vy = work->unk64.vy;
            env->camera->points[2].vz = work->unk64.vz;
            if (frame >= 0x28) {
                work->unk0 = 3;
                entry->frame = -1;
            }
            break;
        case 3: {
            SVECTOR pos;
            struct { s32 x; } z1 = { frame };

        {
            ac = 0x1000;
            b0 = 0x1000;
            bc = 0x1000;
            b4 = 0x78;
            b8 = 0x1000;
            y_1 = x_x0 = 0;
            if (frame < 4) {
                x_x0 = 0x400 - (frame << 10) / 3;
            } else if (frame < 0xB) {
                u_1 = ((frame - 3) << 10) >> 3;
                v_0 = ex->math_cos(u_1);
                y_1 = (v_0 >> 8) * ((frame & 1) * 2 - 1) + ((b8 - ex->math_sin(u_1)) >> 6);
            }
            if (frame >= 8) {
                vb_0 = 0x1000 - ex->math_cos((((frame - 8) << 12) / 17) >> 1);
                pos.vx = work->unk64.vx - (s16)((vb_0 >> 6) - 0x120);
                pos.vz = work->unk64.vz - (vb_0 >> 5);
                pos.vy = work->unk64.vy - (((vb_0 >> 1) * 0x302) >> 12);
                env->camera->points[2].vx = pos.vx;
                env->camera->points[2].vy = pos.vy;
                env->camera->points[2].vz = pos.vz;
            } else {
                pos.vx = work->unk64.vx + ((frame << 12) >> 7) + 0x20;
                pos.vz = work->unk64.vz;
                pos.vy = work->unk64.vy;
                env->camera->points[2].vx = pos.vx;
                env->camera->points[2].vy = pos.vy;
                env->camera->points[2].vz = pos.vz;
            }
            work->unk2C.vy = work->unk2C.vy + y_1 - x_x0;
            ex->Hi_DrawSummonModel(&work->unk44, (VECTOR *)&work->unk2C, (s32)&work->unk38, SUMMON0FB(env)->unk60, 1);
            work->unk2C.vy = work->unk2C.vy + (x_x0 + 0xF0);
            ex->Hi_DrawEffModel(&work->unk44, (VECTOR *)&work->unk2C, (s32)&work->unk38, work->unk11C);
            work->unk2C.vy = work->unk2C.vy - (y_1 + 0xF0);
            if (frame == 3) {
                ex->Hi_GetSummonBonePos(SUMMON0FB(env)->unk60, 0x5B, (u16 *)&work->unk1C);
                work->unk1C.vy += 0x10;
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->unk1C);
                if (p != NULL) {
                    p->unk22 = 0x40;
                }
            }
            if (frame >= 0x19) {
                work->unk0 = 4;
                entry->frame = -1;
                ex->Hi_SetSummonMotion(SUMMON0FB(env)->unk54->unk180[1], SUMMON0FB(env)->unk60);
                ex->Hi_SetSummonMotFrame(SUMMON0FB(env)->unk60, 0);
                D_801EB348 = 1;
                ex->Hi_GetSummonBonePos(SUMMON0FB(env)->unk60, 0x24, (u16 *)&work->unk5C);
                env->camera->points[3].vx = work->unk5C.vx;
                env->camera->points[3].vy = work->unk5C.vy;
                env->camera->points[3].vz = work->unk5C.vz;
                work->unkD8.vz -= 0x5C00;
                work->unkD8.vx -= 0x5C00;
                ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->unk4C);
                ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->unk54);
            }
        }
        } break;
        case 4: {
            SVECTOR a;
            SVECTOR b;
            BtlBone m;
            BtlBone m2;
            struct { s32 x; } z0 = { frame };
            s32 c8;

        {
            t = (frame << 12) / 44;
            ex->Hi_DrawSummonModel(&work->unk44, (VECTOR *)&work->unk2C, (s32)&work->unk38, SUMMON0FB(env)->unk60, 1);
            ex->Hi_SetEffModelOffset(work->unk11C, 0x80);
            ex->Hi_DrawEffModel(&work->unk44, (VECTOR *)&work->unk2C, (s32)&work->unk38, work->unk11C);
            b8 = 0x1000;
            vb_1 = 0x1000 - ex->math_cos(t >> 1);
            a.vx = work->unk5C.vx;
            a.vy = work->unk5C.vy;
            a.vz = work->unk5C.vz;
            a.vy += (vb_1 << 6) >> 12;
            env->camera->points[3].vx = a.vx;
            b0 = 0x1000;
            env->camera->points[3].vy = a.vy;
            b4 = 0x84;
            env->camera->points[3].vz = a.vz;
            if (frame == 0) {
                for (i1 = 0; i1 < 4; i1++) {
                    ex->Hi_GetSummonBonePos(SUMMON0FB(env)->unk60, D_801EB250[i1][0], (u16 *)&a);
                    ex->Hi_GetSummonBonePos(SUMMON0FB(env)->unk60, D_801EB250[i1][1], (u16 *)&b);
                    func_801E7720(&a, &b, 0x3F3, &D_801EB358[i1 & 1], &D_801EB358[(i1 & 1) + 1], 0, D_801EB40C[i1].seg, 1);
                }
                for (i2 = 0; i2 < 10; i2++) {
                    ex->Hi_GetSummonBoneMatrix(SUMMON0FB(env)->unk60, D_801EB260[i2], &m);
                    ex->btlfx_matrixPointAhead((MATRIX *)&m, 0x46, (s32)&a);
                    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 9), (s32)&a, 0, -1, 0x1000, 0x1000, 1,
                                        (u8 *)&D_801EB70C[i2], 2);
                }
            }
            for (i1 = 0; i1 < 4; i1++) {
                ex->Hi_GetSummonBonePos(SUMMON0FB(env)->unk60, D_801EB250[i1][0], (u16 *)&a);
                ex->Hi_GetSummonBonePos(SUMMON0FB(env)->unk60, D_801EB250[i1][1], (u16 *)&b);
                func_801E7720(&a, &b, 0xB, &D_801EB358[i1 & 1], &D_801EB358[(i1 & 1) + 1], 0x1000, D_801EB40C[i1].seg, 1);
            }
            if (frame >= 0x26) {
                c8 = ((((frame - 0x26) << 12) / 6) * 2) + 0x1000;
            } else {
                c8 = (t >> 1) + 0x300;
            }
            for (i1 = 0; i1 < 10; i1++) {
                Fx0fb0Trail16 *pp;

                ex->Hi_GetSummonBoneMatrix(SUMMON0FB(env)->unk60, D_801EB260[i1], &m2);
                ex->btlfx_matrixPointAhead((MATRIX *)&m2, 0x46, (s32)&a);
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 9), (s32)&a, 0, 0, 0x800, 0x1000, 1,
                                    (u8 *)(pp = &D_801EB70C[i1]), 2);
                env->otZ = 0x18;
                ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, 0x10), &a, NULL, c8 >> 1, 0, 1,
                                        ex->math_sin(t >> 2), 0, 0);
                env->otZ = 0;
                j = 0;
                s_0 = (s16 *)pp + 3;
                for (j = 0; j < 16; j++) {
                    *s_0 -= 8 + (t >> 8);
                    s_0 += 4;
                }
            }
            ex->Hi_GetSummonBonePos(SUMMON0FB(env)->unk60, D_801EB260[0], (u16 *)&work->unk4C);
            ex->Hi_GetSummonBonePos(SUMMON0FB(env)->unk60, D_801EB260[5], (u16 *)&work->unk54);
            if (frame >= 0x2C) {
                work->unk0 = 5;
                entry->frame = -1;
            }
        }
        } break;
        case 5: {
            SVECTOR a;
            BtlBone m;
            BtlBone m2;

        {
            b0 = 0x1000;
            b8 = 0x1000;
            b4 = 0x78;
            t = (frame << 12) / 21;
            if (frame == 0x11) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0, 0, 0x40);
            } else if (frame >= 0x11) {
                c = ((frame - 0x11) << 7) / 3;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, c >> 2, c >> 1, c);
            }
            if (frame >= 5) {
                ex->Hi_SetSummonMotFrame(SUMMON0FB(env)->unk60, 0x31);
            }
            ex->Hi_DrawSummonModel(&work->unk44, (VECTOR *)&work->unk2C, (s32)&work->unk38, SUMMON0FB(env)->unk60, 1);
            u = t * 3 + 0x1000;
            for (i11 = 0; i11 < 10; i11++) {
                ex->Hi_GetSummonBoneMatrix(SUMMON0FB(env)->unk60, D_801EB260[i11], &m);
                ex->btlfx_matrixPointAhead((MATRIX *)&m, 0x46, (s32)&a);
                env->otZ = 0x18;
                ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, 0x10), &a, NULL, u, 0, 1,
                                        0x1000, 0, 0);
                env->otZ = 0;
                if (i11 == 0) {
                    FX0FB_W(work->unk14, 0) = FX0FB_W(a, 0);
                    FX0FB_W(work->unk14, 1) = FX0FB_W(a, 1);
                }
                if (i11 == 5) {
                    FX0FB_W(work->unk1C, 0) = FX0FB_W(a, 0);
                    FX0FB_W(work->unk1C, 1) = FX0FB_W(a, 1);
                }
            }
            for (i11 = 0; i11 < 10; i11++) {
                Fx0fb0Trail16 *pp;

                ex->Hi_GetSummonBoneMatrix(SUMMON0FB(env)->unk60, D_801EB260[i11], &m2);
                ex->btlfx_matrixPointAhead((MATRIX *)&m2, 0x46, (s32)&a);
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 9), (s32)&a, 0, 0, 0x800, 0x1000 - t, 1,
                                    (u8 *)(pp = &D_801EB70C[i11]), 2);
                j = 0;
                s_1 = (s16 *)pp + 3;
                for (j = 0; j < 16; j++) {
                    *s_1 -= 0x18;
                    s_1 += 4;
                }
            }
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x14), &work->unk14, NULL, (t >> 1) + 0xAAA, 0, 1,
                              t >> 1);
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x14), &work->unk1C, NULL, (t >> 1) + 0xAAA, 0, 1,
                              t >> 1);
            if (frame == 4) {
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x15), (s32)&work->unk14);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x15), (s32)&work->unk1C);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
            }
            if (frame == 7) {
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xF), (s32)&work->unk14);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xF), (s32)&work->unk1C);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
            }
            if (frame >= 0x15) {
                work->unk0 = 6;
                entry->frame = -1;
                ex->Hi_SetSummonMotion(SUMMON0FB(env)->unk54->unk180[2], SUMMON0FB(env)->unk60);
            }
        }
        } break;
        case 6: {
            VECTOR pos;
            VECTOR sc;
            SVECTOR r1;
            SVECTOR r2;
            BtlBone m;
            BtlBone m2;

        {
            b4 = 0xF1;
            if (frame < 0xD) {
                ex->Hi_SetSummonMotFrame(SUMMON0FB(env)->unk60, frame);
            } else {
                ex->Hi_SetSummonMotFrame(SUMMON0FB(env)->unk60, 0xC);
            }
            ex->Hi_DrawSummonModel(&work->unk44, (VECTOR *)&work->unk2C, (s32)&work->unk38, SUMMON0FB(env)->unk60, 1);
            ex->Hi_DrawEffModel(&work->unk44, (VECTOR *)&work->unk2C, (s32)&work->unk38, work->unk11C);
            env->otZ = 0x40;
            for (i3 = 0; i3 < 10; i3++) {
                ex->Hi_GetSummonBoneMatrix(SUMMON0FB(env)->unk60, D_801EB260[i3], (BtlBone *)&pos);
                ex->btlfx_matrixPointAhead((MATRIX *)&pos, 0x46, (s32)&D_801EB3BC[i3]);
            }
            for (i3 = 0; i3 < 2; i3++) {
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0xA), &D_801EB3C4[i3 * 5], NULL, 0x1000, 0, 1,
                                  0x1000);
            }
            env->otZ = 0;
            if (frame >= 4) {
                w = 0;
                if (frame < 5) {
                    u_2 = frame << 12;
                    v_1 = u_2 >> 2;
                    sc.vx = (0x1000 - ex->math_cos(u_2 >> 4)) / 12;
                    sc.vy = (ex->math_cos(u_2 >> 4) >> 1) + 0xC00;
                    sc.vz = v_1 / 8;
                    x = 0;
                    if (frame == 4) {
                        ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, 0x80, 0x80, 0x80);
                    }
                } else {
                    u_3 = frame - 4;
                    if (frame < 8) {
                        c_1 = 0x80 - ((frame << 7) >> 3);
                        if (frame == 0) {
                            c_1 = 0xFF;
                        }
                        ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, c_1, c_1, c_1);
                    }
                    if (u_3 < 0x20) {
                        x = (ex->math_sin((u_3 << 10) >> 5) >> 5) + 0x2E;
                    } else {
                        x = 0xAE;
                    }
                    if (u_3 < 8) {
                        v_1 = (u_3 << 12) >> 5;
                        sc.vx = ex->math_cos(v_1) + 0x155;
                        sc.vy = 0xC00 - v_1;
                        sc.vz = ((u_3 << 12) >> 6) + 0x200;
                    } else {
                        sc.vx = ((u_3 - 8) << 6) + 0x155;
                        sc.vy = ((u_3 - 8) << 5) + 0x800;
                        w = (u_3 - 8) << 5;
                        sc.vz = ((u_3 - 8) << 6) + 0x400;
                    }
                }
                ex->Hi_GetSummonBoneMatrix(SUMMON0FB(env)->unk60, 7, &m);
                ex->btlfx_matrixAngles((MATRIX *)&m, (s16 *)&r1);
                r1.vz = 0xA00;
                r1.vx -= 0x2F8;
                r1.vy += 0x300;
                ex->Hi_GetSummonBoneMatrix(SUMMON0FB(env)->unk60, 0x31, &m2);
                ex->btlfx_matrixAngles((MATRIX *)&m2, (s16 *)&r2);
                r2.vz = 0x600;
                r2.vx -= 0x2F8;
                r2.vy -= 0x300;
                for (i3 = 9; i3 >= 0; i3--) {
                    pos.vx = D_801EB3BC[i3].vx;
                    pos.vy = D_801EB3BC[i3].vy;
                    pos.vz = D_801EB3BC[i3].vz;
                    if (i3 < 5) {
                        r2.vx += x;
                        sc.vx += w;
                        rp = &r2;
                    } else {
                        r1.vx += x;
                        sc.vx += w;
                        rp = &r1;
                    }
                    ex->Hi_DrawEffModel(rp, &pos, (s32)&sc, work->unk124);
                    if (i3 == 4) {
                        sc.vx -= w * 5;
                    }
                }
            }
            if (frame == 0) {
                work->unk5C.vy += 0x20;
                env->camera->points[3].vx = work->unk5C.vx;
                env->camera->points[3].vy = work->unk5C.vy;
                env->camera->points[3].vz = work->unk5C.vz;
            }
            if (frame == 4) {
                for (i3 = 0; i3 < 2; i3++) {
                    p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&D_801EB3C4[i3 * 5]);
                    if (p != NULL) {
                        p->unk22 = 0x100;
                    }
                }
            }
            if (frame < 2) {
                c_4 = 0xFF - frame * 0xFF / 2;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, c_4, c_4, c_4 >> 1);
            }
            if (frame >= 0x1C) {
                c_5 = (frame - 0x1B) * 0x33;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, c_5, c_5, c_5 >> 1);
            }
            if (frame >= 0x20) {
                for (i11 = 0; i11 < 27; i11++) {
                    ex->Hi_FreeEffModel(work->unk6C[i11]);
                }
                return 1;
            }
        }
        } break;
        }
        {
        SVECTOR pos;

        if (ac != 0) {
            pos.vx = D_801EB248.vx;
            u_4 = ac >> 5;
            pos.vy = D_801EB248.vy;
            pos.vz = D_801EB248.vz;
            pos.vy -= 0x79C0;
            pos.vx += 0xA00;
            pos.vz -= 0x1400;
            func_801E809C(&pos, 0x4000, u_4, 0x118, 0xFFFF, 0x10, 0x90, 0x7D0, work->unk4 * 0x5C, 0xF0, 1,
                          work->unk4 * 0x37);
            pos.vy += 0x640;
            pos.vx -= 0x1400;
            pos.vz += 0x2800;
            func_801E809C(&pos, 0x4000, u_4, 0x118, 0xFFFF, 0x10, 0x90, 0x76C, work->unk4 * 0x5C + 0x400, 0xF0, 1,
                          work->unk4 * 0x37 - 0x200);
        }
        if (a8 != 0) {
            SVECTOR rot;
            VECTOR sc;

            env->otZ = -0x300;
            rot.vx = 0x400;
            sc.vx = 0x1000;
            rot.vz = 0x400;
            sc.vy = 0x1000;
            sc.vz = 0x1000;
            rot.vy = 0;
            pos.vz = (0x1000 - a8) * 2;
            pos.vx = D_801EB248.vx;
            pos.vy = D_801EB248.vy;
            func_801E7B90(&pos, &rot, &sc, ex->math_sin(a8 >> 2) >> 5, 0x118, 0xFFFF, 0x10, 0x90, 0x400, a8, 0x180, 1);
            env->otZ = 0;
        }
        if (ac != 0 || a8 != 0) {
            if (work->unk0 == 0 || (work->unk0 == 1 && frame < 0x10)) {
                ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0xD), work->unk4);
            } else {
                ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0xE), work->unk4);
            }
        }
        if (b0 != 0) {
            work->unkF0.vz += 7;
            ex->Hi_DrawEffModel(&work->unkF0, (VECTOR *)&work->unkD8, (s32)&work->unkE4, work->unk118);
            if (b0 != 0x1000) {
                c_6 = (b0 >> 5) - 0x80;
                ex->Hi_ModifyEffModelRGB(work->unk118, c_6, c_6, c_6);
            }
            if (bc != 0) {
                work->unk110.vy += 4;
                ex->Hi_DrawEffModel(&work->unk110, (VECTOR *)&work->unkF8, (s32)&work->unk104, work->unk120);
                if (bc != 0x1000) {
                    c_7 = (bc >> 5) - 0x80;
                    ex->Hi_ModifyEffModelRGB(work->unk120, c_7, c_7, c_7);
                }
            }
        }
        if (b8 != 0) {
            u_3 = (work->unk4 * 16) % 240;
            ex->btlfx_drawSpriteAtOt(8, (u8 *)ex->btlfx_getFileData(entry->file, 3), 0xA0 - u_3, 0xA0, 0, 3, b8);
            ex->btlfx_drawSpriteAtOt(8, (u8 *)ex->btlfx_getFileData(entry->file, 3), 0x280 - u_3, 0xA0, 0, 3, b8);
        }
        if (b4 != 0) {
            CVECTOR col;
            SVECTOR rot;
            VECTOR sc;

            col.r = col.g = col.b = b4;
            sc.vx = 0x1060;
            sc.vy = 0x1060;
            sc.vz = 0x1000;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            ex->btlfx_drawScreenGrid(0x400, 0xA0, 0x70, 0x4D, 0x37, 4, 4, 2, 2, &rot, &sc, &col, 0, 0);
        }
        }
        work->unk4++;
    }
    return 0;
}

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
        work->unk104 = env->targets->targetCount;
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
        frame = entry->frame;
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
                entry->frame = -1;
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
        v.vx = env->targets->centerX;
        v.vy = env->targets->centerY;
        v.vz = env->targets->centerZ;
        v.vy -= 0x230;
        env->camera->points[2].vx = v.vx;
        env->camera->points[2].vy = v.vy;
        env->camera->points[2].vz = v.vz;
        D_801EB3AC = env;
        return 1;
    } else {
        return 0;
    }
    return 0;
}
