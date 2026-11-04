#include "fxmod/fx_046.h"

s32 fx_046_op80(s32 mode, Fx046Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;
    s32 frame;
    s32 zWord;

    if (mode == 0) {
        *args = 0xC;
    } else if (mode == 1) {
        work->hold0 = 0;
        fx_046_env = env;
        ((s32 *)&work->pos4)[0] = ((s32 *)&env->targets->centerX)[0];
        zWord = ((s32 *)&env->targets->centerX)[1];
        work->pos4.vy = 0;
        ((s32 *)&work->pos4)[1] = zWord;
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FIRE_FLAME_BURST), (s32)&work->pos4);
        if (emitter != NULL) {
            emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
        }
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FIRE_HEAT_RING), (s32)&work->pos4);
        if (emitter != NULL) {
            emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
        }
    } else {
        frame = entry->frame;
        if (work->hold0 == 0 && frame >= 0x3C) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_046/nonmatchings/fx_046", fx_046_headerPtr);
