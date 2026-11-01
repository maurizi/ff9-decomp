#include "fxmod/fx_061.h"

s32 fx_061_op80(s32 mode, Fx061Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;
    s32 frame;
    s32 zWord;

    if (mode == 0) {
        *args = 0xC;
    } else if (mode == 1) {
        work->stop0 = 0;
        fx_061_env = env;
        ((s32 *)&work->pos4)[0] = ((s32 *)&env->targets->centerX)[0];
        zWord = ((s32 *)&env->targets->centerX)[1];
        work->pos4.vy -= 0x180;
        ((s32 *)&work->pos4)[1] = zWord;
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->pos4);
        if (emitter != NULL) {
            emitter->otZ = 0x20;
            emitter->trails = (s32)fx_061_trails;
            emitter->trailSlots = 0x13;
            emitter->trailStride = 0x6C;
        }
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos4);
        if (emitter != NULL) {
            emitter->otZ = 0x20;
            emitter->trails = (s32)fx_061_trails2;
            emitter->trailSlots = 0xC;
            emitter->trailStride = 0x7C;
        }
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos4);
        if (emitter != NULL) {
            emitter->otZ = 0x20;
        }
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->pos4);
        if (emitter != NULL) {
            emitter->otZ = 0x20;
        }
    } else {
        frame = entry->frame;
        if (work->stop0 == 0 && frame >= 0x31) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_061/nonmatchings/fx_061", fx_061_headerPtr);
