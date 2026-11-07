#include "fxmod/fx_006.h"

s32 fx_006_op80(s32 mode, Fx006Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 morph;
    s32 t;
    Pool6CEntry *emitter;
    VECTOR pos;
    VECTOR scale;
    SVECTOR rot;

    if (mode == 0) {
        *args = sizeof(Fx006Work);
    } else if (mode == 1) {
        fx_006_env = env;
        ex->btlfx_initEffModels(fx_006_effModels, FX_BLIZZARD_MODEL_COUNT);
        work->model8 = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_BLIZZARD_ICE_BLOCK), 0xBA, 0x3DC0, 0, 0);
        work->modelC = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_BLIZZARD_ICE_CRACKING), 0xBA, 0x3DC0, 0, 0);
        work->model10 = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_BLIZZARD_ICE_SHARDS), 0xBA, 0x3DC0, 0, 0);
        work->model4 =
            (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_BLIZZARD_ICE_REFLECTION), 0xB8, 0x3D40, 0x80, 0x80);
        work->state0 = FX_BLIZZARD_STATE_SWIRL;
        work->frame24 = 0;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos14);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos1C);
    } else {
        frame = entry->frame;
        switch (work->state0) {
        case FX_BLIZZARD_STATE_SWIRL:
            if (frame == 0) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_BLIZZARD_FROST_SWIRL), (s32)&work->pos14);
                if (emitter != NULL) {
                    emitter->trails = (s32)fx_006_trails;
                    emitter->trailSlots = FX_BLIZZARD_FROST_SWIRL_TRAIL_SLOTS;
                    emitter->trailStride = FX_BLIZZARD_FROST_SWIRL_TRAIL_STRIDE;
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
                }
            }
            if (frame >= FX_BLIZZARD_SWIRL_FRAMES) {
                work->state0 = FX_BLIZZARD_STATE_GROW;
                entry->frame = -1;
            }
            break;
        case FX_BLIZZARD_STATE_GROW:
            t = frame << 10;
            scale.vx = scale.vy = scale.vz = ((0x1000 - ex->math_cos(t >> 2)) * FX_BLIZZARD_ICE_SCALE) >> 12;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            pos.vx = work->pos14.vx;
            pos.vy = work->pos14.vy;
            pos.vz = work->pos14.vz;
            ex->Hi_SetEffModelOffset(work->model8, 2);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model8);
            ex->Hi_ModifyEffModelAbr(work->model8, 0);
            ex->Hi_ModifyEffModelRGB(work->model8, 0, 0, 0);
            if (frame >= FX_BLIZZARD_GROW_FRAMES) {
                work->state0 = FX_BLIZZARD_STATE_GLEAM;
                entry->frame = -1;
            }
            break;
        case FX_BLIZZARD_STATE_GLEAM:
            scale.vz = FX_BLIZZARD_ICE_SCALE;
            scale.vy = FX_BLIZZARD_ICE_SCALE;
            scale.vx = FX_BLIZZARD_ICE_SCALE;
            rot.vx = 0;
            rot.vy = frame << 5;
            rot.vz = 0;
            pos.vx = work->pos14.vx;
            pos.vy = work->pos14.vy;
            pos.vz = work->pos14.vz;
            t = frame << 8;
            ex->Hi_SetEffModelOffset(work->model8, 2);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model8);
            ex->Hi_ModifyEffModelAbr(work->model8, 0);
            ex->Hi_ModifyEffModelRGB(work->model8, 0, 0, 0);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model4);
            ex->Hi_ModifyEffModelAbr(work->model4, 1);
            ex->Hi_ModifyEffModelRGB(work->model4, (ex->math_sin(t >> 1) >> 5) - 0x80, (ex->math_sin(t >> 1) >> 5) - 0x80,
                                     (ex->math_sin(t >> 1) >> 5) - 0x80);
            ex->btlfx_drawScrollMove(((0x1000 - ex->math_cos(t >> 1)) >> 9) + frame * 5, FX_BLIZZARD_REFLECTION_VRAM_X,
                                     FX_BLIZZARD_REFLECTION_VRAM_Y, FX_BLIZZARD_REFLECTION_VRAM_W, FX_BLIZZARD_REFLECTION_VRAM_H,
                                     FX_BLIZZARD_REFLECTION_SRC_X, FX_BLIZZARD_REFLECTION_SRC_Y);
            if (frame >= FX_BLIZZARD_GLEAM_FRAMES) {
                work->state0 = FX_BLIZZARD_STATE_SHATTER;
                entry->frame = -1;
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_BLIZZARD_FREEZE_BURST), (s32)&work->pos14);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_BLIZZARD_FROST_RING), (s32)&work->pos14);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_BLIZZARD_ICE_FLARES), (s32)&work->pos14);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_BLIZZARD_ICE_FLARES_2), (s32)&work->pos14);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
                }
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, FX_BLIZZARD_FREEZE_TINT_RGB);
            }
            break;
        case FX_BLIZZARD_STATE_SHATTER:
            scale.vz = FX_BLIZZARD_ICE_SCALE;
            scale.vy = FX_BLIZZARD_ICE_SCALE;
            scale.vx = FX_BLIZZARD_ICE_SCALE;
            rot.vx = 0;
            rot.vy = 0x200;
            rot.vz = 0;
            pos.vx = work->pos14.vx;
            pos.vy = work->pos14.vy;
            pos.vz = work->pos14.vz;
            if (frame < FX_BLIZZARD_CRACK_FRAMES) {
                morph = 0x2000 - (frame << 11);
            } else {
                morph = 0x1000 - ((frame - FX_BLIZZARD_CRACK_FRAMES) << 12) / FX_BLIZZARD_SHARDS_FRAMES;
            }
            if (morph > 0x1000) {
                ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&scale, work->model8, work->modelC, morph - 0x1000);
                ex->Hi_ModifyEffModelRGB(work->model8, -0x40, -0x40, -0x40);
            } else {
                ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&scale, work->modelC, work->model10, morph);
                ex->Hi_ModifyEffModelRGB(work->modelC, ((morph - 0x800) >> 5) - 0x80, ((morph - 0x800) >> 5) - 0x80,
                                         ((morph - 0x800) >> 5) - 0x80);
            }
            if (frame == FX_BLIZZARD_SHATTER_WAVE_FRAME) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_BLIZZARD_SHATTER_WAVE), (s32)&work->pos14);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
                }
            }
            if (frame >= FX_BLIZZARD_SHATTER_FRAMES) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        work->frame24++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_006/nonmatchings/fx_006", fx_006_headerPtr);
