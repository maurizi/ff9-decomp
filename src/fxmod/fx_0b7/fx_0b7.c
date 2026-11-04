#include "fxmod/fx_0b7.h"

s32 fx_0b7_op80(s32 mode, Fx0b7Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;

    if (mode == 0) {
        *args = 8;
    } else if (mode == 1) {
        fx_0b7_env = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos0);
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_HEAVY_FAN_EMITTER), (s32)&work->pos0);
        if (emitter != NULL) {
            emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
        }
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_HEAVY_GREEN_SWIRL), (s32)&work->pos0);
        if (emitter != NULL) {
            emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
        }
    } else {
        return entry->frame >= 0x45;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0b7/nonmatchings/fx_0b7", fx_0b7_headerPtr);
