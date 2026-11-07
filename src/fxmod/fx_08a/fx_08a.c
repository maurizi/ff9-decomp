#include "fxmod/fx_08a.h"

s32 fx_08a_op80(s32 mode, Fx08aWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx08aWork);
    } else if (mode == 1) {
        fx_08a_env = env;
        work->state0 = FX_AUTO_LIFE_STATE_WINGS;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos4);
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_AUTO_LIFE_FLASH_RINGS), (s32)&work->pos4);
        if (emitter != NULL) {
            emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
        }
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_AUTO_LIFE_SPIRAL_TRAILS), (s32)&work->pos4);
        if (emitter != NULL) {
            emitter->otZ = 0x20;
            emitter->trails = (s32)fx_08a_trails;
            emitter->trailSlots = FX_AUTO_LIFE_SPIRAL_TRAILS_TRAIL_SLOTS;
            emitter->trailStride = FX_AUTO_LIFE_SPIRAL_TRAILS_TRAIL_STRIDE;
        }
    } else {
        frame = entry->frame;
        switch (work->state0) {
        case FX_AUTO_LIFE_STATE_WINGS:
            if (frame == FX_AUTO_LIFE_WINGS_FRAME) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_AUTO_LIFE_GOLD_WINGS_2), (s32)&work->pos4);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_AUTO_LIFE_GOLD_WINGS), (s32)&work->pos4);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
                }
            }
            if (frame >= FX_AUTO_LIFE_FRAMES) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_08a/nonmatchings/fx_08a", fx_08a_headerPtr);
