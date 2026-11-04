#include "fxmod/fx_17d_0.h"

void fx_17d_0_drawCenterBandY(u32 *ot, s32 abr, s32 h, s32 r, s32 g, s32 b) {
    POLY_G4 *poly;
    BtlfxExports *ex;
    s32 i;
    s32 x;

    h >>= 1;
    poly = (POLY_G4 *)fx_17d_0_env->prim;
    ex = BTLFX_EXPORTS(fx_17d_0_env);
    fx_17d_0_env->prim = (u8 *)(poly + 4);
    for (i = 0; i < 4; i++, poly++) {
        setlen(poly, 8);
        setcode(poly, GPU_POLY_G4 | GPU_ABE);
        if (!(i & 1)) {
            poly->r0 = poly->r1 = 0;
            poly->g0 = poly->g1 = 0;
            poly->b0 = poly->b1 = b;
            poly->r2 = poly->r3 = r;
            poly->g2 = poly->g3 = g;
            poly->b2 = poly->b3 = b;
            poly->y0 = poly->y1 = 0x70 - h;
            poly->y2 = poly->y3 = 0x70;
        } else {
            poly->r2 = poly->r3 = 0;
            poly->g2 = poly->g3 = 0;
            poly->b2 = poly->b3 = b;
            poly->r0 = poly->r1 = r;
            poly->g0 = poly->g1 = g;
            poly->b0 = poly->b1 = b;
            poly->y0 = poly->y1 = 0x70;
            poly->y2 = poly->y3 = 0x70 + h;
        }
        x = (i >> 1) * 160;
        poly->x0 = poly->x2 = x;
        poly->x1 = poly->x3 = x + 160;
        ex->btlfx_linkPrimAbr(0x08000000, ot, poly, abr);
    }
}

static void fxm_randDiscPoints(s32 n, s32 radius, SVECTOR *pts) {
    SVECTOR *cur;
    u32 *last = (u32 *)(pts + n);
    BtlfxExports *ex = BTLFX_EXPORTS(fx_17d_0_env);
    s32 i;
    s32 rad;
    s32 ang;

    for (i = 0, cur = pts; i < n; i++, last++, cur++, pts++) {
        *last = 0x7F7F7F7F;
        rad = ex->btlfx_randRange(radius >> 2, radius >> 1);
        ang = ex->btlfx_rand() & 0xFFF;
        pts->vx = (ex->math_cos(ang) * rad) >> 12;
        cur->vy = (ex->math_sin(ang) * rad) >> 12;
        cur->vz = ex->btlfx_rand() % 640 - 0x80;
    }
}

static void fxm_drawStreakField(u32 *ot, s32 n, s32 projH, s32 dz, SVECTOR *rot, SVECTOR *pts, CVECTOR *rgb, s32 fade, s32 abr) {
    Unk800F78A4 *env = fx_17d_0_env;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    LINE_G2 *prim;
    CVECTOR col;
    MATRIX m;
    s32 oldH;
    u32 otz;
    u32 *last;
    SVECTOR *curCopy;
    SVECTOR *cur;
    LINE_G2 *line;
    s32 live;
    s32 i;

    if (fade < 0) {
        fade = 0x1000;
    }
    prim = (LINE_G2 *)env->prim;
    env->prim = (u8 *)(prim + n);
    if (rgb == NULL) {
        col.b = 0;
        col.g = 0;
        col.r = 0;
    } else {
        gte_DpqColorFar_nst(rgb, fade);
        gte_strgb(&col);
    }
    gte_SetRotMatrix(env->viewMatrix);
    gte_SetTransMatrix(env->viewMatrix);
    gte_ReadGeomScreen(&oldH);
    gte_SetGeomScreen(projH);
    ex->math_rotMatrixYXZ(rot, &m);
    m.t[0] = m.t[1] = 0;
    m.t[2] = projH;
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    cur = pts;
    last = (u32 *)(cur + n);
    for (i = 0, curCopy = cur, line = prim; i < n; i++, last++, curCopy++, cur++, line++, prim++) {
        live = *last != 0x7F7F7F7F;
        gte_ldv0(cur);
        gte_rtps();
        curCopy->vz -= dz;
        if (dz > 0 && curCopy->vz < -0x80) {
            curCopy->vz = 0x200;
            *last = 0x7F7F7F7F;
        } else if (dz < 0 && curCopy->vz > 0x180) {
            curCopy->vz = -0x100;
            *last = 0x7F7F7F7F;
        } else {
            gte_stsxy(&line->x0);
            gte_stszotz(&otz);
            if (otz < 0x80 && live) {
                gte_DpqColorFar_nst(&col, ((0x80 - otz) << 12) >> 7);
                gte_strgb(&prim->r0);
                *(s32 *)&line->x1 = *last;
                *(s32 *)&line->r1 = 0;
                line->code = abr == 0xFF ? 0x50 : 0x52;
                ex->btlfx_linkPrimAbr(0x04000000, ot, prim, abr);
            }
            *last = *(s32 *)&line->x0;
        }
    }
    gte_SetGeomScreen(oldH);
}

