#include "fxmod/fx_1cc.h"

static void fxm_drawTexelQuads(SVECTOR *pos, SVECTOR *rot, s32 tp, s32 x, s32 y, s32 div, s32 idx, s32 size, s32 w, s32 h,
                   s32 scale, s32 bright, s32 abr, u16 *colors, s32 skip) {
    Unk800F78A4 *env = fx_1cc_env;
    RECT rect;
    VECTOR sv;
    SVECTOR zero;
    SVECTOR quad[4];
    CVECTOR c1;
    CVECTOR c3;
    CVECTOR c0;
    CVECTOR c2;
    MATRIX m;
    s32 otz;
    s32 nclip;
    DR_TPAGE *tpage;
    s32 code;
    BtlfxExports *ex;
    POLY_G4 *prim;
    u16 *row;
    u16 *src;
    s32 i;
    s32 j;
    s32 px;
    s32 py;
    s32 rows;
    s32 left;
    s32 texel;

    *(s32 *)&zero.vx = 0;
    *(s32 *)&zero.vz = 0;
    ex = BTLFX_EXPORTS(env);
    if (skip == 0) {
        /* The nesting only weights w's references for global-alloc (w gets s4 ahead of py and prim). */
        do { do { do { w++; } while (0); } while (0); } while (0);
        rows = h + 1;
        prim = (POLY_G4 *)env->prim;
        env->prim = (u8 *)(prim + w * rows);
        tpage = ex->btlfx_allocDrMode(0, abr);
        gte_SetRotMatrix(env->viewMatrix);
        gte_SetTransMatrix(env->viewMatrix);
        gte_ldv0(pos);
        if (rot == NULL) {
            rot = &zero;
        }
        gte_rtv0tr_b();
        ex->math_rotMatrixYXZ(rot, &m);
        if (scale >= 0) {
            sv.vx = sv.vy = sv.vz = scale;
            ex->ScaleMatrix(&m, &sv);
        }
        gte_stlvnl(m.t);
        gte_rtps();
        code = 0x38;
        if (abr != 0xFF) {
            code = 0x3A;
        }
        gte_stszotz(&otz);
        if (rot->pad != 0) {
            ex->MulRotMatrix(&m);
        }
        gte_SetRotMatrix(&m);
        gte_SetTransMatrix(&m);
        left = -(w * size) >> 1;
        h = rows;
        py = -(h * size) >> 1;
        row = colors - (u32)(w - 1);
        bright <<= 2;
        gte_ldVZ012_0();
        for (j = 0; j < h; j++, py += size, row += (u32)(w - 1)) {
            px = left;
            src = row;
            *(s32 *)&c0 = 0;
            *(s32 *)&c2 = 0;
            for (i = 0; i < w; i++, prim++, px += size, src++) {
                quad[0].vx = quad[2].vx = px;
                quad[0].vy = quad[1].vy = py;
                quad[1].vx = quad[3].vx = px + size;
                quad[2].vy = quad[3].vy = py + size;
                if (i >= w - 1) {
                    *(s32 *)&c1 = 0;
                    *(s32 *)&c3 = 0;
                } else {
                    if (j == 0) {
                        *(s32 *)&c1 = 0;
                    } else {
                        texel = src[0];
                        c1.r = ((texel & 0x1F) * (bright << 2)) >> 12;
                        c1.g = (((texel >> 5) & 0x1F) * (bright << 2)) >> 12;
                        c1.b = (((texel >> 10) & 0x1F) * (bright << 2)) >> 12;
                    }
                    if (j >= h - 1) {
                        *(s32 *)&c3 = 0;
                    } else {
                        texel = src[w - 1];
                        c3.r = ((texel & 0x1F) * (bright << 2)) >> 12;
                        c3.g = (((texel >> 5) & 0x1F) * (bright << 2)) >> 12;
                        c3.b = (((texel >> 10) & 0x1F) * (bright << 2)) >> 12;
                    }
                }
                gte_ldv3xy(&quad[0], &quad[1], &quad[2]);
                c0.cd = code;
                *(s32 *)&prim->r0 = *(s32 *)&c0;
                gte_rtpt_b();
                *(s32 *)&prim->r1 = *(s32 *)&c1;
                *(s32 *)&prim->r2 = *(s32 *)&c2;
                gte_stsxy3(&prim->x0, &prim->x1, &prim->x2);
                gte_stopz(&nclip);
                gte_ldv0xy(&quad[3]);
                *(s32 *)&prim->r3 = *(s32 *)&c3;
                gte_rtps_b();
                *(s32 *)&c2 = *(s32 *)&c3;
                *(s32 *)&c0 = *(s32 *)&c1;
                gte_stsxy(&prim->x3);
                if (nclip != 0) {
                    ex->btlfx_addPrim(0x8000000, otz, prim);
                }
            }
        }
        ex->btlfx_addPrim(0x1000000, otz, tpage);
    }
    w--;
    h--;
    left = (tp & 0xF) * 64 + x + (idx % div) * w;
    py = (tp & 0x10) * 16 + y + (idx / div) * h;
    rect.x = left;
    rect.y = py;
    rect.w = w;
    rect.h = h;
    ex->StoreImage(&rect, (u32 *)colors);
}

