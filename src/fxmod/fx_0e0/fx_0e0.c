#include "fxmod/fx_0e0.h"

void func_801E7718(s32 ot, s32 x, s32 y, s32 w, s32 h, s32 c, s32 tpage, s32 clut, s32 u, s32 v, s32 abr) {
    POLY_FT4 *p = (POLY_FT4 *)D_801EA918->prim;
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EA918);
    u8 col[4];
    s32 u1;
    s32 v1;
    s32 y1;

    D_801EA918->prim = (u8 *)(p + 2);
    col[0] = col[1] = col[2] = c;
    if (abr != 0xFF) {
        col[3] = 0x2E;
    } else {
        col[3] = 0x2C;
    }
    tpage |= (abr & 3) << 5;
    v1 = v + h - 1;
    p->u0 = p->u2 = u;
    p->u1 = p->u3 = u1 = u + w - 1;
    p->v0 = p->v1 = v;
    p->v2 = p->v3 = v1;
    p->x0 = p->x2 = x - w;
    p->x1 = p->x3 = x;
    p->y0 = p->y1 = y;
    p->clut = clut;
    p->tpage = tpage;
    *(u32 *)&p->r0 = *(u32 *)col;
    y1 = y + h * 7 / 6;
    p->y2 = p->y3 = y1;
    ex->btlfx_addPrim(0x09000000, ot, p);
    p++;
    p->x0 = p->x2 = x;
    p->u0 = p->u2 = u1;
    p->u1 = p->u3 = u;
    p->v0 = p->v1 = v;
    p->v2 = p->v3 = v1;
    p->x1 = p->x3 = x + w;
    p->y0 = p->y1 = y;
    p->y2 = p->y3 = y1;
    p->clut = clut;
    p->tpage = tpage;
    *(u32 *)&p->r0 = *(u32 *)col;
    ex->btlfx_addPrim(0x09000000, ot, p);
}

void func_801E7910(s32 ot, s32 x, s32 y, s32 w, s32 h, s32 c, s32 tpage, s32 clut, s32 u, s32 v, s32 abr, s32 flip) {
    s32 dx;
    s32 w2;
    POLY_FT4 *p;
    BtlfxExports *ex;
    u8 col[4];
    s32 n;
    s32 i;
    s32 u1;
    s32 v1;
    s32 y1;

    n = 2;
    ex = BTLFX_EXPORTS(D_801EA918);
    if (flip != 0) {
        n = 4;
    }
    p = (POLY_FT4 *)D_801EA918->prim;
    D_801EA918->prim = (u8 *)(p + n);
    col[0] = col[1] = col[2] = c;
    if (abr != 0xFF) {
        col[3] = 0x2E;
    } else {
        col[3] = 0x2C;
    }
    tpage |= (abr & 3) << 5;
    dx = -0x140;
    if (flip != 0) {
        dx = 0xA0;
    }
    w2 = 0x140;
    if (flip != 0) {
        w2 = 0xA0;
    }
    for (i = 0; i < n; i++) {
        p->x0 = p->x2 = x;
        p->x1 = p->x3 = x + w2;
        p->y0 = p->y1 = y;
        p->y2 = p->y3 = y + h;
        if (flip != 0) {
            p->u0 = p->u2 = u;
            p->u1 = p->u3 = u + w - 1;
        } else {
            p->u0 = p->u2 = u + w - 1;
            p->u1 = p->u3 = u;
        }
        p->v0 = p->v1 = v;
        p->v2 = p->v3 = v + h - 1;
        p->clut = clut;
        p->tpage = tpage;
        *(u32 *)&p->r0 = *(u32 *)col;
        ex->btlfx_addPrim(0x09000000, ot, p);
        p++;
        x += dx;
    }
}