void fx_17d_0_drawPieces(Unk80100C78Entry *entry, SVECTOR *origin, s32 frame, s32 models, s16 *morphs, s32 scale) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_17d_0_env);
    s16 *morph = morphs; /* a copy: the original loads scale before morphs */
    VECTOR pos;
    VECTOR sc;
    SVECTOR rot;
    s16 *track;
    s32 key;
    s32 i;
    s32 fileIdx;
    s32 modelOfs;

    for (i = 0; i < 21; i++) {
        fileIdx = i * 4 + 2;
        track = (s16 *)ex->btlfx_getFileData(entry->file, ((s16 *)fx_17d_0_pieceFiles)[fileIdx]);
        key = frame;
        if (key >= track[1]) {
            key = track[1] - 1;
        }
        if (i < 16 || i >= 19 || frame < 0x28) {
            if (i != 16) {
                track = track + 2 + key * 6;
                sc.vx = sc.vy = sc.vz = scale;
            } else {
                track = track + 2 + key * 9;
                sc.vx = (track[6] * scale) >> 12;
                sc.vy = (track[7] * scale) >> 12;
                sc.vz = (track[8] * scale) >> 12;
            }
            pos.vx = ((track[0] * scale) >> 12) + origin->vx;
            pos.vy = ((track[1] * scale) >> 12) + origin->vy;
            pos.vz = ((track[2] * scale) >> 12) + origin->vz;
            rot.vx = track[3];
            rot.vy = track[4];
            rot.vz = -track[5];
            modelOfs = i << 1;
            if (*morph < 0) {
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, *(s16 *)(modelOfs + models));
            } else {
                fileIdx = i * 4 + 3;
                track = (s16 *)ex->btlfx_getFileData(entry->file, ((s16 *)fx_17d_0_pieceFiles)[fileIdx]);
                key = frame;
                if (key >= track[1]) {
                    key = track[1] - 1;
                }
                ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&sc, *(s16 *)(modelOfs + models), *morph, *(track = track + 2 + key * 3));
            }
        }
        morph++;
    }
}

static s32 fx_17d_0_op80PoolTask(s32 mode, Fx17d0Part *p, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 1) {
        p->unk0 = 0;
    } else {
        p->pos8.vz += p->speed2 + 0x186;
        env->otZ = -0x80;
        if (frame == 0) {
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_ARK_GREY_TRAIL), (s32)&p->pos8, 0, -1, 0x800, 0x1000, 1,
                              fx_17d_0_partTrails[p->index4], 0);
        }
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_ARK_GREY_TRAIL), (s32)&p->pos8, 0, 0, 0x800, 0x1000, 1,
                          fx_17d_0_partTrails[p->index4], 0);
        env->otZ = 0;
        if (frame >= 0x10) {
            return 1;
        }
    }
    return 0;
}

