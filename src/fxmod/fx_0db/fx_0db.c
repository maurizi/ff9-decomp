#include "fxmod/fx_0db.h"

static s32 fx_0db_op80PoolTask(s32 mode, Fx0dbItem *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame,
                  Unk800DA1B8 *pool, Fx0dbWork80 *work) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR off;
    SVECTOR rot;
    VECTOR scale;
    SVECTOR *shardRot;
    Pool6CEntry *emitter;
    s32 ang;
    s32 drift;
    s32 rgb;
    s32 t;
    s32 fade;

    if (mode == 1) {
        it->state4 = FX_SWDMAG_BLIZZARA_CRYSTAL_STATE_GROW;
        it->frame8 = 0;
        emitter = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWDMAG_BLIZZARA_SHARD_MIST), (s32)&work->pos14);
        it->emit40 = emitter;
        if (emitter != NULL) {
            emitter->otZ = 0x40;
        }
        return 0;
    }
    fade = 0;
    shardRot = &fx_0db_shardAngles[it->shard0];
    switch (it->state4) {
    case FX_SWDMAG_BLIZZARA_CRYSTAL_STATE_GROW:
        t = it->frame8 << 10;
        ang = t >> 2;
        scale.vx = scale.vy = ((0x1000 - ex->math_cos(ang)) * (it->size3C * 5 / 2)) >> 12;
        scale.vz = (ex->math_sin(ang) * it->size3C) >> 12;
        BTLFX_COPY_SVEC(&rot, shardRot);
        rot.vx -= 0x800;
        fade = 0x1000;
        if (it->emit40 != NULL) {
            if (it->emit40->state == BTLFX_EMIT_DONE) {
                it->emit40 = NULL;
            }
            if (it->emit40 != NULL) {
                it->emit40->rot.vx = shardRot->vx;
                it->emit40->rot.vy = shardRot->vy;
                it->emit40->rot.vz = shardRot->vz;
            }
        }
        it->frame8++;
        if (it->frame8 >= FX_SWDMAG_BLIZZARA_CRYSTAL_GROW_FRAMES) {
            it->state4 = FX_SWDMAG_BLIZZARA_CRYSTAL_STATE_HOLD;
            it->frame8 = 0;
        }
        break;
    case FX_SWDMAG_BLIZZARA_CRYSTAL_STATE_HOLD:
        scale.vx = scale.vy = it->size3C * 5 / 2;
        scale.vz = it->size3C;
        BTLFX_COPY_SVEC(&rot, shardRot);
        rot.vx -= 0x800;
        fade = 0x1000;
        if (it->emit40 != NULL) {
            if (it->emit40->state == BTLFX_EMIT_DONE) {
                it->emit40 = NULL;
            }
            if (it->emit40 != NULL) {
                it->emit40->rot.vx = rot.vx;
                it->emit40->rot.vy = rot.vy;
                it->emit40->rot.vz = rot.vz;
            }
        }
        if (work->split24 != 0) {
            it->state4 = FX_SWDMAG_BLIZZARA_CRYSTAL_STATE_SPLIT;
            it->frame8 = 0;
            it->spinX38 += -25 + ex->btlfx_rand() % 50;
            it->spinY3A += -16 + (ex->btlfx_rand() & 0x1F);
            BTLFX_COPY_SVEC(&it->rot30, shardRot);
            it->posC[1].vx = it->posC[0].vx;
            it->posC[1].vy = it->posC[0].vy;
            it->posC[1].vz = it->posC[0].vz;
            it->posC[2].vx = it->posC[0].vx;
            it->posC[2].vy = it->posC[0].vy;
            it->posC[2].vz = it->posC[0].vz;
        }
        break;
    case FX_SWDMAG_BLIZZARA_CRYSTAL_STATE_SPLIT:
        t = (it->frame8 << 12) / FX_SWDMAG_BLIZZARA_PIECES_FRAMES;
        ang = t >> 2;
        scale.vx = scale.vy = (ex->math_cos(ang) * (it->size3C * 5 / 2)) >> 12;
        scale.vz = it->size3C;
        fade = ex->math_cos(ang);
        drift = ex->math_cos(ang) >> 7;
        ex->btlfx_rotateOffset(shardRot, drift, (s32)&off);
        it->posC[0].vx -= off.vx;
        it->posC[0].vy -= off.vy - (it->frame8 - 6);
        it->posC[0].vz -= off.vz;
        ex->btlfx_rotateOffset(shardRot, drift * 2, (s32)&off);
        it->posC[1].vx -= off.vx;
        it->posC[1].vy -= off.vy - (it->frame8 - 6) * 4;
        it->posC[1].vz -= off.vz;
        ex->btlfx_rotateOffset(shardRot, drift * 2 + drift, (s32)&off);
        shardRot = &it->rot30;
        it->posC[2].vx -= off.vx;
        it->posC[2].vy -= off.vy - (it->frame8 - 6) * 8;
        it->posC[2].vz -= off.vz;
        it->rot30.vx += it->spinX38;
        it->rot30.vy += it->spinY3A;
        it->frame8++;
        if (it->frame8 >= FX_SWDMAG_BLIZZARA_PIECES_FRAMES) {
            return 1;
        }
        break;
    }
    scale.vx >>= 1;
    scale.vy >>= 1;
    if (it->state4 < FX_SWDMAG_BLIZZARA_CRYSTAL_STATE_SPLIT) {
        ex->Hi_SetEffModelOffset(work->model4, 2);
        ex->Hi_DrawEffModel(shardRot, (VECTOR *)&it->posC[0], (s32)&scale, work->model4);
        if (fade < 4000) {
            ex->Hi_ModifyEffModelAbr(work->model4, BTLFX_ABR_ADD);
            rgb = fade >> 5;
        } else {
            ex->Hi_ModifyEffModelAbr(work->model4, BTLFX_ABR_HALF);
            rgb = fade * 2;
            rgb += fade;
            rgb >>= 6;
        }
        rgb -= 0x80;
        ex->Hi_ModifyEffModelRGB(work->model4, rgb, rgb, rgb);
        ex->Hi_DrawEffModel(shardRot, (VECTOR *)&it->posC[0], (s32)&scale, work->model10);
    } else {
        ex->Hi_DrawEffModel(shardRot, (VECTOR *)&it->posC[0], (s32)&scale, work->model10);
        if (fade < 4000) {
            ex->Hi_ModifyEffModelAbr(work->model10, BTLFX_ABR_ADD);
            rgb = fade >> 5;
        } else {
            ex->Hi_ModifyEffModelAbr(work->model10, BTLFX_ABR_HALF);
            rgb = fade * 2;
            rgb += fade;
            rgb >>= 6;
        }
        rgb -= 0x80;
        ex->Hi_ModifyEffModelRGB(work->model10, rgb, rgb, rgb);
        ex->Hi_DrawEffModel(shardRot, (VECTOR *)&it->posC[1], (s32)&scale, work->modelC);
        if (fade < 4000) {
            ex->Hi_ModifyEffModelAbr(work->modelC, BTLFX_ABR_ADD);
            rgb = fade >> 5;
        } else {
            ex->Hi_ModifyEffModelAbr(work->modelC, BTLFX_ABR_HALF);
            rgb = fade * 2;
            rgb += fade;
            rgb >>= 6;
        }
        rgb -= 0x80;
        ex->Hi_ModifyEffModelRGB(work->modelC, rgb, rgb, rgb);
        ex->Hi_DrawEffModel(shardRot, (VECTOR *)&it->posC[2], (s32)&scale, work->model8);
        if (fade < 4000) {
            ex->Hi_ModifyEffModelAbr(work->model8, BTLFX_ABR_ADD);
            rgb = fade >> 5;
        } else {
            ex->Hi_ModifyEffModelAbr(work->model8, BTLFX_ABR_HALF);
            rgb = fade * 2;
            rgb += fade;
            rgb >>= 6;
        }
        rgb -= 0x80;
        ex->Hi_ModifyEffModelRGB(work->model8, rgb, rgb, rgb);
    }
    return 0;
}

