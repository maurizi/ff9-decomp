#include "fxmod/fx_0dc.h"

void fx_0dc_shardPoint(s16 *a, s16 *b, s32 i, s16 *out) {
    BTLFX_EXPORTS(fx_0dc_env)->btlfx_pointAhead(a, b, -((fx_0dc_shardDists[i] * 0x455) >> 9), out);
}

static s32 fx_0dc_op80PoolTask(s32 mode, Fx0dcItem *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, s32 a5,
                  Fx0dcWork *work) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR *baseRot;
    Pool6CEntry *emitter;
    Fx0dcItem *piece;
    s32 t;
    s32 bright;
    s32 k;
    s32 pieceIdx;
    s32 data;

    if (mode == 1) {
        it->state0 = FX_SWDMAG_BLIZZAGA_SHARD_STATE_START;
        it->frame8 = 0;
    } else if (it->flying2 == 0) {
        SVECTOR rot;
        VECTOR scale;
        SVECTOR center;
        SVECTOR ringPos;
        SVECTOR piecePos;
        SVECTOR origin;
        SVECTOR dir;

        baseRot = &fx_0dc_shardAngles[it->shard4];
        switch (it->state0) {
        case FX_SWDMAG_BLIZZAGA_SHARD_STATE_START:
            if (it->frame8 == 0) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWDMAG_BLIZZAGA_SHARD_MIST), (s32)&it->pos18);
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
                it->state0 = FX_SWDMAG_BLIZZAGA_SHARD_STATE_GROW;
                it->frame8 = 0;
            }
            break;
        case FX_SWDMAG_BLIZZAGA_SHARD_STATE_GROW:
            {
                s32 ang = it->frame8 << 10;

                scale.vx = scale.vy = (ang * FX_SWDMAG_BLIZZAGA_SHARD_SCALE) >> 12;
                scale.vz = (ex->math_sin(ang >> 2) * FX_SWDMAG_BLIZZAGA_SHARD_SCALE) >> 12;
            }
            BTLFX_COPY_SVEC(&rot, baseRot);
            rot.vx -= 0x800;
            if (it->emit34 != NULL) {
                it->emit34->rot.vx = rot.vx;
                it->emit34->rot.vy = rot.vy;
                it->emit34->rot.vz = rot.vz;
            }
            if (++it->frame8 >= FX_SWDMAG_BLIZZAGA_SHARD_GROW_FRAMES) {
                it->state0 = FX_SWDMAG_BLIZZAGA_SHARD_STATE_HOLD;
                it->frame8 = 0;
            }
            break;
        case FX_SWDMAG_BLIZZAGA_SHARD_STATE_HOLD:
            scale.vx = scale.vy = scale.vz = FX_SWDMAG_BLIZZAGA_SHARD_SCALE;
            BTLFX_COPY_SVEC(&rot, baseRot);
            rot.vx -= 0x800;
            if (it->emit34 != NULL) {
                it->emit34->rot.vx = rot.vx;
                it->emit34->rot.vy = rot.vy;
                it->emit34->rot.vz = rot.vz;
            }
            if (frame < FX_SWDMAG_BLIZZAGA_SHARD_RING_FRAMES) {
                rot.pad = 1;
                center.vx = it->posC.vx;
                center.vy = it->posC.vy;
                center.vz = it->posC.vz;
                {
                    s32 ang = (frame << 10) / FX_SWDMAG_BLIZZAGA_SHARD_RING_FRAMES;
                    s32 ringScale;

                    ex->btlfx_pointAhead((s16 *)&center, (s16 *)&rot, 0x2F6, (s16 *)&ringPos);
                    data = ex->btlfx_getFileData(entry->file, FX_SWDMAG_BLIZZAGA_SNOW_RING_2);
                    ringScale = (ex->math_sin(ang) * 0x1154) >> 12;
                    ex->btlfx_drawRings((u8 *)data, &ringPos, &rot, ringScale, 0, BTLFX_ABR_ADD, ex->math_cos(ang) * 2 / 3);
                }
            }
            pieceIdx = FX_SWDMAG_BLIZZAGA_CRYSTAL_PIECES;
            if (work->split78 != 0) {
                it->frame8 = 0;
                if (it->shard4 < FX_SWDMAG_BLIZZAGA_WIDE_SHARDS) {
                    pieceIdx = FX_SWDMAG_BLIZZAGA_CRYSTAL_PIECES + FX_SWDMAG_BLIZZAGA_SHARD_PIECES;
                }
                BTLFX_COPY_SVEC(&origin, &it->pos18);
                for (k = 0; k < FX_SWDMAG_BLIZZAGA_SHARD_PIECES; k++, pieceIdx++) {
                    piece = it;
                    if (k != 0) {
                        piece = ex->btlfx_poolSpawn((s16 *)entry->pool);
                    }
                    if (piece != NULL) {
                        piece->flying2 = 1;
                        piece->state0 = pieceIdx;
                        piece->shard4 = k;
                        piece->frame8 = 0;
                        piece->model2C = work->model20[pieceIdx];
                        fx_0dc_shardPoint((s16 *)&origin, (s16 *)baseRot, pieceIdx, (s16 *)&piecePos);
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

        t = ((it->frame8 << 12) / FX_SWDMAG_BLIZZAGA_PIECE_FRAMES) >> 2;
        bright = ex->math_cos(t);
        it->posC.vx += it->pos18.vx;
        it->posC.vy += it->pos18.vy;
        it->posC.vz += it->pos18.vz;
        it->pos18.vy += 0x40;
        it->rot20.vx += it->spinX28;
        it->rot20.vy += it->spinY2A;
        bright = (bright * 3) >> 1;
        if (it->state0 < FX_SWDMAG_BLIZZAGA_CRYSTAL_PIECES) {
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
        ex->Hi_ModifyEffModelAbr(it->model2C, BTLFX_ABR_ADD);
        ex->Hi_ModifyEffModelRGB(it->model2C, (bright >> 5) - 0x80, (bright >> 5) - 0x80, (bright >> 5) - 0x80);
        if (++it->frame8 >= FX_SWDMAG_BLIZZAGA_PIECE_FRAMES) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0dc/nonmatchings/fx_0dc", fx_0dc_headerPtr);

s32 fx_0dc_op80(s32 mode, Fx0dcWork80 *w, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR scale;
    SVECTOR rot;
    SVECTOR rot2;
    Fx0dcItem *it;
    Fx0dcItem *piece; /* separate from it: one shard pointer per state (register allocation) */
    Pool6CEntry *emitter;
    SVECTOR *shardRot;
    s32 frame;
    s32 alpha;
    s32 bright;
    s32 tint;
    s32 t;
    s32 i;
    s32 j;
    s32 tp; /* FX_SWDMAG_BLIZZAGA_ICE_TPAGE, held in a register across the calls */
    s32 radius;

    if (mode == 0) {
        init->workSize = sizeof(Fx0dcWork80);
        init->elemSize = sizeof(Fx0dcItem);
        init->elemFn = (s32)fx_0dc_op80PoolTask;
        init->poolCount = FX_SWDMAG_BLIZZAGA_POOL_COUNT;
    } else if (mode == 1) {
        fx_0dc_env = env;
        ex->btlfx_initEffModels(fx_0dc_effModels, FX_SWDMAG_BLIZZAGA_MODEL_COUNT);
        tp = FX_SWDMAG_BLIZZAGA_ICE_TPAGE;
        w->model8 = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_BLIZZAGA_ICE_CRYSTAL), tp, FX_SWDMAG_BLIZZAGA_ICE_CLUT,
                                                  FX_SWDMAG_BLIZZAGA_ICE_U, FX_SWDMAG_BLIZZAGA_ICE_V);
        w->modelC = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_BLIZZAGA_ICE_CRYSTAL_2), tp, FX_SWDMAG_BLIZZAGA_ICE_CLUT,
                                                  FX_SWDMAG_BLIZZAGA_ICE_U, FX_SWDMAG_BLIZZAGA_ICE_V);
        w->model10 = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_BLIZZAGA_ICE_CRYSTAL_3), tp, FX_SWDMAG_BLIZZAGA_ICE_CLUT,
                                                   FX_SWDMAG_BLIZZAGA_ICE_U, FX_SWDMAG_BLIZZAGA_ICE_V);
        w->model14 = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_BLIZZAGA_CRYSTAL_SHEEN), FX_SWDMAG_BLIZZAGA_SHEEN_TPAGE, FX_SWDMAG_BLIZZAGA_SHEEN_CLUT,
                                                   FX_SWDMAG_BLIZZAGA_SHEEN_U, FX_SWDMAG_BLIZZAGA_SHEEN_V);
        w->model18 = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_BLIZZAGA_CRYSTAL_SHEEN_2), FX_SWDMAG_BLIZZAGA_SHEEN_TPAGE, FX_SWDMAG_BLIZZAGA_SHEEN_CLUT,
                                                   FX_SWDMAG_BLIZZAGA_SHEEN_U, FX_SWDMAG_BLIZZAGA_SHEEN_V);
        w->model1C = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_BLIZZAGA_CRYSTAL_SHEEN_3), FX_SWDMAG_BLIZZAGA_SHEEN_TPAGE, FX_SWDMAG_BLIZZAGA_SHEEN_CLUT,
                                                   FX_SWDMAG_BLIZZAGA_SHEEN_U, FX_SWDMAG_BLIZZAGA_SHEEN_V);
        for (i = 0; i < FX_SWDMAG_BLIZZAGA_PIECE_MODELS; i++) {
            w->model20[i] = (s16)ex->Hi_RegisterTexEffModel(
                (BtlModelHeader *)ex->btlfx_getFileData(entry->file, fx_0dc_shardModelBlocks[i]), tp, FX_SWDMAG_BLIZZAGA_ICE_CLUT, FX_SWDMAG_BLIZZAGA_ICE_U,
                FX_SWDMAG_BLIZZAGA_ICE_V);
        }
        w->state0 = FX_SWDMAG_BLIZZAGA_STATE_CRYSTAL;
        w->frame4 = 0;
        w->spawnCount7A = 0;
        w->split78 = 0;
        w->scrollFrame7C = 0;
        w->scroll80 = 0;
        ex->btlfx_getUnitPos(0, (s16 *)&w->pos58);
        BTLFX_COPY_SVEC(&w->pos60, &w->pos58);
        w->pos60.vy -= FX_SWDMAG_BLIZZAGA_BURST_HEIGHT;
        BTLFX_COPY_SVEC(&w->pos70, &w->pos58);
        w->pos70.vy -= 0x80;
        BTLFX_COPY_SVEC(&w->pos68, &w->pos58);
        w->pos68.vy -= FX_SWDMAG_BLIZZAGA_MIST_HEIGHT;
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWDMAG_BLIZZAGA_MIST), (s32)&w->pos58);
        if (emitter != NULL) {
            emitter->otZ = 0x80;
        }
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWDMAG_BLIZZAGA_MIST_HIGH), (s32)&w->pos68);
        if (emitter != NULL) {
            emitter->otZ = 0x80;
        }
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWDMAG_BLIZZAGA_ICE_SPARKLES_2), (s32)&w->pos58);
        if (emitter != NULL) {
            emitter->otZ = 0x80;
        }
        ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), BTLFX_ABR_SUB, FX_SWDMAG_BLIZZAGA_DARKEN_RGB);
        ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), BTLFX_ABR_ADD, FX_SWDMAG_BLIZZAGA_BLUE_TINT_RGB);
    } else {
        frame = entry->frame;
        pos.vx = w->pos58.vx;
        pos.vy = w->pos58.vy;
        pos.vz = w->pos58.vz;
        scale.vx = scale.vy = FX_SWDMAG_BLIZZAGA_CRYSTAL_SCALE;
        scale.vz = FX_SWDMAG_BLIZZAGA_CRYSTAL_SCALE_Z;
        if (w->frame4 < 3) {
            t = w->frame4 << 8;
            scale.vx = scale.vy = (t * scale.vx) >> 10;
            scale.vz = (scale.vz * ex->math_sin(t)) >> 12;
        }
        alpha = 0;
        switch (w->state0) {
        case FX_SWDMAG_BLIZZAGA_STATE_CRYSTAL:
            alpha = 0x1000;
            bright = 0x2000 - (frame << 10);
            rot.vx = -0x400;
            rot.vy = 0;
            rot.vz = -0x400;
            ex->Hi_SetEffModelOffset(w->model8, 2);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, w->model8);
            ex->Hi_ModifyEffModelRGB(w->model8, (bright >> 5) - 0x80, (bright >> 5) - 0x80, (bright >> 5) - 0x80);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, w->model14);
            if (frame < FX_SWDMAG_BLIZZAGA_CRYSTAL_FLASH_FRAMES) {
                bright = 0xFF - frame * 0xFF / FX_SWDMAG_BLIZZAGA_CRYSTAL_FLASH_FRAMES;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), BTLFX_ABR_ADD, bright >> 2, bright * 2 / 3, bright);
                if (frame == 0) {
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), BTLFX_ABR_ADD, FX_SWDMAG_BLIZZAGA_WHITE_FLASH_RGB);
                }
            }
            if (frame >= FX_SWDMAG_BLIZZAGA_CRYSTAL_FRAMES) {
                w->state0 = FX_SWDMAG_BLIZZAGA_STATE_SHARDS;
                entry->frame = -1;
            }
            break;
        case FX_SWDMAG_BLIZZAGA_STATE_SHARDS:
            alpha = 0x1000;
            if (frame & 1) {
                it = ex->btlfx_poolSpawn((s16 *)entry->pool);
                if (it != NULL) {
                    it->flying2 = 0;
                    it->shard4 = w->spawnCount7A;
                    shardRot = &fx_0dc_shardAngles[w->spawnCount7A];
                    if (w->spawnCount7A >= FX_SWDMAG_BLIZZAGA_WIDE_SHARDS) {
                        it->model2C = w->modelC;
                        it->model30 = w->model18;
                    } else {
                        it->model2C = w->model10;
                        it->model30 = w->model1C;
                    }
                    it->posC.vx = w->pos58.vx;
                    it->posC.vy = w->pos58.vy;
                    it->posC.vz = w->pos58.vz;
                    radius = 200;
                    it->posC.vx += (ex->math_sin(shardRot->vy + 0x800) * radius) >> 12;
                    it->posC.vz += (ex->math_cos(shardRot->vy + 0x800) * radius) >> 12;
                    it->pos18.vx = it->posC.vx;
                    it->pos18.vy = it->posC.vy;
                    it->pos18.vz = it->posC.vz;
                    w->spawnCount7A++;
                }
            }
            rot.vx = -0x400;
            rot.vy = 0;
            rot.vz = -0x400;
            ex->Hi_SetEffModelOffset(w->model8, 2);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, w->model8);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, w->model14);
            if (w->spawnCount7A >= FX_SWDMAG_BLIZZAGA_SHARD_COUNT) {
                w->state0 = FX_SWDMAG_BLIZZAGA_STATE_SPIKES;
                entry->frame = -1;
                w->emit84 = emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWDMAG_BLIZZAGA_ICE_SPIKES), (s32)&w->pos58);
                if (emitter != NULL) {
                    emitter->otZ = 0x5C;
                }
            }
            break;
        case FX_SWDMAG_BLIZZAGA_STATE_SPIKES:
            alpha = 0x1000;
            rot.vx = -0x400;
            rot.vy = 0;
            rot.vz = -0x400;
            ex->Hi_SetEffModelOffset(w->model8, 2);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, w->model8);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, w->model14);
            if (frame >= FX_SWDMAG_BLIZZAGA_SPIKES_FRAMES) {
                w->state0 = FX_SWDMAG_BLIZZAGA_STATE_HOLD;
                entry->frame = -1;
            }
            break;
        case FX_SWDMAG_BLIZZAGA_STATE_HOLD:
            alpha = 0x1000;
            rot.vx = -0x400;
            rot.vy = 0;
            rot.vz = -0x400;
            ex->Hi_SetEffModelOffset(w->model8, 2);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, w->model8);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, w->model14);
            rot.pad = 1;
            if (frame >= FX_SWDMAG_BLIZZAGA_HOLD_FRAMES) {
                w->state0 = FX_SWDMAG_BLIZZAGA_STATE_SHATTER;
                w->split78 = 1;
                entry->frame = -1;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), BTLFX_ABR_SUB, FX_SWDMAG_BLIZZAGA_DARKEN_RGB);
                ex->btlfx_stopEmitter(w->emit84);
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWDMAG_BLIZZAGA_ICE_FLARES), (s32)&w->pos58);
                if (emitter != NULL) {
                    emitter->otZ = 0x60;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWDMAG_BLIZZAGA_ICE_FLARES_2), (s32)&w->pos58);
                if (emitter != NULL) {
                    emitter->otZ = 0x60;
                }
                ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWDMAG_BLIZZAGA_SNOW_BURST), (s32)&w->pos60);
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWDMAG_BLIZZAGA_FREEZE_BURST), (s32)&w->pos60);
                if (emitter != NULL) {
                    emitter->otZ = 0x80;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWDMAG_BLIZZAGA_FROST_RING_2), (s32)&w->pos60);
                if (emitter != NULL) {
                    emitter->otZ = 0x80;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWDMAG_BLIZZAGA_ICE_SPARKLES), (s32)&w->pos60);
                if (emitter != NULL) {
                    emitter->otZ = 0x80;
                }
                rot.vx = -0x400;
                rot.vy = 0;
                rot.vz = -0x400;
                for (j = 0; j < FX_SWDMAG_BLIZZAGA_CRYSTAL_PIECES; j++) {
                    piece = ex->btlfx_poolSpawn((s16 *)entry->pool);
                    if (piece != NULL) {
                        piece->flying2 = 1;
                        piece->state0 = j;
                        piece->shard4 = j;
                        piece->frame8 = 0;
                        piece->model2C = w->model20[j];
                        fx_0dc_shardPoint((s16 *)&w->pos58, (s16 *)&rot, j, (s16 *)&rot2);
                        BTLFX_COPY_SVEC(&piece->rot20, &rot);
                        piece->posC.vx = rot2.vx * 16;
                        piece->posC.vy = rot2.vy * 16;
                        piece->posC.vz = rot2.vz * 16;
                        piece->spinX28 = ex->btlfx_randCentered(0x5C);
                        piece->spinY2A = ex->btlfx_randCentered(0x80);
                        ex->btlfx_rotateOffset(&rot, -(((ex->btlfx_rand() & 0xF) + (0x5C - j * 8)) * 16), (s32)&piece->pos18);
                        piece->pos18.vx += ex->btlfx_randCentered(0x3E0);
                        piece->pos18.vz += ex->btlfx_randCentered(0x3E0);
                    }
                }
            }
            break;
        case FX_SWDMAG_BLIZZAGA_STATE_SHATTER:
            rot.vx = -0x400;
            rot.vz = -0x400;
            rot.vy = 0;
            if (frame < FX_SWDMAG_BLIZZAGA_RING_FADE_FRAMES) {
                alpha = ex->math_cos((frame << 10) / FX_SWDMAG_BLIZZAGA_RING_FADE_FRAMES);
            }
            if (frame == FX_SWDMAG_BLIZZAGA_SHATTER_WAVE_FRAME) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWDMAG_BLIZZAGA_SHATTER_WAVE), (s32)&w->pos60);
                if (emitter != NULL) {
                    emitter->otZ = 0x80;
                }
            }
            if (frame < FX_SWDMAG_BLIZZAGA_SHATTER_FLASH_FRAMES) {
                tint = 0xFF - ((frame * 0xFF) >> 3);
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), BTLFX_ABR_ADD, tint >> 2, tint * 2 / 3, tint);
                if (frame == 0) {
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), BTLFX_ABR_ADD, FX_SWDMAG_BLIZZAGA_WHITE_FLASH_RGB);
                }
            }
            if (frame >= FX_SWDMAG_BLIZZAGA_SHATTER_FRAMES) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        if (w->state0 >= FX_SWDMAG_BLIZZAGA_STATE_CRYSTAL && w->state0 < FX_SWDMAG_BLIZZAGA_STATE_SHATTER) {
            w->scrollFrame7C++;
            if (w->scrollFrame7C < FX_SWDMAG_BLIZZAGA_SCROLL_SLOW_FRAMES) {
                w->scroll80 += FX_SWDMAG_BLIZZAGA_SCROLL_SLOW_FRAMES - (w->scrollFrame7C * 10) / FX_SWDMAG_BLIZZAGA_SCROLL_SLOW_FRAMES;
            } else {
                w->scroll80++;
            }
            ex->btlfx_drawScrollMove(w->scroll80, FX_SWDMAG_BLIZZAGA_REFLECTION_VRAM_X, FX_SWDMAG_BLIZZAGA_REFLECTION_VRAM_Y, FX_SWDMAG_BLIZZAGA_REFLECTION_VRAM_W,
                                     FX_SWDMAG_BLIZZAGA_REFLECTION_VRAM_H, FX_SWDMAG_BLIZZAGA_REFLECTION_SRC_X, FX_SWDMAG_BLIZZAGA_REFLECTION_SRC_Y);
        }
        if (alpha != 0) {
            rot2.vx = 0x400;
            rot2.vy = 0;
            rot2.vz = w->frame4 * 41;
            rot2.pad = 1;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_BLIZZAGA_SNOW_RING), &w->pos58, &rot2, (w->frame4 << 6) + 0x15A9,
                              0, BTLFX_ABR_ADD, alpha);
            rot2.vz = w->frame4 * 24 + 0x400;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_BLIZZAGA_SNOW_RING), &w->pos58, &rot2, (w->frame4 << 6) + 0x15A9,
                              0, BTLFX_ABR_ADD, alpha);
            env->otZ = 0x127;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_BLIZZAGA_FROST_RING), &w->pos58, NULL, (w->frame4 << 5) + 0x15A9,
                              0, BTLFX_ABR_ADD, alpha);
            env->otZ = 0;
        }
        w->frame4++;
    }
    return 0;
}

