#include "fxmod/fx_0b5.h"

s32 fx_0b5_op80(s32 mode, Fx0b5Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;
    s32 frame;

    if (mode == 0) {
        *args = 0x10;
    } else if (mode == 1) {
        fx_0b5_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos8);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos0);
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos0);
        if (emitter != NULL) {
            emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
            emitter->trails = (s32)fx_0b5_trails;
            emitter->trailSlots = 0x24;
            emitter->trailStride = 0x44;
        }
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos0);
        if (emitter != NULL) {
            emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
        }
    } else {
        frame = entry->frame;
        if (frame == 0xE) {
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->pos8);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
            }
        }
        if (frame > 0x28) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0b5/nonmatchings/fx_0b5", fx_0b5_headerPtr);