void func_801E7B08(SVECTOR *pos, SVECTOR *rot, s32 scale, s32 c, s32 tpage, s32 clut, s32 u, s32 v, s32 w, s32 h, s32 abr) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EA918);
    MATRIX m;
    VECTOR sc;
    SVECTOR zero;
    SVECTOR vtx[4];
    u8 col[4];
    s32 otz;
    POLY_FT4 *p;
    s32 code;
    s32 a;
    s32 b;

    if (rot == NULL) {
        rot = &zero;
        ((s32 *)&zero)[1] = 0;
        ((s32 *)&zero)[0] = 0;
    }
    gte_SetRotMatrix(D_801EA918->viewMatrix);
    gte_SetTransMatrix(D_801EA918->viewMatrix);
    gte_ldv0(pos);
    p = (POLY_FT4 *)D_801EA918->prim;
    D_801EA918->prim = (u8 *)(p + 1);
    col[0] = col[1] = col[2] = c >> 5;
    col[3] = (abr != 0xFF) ? 0x2E : 0x2C;
    tpage |= (abr & 3) << 5;
    gte_rtv0tr_b();
    ex->math_rotMatrixYXZ(rot, &m);
    sc.vx = sc.vy = sc.vz = scale;
    ex->ScaleMatrix(&m, &sc);
    gte_stlvnl(m.t);
    if (rot->pad != 0) {
        ex->MulRotMatrix(&m);
    }
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    a = w << 6;
    b = -a;
    vtx[1].vx = vtx[3].vx = a;
    vtx[0].vx = vtx[2].vx = b;
    a = h << 6;
    b = -a;
    vtx[2].vy = vtx[3].vy = a;
    vtx[0].vz = vtx[1].vz = vtx[2].vz = vtx[3].vz = 0;
    vtx[0].vy = vtx[1].vy = b;
    gte_ldv3c(vtx);
    gte_rtpt();
    gte_nclip();
    p->tpage = tpage;
    p->clut = clut;
    gte_stopz(&otz);
    if (otz != 0) {
        gte_stsxy3(&p->x0, &p->x1, &p->x2);
        gte_avsz3();
        gte_stszotz(&otz);
        gte_ldv0(&vtx[3]);
        gte_rtps();
        p->u1 = p->u3 = u + w - 1;
        p->u0 = p->u2 = u;
        p->v0 = p->v1 = v;
        p->v2 = p->v3 = v + h - 1;
        *(u32 *)&p->r0 = *(u32 *)col;
        gte_stsxy(&p->x3);
        ex->btlfx_addPrim(0x09000000, otz, p);
    }
}

void func_801E7E18(SVECTOR *pos, SVECTOR *rot, s32 scale, s32 c, s32 tpage, s32 clut, s32 u, s32 v, s32 amp, s32 abr) {
    MATRIX m;
    VECTOR sc;
    SVECTOR vtx[4];
    SVECTOR center;
    s32 otz;
    s32 j;
    s32 i;
    s32 code;
    BtlfxExports *ex;
    POLY_GT4 *p;
    s32 d0;
    s32 d1;
    s32 d2;
    s32 d3;
    s32 r;
    s32 iv;
    s32 u0;
    s32 u1;
    s32 v0;
    s32 v1;

    ex = BTLFX_EXPORTS(D_801EA918);
    ((s32 *)&center)[0] = 0;
    center.vz = 0;
    gte_SetRotMatrix(D_801EA918->viewMatrix);
    gte_SetTransMatrix(D_801EA918->viewMatrix);
    gte_ldv0(pos);
    p = (POLY_GT4 *)D_801EA918->prim;
    code = 0x3C;
    D_801EA918->prim = (u8 *)(p + 576);
    if (abr != 0xFF) {
        code = 0x3E;
    }
    tpage |= (abr & 3) << 5;
    gte_rtv0tr_b();
    ex->math_rotMatrixYXZ(rot, &m);
    sc.vx = sc.vy = sc.vz = scale;
    ex->ScaleMatrix(&m, &sc);
    gte_stlvnl(m.t);
    ex->MulRotMatrix(&m);
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    for (j = 0; j < 24; j++) {
        for (i = 0; i < 24; i++) {
            vtx[0].vx = vtx[2].vx = j * 0x180 - 0x1200;
            vtx[0].vz = vtx[1].vz = vtx[2].vz = vtx[3].vz = 0;
            vtx[0].vy = vtx[1].vy = i * 0x180 - 0x1200;
            vtx[2].vy = vtx[3].vy = i * 0x180 - 0x1080;
            vtx[1].vx = vtx[3].vx = j * 0x180 - 0x1080;
            d0 = ex->btlfx_distance(&vtx[0], &center);
            d1 = ex->btlfx_distance(&vtx[1], &center);
            d2 = ex->btlfx_distance(&vtx[2], &center);
            d3 = ex->btlfx_distance(&vtx[3], &center);
            if (d0 > 0xC00) {
                vtx[0].vz = 0;
            } else {
                vtx[0].vz = ((ex->math_cos((d0 << 11) / 0xC00) + 0x1000) * amp) >> 13;
            }
            if (d1 > 0xC00) {
                vtx[1].vz = 0;
            } else {
                vtx[1].vz = ((ex->math_cos((d1 << 11) / 0xC00) + 0x1000) * amp) >> 13;
            }
            if (d2 > 0xC00) {
                vtx[2].vz = 0;
            } else {
                vtx[2].vz = ((ex->math_cos((d2 << 11) / 0xC00) + 0x1000) * amp) >> 13;
            }
            if (d3 > 0xC00) {
                vtx[3].vz = 0;
            } else {
                vtx[3].vz = ((ex->math_cos((d3 << 11) / 0xC00) + 0x1000) * amp) >> 13;
            }
            gte_ldv3c(vtx);
            gte_rtpt();
            r = c - (vtx[0].vz >> 4);
            if (r < 0) {
                r = 0;
            }
            p->code = code;
            p->r0 = p->g0 = p->b0 = r;
            r = c - (vtx[1].vz >> 4);
            if (r < 0) {
                r = 0;
            }
            p->r1 = p->g1 = p->b1 = r;
            r = c - (vtx[2].vz >> 4);
            if (r < 0) {
                r = 0;
            }
            p->r2 = p->g2 = p->b2 = r;
            r = c - (vtx[3].vz >> 4);
            if (r < 0) {
                r = 0;
            }
            p->r3 = p->g3 = p->b3 = r;
            gte_nclip();
            p->tpage = tpage;
            p->clut = clut;
            gte_stopz(&otz);
            if (otz != 0) {
                gte_stsxy3(&p->x0, &p->x1, &p->x2);
                gte_avsz3();
                gte_stszotz(&otz);
                gte_ldv0(&vtx[3]);
                gte_rtps();
                u0 = u + ((j - 1) & 1) * 48;
                iv = (i - 1) & 1;
                v0 = v + iv * 48;
                u1 = u0 + 0x30;
                if ((j - 1) & 1) {
                    u1 = u0 + 0x2F;
                }
                v1 = v0 + 0x30;
                if (iv != 0) {
                    v1 = v0 + 0x2F;
                }
                p->u0 = p->u2 = u0;
                p->u1 = p->u3 = u1;
                p->v0 = p->v1 = v0;
                p->v2 = p->v3 = v1;
                ex->btlfx_addPrim(0x0C000000, 2, p);
                gte_stsxy(&p->x3);
                p++;
            }
        }
    }
}