static void fxm_drawBulgeGrid14(SVECTOR *pos, SVECTOR *rot, s32 scale, s32 c, s32 tpage, s32 clut, s32 u, s32 v, s32 h, s32 abr) {
    Unk800F78A4 *env = fx_1cc_env;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    MATRIX m;
    VECTOR sc;
    SVECTOR q[4];
    SVECTOR o;
    s32 otz;
    s32 row;
    s32 code;
    s32 rx;
    s32 y;
    POLY_GT4 *prim;
    s32 col;
    s32 d0;
    s32 d1;
    s32 d2;
    s32 d3;
    s32 k;
    s32 tu;
    s32 tu2;
    s32 tv;
    s32 tv2;

    *(s32 *)&o.vx = 0;
    o.vz = 0;
    gte_SetRotMatrix(env->viewMatrix);
    gte_SetTransMatrix(env->viewMatrix);
    gte_ldv0(pos);
    prim = (POLY_GT4 *)env->prim;
    code = 0x3C;
    env->prim = (u8 *)(prim + 14 * 14);
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
    for (row = 0; row < 14; row++) {
        y = (row << 6) - 0x180;
        rx = row << 6;
        for (col = 0; col < 14; col++) {
            q[0].vz = q[1].vz = q[2].vz = q[3].vz = 0;
            q[0].vx = q[2].vx = rx - 0x1C0;
            q[1].vx = q[3].vx = y;
            q[0].vy = q[1].vy = (col << 6) - 0x1C0;
            q[2].vy = q[3].vy = (col << 6) - 0x180;
            d0 = ex->btlfx_distance(&q[0], &o);
            d1 = ex->btlfx_distance(&q[1], &o);
            d2 = ex->btlfx_distance(&q[2], &o);
            d3 = ex->btlfx_distance(&q[3], &o);
            if (d0 > 0x100) {
                q[0].vz = 0;
            } else {
                q[0].vz = -(((ex->math_cos(d0 * 8) + 0x1000) * h) >> 13);
            }
            if (d1 > 0x100) {
                q[1].vz = 0;
            } else {
                q[1].vz = -(((ex->math_cos(d1 * 8) + 0x1000) * h) >> 13);
            }
            if (d2 > 0x100) {
                q[2].vz = 0;
            } else {
                q[2].vz = -(((ex->math_cos(d2 * 8) + 0x1000) * h) >> 13);
            }
            if (d3 > 0x100) {
                q[3].vz = 0;
            } else {
                q[3].vz = -(((ex->math_cos(d3 * 8) + 0x1000) * h) >> 13);
            }
            gte_ldv3c(q);
            gte_rtpt();
            k = c - (-q[0].vz >> 3);
            if (k >= 0x100) {
                k = 0xFF;
            }
            if (d0 > 0x1A0) {
                k = 0;
            }
            prim->code = code;
            prim->r0 = prim->g0 = prim->b0 = k;
            k = c - (-q[1].vz >> 3);
            if (k >= 0x100) {
                k = 0xFF;
            }
            if (d1 > 0x1A0) {
                k = 0;
            }
            prim->r1 = prim->g1 = prim->b1 = k;
            k = c - (-q[2].vz >> 3);
            if (k >= 0x100) {
                k = 0xFF;
            }
            if (d2 > 0x1A0) {
                k = 0;
            }
            prim->r2 = prim->g2 = prim->b2 = k;
            k = c - (-q[3].vz >> 3);
            if (k >= 0x100) {
                k = 0xFF;
            }
            if (d3 > 0x1A0) {
                k = 0;
            }
            prim->r3 = prim->g3 = prim->b3 = k;
            gte_nclip();
            prim->tpage = tpage;
            prim->clut = clut;
            gte_stopz(&otz);
            if (otz != 0) {
                gte_stsxy3(&prim->x0, &prim->x1, &prim->x2);
                gte_avsz3();
                gte_stszotz(&otz);
                gte_ldv0(&q[3]);
                gte_rtps();
                tu = u + ((row & 3) << 5);
                tu2 = tu + 0x20;
                tv = v + ((col & 3) << 5);
                if ((row & 3) == 3) {
                    tu2 = tu + 0x1F;
                }
                tv2 = tv + 0x20;
                if ((col & 3) == 3) {
                    tv2 = tv + 0x1F;
                }
                prim->u0 = prim->u2 = tu;
                prim->u1 = prim->u3 = tu2;
                prim->v0 = prim->v1 = tv;
                prim->v2 = prim->v3 = tv2;
                ex->btlfx_addPrim(0xC000000, otz, prim);
                gte_stsxy(&prim->x3);
                prim++;
            }
        }
    }
}

