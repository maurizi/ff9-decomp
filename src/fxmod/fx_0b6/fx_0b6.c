#include "fxmod/fx_0b6.h"

s32 fx_0b6_op80(s32 mode, Fx0b6Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;

    if (mode == 0) {
        *args = 8;
    } else if (mode == 1) {
        fx_0b6_env = env;
        ex->btlfx_getUnitArgBonePos(0, 0, (u16 *)&work->pos0);
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SCORCH_FIREBALL), (s32)&work->pos0);
        if (emitter != NULL) {
            emitter->otZ = ex->btlfx_addRadiusSixth(0, 0xC8);
        }
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SCORCH_FLAME_STREAKS), (s32)&work->pos0);
        if (emitter != NULL) {
            emitter->otZ = ex->btlfx_addRadiusSixth(0, 0xC8);
        }
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SCORCH_HEAT_RING), (s32)&work->pos0);
        if (emitter != NULL) {
            emitter->otZ = ex->btlfx_addRadiusSixth(0, 0xC8);
        }
    } else {
        return entry->frame >= 0x42;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0b6/nonmatchings/fx_0b6", fx_0b6_headerPtr);
