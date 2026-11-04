#include "fxmod/fx_09f.h"

s32 fx_09f_op80(s32 mode, Fx09fWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;
    s32 frame;

    if (mode == 0) {
        *args = 0x14;
    } else if (mode == 1) {
        fx_09f_env = env;
        work->state0 = 0;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->posC);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos4);
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_PYRO_FLAMES), (s32)&work->pos4);
        if (emitter != NULL) {
            emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x10);
        }
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_PYRO_SPARK_ARCS), (s32)&work->pos4);
        if (emitter != NULL) {
            emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x10);
        }
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_PYRO_COLOR_PUFFS), (s32)&work->pos4);
        if (emitter != NULL) {
            emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x10);
        }
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_PYRO_FLAMES_2), (s32)&work->posC);
        if (emitter != NULL) {
            emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x10);
        }
        ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_PYRO_ORANGE_RING), (s32)&work->pos4);
    } else {
        frame = entry->frame;
        switch (work->state0) {
        case 0:
            if (frame == 8) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_PYRO_SMOKE), (s32)&work->pos4);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
                }
            }
            if (frame >= 0x3C) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_09f/nonmatchings/fx_09f", fx_09f_headerPtr);