s32 fx_17d_0_op80(s32 mode, Fx17d0Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 gridScale;
    s32 code;
    s32 abr;
    s32 alpha;
    s32 frame;
    s32 gridBright;
    s32 t;
    s32 i;
    s32 piece;
    s32 spawn;
    s32 byteOfs;

    if (mode == 0) {
        init->workSize = 0xBC;
        init->elemSize = 0x10;
        init->elemFn = (s32)fx_17d_0_op80PoolTask;
        init->poolCount = 0x20;
    } else if (mode == 1) {
        work->state0 = 0xA;
        work->frame4 = 0;
        fx_17d_0_env = env;
        ex->btlfx_initEffModels(fx_17d_0_effModels, 0x20);
        work->modelBA = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_ARK_STAR_SPHERE), 0);
        BTLFX_COPY_SVEC(&work->pos8, &fx_17d_0_basePos);
        work->pos18.vy = -0x1000;
        work->pos18.vx = 0;
        work->pos18.vz = 0;
        work->unk30.vx = 0;
        work->unk30.vy = 0;
        work->unk30.vz = 0;
        fx_17d_0_modelScale.vx = 0x1000;
        fx_17d_0_modelScale.vy = 0x1000;
        fx_17d_0_modelScale.vz = 0x1000;
        BTLFX_COPY_SVEC(&work->pos48, &work->pos8);
        work->pos48.vy -= 0x180;
        env->camera->points[3].vx = work->pos48.vx;
        env->camera->points[3].vy = work->pos48.vy;
        env->camera->points[3].vz = work->pos48.vz;
        fxm_randDiscPoints(0x140, 0x200, fx_17d_0_streakPoints);
        work->count58 = 0;
    } else {
        gridScale = 0;
        code = 0;
        abr = 0;
        alpha = 0;
        frame = entry->frame;
        gridBright = 0;
        switch (work->state0) {
            SVECTOR rot;

        case 0xA: {
            CVECTOR col;
            s32 swayAng;

            t = frame << 7;
            col.r = 0x80;
            col.g = 0xC8;
            col.b = 0xFF;
            swayAng = t >> 1;
            rot.vx = ((0x1000 - ex->math_cos(swayAng)) >> 1) / 3;
            rot.vy = ex->math_sin(t >> 2) >> 2;
            rot.vz = work->frame4 * 4;
            work->rot38.vx = rot.vx >> 1;
            work->rot38.vy = rot.vy >> 1;
            work->rot38.vz = rot.vz >> 1;
            fxm_drawStreakField((u32 *)env->ot + 8, 0x140, 0x80, ((ex->math_sin(swayAng) * 0x18) >> 12) - 1, &rot, fx_17d_0_streakPoints,
                          &col, t, 1);
            if (frame >= 0x19) {
                code = 1;
                abr = 1;
                gridBright = (frame - 0x18) * 12;
                gridScale = 0;
            }
            if (frame >= 0x20) {
                work->state0 = 0;
                entry->frame = -1;
            }
            break;
        }
        case 0: {
            CVECTOR col;
            s32 phase;
            s32 swayAng;
            s32 spriteX;
            s32 spriteY;

            t = (frame << 12) / 80;
            phase = t >> 2;
            alpha = ex->math_sin(phase);
            col.r = 0x80;
            col.g = 0xC8;
            col.b = 0xFF;
            swayAng = t >> 1;
            rot.vx = (0x1000 - ((0x1000 - ex->math_cos(swayAng)) >> 1)) / 3;
            gridScale = t >> 7;
            rot.vy = (0x1000 - ex->math_sin(phase)) >> 2;
            rot.vz = work->frame4 * 4;
            work->rot38.vx = rot.vx >> 1;
            work->rot38.vy = rot.vy >> 1;
            work->rot38.vz = rot.vz >> 1;
            fxm_drawStreakField((u32 *)env->ot + 8, 0x140, 0x80, ~((t * 20) >> 12), &rot, fx_17d_0_streakPoints, &col, 0x1000, 1);
            code = 1;
            abr = 1;
            spriteX = ((0x1000 - ex->math_sin(phase)) >> 5) + 0xA0;
            spriteY = 0x6E - ((0x1000 - ((0x1000 - ex->math_cos(swayAng)) >> 1)) >> 3);
            ex->btlfx_drawSpriteAtOt(0xC00, (u8 *)ex->btlfx_getFileData(entry->file, FX_ARK_RED_ROCK_LAYER), spriteX, spriteY, 0, 0xFF, t);
            gridBright = 0x60 - gridScale;
            if (frame >= 0x50) {
                work->state0 = code;
                entry->frame = -1;
                for (piece = 0; piece < 21; piece++) {
                    s32 fileId;
                    s32 pieceModel;

                    work->model5C[piece] = ex->btlfx_registerEffectModel(
                        (Unk800E8F50 *)ex->btlfx_getFileData(entry->file, ((s16 *)fx_17d_0_pieceFiles)[piece * 4]), 0);
                    byteOfs = piece * 8 + 2; /* byte offset: a separate 8i + 2 giv added to the table base */
                    fileId = *(s16 *)((u8 *)fx_17d_0_pieceFiles + byteOfs);
                    pieceModel = -1;
                    if (fileId >= 0) {
                        pieceModel = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, fileId), 0);
                    }
                    work->model86[piece] = pieceModel;
                }
            }
            break;
        }
        case 1: {
            SVECTOR rot;
            CVECTOR col;
            s32 phase;
            s32 pieceScale;
            s32 streakSpeed;
            s32 spriteY;

            t = (frame << 12) / 88;
            t = 0x1000 - ex->math_cos(t >> 2);
            if (frame == 0) {
                Pool6CEntry *emitter;

                BTLFX_COPY_SVEC(&work->pos40, &work->pos8);
                work->pos40.vy -= 0x140;
                work->pos40.vz += 0x1000;
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ARK_VIOLET_RING), (s32)&work->pos40);
                if (emitter != NULL) {
                    emitter->otZ = -0x100;
                }
            }
            phase = t >> 2;
            alpha = 0x1000;
            work->rot38.vx -= 1;
            work->rot38.vy += 1;
            /* mode (dead here) holds the intermediate: as a global pseudo with an a0 preference it gets a0 */
            mode = 0x1000 - ex->math_cos((0x1000 - ex->math_cos(phase)) >> 2);
            pieceScale = mode >> 2;
            work->pos10.vx = 0;
            work->pos10.vy = 0x200 - ((ex->math_cos(pieceScale) * 0x300) >> 12);
            work->pos10.vz = ex->math_cos(pieceScale);
            col.r = 0x80;
            col.g = 0xC8;
            col.b = 0xFF;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = work->frame4 * 4;
            streakSpeed = (t >> 8) + 0x14;
            fxm_drawStreakField((u32 *)env->ot + 0x400, 0x140, 0x80, -streakSpeed, &rot, fx_17d_0_streakPoints, &col,
                          (ex->math_cos(phase) >> 1) + 0x800, 1);
            pieceScale = 0x1000 - ex->math_cos(phase);
            pieceScale = 0x1000 - ex->math_cos(pieceScale >> 2);
            if (t > 0xC00) {
                pieceScale = pieceScale - (ex->math_cos(t - 0xC00) - 0x1000);
            }
            fx_17d_0_drawPieces(entry, &work->pos10, frame % 10, (s32)work->model5C, work->model86, pieceScale);
            if (frame < 0x58) {
                spriteY = (frame >> 2) + 0x6E;
                ex->btlfx_drawSpriteAtOt(0xC00, (u8 *)ex->btlfx_getFileData(entry->file, FX_ARK_RED_ROCK_LAYER), 0xA0, spriteY, 0, 0xFF, alpha);
            }
            code = 1;
            abr = 1;
            gridScale = (t >> 4) + 0x20;
            gridBright = 0x40;
            if (frame >= 0x58) {
                u16 texInfo[2][2];
                u16 texPos[2][2];

                work->state0 = 2;
                entry->frame = -1;
                work->pos10.vy = 0;
                BTLFX_COPY_SVEC(&work->pos8, &work->pos18);
                for (i = 0; i < 2; i++) {
                    texInfo[i][0] = ((Fx17d0Summon *)env->loader)->summon54->texInfo18[fx_17d_0_texSlots[i]];
                    texInfo[i][1] = ((Fx17d0Summon *)env->loader)->summon54->texInfo24[fx_17d_0_texSlots[i]];
                    texPos[i][0] = 0;
                    texPos[i][1] = ((Fx17d0Summon *)env->loader)->summon54->texPos30[fx_17d_0_texSlots[i]];
                }
                for (i = 0; i < 5; i++) {
                    work->modelB0[i] = ((s32 (*)(void *, void *, void *))ex->Hi_RegisterTexListModel)(
                        (void *)ex->btlfx_getFileData(entry->file, fx_17d_0_texModelFileIds[i]), texInfo, texPos);
                }
            }
            break;
        }
        case 2: {
            VECTOR pos;
            SVECTOR rot;
            s16 *track;
            s32 key;

            alpha = 0x1000;
            work->rot38.vx += 0xC;
            work->rot38.vy += 6;
            fx_17d_0_drawPieces(entry, &work->pos10, frame, (s32)work->model5C, work->model86, 0x1000);
            if (frame >= 0x79) {
                s32 unused;
                s32 unused2;
                s32 unused3;

                track = (s16 *)ex->btlfx_getFileData(entry->file, FX_ARK_MORPH_PATH);
                key = frame;
                if (key >= track[1]) {
                    key = track[1] - 1;
                }
                track = track + 2 + key * 6;
                pos.vx = track[0];
                pos.vy = track[1];
                pos.vz = track[2];
                rot.vx = track[3];
                rot.vy = track[4];
                rot.vz = -track[5];
                track = (s16 *)ex->btlfx_getFileData(entry->file, FX_ARK_MORPH_KEYS);
                if (frame >= track[1]) {
                    track = (s16 *)ex->btlfx_getFileData(entry->file, FX_ARK_MORPH_KEYS_2);
                    key = frame - (track[1] - 1);
                    if (key >= track[1]) {
                        key = track[1] - 1;
                    }
                    key = 0x1000 - *(track += 2 + key * 3);
                    ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&fx_17d_0_modelScale, work->modelB0[1], work->modelB0[2], key);
                } else {
                    key = *(track += 2 + frame * 3);
                    ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&fx_17d_0_modelScale, work->modelB0[0], work->modelB0[1], key);
                }
            }
            if (frame >= 0x8E) {
                track = (s16 *)ex->btlfx_getFileData(entry->file, FX_ARK_MORPH_PATH_2);
                key = frame;
                if (key >= track[1]) {
                    key = track[1] - 1;
                }
                track = track + 2 + key * 6;
                pos.vx = track[0];
                pos.vy = track[1];
                pos.vz = track[2];
                rot.vx = track[3];
                rot.vy = track[4];
                rot.vz = -track[5];
                track = (s16 *)ex->btlfx_getFileData(entry->file, FX_ARK_MORPH_KEYS_3);
                key = frame;
                if (key >= track[1]) {
                    key = track[1] - 1;
                }
                key = *(track += 2 + key * 3);
                ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&fx_17d_0_modelScale, work->modelB0[3], work->modelB0[4], key);
            }
            if (frame < 0x9D) {
                for (spawn = 0; spawn < 2; spawn++) {
                    Fx17d0Part *part = ex->btlfx_poolSpawn((s16 *)entry->pool);

                    if (part != NULL) {
                        s32 ang;
                        s32 radius;

                        BTLFX_COPY_SVEC(&part->pos8, &work->pos18);
                        ang = ex->btlfx_rand();
                        radius = (ex->btlfx_rand() & 0x7F) + 0x400;
                        part->pos8.vz -= 0x5DC;
                        part->pos8.vx += (ex->math_cos(ang) * radius) >> 12;
                        part->pos8.vy = part->pos8.vy + (((ex->math_sin(ang) * radius) >> 12) - 0x19A);
                        part->speed2 = ex->btlfx_rand() & 0x7F;
                        part->index4 = work->count58 & 0x1F;
                        work->count58++;
                    }
                }
            }
            if (frame >= 0xA1) {
                fx_17d_0_drawCenterBandY((u32 *)env->ot + 1, 1,
                              ((0x1000 - ex->math_cos((((frame - 0xA1) << 12) / 7) >> 2)) * 0xE0) >> 12, 0xF0, 0xF0,
                              0xF0);
            }
            if (frame >= 0xA3) {
                s32 tint = ((frame - 0xA3) * 0xFF) / 6;

                ex->btlfx_drawScreenTint((u32 *)env->ot + 1, 1, tint, tint, tint);
            }
            if (frame >= 0xA9) {
                ex->btlfx_poolClear((Unk800DA1B8 *)entry->pool);
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        }
        if (gridBright != 0) {
            CVECTOR col;
            SVECTOR rot;
            VECTOR sc;

            if (abr >= 0) {
                col.r = col.g = col.b = gridBright;
            } else {
                abr = 0xFF;
                col.b = gridBright;
                col.g = 0;
                col.r = 0;
            }
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            sc.vy = sc.vx = gridScale + 0x1000;
            sc.vz = 0x1000;
            ex->btlfx_drawScreenGrid(code, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &rot, &sc, &col, abr, 0);
        }
        if (alpha != 0) {
            VECTOR pos;
            VECTOR sc;
            s32 modelScale;
            s32 bright;

            pos.vx = work->pos18.vx;
            pos.vy = work->pos18.vy;
            pos.vz = work->pos18.vz;
            modelScale = 0x4000;
            if (work->state0 == 2) {
                modelScale = 0xAAA;
            }
            sc.vx = sc.vy = sc.vz = modelScale;
            ex->Hi_DrawEffModel(&work->rot38, &pos, (s32)&sc, work->modelBA);
            bright = (alpha >> 5) - 0x80;
            ex->Hi_ModifyEffModelRGB(work->modelBA, bright, bright, bright);
        }
        work->frame4++;
    }
    return 0;
}