void fx_1cc_piecePos(s16 *from, s16 *to, s32 i, s16 *out) {
    BTLFX_EXPORTS(fx_1cc_env)->btlfx_pointAhead(from, to, -(fx_1cc_pieceDists[i] * 4), out);
}

static s32 fx_1cc_op80PoolTask(s32 mode, Fx1ccObj *o, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, s32 pool,
                  Fx1ccInst *inst) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 t;
    s32 k;

    if (mode == 1) {
        o->state0 = 0;
        o->frame8 = 0;
    } else if (o->kind2 == 0) {
        SVECTOR a;
        VECTOR sc;
        SVECTOR p;
        SVECTOR q;
        SVECTOR b;
        SVECTOR c;
        SVECTOR d;
        SVECTOR *tab;
        Fx1ccObj *ch;
        Pool6CEntry *emitter;
        u8 *f;
        s32 n;
        s32 i;
        s32 r;
        s32 m;
        s32 w;
        s32 x;
        s32 u;

        tab = &fx_1cc_dirRots[o->index4];
        switch (o->state0) {
        case 0:
            if (o->frame8 == 0) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xE), (s32)&o->velX18);
                o->emit34 = emitter;
                if (emitter != NULL) {
                    emitter->otZ = 0x80;
                }
            }
            sc.vx = 0;
            BTLFX_COPY_SVEC(&a, tab);
            a.vx -= 0x800;
            if (o->emit34 != NULL) {
                o->emit34->rot.vx = a.vx;
                o->emit34->rot.vy = a.vy;
                o->emit34->rot.vz = a.vz;
            }
            if (++o->frame8 > 0) {
                o->state0 = 1;
                o->frame8 = 0;
            }
            break;
        case 1:
            w = o->frame8;
            sc.vx = sc.vy = ((w << 10) * 0x1000) >> 12;
            sc.vz = (ex->math_sin((w << 10) >> 2) * 0x1000) >> 12;
            BTLFX_COPY_SVEC(&a, tab);
            a.vx -= 0x800;
            if (o->emit34 != NULL) {
                o->emit34->rot.vx = a.vx;
                o->emit34->rot.vy = a.vy;
                o->emit34->rot.vz = a.vz;
            }
            if (++o->frame8 >= 4) {
                o->state0 = 2;
                o->frame8 = 0;
            }
            break;
        case 2:
            sc.vx = sc.vy = sc.vz = 0x1000;
            BTLFX_COPY_SVEC(&a, tab);
            a.vx -= 0x800;
            if (o->emit34 != NULL) {
                o->emit34->rot.vx = a.vx;
                o->emit34->rot.vy = a.vy;
                o->emit34->rot.vz = a.vz;
            }
            if (frame < 12) {
                u = (frame << 10) / 12;
                a.pad = 1;
                p.vx = o->posXC;
                p.vy = o->posY10;
                p.vz = o->posZ14;
                ex->btlfx_pointAhead((s16 *)&p, (s16 *)&a, 0x578, (s16 *)&q);
                f = (u8 *)ex->btlfx_getFileData(entry->file, 0x26);
                k = (ex->math_sin(u) * 0x2000) >> 12;
                ex->btlfx_drawRings(f, &q, &a, k, 0, 1, ex->math_cos(u) * 2 / 3);
            }
            if (inst->flag78 != 0) {
                n = 6;
                o->frame8 = 0;
                if (o->index4 < 3) {
                    n = 10;
                }
                BTLFX_COPY_SVEC(&c, &o->velX18);
                for (i = 0, m = 0; i < 4; m += 0x29, i++, n++) {
                    ch = o;
                    if (i != 0) {
                        ch = ex->btlfx_poolSpawn((s16 *)entry->pool);
                    }
                    if (ch != NULL) {
                        ch->kind2 = 1;
                        ch->state0 = n;
                        ch->index4 = i;
                        ch->frame8 = 0;
                        ch->model2C = inst->model20[n];
                        fx_1cc_piecePos((s16 *)&c, (s16 *)tab, n, (s16 *)&b);
                        BTLFX_COPY_SVEC(&ch->rot20, tab);
                        ch->posXC = b.vx << 4;
                        ch->posY10 = b.vy << 4;
                        ch->posZ14 = b.vz << 4;
                        ch->spin28 = ex->btlfx_randCentered(0x84);
                        ch->spin2A = ex->btlfx_randCentered(0x84);
                        x = (ex->btlfx_rand() & 0xF) + 0x80;
                        r = (x - m) << 4;
                        BTLFX_COPY_SVEC(&d, tab);
                        d.vx += 0x80;
                        ex->btlfx_rotateOffset(&d, -r, (s32)&ch->velX18);
                        ch->velX18 += ex->btlfx_randCentered(0x200);
                        ch->velY1A += ex->btlfx_randCentered(0x200);
                        ch->velZ1C += ex->btlfx_randCentered(0x200);
                    }
                }
            }
            break;
        }
        if (sc.vx != 0) {
            sc.vx *= 2;
            sc.vy *= 2;
            sc.vz = sc.vz * 5 / 2;
            ex->Hi_SetEffModelOffset(o->model2C, 2);
            ex->Hi_DrawEffModel(tab, (VECTOR *)&o->posXC, (s32)&sc, o->model2C);
            ex->Hi_DrawEffModel(tab, (VECTOR *)&o->posXC, (s32)&sc, o->model30);
        }
    } else {
        VECTOR pos;
        VECTOR sc;
        s32 c;

        t = ((o->frame8 << 12) / 22) >> 2;
        c = ex->math_cos(t);
        o->posXC += o->velX18;
        o->posY10 += o->velY1A;
        o->posZ14 += o->velZ1C;
        o->velY1A += 0x40;
        o->rot20.vx += o->spin28;
        o->rot20.vy += o->spin2A;
        c = (c * 3) >> 1;
        if (o->state0 < 6) {
            sc.vx = sc.vy = (ex->math_cos(t) * 0x3000) >> 12;
            sc.vz = (ex->math_cos(t) * 0x2000) >> 12;
        } else {
            sc.vx = sc.vy = (ex->math_cos(t) * 0x2000) >> 12;
            sc.vz = (ex->math_cos(t) * 0x2800) >> 12;
        }
        pos.vx = o->posXC >> 4;
        pos.vy = o->posY10 >> 4;
        pos.vz = o->posZ14 >> 4;
        ex->Hi_DrawEffModel(&o->rot20, &pos, (s32)&sc, o->model2C);
        ex->Hi_ModifyEffModelAbr(o->model2C, 1);
        ex->Hi_ModifyEffModelRGB(o->model2C, (c >> 5) - 0x80, (c >> 5) - 0x80, (c >> 5) - 0x80);
        if (++o->frame8 >= 0x16) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1cc/nonmatchings/fx_1cc", fx_1cc_headerPtr);

s32 fx_1cc_op80(s32 mode, Fx1ccWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR sc;
    SVECTOR rot;
    SVECTOR r;
    s32 c;
    s32 frame;
    s32 i;
    s32 j;
    s32 k;
    s32 t;
    s32 a;
    s32 v;
    s32 rnd;
    s32 acc;
    s32 rad;
    u8 *f;
    SVECTOR *tab;
    Fx1ccObj *ch;
    Fx1ccObj *ch3;
    Pool6CEntry *p;
    Pool6CEntry *p1;

    if (mode == 0) {
        init->workSize = 0x88;
        init->elemSize = 0x38;
        init->elemFn = (s32)fx_1cc_op80PoolTask;
        init->poolCount = 0x20;
    } else if (mode == 1) {
        fx_1cc_env = env;
        ex->btlfx_initEffModels(fx_1cc_effModels, 0x19);
        k = 0x9A;
        work->model8 = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x29), k, 0x3DC0, 0x80, 0);
        work->modelC = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x27), k, 0x3DC0, 0x80, 0);
        work->model10 = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x28), k, 0x3DC0, 0x80, 0);
        work->model14 = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x20), 0xBA, 0x3D40, 0, 0x80);
        work->model18 = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x1E), 0xBA, 0x3D40, 0, 0x80);
        work->model1C = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x1F), 0xBA, 0x3D40, 0, 0x80);
        for (i = 0; i < 14; i++) {
            work->model20[i] = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, fx_1cc_pieceFileIds[i]), k, 0x3DC0, 0x80, 0);
        }
        work->state0 = 0;
        work->frame4 = 0;
        work->index7A = 3;
        work->flag78 = 0;
        work->frame7C = 0;
        work->scroll80 = 0;
        work->pos58.vx = env->targets->centerX;
        work->pos58.vy = env->targets->centerY;
        work->pos58.vz = env->targets->centerZ;
        work->pos60.vx = work->pos58.vx;
        work->pos60.vy = work->pos58.vy;
        work->pos60.vz = work->pos58.vz;
        work->pos60.vy -= 0x555;
        BTLFX_COPY_SVEC(&work->unk70, &work->pos58);
        work->unk70.vy -= 0x80;
    } else {
        frame = entry->frame;
        pos.vx = work->pos58.vx;
        pos.vy = work->pos58.vy;
        pos.vz = work->pos58.vz;
        sc.vx = sc.vy = 0x3400;
        sc.vz = 0x2200;
        c = 0;
        switch (work->state0) {
        case 0:
            rot.vx = 0x400;
            rot.vz = 0x200;
            rot.vy = 0;
            t = (frame << 12) / 6;
            fxm_drawBulgeGrid14(&work->pos58, &rot, 0x4400, t >> 5, 0x9A, 0x3D40, 0, 0x80, 0, 1);
            ex->btlfx_drawScrollMove(work->frame4 * 3, 0x280, 0x180, 0x40, 0x80, 0x280, 0x100);
            if (frame >= 6) {
                work->state0 = 1;
                entry->frame = -1;
            }
            break;
        case 1:
            t = (frame << 12) / 10;
            a = t >> 2;
            rot.vx = 0x400;
            rot.vy = 0;
            rot.vz = 0x200;
            fxm_drawBulgeGrid14(&work->pos58, &rot, 0x4400, 0x80, 0x9A, 0x3D40, 0, 0x80, -((ex->math_sin(a) * 700) >> 12), 1);
            ex->btlfx_drawScrollMove(work->frame4 * 3, 0x280, 0x180, 0x40, 0x80, 0x280, 0x100);
            c = t;
            rot.pad = 1;
            f = (u8 *)ex->btlfx_getFileData(entry->file, 0x26);
            ex->btlfx_drawRings(f, &work->pos58, &rot, ((((ex->math_sin(a) * 0x3000) >> 12) + 0x2000) * 3) >> 2, 0, 1,
                              ex->math_cos(a));
            if (frame >= 10) {
                work->state0 = 2;
                entry->frame = -1;
                BTLFX_COPY_SVEC(&work->pos68, &work->pos58);
                work->pos68.vy -= 0xAAA;
                p1 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xD), (s32)&work->pos58);
                if (p1 != NULL) {
                    p1->otZ = 0x80;
                }
                p1 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xF), (s32)&work->pos68);
                if (p1 != NULL) {
                    p1->otZ = 0x80;
                }
                p1 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xB), (s32)&work->pos58);
                if (p1 != NULL) {
                    p1->otZ = 0x80;
                }
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, 0xFF, 0xFF, 0);
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0, 0, 0x80);
            }
            break;
        case 2:
            c = 0x1000;
            t = frame << 10;
            t = 0x2000 - t;
            rot.vx = -0x400;
            rot.vy = 0;
            rot.vz = -0x400;
            ex->Hi_SetEffModelOffset(work->model8, 2);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->model8);
            ex->Hi_ModifyEffModelRGB(work->model8, (t >> 5) - 0x80, (t >> 5) - 0x80, (t >> 5) - 0x80);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->model14);
            if (frame < 6) {
                t = 0xFF - (frame * 0xFF) / 6;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, t >> 2, (t * 2) / 3, t);
                if (frame == 0) {
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0xFF, 0xFF, 0xFF);
                }
            }
            if (frame >= 4) {
                work->state0 = 3;
                entry->frame = -1;
            }
            break;
        case 3:
            c = 0x1000;
            if (frame & 1) {
                ch3 = ex->btlfx_poolSpawn((s16 *)entry->pool);
                if (ch3 != NULL) {
                    ch3->kind2 = 0;
                    ch3->index4 = work->index7A;
                    tab = &fx_1cc_dirRots[work->index7A];
                    if (work->index7A >= 3) {
                        ch3->model2C = work->modelC;
                        ch3->model30 = work->model18;
                    } else {
                        ch3->model2C = work->model10;
                        ch3->model30 = work->model1C;
                    }
                    ch3->posXC = work->pos58.vx;
                    ch3->posY10 = work->pos58.vy;
                    ch3->posZ14 = work->pos58.vz;
                    rad = 0x1DD;
                    ch3->posXC += (ex->math_sin(tab->vy + 0x800) * rad) >> 12;
                    ch3->posZ14 += (ex->math_cos(tab->vy + 0x800) * rad) >> 12;
                    ch3->velX18 = ch3->posXC;
                    ch3->velY1A = ch3->posY10;
                    ch3->velZ1C = ch3->posZ14;
                    work->index7A++;
                }
            }
            rot.vx = -0x400;
            rot.vy = 0;
            rot.vz = -0x400;
            ex->Hi_SetEffModelOffset(work->model8, 2);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->model8);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->model14);
            if (work->index7A >= 6) {
                work->state0 = 4;
                entry->frame = -1;
            }
            break;
        case 4:
            c = 0x1000;
            rot.vx = -0x400;
            rot.vy = 0;
            rot.vz = -0x400;
            ex->Hi_SetEffModelOffset(work->model8, 2);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->model8);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->model14);
            if (frame >= 8) {
                work->state0 = 5;
                entry->frame = -1;
            }
            break;
        case 5:
            c = 0x1000;
            t = (frame << 12) / 10;
            ex->btlfx_fadeUnitById(0, (t << 7) >> 12);
            rot.vx = -0x400;
            rot.vy = 0;
            rot.vz = -0x400;
            ex->Hi_SetEffModelOffset(work->model8, 2);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->model8);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->model14);
            rot.pad = 1;
            if (frame >= 10) {
                work->state0 = 6;
                work->flag78 = 1;
                entry->frame = -1;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, 0xFF, 0xFF, 0);
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->pos58);
                if (p != NULL) {
                    p->otZ = 0x60;
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->pos58);
                if (p != NULL) {
                    p->otZ = 0x60;
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos60);
                if (p != NULL) {
                    p->otZ = 0x80;
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->pos60);
                if (p != NULL) {
                    p->otZ = 0x80;
                }
                rot.vx = -0x400;
                rot.vy = 0;
                rot.vz = -0x400;
                for (j = 0; j < 6; j++) {
                    ch = ex->btlfx_poolSpawn((s16 *)entry->pool);
                    if (ch != NULL) {
                        ch->kind2 = 1;
                        ch->state0 = j;
                        ch->index4 = j;
                        ch->frame8 = 0;
                        ch->model2C = work->model20[j];
                        fx_1cc_piecePos((s16 *)&work->pos58, (s16 *)&rot, j, (s16 *)&r);
                        ((s32 *)&ch->rot20)[0] = ((s32 *)&rot)[0];
                        ((s32 *)&ch->rot20)[1] = ((s32 *)&rot)[1];
                        ch->posXC = r.vx << 4;
                        ch->posY10 = r.vy << 4;
                        ch->posZ14 = r.vz << 4;
                        ch->spin28 = ex->btlfx_randCentered(0x5C);
                        ch->spin2A = ex->btlfx_randCentered(0x80);
                        rnd = (ex->btlfx_rand() & 0xF) + 0x5C;
                        ex->btlfx_rotateOffset(&rot, -((rnd - j * 8) << 4), (s32)&ch->velX18);
                        ch->velX18 += ex->btlfx_randCentered(0x3E0);
                        ch->velZ1C += ex->btlfx_randCentered(0x3E0);
                    }
                }
            }
            break;
        case 6:
            rot.vx = -0x400;
            rot.vz = -0x400;
            rot.vy = 0;
            if (frame < 18) {
                c = ex->math_cos((frame << 10) / 18);
            }
            if (frame < 8) {
                v = 0xFF - ((frame * 0xFF) >> 3);
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, v >> 2, (v * 2) / 3, v);
                if (frame == 0) {
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0xFF, 0xFF, 0xFF);
                }
            }
            if (frame >= 24) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        if ((u32)(work->state0 - 2) < 4) {
            if (++work->frame7C < 11) {
                acc = work->scroll80 + 11;
                work->scroll80 = acc - (work->frame7C * 10) / 11;
            } else {
                work->scroll80++;
            }
            ex->btlfx_drawScrollMove(work->scroll80, 0x280, 0x180, 0x40, 0x80, 0x280, 0x100);
        }
        if (c != 0) {
            r.vx = 0x400;
            r.vy = 0;
            r.vz = work->frame4 * 41;
            r.pad = 1;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x24), &work->pos58, &r, (work->frame4 << 6) + 0x2800,
                              0, 1, c);
            r.vz = work->frame4 * 24 + 0x400;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x24), &work->pos58, &r, (work->frame4 << 6) + 0x2800,
                              0, 1, c);
            env->otZ = 0x127;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x25), &work->pos58, NULL, (work->frame4 << 5) + 0x2800,
                              0, 1, c);
            env->otZ = 0;
        }
        work->frame4++;
    }
    return 0;
}
