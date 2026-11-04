#include "fxmod/fx_00f.h"

s32 fx_00f_op80(s32 mode, Fx00fWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;
    s32 frame;

    if (mode == 0) {
        *args = 0xC;
    } else if (mode == 1) {
        fx_00f_env = env;
        work->hold0 = 0;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos4);
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FULL_LIFE_HALO_RINGS), (s32)&work->pos4);
        if (emitter != NULL) {
            emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
        }
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FULL_LIFE_LIGHT_TRAILS), (s32)&work->pos4);
        if (emitter != NULL) {
            emitter->otZ = 0x20;
            emitter->trails = (s32)fx_00f_trails;
            emitter->trailSlots = 0x14;
            emitter->trailStride = 0x34;
        }
    } else {
        frame = entry->frame;
        if (work->hold0 == 0) {
            if (frame == 6) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FULL_LIFE_WINGS), (s32)&work->pos4);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FULL_LIFE_DARK_RINGS), (s32)&work->pos4);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
                }
            }
            if (frame >= 0x28) {
                return 1;
            }
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_00f/nonmatchings/fx_00f", fx_00f_headerPtr);