s32 fx_17d_0_op82(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR camPos;
    s16 *track;
    s32 key;
    s32 frame;

    if (mode == 0) {
        *args = 4;
    } else if (mode == 1) {
        fx_17d_0_env = env;
    } else {
        frame = entry->frame;
        track = (s16 *)ex->btlfx_getFileData(entry->file, FX_ARK_CAMERA_KEYS);
        key = frame;
        if (key >= track[1]) {
            key = track[1] - 1;
        }
        track += key * 3 + 2;
        camPos.vx = track[0];
        camPos.vy = track[1];
        camPos.vz = track[2];
        env->camera->points[0].vx = camPos.vx;
        env->camera->points[0].vy = camPos.vy;
        env->camera->points[0].vz = camPos.vz;
        track = (s16 *)ex->btlfx_getFileData(entry->file, FX_ARK_CAMERA_ROLL_KEYS);
        key = frame;
        if (key >= track[1]) {
            key = track[1] - 1;
        }
        track += key * 3 + 2;
        env->camera->pointRots[0].vz = track[2];
        track = (s16 *)ex->btlfx_getFileData(entry->file, FX_ARK_CAMERA_KEYS_2);
        key = frame;
        if (key >= track[1]) {
            key = track[1] - 1;
        }
        track += key * 3 + 2;
        camPos.vx = track[0];
        camPos.vy = track[1];
        camPos.vz = track[2];
        env->camera->points[1].vx = camPos.vx;
        env->camera->points[1].vy = camPos.vy;
        env->camera->points[1].vz = camPos.vz;
        if (frame >= 0xA9) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_17d_0/nonmatchings/fx_17d_0", fx_17d_0_headerPtr);