s32 fx_0dc_op84(s32 mode, Fx0dcWork84 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    s32 frame;
    s32 alpha;
    s32 scale0;
    s32 scale1;
    s32 flicker;
    s32 ang;
    s32 t;
    u16 ry;

    if (mode == 0) {
        *args = sizeof(Fx0dcWork84);
    } else {
        if (mode == 1) {
            fx_0dc_env = env;
            work->state0 = FX_SWDMAG_BLIZZAGA_SWORD_STATE_APPEAR;
            work->frame4 = 0;
        } else {
            ry = scale1 = scale0 = alpha = 0;
            frame = entry->frame;
            ex->btlfx_getWeaponPos(BTLFX_UNIT_CASTER, 1, (s16 *)&work->pos8);
            switch (work->state0) {
            case FX_SWDMAG_BLIZZAGA_SWORD_STATE_APPEAR:
                ry = 0x200;
                alpha = (frame << 12) / FX_SWDMAG_BLIZZAGA_SWORD_APPEAR_FRAMES;
                scale0 = ex->math_sin(alpha >> 2) >> 1;
                scale1 = ((0x1000 - alpha) >> 1) + 0x800;
                if (frame >= FX_SWDMAG_BLIZZAGA_SWORD_APPEAR_FRAMES) {
                    work->state0 = FX_SWDMAG_BLIZZAGA_SWORD_STATE_FLICKER;
                    entry->frame = -1;
                }
                break;
            case FX_SWDMAG_BLIZZAGA_SWORD_STATE_FLICKER:
                flicker = (frame & 1) << 8;
                scale1 = flicker + 0x1000;
                t = (frame << 12) / FX_SWDMAG_BLIZZAGA_SWORD_FLICKER_FRAMES;
                alpha = (flicker + 0x800) + (t >> 1);
                ry = (t >> 3) + 0x200;
                scale0 = scale1;
                if (frame >= FX_SWDMAG_BLIZZAGA_SWORD_FLICKER_FRAMES) {
                    work->state0 = FX_SWDMAG_BLIZZAGA_SWORD_STATE_FADE;
                    entry->frame = -1;
                }
                break;
            case FX_SWDMAG_BLIZZAGA_SWORD_STATE_FADE:
                t = (frame << 12) / FX_SWDMAG_BLIZZAGA_SWORD_FADE_FRAMES;
                ang = t >> 2;
                ry = 0x400 - ang;
                alpha = ex->math_cos(ang);
                scale0 = ex->math_sin(ang) + 0x1000;
                scale1 = ex->math_sin(ang) * 2 + 0x1000;
                if (frame >= FX_SWDMAG_BLIZZAGA_SWORD_FADE_FRAMES) {
                    return 1;
                }
                break;
            }
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = ry;
            rot.pad = 0;
            env->otZ = 0x80;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_BLIZZAGA_SWORD_GLINT), &work->pos8, NULL, scale0 / 3, 0, BTLFX_ABR_ADD, alpha / 2);
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_BLIZZAGA_SWORD_FLASH), &work->pos8, &rot, scale1, 0, BTLFX_ABR_ADD, alpha * 2 / 3);
            env->otZ = 0;
            work->frame4++;
        }
    }
    return 0;
}