void func_801E8430(Unk80100C78Entry *entry, Fx0e0Bolt *bolt, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1, s32 ang,
                   s32 dang, s32 arg8, s32 arg9, s32 pad) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EA918);
    SVECTOR p;
    SVECTOR rot;
    SVECTOR *pt;
    s32 i;
    s32 t;
    s32 a;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&rot);
    pt = bolt->unk4;
    bolt->unk2 = 0x18;
    bolt->unk0 = 0x18;
    bolt->unk1 = 0;
    for (i = 0; i < 25; i++) {
        t = 0x1000 - (i << 12) / 24;
        t += ex->btlfx_randCentered(0x100);
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&p);
        a = ex->btlfx_randCentered(0xA0);
        a = (ex->math_sin(t >> 1) * a) >> 12;
        a += ex->btlfx_lerp(amp0, amp1, t);
        ex->btlfx_offsetPoint(&p, (s32)pt, 0, (XY32 *)&rot, a, ang);
        pt->pad = pad;
        ang += dang + ex->btlfx_randCentered(0x80);
        pt++;
    }
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 0x1D), 0, 0, arg8, 0x1000, arg9, 1, (u8 *)bolt, 0);
}

INCLUDE_RODATA("asm/us/fx_0e0/nonmatchings/fx_0e0", fx_0e0_headerPtr);

