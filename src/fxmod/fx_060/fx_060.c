#include "fxmod/fx_060.h"

s32 fx_060_op80(s32 mode, Fx060Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;
    s32 frame;

    if (mode == 0) {
        *args = 0xC;
    } else if (mode == 1) {
        fx_060_env = env;
        work->stop0 = 0;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos4);
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos4);
        if (emitter != NULL) {
            emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x18);
            emitter->trails = (s32)fx_060_trails;
            emitter->trailSlots = 0x13;
            emitter->trailStride = 0x6C;
        }
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 9), (s32)&work->pos4);
        if (emitter != NULL) {
            emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
            emitter->trails = (s32)fx_060_trails2;
            emitter->trailSlots = 0xC;
            emitter->trailStride = 0x7C;
        }
    } else {
        frame = entry->frame;
        if (work->stop0 == 0) {
            if (frame == 0x10) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos4);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
                }
            }
            if (frame >= 0x31) {
                return 1;
            }
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_060/nonmatchings/fx_060", fx_060_headerPtr);