s32 fx_0db_op80(s32 mode, Fx0dbWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR shardPos;
    Fx0dbItem *part;
    Pool6CEntry *emitter;
    s32 frame;
    s32 i;
    s32 tint;
    s16 *angX;
    s16 *angY;

    if (mode == 0) {
        ((EffInit *)args)->workSize = sizeof(Fx0dbWork80);
        ((EffInit *)args)->elemSize = sizeof(Fx0dbItem);
        ((EffInit *)args)->elemFn = (s32)fx_0db_op80PoolTask;
        ((EffInit *)args)->poolCount = FX_SWDMAG_BLIZZARA_CRYSTAL_COUNT;
    } else if (mode == 1) {
        fx_0db_env = env;
        ex->btlfx_initEffModels(fx_0db_effModels, FX_SWDMAG_BLIZZARA_MODEL_COUNT);
        work->model4 = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_BLIZZARA_ICE_CRYSTAL), FX_SWDMAG_BLIZZARA_ICE_CRYSTAL_TPAGE, FX_SWDMAG_BLIZZARA_ICE_CRYSTAL_CLUT, FX_SWDMAG_BLIZZARA_ICE_CRYSTAL_U, FX_SWDMAG_BLIZZARA_ICE_CRYSTAL_V);
        work->model8 = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_BLIZZARA_CRYSTAL_PIECE_0), FX_SWDMAG_BLIZZARA_CRYSTAL_PIECE_0_TPAGE, FX_SWDMAG_BLIZZARA_CRYSTAL_PIECE_0_CLUT, FX_SWDMAG_BLIZZARA_CRYSTAL_PIECE_0_U, FX_SWDMAG_BLIZZARA_CRYSTAL_PIECE_0_V);
        work->modelC = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_BLIZZARA_CRYSTAL_PIECE_1), FX_SWDMAG_BLIZZARA_CRYSTAL_PIECE_1_TPAGE, FX_SWDMAG_BLIZZARA_CRYSTAL_PIECE_1_CLUT, FX_SWDMAG_BLIZZARA_CRYSTAL_PIECE_1_U, FX_SWDMAG_BLIZZARA_CRYSTAL_PIECE_1_V);
        work->model10 = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_BLIZZARA_CRYSTAL_PIECE_2), FX_SWDMAG_BLIZZARA_CRYSTAL_PIECE_2_TPAGE, FX_SWDMAG_BLIZZARA_CRYSTAL_PIECE_2_CLUT, FX_SWDMAG_BLIZZARA_CRYSTAL_PIECE_2_U, FX_SWDMAG_BLIZZARA_CRYSTAL_PIECE_2_V);
        work->model10 = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_BLIZZARA_ICE_REFLECTION), FX_SWDMAG_BLIZZARA_ICE_REFLECTION_TPAGE, FX_SWDMAG_BLIZZARA_ICE_REFLECTION_CLUT, FX_SWDMAG_BLIZZARA_ICE_REFLECTION_U, FX_SWDMAG_BLIZZARA_ICE_REFLECTION_V);
        work->state0 = FX_SWDMAG_BLIZZARA_STATE_CRYSTALS;
        work->spawnCount26 = 0;
        work->frame28 = 0;
        work->split24 = 0;
        work->scroll2C = 0;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos14);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos1C);
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWDMAG_BLIZZARA_FROST_MIST), (s32)&work->pos14);
        if (emitter != NULL) {
            emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x46);
        }
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWDMAG_BLIZZARA_WHITE_RING), (s32)&work->pos14);
        if (emitter != NULL) {
            emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x46);
        }
        angX = &fx_0db_shardAngles[0].vx;
        for (i = 0, angY = angX + 1; i < FX_SWDMAG_BLIZZARA_CRYSTAL_COUNT; i++) {
            *angX += 0x5C;
            angX += 4;
            *angY += 0x200;
            angY += 4;
        }
        return 0;
    } else {
        frame = entry->frame;
        switch (work->state0) {
        case FX_SWDMAG_BLIZZARA_STATE_CRYSTALS:
            part = ex->btlfx_poolSpawn((s16 *)entry->pool);
            if (part != NULL) {
                part->shard0 = work->spawnCount26;
                work->spawnCount26++;
                ex->btlfx_pointAhead((s16 *)&work->pos14, (s16 *)&fx_0db_shardAngles[part->shard0], -0x10, (s16 *)&shardPos);
                part->posC[0].vx = shardPos.vx;
                part->posC[0].vy = shardPos.vy;
                part->posC[0].vz = shardPos.vz;
                part->size3C = fx_0db_shardAngles[part->shard0].pad * 3;
            }
            work->scroll2C += 0x20;
            if (frame >= FX_SWDMAG_BLIZZARA_CRYSTALS_FRAMES) {
                work->state0 = FX_SWDMAG_BLIZZARA_STATE_GLEAM;
                entry->frame = -1;
            }
            break;
        case FX_SWDMAG_BLIZZARA_STATE_GLEAM:
            work->scroll2C += (ex->math_cos((frame << 10) / FX_SWDMAG_BLIZZARA_GLEAM_FRAMES) << 5) >> 12;
            if (frame >= FX_SWDMAG_BLIZZARA_GLEAM_FRAMES) {
                work->state0 = FX_SWDMAG_BLIZZARA_STATE_SHATTER;
                entry->frame = -1;
                work->split24 = 1;
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWDMAG_BLIZZARA_FREEZE_BURST), (s32)&work->pos14);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWDMAG_BLIZZARA_FROST_RING), (s32)&work->pos14);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWDMAG_BLIZZARA_ICE_FLARES), (s32)&work->pos14);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWDMAG_BLIZZARA_ICE_FLARES_2), (s32)&work->pos14);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
                }
            }
            break;
        case FX_SWDMAG_BLIZZARA_STATE_SHATTER:
            if (frame < FX_SWDMAG_BLIZZARA_FLASH_FRAMES) {
                tint = (FX_SWDMAG_BLIZZARA_FLASH_FRAMES - 1 - frame) * 0xFA / (FX_SWDMAG_BLIZZARA_FLASH_FRAMES - 1);
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), BTLFX_ABR_ADD, tint >> 2, tint >> 1, tint);
            }
            if (frame == 0) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWDMAG_BLIZZARA_SHATTER_WAVE), (s32)&work->pos14);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
                }
            }
            if (frame >= FX_SWDMAG_BLIZZARA_SHATTER_FRAMES) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        ex->btlfx_drawScrollMove(work->scroll2C, FX_SWDMAG_BLIZZARA_REFLECTION_VRAM_X, FX_SWDMAG_BLIZZARA_REFLECTION_VRAM_Y, FX_SWDMAG_BLIZZARA_REFLECTION_VRAM_W,
                                 FX_SWDMAG_BLIZZARA_REFLECTION_VRAM_H, FX_SWDMAG_BLIZZARA_REFLECTION_SRC_X, FX_SWDMAG_BLIZZARA_REFLECTION_SRC_Y);
        work->frame28++;
    }
    return 0;
}