s32 func_801E8628(s32 mode, Fx0e0Obj *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, s32 arg5,
                  Fx0e0Inst *parent) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR scale;
    SVECTOR ang;
    SVECTOR d;
    SVECTOR old;
    SVECTOR t;
    SVECTOR pad;
    Pool6CEntry *e;
    s32 k;
    s32 a;
    s32 c;
    s32 i;
    s32 sh;

    if (mode == 1) {
        it->unk2 = 0;
        it->unk14.vx = ex->btlfx_rand();
        it->unk14.vy = ex->btlfx_rand();
        it->unk14.vz = 0;
        it->unk1C = ex->btlfx_rand() % 64;
        it->unk1E = ex->btlfx_rand() % 32;
        it->unk20 = NULL;
    } else {
        switch (it->unk0) {
        case 0:
        case 1:
            k = 0;
            switch (it->unk2) {
            case 0:
                it->unk14.vx += it->unk1C;
                it->unk14.vy += it->unk1E;
                old.vx = it->unkC.vx;
                old.vy = it->unkC.vy;
                old.vz = it->unkC.vz;
                it->unkC.vx += it->unk4.vx;
                it->unkC.vy += it->unk4.vy;
                it->unkC.vz += it->unk4.vz;
                pos.vx = it->unkC.vx;
                pos.vy = it->unkC.vy;
                pos.vz = it->unkC.vz;
                scale.vx = scale.vy = scale.vz = D_801EA8B8[it->unk4.pad];
                ex->Hi_DrawEffModel(&it->unk14, &pos, (s32)&scale, parent->unk5C);
                d.vx = parent->unk8.vx - it->unkC.vx;
                d.vy = parent->unk8.vy - it->unkC.vy;
                d.vz = parent->unk8.vz - it->unkC.vz;
                if (frame < 8) {
                    k = (frame << 12) >> 3;
                } else {
                    k = 0x1000;
                }
                if (d.vx * d.vx + d.vy * d.vy + d.vz * d.vz < D_801EB1D8 * D_801EB1D8 * D_801EB1D8) {
                    it->unk2 = 1;
                    ((s16 *)it)[-1] = -1;
                    it->unk20 = e = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xD), (s32)&it->unkC);
                    if (e != NULL) {
                        e->unk22 = 0x20;
                    }
                }
                break;
            case 1:
                scale.vx = scale.vy = scale.vz = D_801EA8B8[it->unk4.pad];
                pos.vx = it->unkC.vx;
                pos.vy = it->unkC.vy;
                pos.vz = it->unkC.vz;
                ex->Hi_DrawEffModel(&it->unk14, &pos, (s32)&scale, parent->unk5C);
                ex->btlfx_aimAngles((s16 *)&parent->unk8, (s16 *)&it->unkC, (s16 *)&ang);
                ang.pad = 1;
                if (it->unk20 != NULL) {
                    it->unk20->unk5C.vx = ang.vx;
                    it->unk20->unk5C.vy = ang.vy;
                    it->unk20->unk5C.vz = ang.vz;
                }
                if (frame < 4) {
                    k = 0x1000 - ((frame << 12) >> 2);
                }
                it->unk4.vx = it->unk4.vx * 2 / 3;
                it->unk4.vy = it->unk4.vy * 2 / 3;
                it->unk4.vz = it->unk4.vz * 2 / 3;
                env->otZ = 0x1F4;
                if (frame < 8) {
                    ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0xA), &it->unkC, NULL, 0x3000, frame, 1,
                                      ex->math_cos(frame << 7));
                }
                env->otZ = 0x20;
                if (frame < 8) {
                    ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 9), &it->unkC, &ang, 0x2000, frame, 1,
                                      ex->math_cos(frame << 7));
                }
                env->otZ = 0;
                if (D_801EB1DC != 0) {
                    return 1;
                }
                break;
            }
            if (k != 0) {
                env->otZ = 0x180;
                BTLFX_COPY_SVEC(&t, &it->unkC);
                t.vx -= it->unk4.vx;
                t.vy -= it->unk4.vy;
                t.vz -= it->unk4.vz;
                for (i = 0; i < 12; i++) {
                    t.vx -= it->unk4.vx;
                    t.vy -= it->unk4.vy;
                    t.vz -= it->unk4.vz;
                    func_801E7B08(&t, NULL, 0x600, k, 0x1A, 0x3D06, (i & 3) << 6, (i >> 2) << 6, 0x40, 0x40, 1);
                }
                env->otZ = 0;
            }
            break;
        case 2:
            a = (frame << 12) / 24;
            scale.vx = scale.vy = (ex->math_sin(a >> 2) >> 1) + 0x800;
            scale.vz = it->unk4.pad + 0x200;
            it->unk14.vz -= 0x40;
            it->unkC.vy -= 0x4D;
            pos.vx = it->unkC.vx;
            pos.vy = it->unkC.vy;
            pos.vz = it->unkC.vz;
            ex->Hi_DrawEffModel(&it->unk14, &pos, (s32)&scale, parent->unk70);
            ex->Hi_ModifyEffModelRGB(parent->unk70, (ex->math_sin(a >> 1) >> 5) - 0x80, (ex->math_sin(a >> 1) >> 5) - 0x80,
                                     (ex->math_sin(a >> 1) >> 5) - 0x80);
            if (frame >= 24) {
                return 1;
            }
            break;
        case 3:
            it->unk14.vx += it->unk1C * 2;
            it->unk14.vy += it->unk1E * 2;
            it->unkC.vx += it->unk4.vx;
            it->unkC.vy += it->unk4.vy;
            it->unkC.vz += it->unk4.vz;
            pos.vx = it->unkC.vx;
            pos.vy = it->unkC.vy;
            pos.vz = it->unkC.vz;
            scale.vz = ((it->unk4.pad * ex->math_cos((frame << 8) >> 2)) >> 12) * 2;
            scale.vx = scale.vy = scale.vz / 3;
            ex->Hi_DrawEffModel(&it->unk14, &pos, (s32)&scale, parent->unk5C);
            ex->Hi_ModifyEffModelRGB(parent->unk5C, -0x4B, -0x4B, -0x4B);
            if (frame >= 16) {
                return 1;
            }
            break;
        case 4:
            it->unk14.vx += it->unk1C;
            it->unk14.vy += it->unk1E;
            it->unkC.vx += it->unk4.vx;
            it->unkC.vy += it->unk4.vy;
            it->unkC.vz += it->unk4.vz;
            it->unk4.vx = (it->unk4.vx * 31) >> 5;
            it->unk4.vy = (it->unk4.vy * 31) >> 5;
            it->unk4.vz = (it->unk4.vz * 31) >> 5;
            pos.vx = it->unkC.vx;
            pos.vy = it->unkC.vy;
            sh = frame << 6;
            /* Scheduling barrier: frame << 6 fills the pos.vy load delay and pos.vz's stays a nop. */
            do {
            } while (0);
            pos.vz = it->unkC.vz;
            scale.vz = ((it->unk4.pad * ex->math_cos(sh >> 2)) >> 12) * 2;
            scale.vx = scale.vy = scale.vz >> 1;
            ex->Hi_DrawEffModel(&it->unk14, &pos, (s32)&scale, parent->unk5C);
            ex->Hi_ModifyEffModelRGB(parent->unk5C, 0, -0x4B, -0x4B);
            if (frame >= 0x40) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

