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
        ex->btlfx_linkPrimAbr(PRIM_TAG(POLY_G4), ot, poly, abr);
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
        *last = FX_ARK_STREAK_NO_PREV;
        rad = ex->btlfx_randRange(radius >> 2, radius >> 1);
        ang = ex->btlfx_rand() & 0xFFF;
        pts->vx = (ex->math_cos(ang) * rad) >> 12;
        cur->vy = (ex->math_sin(ang) * rad) >> 12;
        cur->vz = ex->btlfx_rand() % (FX_ARK_STREAK_FAR_Z - FX_ARK_STREAK_NEAR_Z) + FX_ARK_STREAK_NEAR_Z;
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
        live = *last != FX_ARK_STREAK_NO_PREV;
        gte_ldv0(cur);
        gte_rtps();
        curCopy->vz -= dz;
        if (dz > 0 && curCopy->vz < FX_ARK_STREAK_NEAR_Z) {
            curCopy->vz = FX_ARK_STREAK_FAR_Z;
            *last = FX_ARK_STREAK_NO_PREV;
        } else if (dz < 0 && curCopy->vz > 0x180) {
            curCopy->vz = -0x100;
            *last = FX_ARK_STREAK_NO_PREV;
        } else {
            gte_stsxy(&line->x0);
            gte_stszotz(&otz);
            if (otz < FX_ARK_STREAK_FADE_Z && live) {
                gte_DpqColorFar_nst(&col, ((FX_ARK_STREAK_FADE_Z - otz) << 12) >> 7);
                gte_strgb(&prim->r0);
                *(s32 *)&line->x1 = *last;
                *(s32 *)&line->r1 = 0;
                line->code = abr == BTLFX_ABR_OPAQUE ? GPU_LINE_G2 : GPU_LINE_G2 | GPU_ABE;
                ex->btlfx_linkPrimAbr(PRIM_TAG(LINE_G2), ot, prim, abr);
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

    for (i = 0; i < FX_ARK_PIECES; i++) {
        fileIdx = i * 4 + 2;
        track = (s16 *)ex->btlfx_getFileData(entry->file, ((s16 *)fx_17d_0_pieceFiles)[fileIdx]);
        key = frame;
        if (key >= track[1]) {
            key = track[1] - 1;
        }
        if (i < FX_ARK_SHED_PIECES_FIRST || i >= FX_ARK_SHED_PIECES_END || frame < FX_ARK_SHED_FRAME) {
            if (i != FX_ARK_SCALED_PIECE) {
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
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_ARK_GREY_TRAIL), (s32)&p->pos8, 0, -1, 0x800, 0x1000, BTLFX_ABR_ADD,
                              fx_17d_0_partTrails[p->index4], 0);
        }
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_ARK_GREY_TRAIL), (s32)&p->pos8, 0, 0, 0x800, 0x1000, BTLFX_ABR_ADD,
                          fx_17d_0_partTrails[p->index4], 0);
        env->otZ = 0;
        if (frame >= FX_ARK_PART_FRAMES) {
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
        init->workSize = sizeof(Fx17d0Work80);
        init->elemSize = 0x10;
        init->elemFn = (s32)fx_17d_0_op80PoolTask;
        init->poolCount = FX_ARK_PART_POOL;
    } else if (mode == 1) {
        work->state0 = FX_ARK_STATE_STREAKS;
        work->frame4 = 0;
        fx_17d_0_env = env;
        ex->btlfx_initEffModels(fx_17d_0_effModels, FX_ARK_MODEL_COUNT);
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
        fxm_randDiscPoints(FX_ARK_STREAKS, FX_ARK_STREAK_RADIUS, fx_17d_0_streakPoints);
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

        case FX_ARK_STATE_STREAKS: {
            CVECTOR col;
            s32 swayAng;

            t = frame << 7;
            col.r = FX_ARK_STREAK_R;
            col.g = FX_ARK_STREAK_G;
            col.b = FX_ARK_STREAK_B;
            swayAng = t >> 1;
            rot.vx = ((0x1000 - ex->math_cos(swayAng)) >> 1) / 3;
            rot.vy = ex->math_sin(t >> 2) >> 2;
            rot.vz = work->frame4 * 4;
            work->rot38.vx = rot.vx >> 1;
            work->rot38.vy = rot.vy >> 1;
            work->rot38.vz = rot.vz >> 1;
            fxm_drawStreakField((u32 *)env->ot + 8, FX_ARK_STREAKS, 0x80, ((ex->math_sin(swayAng) * 0x18) >> 12) - 1, &rot, fx_17d_0_streakPoints,
                          &col, t, 1);
            if (frame >= FX_ARK_GRID_FRAME) {
                code = 1;
                abr = 1;
                gridBright = (frame - (FX_ARK_GRID_FRAME - 1)) * 12;
                gridScale = 0;
            }
            if (frame >= FX_ARK_STREAKS_FRAMES) {
                work->state0 = FX_ARK_STATE_SPACE;
                entry->frame = -1;
            }
            break;
        }
        case FX_ARK_STATE_SPACE: {
            CVECTOR col;
            s32 phase;
            s32 swayAng;
            s32 spriteX;
            s32 spriteY;

            t = (frame << 12) / FX_ARK_SPACE_FRAMES;
            phase = t >> 2;
            alpha = ex->math_sin(phase);
            col.r = FX_ARK_STREAK_R;
            col.g = FX_ARK_STREAK_G;
            col.b = FX_ARK_STREAK_B;
            swayAng = t >> 1;
            rot.vx = (0x1000 - ((0x1000 - ex->math_cos(swayAng)) >> 1)) / 3;
            gridScale = t >> 7;
            rot.vy = (0x1000 - ex->math_sin(phase)) >> 2;
            rot.vz = work->frame4 * 4;
            work->rot38.vx = rot.vx >> 1;
            work->rot38.vy = rot.vy >> 1;
            work->rot38.vz = rot.vz >> 1;
            fxm_drawStreakField((u32 *)env->ot + 8, FX_ARK_STREAKS, 0x80, ~((t * 20) >> 12), &rot, fx_17d_0_streakPoints, &col, 0x1000, 1);
            code = 1;
            abr = 1;
            spriteX = ((0x1000 - ex->math_sin(phase)) >> 5) + 0xA0;
            spriteY = 0x6E - ((0x1000 - ((0x1000 - ex->math_cos(swayAng)) >> 1)) >> 3);
            ex->btlfx_drawSpriteAtOt(FX_ARK_RED_ROCK_SCALE, (u8 *)ex->btlfx_getFileData(entry->file, FX_ARK_RED_ROCK_LAYER), spriteX, spriteY, 0, 0xFF, t);
            gridBright = 0x60 - gridScale;
            if (frame >= FX_ARK_SPACE_FRAMES) {
                work->state0 = code;
                entry->frame = -1;
                for (piece = 0; piece < FX_ARK_PIECES; piece++) {
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
        case FX_ARK_STATE_PIECES: {
            SVECTOR rot;
            CVECTOR col;
            s32 phase;
            s32 pieceScale;
            s32 streakSpeed;
            s32 spriteY;

            t = (frame << 12) / FX_ARK_PIECES_FRAMES;
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
            col.r = FX_ARK_STREAK_R;
            col.g = FX_ARK_STREAK_G;
            col.b = FX_ARK_STREAK_B;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = work->frame4 * 4;
            streakSpeed = (t >> 8) + 0x14;
            fxm_drawStreakField((u32 *)env->ot + 0x400, FX_ARK_STREAKS, 0x80, -streakSpeed, &rot, fx_17d_0_streakPoints, &col,
                          (ex->math_cos(phase) >> 1) + 0x800, 1);
            pieceScale = 0x1000 - ex->math_cos(phase);
            pieceScale = 0x1000 - ex->math_cos(pieceScale >> 2);
            if (t > 0xC00) {
                pieceScale = pieceScale - (ex->math_cos(t - 0xC00) - 0x1000);
            }
            fx_17d_0_drawPieces(entry, &work->pos10, frame % FX_ARK_PIECE_LOOP_FRAMES, (s32)work->model5C, work->model86, pieceScale);
            if (frame < FX_ARK_PIECES_FRAMES) {
                spriteY = (frame >> 2) + 0x6E;
                ex->btlfx_drawSpriteAtOt(FX_ARK_RED_ROCK_SCALE, (u8 *)ex->btlfx_getFileData(entry->file, FX_ARK_RED_ROCK_LAYER), 0xA0, spriteY, 0, 0xFF, alpha);
            }
            code = 1;
            abr = 1;
            gridScale = (t >> 4) + 0x20;
            gridBright = 0x40;
            if (frame >= FX_ARK_PIECES_FRAMES) {
                u16 texInfo[2][2];
                u16 texPos[2][2];

                work->state0 = FX_ARK_STATE_ARK;
                entry->frame = -1;
                work->pos10.vy = 0;
                BTLFX_COPY_SVEC(&work->pos8, &work->pos18);
                for (i = 0; i < 2; i++) {
                    texInfo[i][0] = ((Fx17d0Summon *)env->loader)->summon54->texInfo18[fx_17d_0_texSlots[i]];
                    texInfo[i][1] = ((Fx17d0Summon *)env->loader)->summon54->texInfo24[fx_17d_0_texSlots[i]];
                    texPos[i][0] = 0;
                    texPos[i][1] = ((Fx17d0Summon *)env->loader)->summon54->texPos30[fx_17d_0_texSlots[i]];
                }
                for (i = 0; i < FX_ARK_BODY_MODELS; i++) {
                    work->modelB0[i] = ((s32 (*)(void *, void *, void *))ex->Hi_RegisterTexListModel)(
                        (void *)ex->btlfx_getFileData(entry->file, fx_17d_0_texModelFileIds[i]), texInfo, texPos);
                }
            }
            break;
        }
        case FX_ARK_STATE_ARK: {
            VECTOR pos;
            SVECTOR rot;
            s16 *track;
            s32 key;

            alpha = 0x1000;
            work->rot38.vx += 0xC;
            work->rot38.vy += 6;
            fx_17d_0_drawPieces(entry, &work->pos10, frame, (s32)work->model5C, work->model86, 0x1000);
            if (frame >= FX_ARK_MORPH_FRAME) {
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
            if (frame >= FX_ARK_MORPH_2_FRAME) {
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
            if (frame < FX_ARK_TRAILS_FRAMES) {
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
                        part->index4 = work->count58 & (FX_ARK_PART_TRAILS - 1);
                        work->count58++;
                    }
                }
            }
            if (frame >= FX_ARK_BAND_FRAME) {
                fx_17d_0_drawCenterBandY((u32 *)env->ot + 1, 1,
                              ((0x1000 - ex->math_cos((((frame - FX_ARK_BAND_FRAME) << 12) / FX_ARK_BAND_OPEN_FRAMES) >> 2)) * FX_ARK_BAND_HEIGHT) >> 12, FX_ARK_BAND_RGB);
            }
            if (frame >= FX_ARK_WHITE_FRAME) {
                s32 tint = ((frame - FX_ARK_WHITE_FRAME) * 0xFF) / (FX_ARK_FRAMES - FX_ARK_WHITE_FRAME);

                ex->btlfx_drawScreenTint((u32 *)env->ot + 1, BTLFX_ABR_ADD, tint, tint, tint);
            }
            if (frame >= FX_ARK_FRAMES) {
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
                abr = BTLFX_ABR_OPAQUE;
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
            modelScale = FX_ARK_SPHERE_SCALE;
            if (work->state0 == FX_ARK_STATE_ARK) {
                modelScale = FX_ARK_SPHERE_ARK_SCALE;
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
        if (frame >= FX_ARK_FRAMES) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_17d_0/nonmatchings/fx_17d_0", fx_17d_0_headerPtr);