s32 fx_0db_op84(s32 mode, Fx0dbWork84 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    s32 frame;
    s32 alpha;
    s32 scale0;
    s32 scale1;
    s32 ang;
    s32 t;
    s32 flicker;
    s32 grow;
    u16 ry;

    if (mode == 0) {
        *args = sizeof(Fx0dbWork84);
    } else if (mode == 1) {
        fx_0db_env = env;
        work->state0 = FX_SWDMAG_BLIZZARA_SWORD_STATE_APPEAR;
        work->frame4 = 0;
    } else {
        alpha = 0;
        scale0 = 0;
        scale1 = 0;
        frame = entry->frame;
        ex->btlfx_getWeaponPos(0x10, 1, (s16 *)&work->pos8);
        ry = 0;
        switch (work->state0) {
        case FX_SWDMAG_BLIZZARA_SWORD_STATE_APPEAR:
            ry = 0x200;
            alpha = (frame << 12) / FX_SWDMAG_BLIZZARA_SWORD_APPEAR_FRAMES;
            t = ex->math_sin(alpha >> 2);
            scale0 = t >> 1;
            scale1 = ((0x1000 - alpha) >> 1) + 0x800;
            if (frame >= FX_SWDMAG_BLIZZARA_SWORD_APPEAR_FRAMES) {
                work->state0 = FX_SWDMAG_BLIZZARA_SWORD_STATE_FLICKER;
                entry->frame = -1;
            }
            break;
        case FX_SWDMAG_BLIZZARA_SWORD_STATE_FLICKER:
            t = (frame << 12) / FX_SWDMAG_BLIZZARA_SWORD_FLICKER_FRAMES;
            flicker = (frame & 1) << 8;
            scale1 = flicker + 0x1000;
            grow = (t >> 1) + 0x800;
            alpha = flicker + grow;
            ry = (t >> 3) + 0x200;
            scale0 = scale1;
            if (frame >= FX_SWDMAG_BLIZZARA_SWORD_FLICKER_FRAMES) {
                work->state0 = FX_SWDMAG_BLIZZARA_SWORD_STATE_FADE;
                entry->frame = -1;
            }
            break;
        case FX_SWDMAG_BLIZZARA_SWORD_STATE_FADE:
            t = (frame << 12) / FX_SWDMAG_BLIZZARA_SWORD_FADE_FRAMES;
            ang = t >> 2;
            ry = 0x400 - ang;
            alpha = ex->math_cos(ang);
            scale0 = ex->math_sin(ang) + 0x1000;
            scale1 = ex->math_sin(ang) * 3 / 2 + 0x1000;
            if (frame >= FX_SWDMAG_BLIZZARA_SWORD_FADE_FRAMES) {
                return 1;
            }
            break;
        }
        rot.vx = 0;
        rot.vy = 0;
        rot.vz = ry;
        rot.pad = 0;
        env->otZ = 0x80;
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_BLIZZARA_SWORD_GLINT), &work->pos8, NULL, scale0 / 3, 0, BTLFX_ABR_ADD, alpha / 2);
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_BLIZZARA_SWORD_FLASH), &work->pos8, &rot, scale1 / 2, 0, BTLFX_ABR_ADD, alpha * 2 / 3);
        env->otZ = 0;
        work->frame4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0db/nonmatchings/fx_0db", fx_0db_headerPtr);
