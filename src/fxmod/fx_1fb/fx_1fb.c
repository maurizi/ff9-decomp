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
        return entry->frame >= 0x40;
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
        fx_1fb_fadeTimers[0] = 0;
        fx_1fb_fadeTimers[1] = 0;
        fx_1fb_env = env;
        for (i = 0; i < env->targets->targetCount; i++) {
            ex->btlfx_getUnitBodyPos(i, 0, (s16 *)&fx_1fb_emitPos1[i]);
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_WHITE_STREAK), (s32)&fx_1fb_emitPos1[i]);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(i, 0x200);
                emitter->trailSlots = 0x1E;
                emitter->trails = (s32)fx_1fb_op81Trails0[i];
                emitter->trailStride = 0x9C;
            }
        }
    } else {
        frame = entry->frame;
        switch (frame) {
        case 16:
            for (i = 0; i < env->targets->targetCount; i++) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_WHITE_STREAK_2), (s32)&fx_1fb_emitPos1[i]);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(i, 0x200);
                    emitter->trailSlots = 0x1E;
                    emitter->trails = (s32)fx_1fb_op81Trails1[i];
                    emitter->trailStride = 0x9C;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_GOLD_GLOW), (s32)&fx_1fb_emitPos1[i]);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x200);
                }
            }
            break;
        case 24:
            fx_1fb_fadeTimers[0] = 0x10;
            break;
        case 40:
            fx_1fb_fadeTimers[1] = 0x10;
            break;
        }
        if (fx_1fb_fadeTimers[0] != 0) {
            fx_1fb_fadeTimers[0]--;
            for (i = 0; i < env->targets->targetCount; i++) {
                ex->btlfx_fadeUnitById(i, fx_1fb_fadeTimers[0] * 8);
            }
        }
        if (fx_1fb_fadeTimers[1] != 0) {
            fx_1fb_fadeTimers[1]--;
            for (i = 0; i < env->targets->targetCount; i++) {
                ex->btlfx_fadeUnitById(i, 0x80 - fx_1fb_fadeTimers[1] * 8);
            }
        }
        return frame >= 0x38;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1fb/nonmatchings/fx_1fb", fx_1fb_headerPtr);
