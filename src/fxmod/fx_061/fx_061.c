#include "fxmod/fx_061.h"

s32 fx_061_op80(s32 mode, Fx061Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;
    s32 frame;
    s32 zWord;

    if (mode == 0) {
        *args = sizeof(Fx061Work);
    } else if (mode == 1) {
        work->hold0 = 0;
        fx_061_env = env;
        ((s32 *)&work->pos4)[0] = ((s32 *)&env->targets->centerX)[0];
        zWord = ((s32 *)&env->targets->centerX)[1];
        work->pos4.vy -= FX_CONFUSE_HEIGHT;
        ((s32 *)&work->pos4)[1] = zWord;
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CONFUSE_STAFF_WAVES), (s32)&work->pos4);
        if (emitter != NULL) {
            emitter->otZ = 0x20;
            emitter->trails = (s32)fx_061_trails;
            emitter->trailSlots = FX_CONFUSE_STAFF_WAVES_TRAIL_SLOTS;
            emitter->trailStride = FX_CONFUSE_STAFF_WAVES_TRAIL_STRIDE;
        }
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CONFUSE_STAFF_SWIRL), (s32)&work->pos4);
        if (emitter != NULL) {
            emitter->otZ = 0x20;
            emitter->trails = (s32)fx_061_trails2;
            emitter->trailSlots = FX_CONFUSE_STAFF_SWIRL_TRAIL_SLOTS;
            emitter->trailStride = FX_CONFUSE_STAFF_SWIRL_TRAIL_STRIDE;
        }
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CONFUSE_SWIRLS), (s32)&work->pos4);
        if (emitter != NULL) {
            emitter->otZ = 0x20;
        }
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CONFUSE_SWIRLS_2), (s32)&work->pos4);
        if (emitter != NULL) {
            emitter->otZ = 0x20;
        }
    } else {
        frame = entry->frame;
        if (work->hold0 == 0 && frame >= FX_CONFUSE_FRAMES) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_061/nonmatchings/fx_061", fx_061_headerPtr);
