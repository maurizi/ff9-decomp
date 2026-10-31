#include "fxmod/fx_097.h"

static void fxm_drawTexelQuads(SVECTOR *pos, SVECTOR *rot, s32 tp, s32 x, s32 y, s32 div, s32 idx, s32 size, s32 w, s32 h,
                   s32 scale, s32 bright, s32 abr, u16 *colors, s32 skip) {
    Unk800F78A4 *env = fx_097_env;
    RECT rect;
    VECTOR sv;
    SVECTOR zero;
    SVECTOR v[4];
    CVECTOR c1;
    CVECTOR c3;
    CVECTOR c0;
    CVECTOR c2;
    MATRIX m;
    s32 z;
    s32 flag;
    DR_TPAGE *tpage;
    s32 code;
    BtlfxExports *ex;
    POLY_G4 *prim;
    u16 *row;
    u16 *p;
    s32 i;
    s32 j;
    s32 px;
    s32 py;
    s32 hh;
    s32 x0;
    s32 col;

    *(s32 *)&zero.vx = 0;
    *(s32 *)&zero.vz = 0;
    ex = BTLFX_EXPORTS(env);
    if (skip == 0) {
        /* The nesting only weights w's references for global-alloc (w gets s4 ahead of py and prim). */
        do { do { do { w++; } while (0); } while (0); } while (0);
        hh = h + 1;
        prim = (POLY_G4 *)env->prim;
        env->prim = (u8 *)(prim + w * hh);
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
        gte_stszotz(&z);
        if (rot->pad != 0) {
            ex->MulRotMatrix(&m);
        }
        gte_SetRotMatrix(&m);
        gte_SetTransMatrix(&m);
        x0 = -(w * size) >> 1;
        h = hh;
        py = -(h * size) >> 1;
        row = colors - (u32)(w - 1);
        bright <<= 2;
        gte_ldVZ012_0();
        for (j = 0; j < h; j++, py += size, row += (u32)(w - 1)) {
            px = x0;
            p = row;
            *(s32 *)&c0 = 0;
            *(s32 *)&c2 = 0;
            for (i = 0; i < w; i++, prim++, px += size, p++) {
                v[0].vx = v[2].vx = px;
                v[0].vy = v[1].vy = py;
                v[1].vx = v[3].vx = px + size;
                v[2].vy = v[3].vy = py + size;
                if (i >= w - 1) {
                    *(s32 *)&c1 = 0;
                    *(s32 *)&c3 = 0;
                } else {
                    if (j == 0) {
                        *(s32 *)&c1 = 0;
                    } else {
                        col = p[0];
                        c1.r = ((col & 0x1F) * (bright << 2)) >> 12;
                        c1.g = (((col >> 5) & 0x1F) * (bright << 2)) >> 12;
                        c1.b = (((col >> 10) & 0x1F) * (bright << 2)) >> 12;
                    }
                    if (j >= h - 1) {
                        *(s32 *)&c3 = 0;
                    } else {
                        col = p[w - 1];
                        c3.r = ((col & 0x1F) * (bright << 2)) >> 12;
                        c3.g = (((col >> 5) & 0x1F) * (bright << 2)) >> 12;
                        c3.b = (((col >> 10) & 0x1F) * (bright << 2)) >> 12;
                    }
                }
                gte_ldv3xy(&v[0], &v[1], &v[2]);
                c0.cd = code;
                *(s32 *)&prim->r0 = *(s32 *)&c0;
                gte_rtpt_b();
                *(s32 *)&prim->r1 = *(s32 *)&c1;
                *(s32 *)&prim->r2 = *(s32 *)&c2;
                gte_stsxy3(&prim->x0, &prim->x1, &prim->x2);
                gte_stopz(&flag);
                gte_ldv0xy(&v[3]);
                *(s32 *)&prim->r3 = *(s32 *)&c3;
                gte_rtps_b();
                *(s32 *)&c2 = *(s32 *)&c3;
                *(s32 *)&c0 = *(s32 *)&c1;
                gte_stsxy(&prim->x3);
                if (flag != 0) {
                    ex->btlfx_addPrim(0x8000000, z, prim);
                }
            }
        }
        ex->btlfx_addPrim(0x1000000, z, tpage);
    }
    w--;
    h--;
    x0 = (tp & 0xF) * 64 + x + (idx % div) * w;
    py = (tp & 0x10) * 16 + y + (idx / div) * h;
    rect.x = x0;
    rect.y = py;
    rect.w = w;
    rect.h = h;
    ex->StoreImage(&rect, (u32 *)colors);
}

