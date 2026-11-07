#include "fxmod/fx_1fb.h"

s32 fx_1fb_op80(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *args = 0;
    } else if (mode == 1) {
        fx_1fb_emitPos0.vx = 0;
        fx_1fb_emitPos0.vy = 0;
        fx_1fb_emitPos0.vz = -0x618;
        fx_1fb_env = env;
        ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_ORBIT_GLOW_2), (s32)&fx_1fb_emitPos0);
    } else {
        return entry->frame >= FX_CARBUNCLE_GLOW_FRAMES;
    }
    return 0;
}

s32 fx_1fb_op81(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;
    s32 frame;
    s32 i;

    if (mode == 0) {
        *args = 0;
    } else if (mode == 1) {
        fx_1fb_fadeTimers[FX_CARBUNCLE_T_FADE_OUT] = 0;
        fx_1fb_fadeTimers[FX_CARBUNCLE_T_FADE_IN] = 0;
        fx_1fb_env = env;
        for (i = 0; i < env->targets->targetCount; i++) {
            ex->btlfx_getUnitBodyPos(i, 0, (s16 *)&fx_1fb_emitPos1[i]);
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_WHITE_STREAK), (s32)&fx_1fb_emitPos1[i]);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(i, 0x200);
                emitter->trailSlots = FX_CARBUNCLE_STREAK_TRAIL_SLOTS;
                emitter->trails = (s32)fx_1fb_op81Trails0[i];
                emitter->trailStride = FX_CARBUNCLE_STREAK_TRAIL_STRIDE;
            }
        }
    } else {
        frame = entry->frame;
        switch (frame) {
        case FX_CARBUNCLE_STREAK_2_FRAME:
            for (i = 0; i < env->targets->targetCount; i++) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_WHITE_STREAK_2), (s32)&fx_1fb_emitPos1[i]);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(i, 0x200);
                    emitter->trailSlots = FX_CARBUNCLE_STREAK_TRAIL_SLOTS;
                    emitter->trails = (s32)fx_1fb_op81Trails1[i];
                    emitter->trailStride = FX_CARBUNCLE_STREAK_TRAIL_STRIDE;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_GOLD_GLOW), (s32)&fx_1fb_emitPos1[i]);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x200);
                }
            }
            break;
        case FX_CARBUNCLE_FADE_OUT_FRAME:
            fx_1fb_fadeTimers[FX_CARBUNCLE_T_FADE_OUT] = FX_CARBUNCLE_FADE_FRAMES;
            break;
        case FX_CARBUNCLE_FADE_IN_FRAME:
            fx_1fb_fadeTimers[FX_CARBUNCLE_T_FADE_IN] = FX_CARBUNCLE_FADE_FRAMES;
            break;
        }
        if (fx_1fb_fadeTimers[FX_CARBUNCLE_T_FADE_OUT] != 0) {
            fx_1fb_fadeTimers[FX_CARBUNCLE_T_FADE_OUT]--;
            for (i = 0; i < env->targets->targetCount; i++) {
                ex->btlfx_fadeUnitById(i, fx_1fb_fadeTimers[FX_CARBUNCLE_T_FADE_OUT] * 8);
            }
        }
        if (fx_1fb_fadeTimers[FX_CARBUNCLE_T_FADE_IN] != 0) {
            fx_1fb_fadeTimers[FX_CARBUNCLE_T_FADE_IN]--;
            for (i = 0; i < env->targets->targetCount; i++) {
                ex->btlfx_fadeUnitById(i, 0x80 - fx_1fb_fadeTimers[FX_CARBUNCLE_T_FADE_IN] * 8);
            }
        }
        return frame >= FX_CARBUNCLE_OP81_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1fb/nonmatchings/fx_1fb", fx_1fb_headerPtr);