/* Matching notes: case 6's two loop counters are different variables (n, then i; both s3), with i = 0 after the
 * if, and an empty do { } while (0); barrier before the if keeps the call-crossing constant set from floating up
 * in sched1; `one` is set in the mode test and passed as case 10's 13th argument (the original's s0); the frame and
 * state loads come before the col/scl/xo inits; a dead-store-only if/else after them ends cse's block so the
 * per-case xo = 0 stores survive; case 6/7's unk44/unk50/unk24 updates in searched order; from's fields copied
 * before both += adjustments (flow keeps both stores); case 7's tint in a variable and t * 3 for frame * 0x300; case
 * 8 reuses t, writes t / 5 and casts the random part to s16. */
s32 fx_0e0_op80(s32 mode, Fx0e0Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx0e0Tmp u;
    VECTOR sc;
    SVECTOR rot;
    SVECTOR from;
    SVECTOR to;
    Fx0e0Spark *sp;
    s32 a1;
    s32 a2;
    s32 a3;
    s32 a4;
    s32 c1;
    s32 r1;
    s32 r2;
    s32 d1;
    s32 tc;
    s32 frame;
    s32 col;
    s32 scl;
    s32 xo;
    s32 t;
    s32 a;
    s32 c;
    s32 i;
    s32 r;
    s32 state;
    s32 n;
    s32 one;

    if (mode == 0) {
        init->workSize = 0x84;
        init->elemSize = 0x24;
        init->elemFn = (s32)func_801E8628;
        init->poolCount = 0x31;
    } else if (one = 1, mode == one) {
        work->unk0 = 6;
        work->unk4 = 0;
        D_801EA918 = env;
        ex->btlfx_initEffModels(D_801EAB98, 8);
        work->unk5C = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 8), 0x98, 0x3D80, 0, 0);
        work->unk60 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 0xB), 0x118, 0xFFFF, 0x40, 0);
        work->unk64 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 0x14), 0x118, 0xFFFF, 0x40, 0);
        work->unk68 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 0x11), 0x37, 0x3D07, 0, 0);
        work->unk6C = ex->Hi_RegisterGouEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 0x13));
        work->unk70 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 0x12), 0x37, 0x3D07, 0, 0);
        work->unk74 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 0x19), 0xBC, 0x3E00, 0x80, 0);
        D_801EA8B0.vz += 0x400;
        BTLFX_COPY_SVEC(&work->unk8, &D_801EA8B0);
        D_801EA8B0.vz -= 0x400;
        work->unk30.vy = work->unk30.vx = 0x2800;
        work->unk24.vx = work->unk8.vx;
        work->unk24.vy = work->unk8.vy;
        work->unk24.vz = work->unk8.vz;
        work->unk24.vy -= 0x3800;
        work->unk24.vz -= 0x1C00;
        work->unk30.vz = 0x1800;
        work->unk10.vx = work->unk24.vx;
        work->unk10.vy = work->unk24.vy;
        work->unk10.vz = work->unk24.vz;
        ex->btlfx_aimAngles((s16 *)&work->unk8, (s16 *)&work->unk10, (s16 *)&work->unk3C);
        BTLFX_COPY_SVEC(&work->unk50, &work->unk3C);
    } else {
        state = work->unk0;
        frame = entry->frame;
        col = 0;
        scl = 0;
        xo = 0;
        if (mode) {
            a1 = 1;
            a2 = 1;
        } else {
            a1 = 2;
            a2 = 2;
        }
        switch (state) {
        case 6:
            t = (frame << 12) / 66;
            col = ((t << 5) >> 12) + 0x8D;
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, (t * 0x58) >> 12, 0, 0);
            a1 = t >> 2;
            BTLFX_COPY_SVEC(&work->unk18, &work->unk8);
            work->unk18.vy -= 0x400 + (ex->math_sin(a1) >> 1);
            scl = 0x80;
            xo = 0;
            env->camera->points[3].vx = work->unk18.vx;
            env->camera->points[3].vy = work->unk18.vy;
            env->camera->points[3].vz = work->unk18.vz;
            work->unk3C.vz += 8;
            work->unk24.vy += 0x90;
            work->unk24.vz += 0x46;
            ex->Hi_DrawEffModel(&work->unk3C, (VECTOR *)&work->unk24, (s32)&work->unk30, work->unk64);
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0x1C), work->unk4);
            work->unk44.vx = work->unk44.vy = 0x3800;
            work->unk44.vz = 0x2000;
            work->unk24.vy -= 0x5DC;
            work->unk24.vz += 0x2EE;
            work->unk50.vz += 0x14;
            ex->Hi_DrawEffModel(&work->unk50, (VECTOR *)&work->unk24, (s32)&work->unk44, work->unk68);
            ex->Hi_ModifyEffModelRGB(work->unk68, (((ex->math_cos(a1) >> 1) + 0x800) >> 5) - 0x80,
                                     (((ex->math_cos(a1) >> 1) + 0x800) >> 5) - 0x80,
                                     (((ex->math_cos(a1) >> 1) + 0x800) >> 5) - 0x80);
            work->unk50.vz -= 0x200;
            ex->Hi_DrawEffModel(&work->unk50, (VECTOR *)&work->unk24, (s32)&work->unk44, work->unk68);
            ex->Hi_ModifyEffModelRGB(work->unk68, (((ex->math_cos(a1) >> 1) + 0x800) >> 5) - 0x80,
                                     (((ex->math_cos(a1) >> 1) + 0x800) >> 5) - 0x80,
                                     (((ex->math_cos(a1) >> 1) + 0x800) >> 5) - 0x80);
            work->unk44.vx = work->unk44.vy = 0x2000;
            work->unk50.vz += 0x200;
            work->unk24.vy += 0x5DC;
            work->unk24.vz -= 0x2EE;
            work->unk44.vz = 0x3000;
            work->unk24.vy -= 0x125C;
            work->unk24.vz -= 0x8FC;
            ex->Hi_DrawEffModel(&work->unk50, (VECTOR *)&work->unk24, (s32)&work->unk44, work->unk6C);
            c1 = (((t * 2) - 0x1000) >> 5) - 0x80;
            ex->Hi_ModifyEffModelRGB(work->unk6C, c1, c1, c1);
            work->unk24.vy += 0x125C;
            work->unk24.vz += 0x8FC;
            if (frame > 0x32) {
                n = 0;
                a = (frame - 0x32) << 8;
                sc.vx = sc.vy = (ex->math_sin(a >> 2) * 5) / 2 + 0x400;
                a2 = a >> 2;
                sc.vz = a + 0x1800;
                rot.vx = -0x400;
                rot.vy = 0;
                u.v.vx = D_801EA8B0.vx;
                rot.vz = frame * 0x8D;
                u.v.vy = D_801EA8B0.vy;
                u.v.vz = D_801EA8B0.vz;
                u.v.vy -= 0x100;
                do {
                    ex->Hi_DrawEffModel(&rot, &u.v, (s32)&sc, work->unk70);
                    ex->Hi_ModifyEffModelRGB(work->unk70, (ex->math_sin(a2) >> 5) - 0x80, (ex->math_sin(a2) >> 5) - 0x80,
                                             (ex->math_sin(a2) >> 5) - 0x80);
                    n++;
                    sc.vx = sc.vy = (sc.vx * 3) >> 2;
                    rot.vz += 0x400;
                    u.v.vy += 0x20;
                } while (n <= 0);
                sc.vx = sc.vz = (ex->math_sin(a >> 2) * 5) / 2 + 0x1000;
                sc.vy = ((a * 0x300) >> 12) + 0x800;
                rot.vx = 0;
                rot.vy = frame * 0x8D;
                rot.vz = 0;
                u.v.vx = D_801EA8B0.vx;
                u.v.vy = D_801EA8B0.vy;
                u.v.vz = D_801EA8B0.vz;
                ex->Hi_DrawEffModel(&rot, &u.v, (s32)&sc, work->unk74);
                a3 = a >> 1;
                ex->Hi_ModifyEffModelRGB(work->unk74, (ex->math_sin(a3) >> 5) - 0x80, (ex->math_sin(a3) >> 5) - 0x80,
                                         (ex->math_sin(a3) >> 5) - 0x80);
            }
            from.vx = work->unk24.vx;
            from.vy = work->unk24.vy;
            from.vz = work->unk24.vz;
            from.vy += 0x1838;
            from.vz += 0xC1C;
            for (i = 0; i < 1; i++) {
                r2 = ex->btlfx_rand();
                d1 = ex->btlfx_randCentered(0x400) + 0xC00;
                ((s32 *)&to)[0] = work->unk24.vx;
                ((s32 *)&to)[1] = work->unk24.vy;
                ex->btlfx_offsetPoint(&from, (s32)&to, 0x1000, (XY32 *)&work->unk3C, 0x3000, r2);
                func_801E8430(entry, &D_801EA928[i], &from, &to, 0x100, 0x200, r2, 0xC0, 0, d1, (ex->btlfx_rand() & 0x1F) + 0x58);
            }
            if (frame >= 0x42) {
                work->unk0 = 7;
                entry->frame = -1;
                work->unk8.vx = work->unk24.vx;
                work->unk8.vy = work->unk24.vy;
                work->unk8.vz = work->unk24.vz;
                work->unk10.vx = work->unk24.vx;
                work->unk10.vy = work->unk24.vy;
                work->unk10.vz = work->unk24.vz;
            }
            break;
        case 7:
            xo = 0;
            t = frame << 8;
            a4 = t >> 2;
            col = 0xAD;
            scl = a4 + 0x80;
            D_801EA8C8[1].r = (ex->math_sin(a4) * 0xFF) >> 12;
            D_801EA8C8[1].g = (ex->math_sin(a4) * 0xFF) >> 12;
            D_801EA8C8[1].b = (u32)ex->math_sin(a4) >> 6;
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, 0x58, 0, 0);
            ex->btlfx_drawRippleGrid((s32)(env->ot + 0xFF0), 0x28, 2, 2, 0xC8, 0xFF, D_801EA8C8, 0, 0, 0, 0);
            if (frame > 8) {
                c = ((frame - 8) * 0xFF) / 8;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, c, c, c);
            }
            work->unk24.vy = work->unk8.vy + (t >> 6) + ((((frame & 1) * 2 - 1) * ex->math_cos(t >> 2)) >> 6);
            ex->Hi_DrawEffModel(&work->unk3C, (VECTOR *)&work->unk24, (s32)&work->unk30, work->unk64);
            ex->Hi_ModifyEffModelRGB(work->unk64, ((((ex->math_cos(t >> 2) * 3) >> 2) + 0x400) >> 5) - 0x80,
                                     ((((ex->math_cos(t >> 2) * 3) >> 2) + 0x400) >> 5) - 0x80,
                                     ((((ex->math_cos(t >> 2) * 3) >> 2) + 0x400) >> 5) - 0x80);
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0x1C), work->unk4);
            work->unk44.vx = work->unk44.vy = 0x3800;
            work->unk44.vz = 0x2000;
            work->unk24.vy -= 0x5DC;
            work->unk24.vz += 0x2EE;
            work->unk50.vz += 8;
            ex->Hi_DrawEffModel(&work->unk50, (VECTOR *)&work->unk24, (s32)&work->unk44, work->unk68);
            ex->Hi_ModifyEffModelRGB(work->unk68, (ex->math_cos(t >> 2) >> 6) - 0x80, (ex->math_cos(t >> 2) >> 6) - 0x80,
                                     (ex->math_cos(t >> 2) >> 6) - 0x80);
            work->unk50.vz -= 0x200;
            ex->Hi_DrawEffModel(&work->unk50, (VECTOR *)&work->unk24, (s32)&work->unk44, work->unk68);
            ex->Hi_ModifyEffModelRGB(work->unk68, (ex->math_cos(t >> 2) >> 6) - 0x80, (ex->math_cos(t >> 2) >> 6) - 0x80,
                                     (ex->math_cos(t >> 2) >> 6) - 0x80);
            work->unk44.vx = work->unk44.vy = 0x2000;
            work->unk50.vz += 0x200;
            work->unk24.vy += 0x5DC;
            work->unk24.vz -= 0x2EE;
            work->unk44.vz = 0x3000;
            work->unk24.vy -= 0x1388;
            work->unk24.vz -= 0x9C4;
            ex->Hi_DrawEffModel(&work->unk50, (VECTOR *)&work->unk24, (s32)&work->unk44, work->unk6C);
            ex->Hi_ModifyEffModelRGB(work->unk6C, (ex->math_cos(t >> 2) >> 5) - 0x80, (ex->math_cos(t >> 2) >> 5) - 0x80,
                                     (ex->math_cos(t >> 2) >> 5) - 0x80);
            work->unk24.vy += 0x1388;
            work->unk24.vz += 0x9C4;
            env->otZ = 0x898;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x1A), &D_801EA8B0, NULL, t * 3 + 0x1000,
                              frame, 1, ex->math_sin(t >> 2));
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x1B), &D_801EA8B0, NULL, t * 2 + 0x4000, frame, 1,
                              ex->math_sin(t >> 2));
            env->otZ = 0;
            if (frame >= 0x10) {
                work->unk0 = 8;
                entry->frame = -1;
                work->unk10.vx = work->unk18.vx;
                work->unk10.vy = work->unk18.vy;
                work->unk10.vz = work->unk18.vz;
                work->unk10.vx -= 0x2800;
                work->unk10.vy += 0xB86;
                ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x20), (s32)&work->unk10);
            }
            break;
        case 8:
            t = frame << 7;
            col = 0xBC;
            scl = 0x200;
            xo = 0;
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 0xFF8), 0xFF, 0xFF, 0xFF, 0xC8);
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0x40, 0, 0);
            if (frame >= 0x1A) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, ((frame - 0x1A) << 7) / 6, ((frame - 0x1A) << 7) / 6,
                                         ((frame - 0x1A) << 7) / 6);
            }
            if (frame >= 0xA) {
                for (r1 = 0; r1 < 3; r1++) {
                    sp = ex->btlfx_poolSpawn((s16 *)entry->pool);
                    if (sp != NULL) {
                        sp->unkC = work->unk18.vx - ((t >> 4) + 0x1800) + ex->btlfx_randCentered(0x200);
                        sp->unkE = 0x6A4 - ((0x1000 - t) >> 1);
                        sp->unk10 = work->unk18.vz + ex->btlfx_randCentered(0x1C00);
                        sp->unk4 = ex->btlfx_randCentered(0x100);
                        sp->unk6 = -0x140 - (ex->btlfx_rand() & 0xFF);
                        sp->unk8 = ex->btlfx_randCentered(0x100);
                        sp->unk14 = ex->btlfx_rand();
                        sp->unk16 = ex->btlfx_rand();
                        sp->unk18 = ex->btlfx_rand();
                        sp->unk0 = 3;
                        sp->unkA = t / 5 + (s16)((ex->btlfx_rand() & 0x3FF) + 0x80);
                    }
                }
            }
            if (frame == 1) {
                ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x21), (s32)&work->unk10);
            }
            if (frame == 4) {
                ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x21), (s32)&work->unk10);
            }
            if (frame >= 0x20) {
                work->unk0 = 0xA;
                entry->frame = -1;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, 0xFF, 0xFF, 0xFF);
                ex->btlfx_poolClear(entry->pool);
                ex->btlfx_initParticleSystem();
                work->unk18.vx = 0;
                work->unk18.vy = -0x200;
                work->unk18.vz = 0;
                env->camera->points[3].vx = work->unk18.vx;
                env->camera->points[3].vy = work->unk18.vy;
                env->camera->points[3].vz = work->unk18.vz;
            }
            break;
        case 10:
            t = frame << 8;
            tc = 0xF5 - ((t * 0xF5) >> 12);
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, tc, tc, tc);
            u.s.rot.vx = 0;
            u.s.rot.vy = 0;
            u.s.rot.vz = 0x20;
            sc.vy = sc.vx = 0x1100;
            sc.vz = 0x1000;
            tc = 0xA0 - ((frame << 15) >> 12);
            u.s.col.r = u.s.col.g = u.s.col.b = tc;
            ex->btlfx_drawScreenGrid(1, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &u.s.rot, &sc, &u.s.col, one, NULL);
            if (frame >= 0x10) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        if (col != 0) {
            u.s.col.r = u.s.col.g = u.s.col.b = col;
            u.s.rot.vx = 0;
            u.s.rot.vy = 0;
            u.s.rot.vz = 0;
            sc.vy = sc.vx = scl + 0x1000;
            sc.vz = 0x1000;
            ex->btlfx_drawScreenGrid(1, xo + 0xA0, 0x78, 0x50, 0x3C, 4, 4, 2, 2, &u.s.rot, &sc, &u.s.col, 0, NULL);
        }
        if ((u32)(work->unk0 - 3) < 5) {
            ex->btlfx_drawScrollMove((0x1F4 - work->unk4) * 8, 0x1E0, 0x100, 0x20, 0x80, 0x300, 0x100);
        }
        work->unk4++;
    }
    return 0;
}

