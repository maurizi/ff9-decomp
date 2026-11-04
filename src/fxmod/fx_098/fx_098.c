#include "fxmod/fx_098.h"

static void fxm_drawTexelQuads(SVECTOR *pos, SVECTOR *rot, s32 tp, s32 x, s32 y, s32 div, s32 idx, s32 size, s32 w, s32 h,
                   s32 scale, s32 bright, s32 abr, u16 *colors, s32 skip) {
    Unk800F78A4 *env = fx_098_env;
    RECT rect;
    VECTOR sv;
    SVECTOR zero;
    SVECTOR quad[4];
    CVECTOR colTR;
    CVECTOR colBR;
    CVECTOR colTL;
    CVECTOR colBL;
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
            *(s32 *)&colTL = 0;
            *(s32 *)&colBL = 0;
            for (i = 0; i < w; i++, prim++, px += size, src++) {
                quad[0].vx = quad[2].vx = px;
                quad[0].vy = quad[1].vy = py;
                quad[1].vx = quad[3].vx = px + size;
                quad[2].vy = quad[3].vy = py + size;
                if (i >= w - 1) {
                    *(s32 *)&colTR = 0;
                    *(s32 *)&colBR = 0;
                } else {
                    if (j == 0) {
                        *(s32 *)&colTR = 0;
                    } else {
                        texel = src[0];
                        colTR.r = ((texel & 0x1F) * (bright << 2)) >> 12;
                        colTR.g = (((texel >> 5) & 0x1F) * (bright << 2)) >> 12;
                        colTR.b = (((texel >> 10) & 0x1F) * (bright << 2)) >> 12;
                    }
                    if (j >= h - 1) {
                        *(s32 *)&colBR = 0;
                    } else {
                        texel = src[w - 1];
                        colBR.r = ((texel & 0x1F) * (bright << 2)) >> 12;
                        colBR.g = (((texel >> 5) & 0x1F) * (bright << 2)) >> 12;
                        colBR.b = (((texel >> 10) & 0x1F) * (bright << 2)) >> 12;
                    }
                }
                gte_ldv3xy(&quad[0], &quad[1], &quad[2]);
                colTL.cd = code;
                *(s32 *)&prim->r0 = *(s32 *)&colTL;
                gte_rtpt_b();
                *(s32 *)&prim->r1 = *(s32 *)&colTR;
                *(s32 *)&prim->r2 = *(s32 *)&colBL;
                gte_stsxy3(&prim->x0, &prim->x1, &prim->x2);
                gte_stopz(&nclip);
                gte_ldv0xy(&quad[3]);
                *(s32 *)&prim->r3 = *(s32 *)&colBR;
                gte_rtps_b();
                *(s32 *)&colBL = *(s32 *)&colBR;
                *(s32 *)&colTL = *(s32 *)&colTR;
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
    left = (tp & TPAGE_X_MASK) * 64 + x + (idx % div) * w;
    py = (tp & TPAGE_Y_BIT) * 16 + y + (idx / div) * h;
    rect.x = left;
    rect.y = py;
    rect.w = w;
    rect.h = h;
    ex->StoreImage(&rect, (u32 *)colors);
}

static void fxm_drawBulgeGrid14(SVECTOR *pos, SVECTOR *rot, s32 scale, s32 bright, s32 tpage, s32 clut, s32 u, s32 v, s32 amp, s32 abr) {
    Unk800F78A4 *env = fx_098_env;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    MATRIX m;
    VECTOR sc;
    SVECTOR vtx[4];
    SVECTOR center;
    s32 otz;
    s32 row;
    s32 code;
    s32 rowX;
    s32 right;
    POLY_GT4 *prim;
    s32 col;
    s32 dist0;
    s32 dist1;
    s32 dist2;
    s32 dist3;
    s32 shade;
    s32 u0;
    s32 u1;
    s32 v0;
    s32 v1;

    *(s32 *)&center.vx = 0;
    center.vz = 0;
    gte_SetRotMatrix(env->viewMatrix);
    gte_SetTransMatrix(env->viewMatrix);
    gte_ldv0(pos);
    prim = (POLY_GT4 *)env->prim;
    code = 0x3C;
    env->prim = (u8 *)(prim + 14 * 14);
    if (abr != 0xFF) {
        code = 0x3E;
    }
    tpage |= TPAGE_ABR(abr);
    gte_rtv0tr_b();
    ex->math_rotMatrixYXZ(rot, &m);
    sc.vx = sc.vy = sc.vz = scale;
    ex->ScaleMatrix(&m, &sc);
    gte_stlvnl(m.t);
    ex->MulRotMatrix(&m);
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    for (row = 0; row < 14; row++) {
        right = (row << 6) - 0x180;
        rowX = row << 6;
        for (col = 0; col < 14; col++) {
            vtx[0].vz = vtx[1].vz = vtx[2].vz = vtx[3].vz = 0;
            vtx[0].vx = vtx[2].vx = rowX - 0x1C0;
            vtx[1].vx = vtx[3].vx = right;
            vtx[0].vy = vtx[1].vy = (col << 6) - 0x1C0;
            vtx[2].vy = vtx[3].vy = (col << 6) - 0x180;
            dist0 = ex->btlfx_distance(&vtx[0], &center);
            dist1 = ex->btlfx_distance(&vtx[1], &center);
            dist2 = ex->btlfx_distance(&vtx[2], &center);
            dist3 = ex->btlfx_distance(&vtx[3], &center);
            if (dist0 > 0x100) {
                vtx[0].vz = 0;
            } else {
                vtx[0].vz = -(((ex->math_cos(dist0 * 8) + 0x1000) * amp) >> 13);
            }
            if (dist1 > 0x100) {
                vtx[1].vz = 0;
            } else {
                vtx[1].vz = -(((ex->math_cos(dist1 * 8) + 0x1000) * amp) >> 13);
            }
            if (dist2 > 0x100) {
                vtx[2].vz = 0;
            } else {
                vtx[2].vz = -(((ex->math_cos(dist2 * 8) + 0x1000) * amp) >> 13);
            }
            if (dist3 > 0x100) {
                vtx[3].vz = 0;
            } else {
                vtx[3].vz = -(((ex->math_cos(dist3 * 8) + 0x1000) * amp) >> 13);
            }
            gte_ldv3c(vtx);
            gte_rtpt();
            shade = bright - (-vtx[0].vz >> 3);
            if (shade >= 0x100) {
                shade = 0xFF;
            }
            if (dist0 > 0x1A0) {
                shade = 0;
            }
            prim->code = code;
            prim->r0 = prim->g0 = prim->b0 = shade;
            shade = bright - (-vtx[1].vz >> 3);
            if (shade >= 0x100) {
                shade = 0xFF;
            }
            if (dist1 > 0x1A0) {
                shade = 0;
            }
            prim->r1 = prim->g1 = prim->b1 = shade;
            shade = bright - (-vtx[2].vz >> 3);
            if (shade >= 0x100) {
                shade = 0xFF;
            }
            if (dist2 > 0x1A0) {
                shade = 0;
            }
            prim->r2 = prim->g2 = prim->b2 = shade;
            shade = bright - (-vtx[3].vz >> 3);
            if (shade >= 0x100) {
                shade = 0xFF;
            }
            if (dist3 > 0x1A0) {
                shade = 0;
            }
            prim->r3 = prim->g3 = prim->b3 = shade;
            gte_nclip();
            prim->tpage = tpage;
            prim->clut = clut;
            gte_stopz(&otz);
            if (otz != 0) {
                gte_stsxy3(&prim->x0, &prim->x1, &prim->x2);
                gte_avsz3();
                gte_stszotz(&otz);
                gte_ldv0(&vtx[3]);
                gte_rtps();
                u0 = u + ((row & 3) << 5);
                u1 = u0 + 0x20;
                v0 = v + ((col & 3) << 5);
                if ((row & 3) == 3) {
                    u1 = u0 + 0x1F;
                }
                v1 = v0 + 0x20;
                if ((col & 3) == 3) {
                    v1 = v0 + 0x1F;
                }
                prim->u0 = prim->u2 = u0;
                prim->u1 = prim->u3 = u1;
                prim->v0 = prim->v1 = v0;
                prim->v2 = prim->v3 = v1;
                ex->btlfx_addPrim(0xC000000, otz, prim);
                gte_stsxy(&prim->x3);
                prim++;
            }
        }
    }
}

void fx_098_piecePos(s16 *a, s16 *b, s32 i, s16 *out) {
    BTLFX_EXPORTS(fx_098_env)->btlfx_pointAhead(a, b, -(fx_098_pieceDist[i] * 3), out);
}

static s32 fx_098_op80PoolTask(s32 mode, Fx098Item *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, s32 a5,
                  Fx098Work *work) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR *baseRot;
    Pool6CEntry *emitter;
    Fx098Item *piece;
    s32 t;
    s32 bright;
    s32 k;
    s32 pieceIdx;
    s32 data;

    if (mode == 1) {
        it->state0 = 0;
        it->frame8 = 0;
    } else if (it->flying2 == 0) {
        SVECTOR rot;
        VECTOR scale;
        SVECTOR center;
        SVECTOR ringPos;
        SVECTOR piecePos;
        SVECTOR origin;
        SVECTOR dir;

        baseRot = &fx_098_shardRot[it->index4];
        switch (it->state0) {
        case 0:
            if (it->frame8 == 0) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_BLIZZAGA_SHARD_MIST), (s32)&it->pos18);
                it->emit34 = emitter;
                if (emitter != NULL) {
                    emitter->otZ = 0x80;
                }
            }
            scale.vx = 0;
            BTLFX_COPY_SVEC(&rot, baseRot);
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
                s32 ang = it->frame8 << 10;

                scale.vx = scale.vy = (ang * 0xC00) >> 12;
                scale.vz = (ex->math_sin(ang >> 2) * 0xC00) >> 12;
            }
            BTLFX_COPY_SVEC(&rot, baseRot);
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
            scale.vx = scale.vy = scale.vz = 0xC00;
            BTLFX_COPY_SVEC(&rot, baseRot);
            rot.vx -= 0x800;
            if (it->emit34 != NULL) {
                it->emit34->rot.vx = rot.vx;
                it->emit34->rot.vy = rot.vy;
                it->emit34->rot.vz = rot.vz;
            }
            if (frame < 12) {
                rot.pad = 1;
                center.vx = it->posC.vx;
                center.vy = it->posC.vy;
                center.vz = it->posC.vz;
                {
                    s32 ang = (frame << 10) / 12;
                    s32 ringScale;

                    ex->btlfx_pointAhead((s16 *)&center, (s16 *)&rot, 0x41A, (s16 *)&ringPos);
                    data = ex->btlfx_getFileData(entry->file, FX_BLIZZAGA_SNOW_RING_2);
                    ringScale = (ex->math_sin(ang) * 0x1800) >> 12;
                    ex->btlfx_drawRings((u8 *)data, &ringPos, &rot, ringScale, 0, 1, ex->math_cos(ang) * 2 / 3);
                }
            }
            pieceIdx = 6;
            if (work->shatter78 != 0) {
                it->frame8 = 0;
                if (it->index4 < 3) {
                    pieceIdx = 10;
                }
                BTLFX_COPY_SVEC(&origin, &it->pos18);
                for (k = 0; k < 4; k++, pieceIdx++) {
                    piece = it;
                    if (k != 0) {
                        piece = ex->btlfx_poolSpawn((s16 *)entry->pool);
                    }
                    if (piece != NULL) {
                        piece->flying2 = 1;
                        piece->state0 = pieceIdx;
                        piece->index4 = k;
                        piece->frame8 = 0;
                        piece->model2C = work->models20[pieceIdx];
                        fx_098_piecePos((s16 *)&origin, (s16 *)baseRot, pieceIdx, (s16 *)&piecePos);
                        BTLFX_COPY_SVEC(&piece->rot20, baseRot);
                        piece->posC.vx = piecePos.vx << 4;
                        piece->posC.vy = piecePos.vy << 4;
                        piece->posC.vz = piecePos.vz << 4;
                        piece->spinX28 = ex->btlfx_randCentered(0x84);
                        piece->spinY2A = ex->btlfx_randCentered(0x84);
                        data = 0x80; /* through a variable, so the 0x80 isn't folded into k * 0x29 */
                        t = -(((ex->btlfx_rand() & 0xF) + data - k * 0x29) * 16);
                        BTLFX_COPY_SVEC(&dir, baseRot);
                        dir.vx += 0x80;
                        ex->btlfx_rotateOffset(&dir, t, (s32)&piece->pos18);
                        piece->pos18.vx += ex->btlfx_randCentered(0x200);
                        piece->pos18.vy += ex->btlfx_randCentered(0x200);
                        piece->pos18.vz += ex->btlfx_randCentered(0x200);
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
            ex->Hi_DrawEffModel(baseRot, (VECTOR *)&it->posC, (s32)&scale, it->model2C);
            ex->Hi_DrawEffModel(baseRot, (VECTOR *)&it->posC, (s32)&scale, it->model30);
            return 0;
        }
    } else {
        VECTOR pos;
        VECTOR scale;

        t = ((it->frame8 << 12) / 22) >> 2;
        bright = ex->math_cos(t);
        it->posC.vx += it->pos18.vx;
        it->posC.vy += it->pos18.vy;
        it->posC.vz += it->pos18.vz;
        it->pos18.vy += 0x40;
        it->rot20.vx += it->spinX28;
        it->rot20.vy += it->spinY2A;
        bright = (bright * 3) >> 1;
        if (it->state0 < 6) {
            scale.vx = scale.vy = (ex->math_cos(t) * 0x2400) >> 12;
            scale.vz = (ex->math_cos(t) * 0x1800) >> 12;
        } else {
            scale.vx = scale.vy = (ex->math_cos(t) * 0x1800) >> 12;
            scale.vz = (ex->math_cos(t) * 0x1E00) >> 12;
        }
        pos.vx = it->posC.vx >> 4;
        pos.vy = it->posC.vy >> 4;
        pos.vz = it->posC.vz >> 4;
        ex->Hi_DrawEffModel(&it->rot20, &pos, (s32)&scale, it->model2C);
        ex->Hi_ModifyEffModelAbr(it->model2C, 1);
        ex->Hi_ModifyEffModelRGB(it->model2C, (bright >> 5) - 0x80, (bright >> 5) - 0x80, (bright >> 5) - 0x80);
        if (++it->frame8 >= 22) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_098/nonmatchings/fx_098", fx_098_headerPtr);

s32 fx_098_op80(s32 mode, Fx098Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR sc;
    SVECTOR rot;
    SVECTOR vec;
    s32 alpha;
    s32 frame;
    s32 i;
    s32 j;
    s32 tpage;
    s32 t;
    s32 ang;
    s32 tint;
    s32 rnd;
    s32 acc;
    s32 radius;
    u8 *data;
    SVECTOR *shardRot;
    Fx098Item *piece;
    Fx098Item *shard;
    Pool6CEntry *emitter;
    Pool6CEntry *flashEmitter;

    if (mode == 0) {
        init->workSize = 0x88;
        init->elemSize = 0x38;
        init->elemFn = (s32)fx_098_op80PoolTask;
        init->poolCount = 0x20;
    } else if (mode == 1) {
        fx_098_env = env;
        ex->btlfx_initEffModels(fx_098_effModels, 0x19);
        tpage = 0x9A;
        work->model8 = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_BLIZZAGA_ICE_CRYSTAL), tpage, 0x3DC0, 0x80, 0);
        work->modelC = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_BLIZZAGA_ICE_CRYSTAL_2), tpage, 0x3DC0, 0x80, 0);
        work->model10 = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_BLIZZAGA_ICE_CRYSTAL_3), tpage, 0x3DC0, 0x80, 0);
        work->model14 = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_BLIZZAGA_CRYSTAL_SHEEN), 0xBA, 0x3D40, 0, 0x80);
        work->model18 = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_BLIZZAGA_CRYSTAL_SHEEN_2), 0xBA, 0x3D40, 0, 0x80);
        work->model1C = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_BLIZZAGA_CRYSTAL_SHEEN_3), 0xBA, 0x3D40, 0, 0x80);
        for (i = 0; i < 14; i++) {
            work->model20[i] = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, fx_098_pieceBlocks[i]), tpage, 0x3DC0, 0x80, 0);
        }
        work->state0 = 0;
        work->frame4 = 0;
        work->shardCount7A = 0;
        work->shatter78 = 0;
        work->scrollStep7C = 0;
        work->scroll80 = 0;
        work->pos58.vx = env->targets->centerX;
        work->pos58.vy = env->targets->centerY;
        work->pos58.vz = env->targets->centerZ;
        work->pos60.vx = work->pos58.vx;
        work->pos60.vy = work->pos58.vy;
        work->pos60.vz = work->pos58.vz;
        work->pos60.vy -= 0x400;
        BTLFX_COPY_SVEC(&work->unk70, &work->pos58);
        work->unk70.vy -= 0x80;
    } else {
        frame = entry->frame;
        pos.vx = work->pos58.vx;
        pos.vy = work->pos58.vy;
        pos.vz = work->pos58.vz;
        sc.vx = sc.vy = 0x2800;
        sc.vz = 0x1A00;
        alpha = 0;
        switch (work->state0) {
        case 0:
            rot.vx = 0x400;
            rot.vz = 0x200;
            rot.vy = 0;
            t = (frame << 12) / 6;
            fxm_drawBulgeGrid14(&work->pos58, &rot, 0x3400, t >> 5, 0x9A, 0x3D40, 0, 0x80, 0, 1);
            ex->btlfx_drawScrollMove(work->frame4 * 3, 0x280, 0x180, 0x40, 0x80, 0x280, 0x100);
            if (frame >= 6) {
                work->state0 = 1;
                entry->frame = -1;
            }
            break;
        case 1:
            t = (frame << 12) / 10;
            ang = t >> 2;
            rot.vx = 0x400;
            rot.vy = 0;
            rot.vz = 0x200;
            fxm_drawBulgeGrid14(&work->pos58, &rot, 0x3400, 0x80, 0x9A, 0x3D40, 0, 0x80, -((ex->math_sin(ang) * 700) >> 12), 1);
            ex->btlfx_drawScrollMove(work->frame4 * 3, 0x280, 0x180, 0x40, 0x80, 0x280, 0x100);
            alpha = t;
            rot.pad = 1;
            data = (u8 *)ex->btlfx_getFileData(entry->file, FX_BLIZZAGA_SNOW_RING_2);
            ex->btlfx_drawRings(data, &work->pos58, &rot, ((((ex->math_sin(ang) * 0x2400) >> 12) + 0x1800) * 3) >> 2, 0, 1,
                              ex->math_cos(ang));
            if (frame >= 10) {
                work->state0 = 2;
                entry->frame = -1;
                BTLFX_COPY_SVEC(&work->pos68, &work->pos58);
                work->pos68.vy -= 0x800;
                flashEmitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_BLIZZAGA_MIST), (s32)&work->pos58);
                if (flashEmitter != NULL) {
                    flashEmitter->otZ = 0x80;
                }
                flashEmitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_BLIZZAGA_MIST_HIGH), (s32)&work->pos68);
                if (flashEmitter != NULL) {
                    flashEmitter->otZ = 0x80;
                }
                flashEmitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_BLIZZAGA_ICE_SPARKLES_2), (s32)&work->pos58);
                if (flashEmitter != NULL) {
                    flashEmitter->otZ = 0x80;
                }
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, 0xFF, 0xFF, 0);
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0, 0, 0x80);
            }
            break;
        case 2:
            alpha = 0x1000;
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
            alpha = 0x1000;
            if (frame & 1) {
                shard = ex->btlfx_poolSpawn((s16 *)entry->pool);
                if (shard != NULL) {
                    shard->flying2 = 0;
                    shard->index4 = work->shardCount7A;
                    shardRot = &fx_098_shardRot[work->shardCount7A];
                    if (work->shardCount7A >= 3) {
                        shard->model2C = work->modelC;
                        shard->model30 = work->model18;
                    } else {
                        shard->model2C = work->model10;
                        shard->model30 = work->model1C;
                    }
                    shard->posC.vx = work->pos58.vx;
                    shard->posC.vy = work->pos58.vy;
                    shard->posC.vz = work->pos58.vz;
                    radius = 0x165;
                    shard->posC.vx += (ex->math_sin(shardRot->vy + 0x800) * radius) >> 12;
                    shard->posC.vz += (ex->math_cos(shardRot->vy + 0x800) * radius) >> 12;
                    shard->pos18.vx = shard->posC.vx;
                    shard->pos18.vy = shard->posC.vy;
                    shard->pos18.vz = shard->posC.vz;
                    work->shardCount7A++;
                }
            }
            rot.vx = -0x400;
            rot.vy = 0;
            rot.vz = -0x400;
            ex->Hi_SetEffModelOffset(work->model8, 2);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->model8);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->model14);
            if (work->shardCount7A >= 6) {
                work->state0 = 4;
                entry->frame = -1;
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_BLIZZAGA_FAN_EMITTER), (s32)&work->pos58);
                work->emit84 = emitter;
                if (emitter != NULL) {
                    emitter->otZ = 0x5C;
                }
            }
            break;
        case 4:
            alpha = 0x1000;
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
            alpha = 0x1000;
            rot.vx = -0x400;
            rot.vy = 0;
            rot.vz = -0x400;
            ex->Hi_SetEffModelOffset(work->model8, 2);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->model8);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->model14);
            rot.pad = 1;
            if (frame >= 10) {
                work->state0 = 6;
                work->shatter78 = 1;
                entry->frame = -1;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, 0xFF, 0xFF, 0);
                ex->btlfx_stopEmitter(work->emit84);
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_BLIZZAGA_ICE_FLARES), (s32)&work->pos58);
                if (emitter != NULL) {
                    emitter->otZ = 0x60;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_BLIZZAGA_ICE_FLARES_2), (s32)&work->pos58);
                if (emitter != NULL) {
                    emitter->otZ = 0x60;
                }
                ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_BLIZZAGA_SNOW_BURST), (s32)&work->pos60);
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_BLIZZAGA_FREEZE_BURST), (s32)&work->pos60);
                if (emitter != NULL) {
                    emitter->otZ = 0x80;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_BLIZZAGA_FROST_RING_2), (s32)&work->pos60);
                if (emitter != NULL) {
                    emitter->otZ = 0x80;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_BLIZZAGA_ICE_SPARKLES), (s32)&work->pos58);
                if (emitter != NULL) {
                    emitter->otZ = 0x80;
                }
                rot.vx = -0x400;
                rot.vy = 0;
                rot.vz = -0x400;
                for (j = 0; j < 6; j++) {
                    piece = ex->btlfx_poolSpawn((s16 *)entry->pool);
                    if (piece != NULL) {
                        piece->flying2 = 1;
                        piece->state0 = j;
                        piece->index4 = j;
                        piece->frame8 = 0;
                        piece->model2C = work->model20[j];
                        fx_098_piecePos((s16 *)&work->pos58, (s16 *)&rot, j, (s16 *)&vec);
                        ((s32 *)&piece->rot20)[0] = ((s32 *)&rot)[0];
                        ((s32 *)&piece->rot20)[1] = ((s32 *)&rot)[1];
                        piece->posC.vx = vec.vx << 4;
                        piece->posC.vy = vec.vy << 4;
                        piece->posC.vz = vec.vz << 4;
                        piece->spinX28 = ex->btlfx_randCentered(0x5C);
                        piece->spinY2A = ex->btlfx_randCentered(0x80);
                        rnd = (ex->btlfx_rand() & 0xF) + 0x5C;
                        ex->btlfx_rotateOffset(&rot, -((rnd - j * 8) << 4), (s32)&piece->pos18.vx);
                        piece->pos18.vx += ex->btlfx_randCentered(0x3E0);
                        piece->pos18.vz += ex->btlfx_randCentered(0x3E0);
                    }
                }
            }
            break;
        case 6:
            rot.vx = -0x400;
            rot.vz = -0x400;
            rot.vy = 0;
            if (frame < 18) {
                alpha = ex->math_cos((frame << 10) / 18);
            }
            if (frame == 5) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_BLIZZAGA_SHATTER_WAVE), (s32)&work->pos60);
                if (emitter != NULL) {
                    emitter->otZ = 0x80;
                }
            }
            if (frame < 8) {
                tint = 0xFF - ((frame * 0xFF) >> 3);
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, tint >> 2, (tint * 2) / 3, tint);
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
            if (++work->scrollStep7C < 11) {
                acc = work->scroll80 + 11;
                work->scroll80 = acc - (work->scrollStep7C * 10) / 11;
            } else {
                work->scroll80++;
            }
            ex->btlfx_drawScrollMove(work->scroll80, 0x280, 0x180, 0x40, 0x80, 0x280, 0x100);
        }
        if (alpha != 0) {
            vec.vx = 0x400;
            vec.vy = 0;
            vec.vz = work->frame4 * 41;
            vec.pad = 1;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_BLIZZAGA_SNOW_RING), &work->pos58, &vec, (work->frame4 << 6) + 0x1E00,
                              0, 1, alpha);
            vec.vz = work->frame4 * 24 + 0x400;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_BLIZZAGA_SNOW_RING), &work->pos58, &vec, (work->frame4 << 6) + 0x1E00,
                              0, 1, alpha);
            env->otZ = 0x127;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_BLIZZAGA_FROST_RING), &work->pos58, NULL, (work->frame4 << 5) + 0x1E00,
                              0, 1, alpha);
            env->otZ = 0;
        }
        work->frame4++;
    }
    return 0;
}