static void fxm_drawBulgeGrid14(SVECTOR *pos, SVECTOR *rot, s32 scale, s32 c, s32 tpage, s32 clut, s32 u, s32 v, s32 h, s32 abr) {
    Unk800F78A4 *env = fx_097_env;
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

void fx_097_piecePos(s16 *a, s16 *b, s32 i, s16 *out) {
    BTLFX_EXPORTS(fx_097_env)->btlfx_pointAhead(a, b, -((fx_097_pieceDist[i] * 0x455) >> 9), out);
}

static s32 fx_097_op80PoolTask(s32 mode, Fx097Item *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, s32 a5,
                  Fx097Work *work) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR *p;
    Pool6CEntry *e;
    Fx097Item *q;
    s32 t;
    s32 c;
    s32 k;
    s32 n;
    s32 d;

    if (mode == 1) {
        it->state0 = 0;
        it->frame8 = 0;
    } else if (it->flying2 == 0) {
        SVECTOR rot;
        VECTOR scale;
        SVECTOR v38;
        SVECTOR v40;
        SVECTOR v48;
        SVECTOR v50;
        SVECTOR v58;

        p = &fx_097_shardRot[it->index4];
        switch (it->state0) {
        case 0:
            if (it->frame8 == 0) {
                e = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x25), (s32)&it->pos18);
                it->emit34 = e;
                if (e != NULL) {
                    e->otZ = 0x80;
                }
            }
            scale.vx = 0;
            BTLFX_COPY_SVEC(&rot, p);
            rot.vx -= 0x800;
            if (it->emit34 != NULL) {
                it->emit34->rot.vx = rot.vx;
                it->emit34->rot.vy = rot.vy;
                it->emit34->rot.vz = rot.vz;
            }
            if (++it->frame8 > 0) {
                it->state0 = 1;
                it->frame8 = 0;
            }
            break;
        case 1:
            {
                s32 a = it->frame8 << 10;

                scale.vx = scale.vy = (a * 0x8AA) >> 12;
                scale.vz = (ex->math_sin(a >> 2) * 0x8AA) >> 12;
            }
            BTLFX_COPY_SVEC(&rot, p);
            rot.vx -= 0x800;
            if (it->emit34 != NULL) {
                it->emit34->rot.vx = rot.vx;
                it->emit34->rot.vy = rot.vy;
                it->emit34->rot.vz = rot.vz;
            }
            if (++it->frame8 >= 4) {
                it->state0 = 2;
                it->frame8 = 0;
            }
            break;
        case 2:
            scale.vx = scale.vy = scale.vz = 0x8AA;
            BTLFX_COPY_SVEC(&rot, p);
            rot.vx -= 0x800;
            if (it->emit34 != NULL) {
                it->emit34->rot.vx = rot.vx;
                it->emit34->rot.vy = rot.vy;
                it->emit34->rot.vz = rot.vz;
            }
            if (frame < 12) {
                rot.pad = 1;
                v38.vx = it->posC.vx;
                v38.vy = it->posC.vy;
                v38.vz = it->posC.vz;
                {
                    s32 a = (frame << 10) / 12;
                    s32 b;

                    ex->btlfx_pointAhead((s16 *)&v38, (s16 *)&rot, 0x2F6, (s16 *)&v40);
                    d = ex->btlfx_getFileData(entry->file, 0x24);
                    b = (ex->math_sin(a) * 0x1154) >> 12;
                    ex->btlfx_drawRings((u8 *)d, &v40, &rot, b, 0, 1, ex->math_cos(a) * 2 / 3);
                }
            }
            n = 6;
            if (work->shatter78 != 0) {
                it->frame8 = 0;
                if (it->index4 < 3) {
                    n = 10;
                }
                BTLFX_COPY_SVEC(&v50, &it->pos18);
                for (k = 0; k < 4; k++, n++) {
                    q = it;
                    if (k != 0) {
                        q = ex->btlfx_poolSpawn((s16 *)entry->pool);
                    }
                    if (q != NULL) {
                        q->flying2 = 1;
                        q->state0 = n;
                        q->index4 = k;
                        q->frame8 = 0;
                        q->model2C = work->models20[n];
                        fx_097_piecePos((s16 *)&v50, (s16 *)p, n, (s16 *)&v48);
                        BTLFX_COPY_SVEC(&q->rot20, p);
                        q->posC.vx = v48.vx << 4;
                        q->posC.vy = v48.vy << 4;
                        q->posC.vz = v48.vz << 4;
                        q->spinX28 = ex->btlfx_randCentered(0x84);
                        q->spinY2A = ex->btlfx_randCentered(0x84);
                        d = 0x80; /* through a variable, so the 0x80 isn't folded into k * 0x29 */
                        t = -(((ex->btlfx_rand() & 0xF) + d - k * 0x29) * 16);
                        BTLFX_COPY_SVEC(&v58, p);
                        v58.vx += 0x80;
                        ex->btlfx_rotateOffset(&v58, t, (s32)&q->pos18);
                        q->pos18.vx += ex->btlfx_randCentered(0x200);
                        q->pos18.vy += ex->btlfx_randCentered(0x200);
                        q->pos18.vz += ex->btlfx_randCentered(0x200);
                    }
                }
            }
            break;
        }
        if (scale.vx != 0) {
            scale.vx *= 2;
            scale.vy *= 2;
            scale.vz = scale.vz * 5 / 2;
            ex->Hi_SetEffModelOffset(it->model2C, 2);
            ex->Hi_DrawEffModel(p, (VECTOR *)&it->posC, (s32)&scale, it->model2C);
            ex->Hi_DrawEffModel(p, (VECTOR *)&it->posC, (s32)&scale, it->model30);
            return 0;
        }
    } else {
        VECTOR pos;
        VECTOR scale;

        t = ((it->frame8 << 12) / 22) >> 2;
        c = ex->math_cos(t);
        it->posC.vx += it->pos18.vx;
        it->posC.vy += it->pos18.vy;
        it->posC.vz += it->pos18.vz;
        it->pos18.vy += 0x40;
        it->rot20.vx += it->spinX28;
        it->rot20.vy += it->spinY2A;
        c = (c * 3) >> 1;
        if (it->state0 < 6) {
            scale.vx = scale.vy = (ex->math_cos(t) * 0x19FE) >> 12;
            scale.vz = (ex->math_cos(t) * 0x1154) >> 12;
        } else {
            scale.vx = scale.vy = (ex->math_cos(t) * 0x1154) >> 12;
            scale.vz = (ex->math_cos(t) * 0x15A9) >> 12;
        }
        pos.vx = it->posC.vx >> 4;
        pos.vy = it->posC.vy >> 4;
        pos.vz = it->posC.vz >> 4;
        ex->Hi_DrawEffModel(&it->rot20, &pos, (s32)&scale, it->model2C);
        ex->Hi_ModifyEffModelAbr(it->model2C, 1);
        ex->Hi_ModifyEffModelRGB(it->model2C, (c >> 5) - 0x80, (c >> 5) - 0x80, (c >> 5) - 0x80);
        if (++it->frame8 >= 22) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_097/nonmatchings/fx_097", fx_097_headerPtr);

s32 fx_097_op80(s32 mode, Fx097Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR scale;
    SVECTOR rot;
    SVECTOR v;
    s32 k;
    Fx097Item *it;
    Fx097Item *it2;
    Pool6CEntry *p;
    Pool6CEntry *q0;
    Pool6CEntry *q1;
    Pool6CEntry *q2;
    SVECTOR *a;
    s32 frame;
    s32 i;
    s32 j;
    SVECTOR *pr;
    s32 t;
    s32 u;
    s32 c;
    s32 c_7;
    s32 c_5;
    s32 c_3;
    s32 d;

    if (mode == 0) {
        init->workSize = sizeof(Fx097Work80);
        init->elemSize = sizeof(Fx097Item);
        init->elemFn = (s32)fx_097_op80PoolTask;
        init->poolCount = 0x20;
    } else if (mode == 1) {
        fx_097_env = env;
        ex->btlfx_initEffModels(fx_097_effModels, 0x19);
        c = 0x9A;
        i = 0;
        work->model8 = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x14), c, 0x3DC0, 0x80, 0);
        work->modelC = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x12), c, 0x3DC0, 0x80, 0);
        work->model10 = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x13), c, 0x3DC0, 0x80, 0);
        work->model14 = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x11), 0xBA, 0x3D40, 0, 0x80);
        work->model18 = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0xF), 0xBA, 0x3D40, 0, 0x80);
        work->model1C = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x10), 0xBA, 0x3D40, 0, 0x80);
        for (; i < 14; i++) {
            work->model20[i] =
                (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, fx_097_pieceBlocks[i]), c, 0x3DC0, 0x80, 0);
        }
        work->state0 = 0;
        work->frame4 = 0;
        work->shardCount7A = 0;
        work->shatter78 = 0;
        work->scrollStep7C = 0;
        work->scroll80 = 0;
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos58);
        BTLFX_COPY_SVEC(&work->pos60, &work->pos58);
        BTLFX_COPY_SVEC(&work->unk70, &work->pos58);
        work->pos60.vy -= 0x22A;
        work->unk70.vy -= 0x80;
    } else {
        frame = entry->frame;
        pos.vx = work->pos58.vx;
        pos.vy = work->pos58.vy;
        pos.vz = work->pos58.vz;
        scale.vx = scale.vy = 0x1DFE;
        scale.vz = 0x1354;
        k = 0;
        switch (work->state0) {
        case 0:
            rot.vx = 0x400;
            rot.vz = 0x200;
            rot.vy = 0;
            t = (frame << 12) / 6;
            fxm_drawBulgeGrid14(&work->pos58, &rot, 0x26A8, t >> 5, 0x9A, 0x3D40, 0, 0x80, 0, 1);
            ex->btlfx_drawScrollMove(work->frame4 * 3, 0x280, 0x180, 0x40, 0x80, 0x280, 0x100);
            if (frame >= 6) {
                work->state0 = 1;
                entry->frame = -1;
            }
            break;
        case 1:
            t = (frame << 12) / 10;
            u = t >> 2;
            rot.vx = 0x400;
            rot.vy = 0;
            rot.vz = 0x200;
            fxm_drawBulgeGrid14(&work->pos58, &rot, 0x26A8, 0x80, 0x9A, 0x3D40, 0, 0x80, -((ex->math_sin(u) * 0x2BC) >> 12), 1);
            ex->btlfx_drawScrollMove(work->frame4 * 3, 0x280, 0x180, 0x40, 0x80, 0x280, 0x100);
            k = t;
            rot.pad = 1;
            d = ex->btlfx_getFileData(entry->file, 0x24);
            ex->btlfx_drawRings((u8 *)d, &work->pos58, &rot, ((((ex->math_sin(u) * 0x19FE) >> 12) + 0x1154) * 3) >> 2, 0, 1,
                              ex->math_cos(u));
            if (frame >= 10) {
                work->state0 = 2;
                entry->frame = -1;
                BTLFX_COPY_SVEC(&work->pos68, &work->pos58);
                work->pos68.vy -= 0x5C6;
                q0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x26), (s32)&work->pos58);
                if (q0 != NULL) {
                    q0->otZ = 0x80;
                }
                q1 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x27), (s32)&work->pos68);
                if (q1 != NULL) {
                    q1->otZ = 0x80;
                }
                q2 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x29), (s32)&work->pos58);
                if (q2 != NULL) {
                    q2->otZ = 0x80;
                }
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, 0xFF, 0xFF, 0);
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0, 0, 0x80);
            }
            break;
        case 2:
            k = 0x1000;
            t = frame << 10;
            t = 0x2000 - t;
            rot.vx = -0x400;
            rot.vy = 0;
            rot.vz = -0x400;
            ex->Hi_SetEffModelOffset(work->model8, 2);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model8);
            ex->Hi_ModifyEffModelRGB(work->model8, (t >> 5) - 0x80, (t >> 5) - 0x80, (t >> 5) - 0x80);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model14);
            if (frame < 6) {
                t = 0xFF - (frame * 0xFF) / 6;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, t >> 2, t * 2 / 3, t);
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
            k = 0x1000;
            if (frame & 1) {
                it = ex->btlfx_poolSpawn((s16 *)entry->pool);
                if (it != NULL) {
                    it->flying2 = 0;
                    it->index4 = work->shardCount7A;
                    a = &fx_097_shardRot[work->shardCount7A];
                    if (work->shardCount7A >= 3) {
                        it->model2C = work->modelC;
                        it->model30 = work->model18;
                    } else {
                        it->model2C = work->model10;
                        it->model30 = work->model1C;
                    }
                    it->posC.vx = work->pos58.vx;
                    it->posC.vy = work->pos58.vy;
                    it->posC.vz = work->pos58.vz;
                    t = ex->math_sin(a->vy + 0x800);
                    c_3 = 0xC8;
                    it->posC.vx += (t * c_3) >> 12;
                    it->posC.vz += (ex->math_cos(a->vy + 0x800) * c_3) >> 12;
                    it->pos18.vx = it->posC.vx;
                    it->pos18.vy = it->posC.vy;
                    it->pos18.vz = it->posC.vz;
                    work->shardCount7A++;
                }
            }
            rot.vx = -0x400;
            rot.vy = 0;
            rot.vz = -0x400;
            ex->Hi_SetEffModelOffset(work->model8, 2);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model8);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model14);
            if (work->shardCount7A >= 6) {
                work->state0 = 4;
                entry->frame = -1;
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x1C), (s32)&work->pos58);
                work->emit84 = p;
                if (p != NULL) {
                    p->otZ = 0x5C;
                }
            }
            break;
        case 4:
            k = 0x1000;
            rot.vx = -0x400;
            rot.vy = 0;
            rot.vz = -0x400;
            ex->Hi_SetEffModelOffset(work->model8, 2);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model8);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model14);
            if (frame >= 8) {
                work->state0 = 5;
                entry->frame = -1;
            }
            break;
        case 5:
            pr = &rot; /* one pseudo (s2) for case 5's calls and the spawn loop's */
            k = 0x1000;
            rot.vx = -0x400;
            rot.vy = 0;
            rot.vz = -0x400;
            ex->Hi_SetEffModelOffset(work->model8, 2);
            ex->Hi_DrawEffModel(pr, &pos, (s32)&scale, work->model8);
            ex->Hi_DrawEffModel(pr, &pos, (s32)&scale, work->model14);
            rot.pad = 1;
            if (frame >= 10) {
                work->state0 = 6;
                work->shatter78 = 1;
                entry->frame = -1;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, 0xFF, 0xFF, 0);
                ex->btlfx_stopEmitter(work->emit84);
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x20), (s32)&work->pos58);
                if (p != NULL) {
                    p->otZ = 0x60;
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x1F), (s32)&work->pos58);
                if (p != NULL) {
                    p->otZ = 0x60;
                }
                ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x1D), (s32)&work->pos60);
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x21), (s32)&work->pos60);
                if (p != NULL) {
                    p->otZ = 0x80;
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x22), (s32)&work->pos60);
                if (p != NULL) {
                    p->otZ = 0x80;
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x28), (s32)&work->pos60);
                if (p != NULL) {
                    p->otZ = 0x80;
                }
                j = 0;
                rot.vx = -0x400;
                rot.vy = 0;
                rot.vz = -0x400;
            loop:
                it2 = ex->btlfx_poolSpawn((s16 *)entry->pool);
                if (it2 != NULL) {
                    it2->flying2 = 1;
                    it2->state0 = j;
                    it2->index4 = j;
                    it2->frame8 = 0;
                    it2->model2C = work->model20[j];
                    fx_097_piecePos((s16 *)&work->pos58, (s16 *)pr, j, (s16 *)&v);
                    BTLFX_COPY_SVEC(&it2->rot20, &rot);
                    it2->posC.vx = v.vx << 4;
                    it2->posC.vy = v.vy << 4;
                    it2->posC.vz = v.vz << 4;
                    it2->spinX28 = ex->btlfx_randCentered(0x5C);
                    it2->spinY2A = ex->btlfx_randCentered(0x80);
                    c = 0x5C;
                    ex->btlfx_rotateOffset(pr, -(((ex->btlfx_rand() & 0xF) + c - (j << 3)) << 4), (s32)&it2->pos18);
                    it2->pos18.vx += ex->btlfx_randCentered(0x3E0);
                    it2->pos18.vz += ex->btlfx_randCentered(0x3E0);
                }
                j++;
                if (j < 6) {
                    goto loop;
                }
            }
            break;
        case 6:
            rot.vx = -0x400;
            rot.vz = -0x400;
            rot.vy = 0;
            if (frame < 18) {
                k = ex->math_cos((frame << 10) / 18);
            }
            if (frame == 5) {
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x1E), (s32)&work->pos60);
                if (p != NULL) {
                    p->otZ = 0x80;
                }
            }
            if (frame < 8) {
                c_5 = 0xFF - ((frame * 0xFF) >> 3);
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, c_5 >> 2, c_5 * 2 / 3, c_5);
                if (frame == 0) {
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0xFF, 0xFF, 0xFF);
                }
            }
            if (frame >= 0x18) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        if (work->state0 - 2 < 4) {
            if (++work->scrollStep7C < 11) {
                c_7 = work->scroll80 + 11;
                work->scroll80 = c_7 - (work->scrollStep7C * 10) / 11;
            } else {
                work->scroll80++;
            }
            ex->btlfx_drawScrollMove(work->scroll80, 0x280, 0x180, 0x40, 0x80, 0x280, 0x100);
        }
        if (k != 0) {
            v.vx = 0x400;
            v.vy = 0;
            v.vz = work->frame4 * 0x29;
            v.pad = 1;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x23), &work->pos58, &v, (work->frame4 << 6) + 0x15A9, 0,
                              1, k);
            v.vz = work->frame4 * 0x18 + 0x400;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x23), &work->pos58, &v, (work->frame4 << 6) + 0x15A9, 0,
                              1, k);
            env->otZ = 0x127;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x1B), &work->pos58, NULL, (work->frame4 << 5) + 0x15A9, 0,
                              1, k);
            env->otZ = 0;
        }
        work->frame4++;
    }
    return 0;
}