s32 fx_0e0_op84(s32 mode, Fx0e0Work84 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    s32 frame;
    s32 a;
    s32 b;
    s32 c;
    s32 k;
    s32 t;
    s32 d;
    u16 r;

    if (mode == 0) {
        *arg4 = 0x10;
    } else {
        if (mode == 1) {
            D_801EA918 = env;
            work->unk0 = 0;
            work->unk4 = 0;
        } else {
            r = c = b = a = 0;
            frame = entry->frame;
            ex->btlfx_getWeaponPos(0x10, 1, (s16 *)&work->unk8);
            switch (work->unk0) {
            case 0:
                r = 0x200;
                a = (frame << 12) / 3;
                b = ex->math_sin(a >> 2) >> 1;
                c = ((0x1000 - a) >> 1) + 0x800;
                if (frame >= 3) {
                    work->unk0 = 1;
                    entry->frame = -1;
                }
                break;
            case 1:
                k = (frame & 1) << 8;
                c = k + 0x1000;
                d = (frame << 12) / 14;
                a = (k + 0x800) + (d >> 1);
                r = (d >> 3) + 0x200;
                b = c;
                if (frame >= 0xE) {
                    work->unk0 = 2;
                    entry->frame = -1;
                }
                break;
            case 2:
                d = (frame << 12) / 14;
                t = d >> 2;
                r = 0x400 - t;
                a = ex->math_cos(t);
                b = ex->math_sin(t) + 0x1000;
                c = ex->math_sin(t) * 2 + 0x1000;
                if (frame >= 0xE) {
                    return 1;
                }
                break;
            }
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = r;
            rot.pad = 0;
            env->otZ = 0x18;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x23), &work->unk8, NULL, b * 3 / 4, 0, 1, a / 2);
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x22), &work->unk8, &rot, c, 0, 1, a * 2 / 3);
            env->otZ = 0;
            work->unk4++;
        }
    }
    return 0;
}
