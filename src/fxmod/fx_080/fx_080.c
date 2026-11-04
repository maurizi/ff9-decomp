#include "fxmod/fx_080.h"

s32 fx_080_op80(s32 mode, Fx080Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;
    s32 frame;

    if (mode == 0) {
        *args = 0xC;
    } else if (mode == 1) {
        fx_080_env = env;
        work->state0 = 0;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos4);
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DISPEL_GOLD_SEAL), (s32)&work->pos4);
        if (emitter != NULL) {
            emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
        }
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DISPEL_SEAL_DISCS), (s32)&work->pos4);
        if (emitter != NULL) {
            emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
        }
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DISPEL_FLASH_RINGS), (s32)&work->pos4);
        if (emitter != NULL) {
            emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x2F);
        }
    } else {
        frame = entry->frame;
        switch (work->state0) {
        case 0:
            if (frame == 0x18) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DISPEL_SEAL_SHATTER), (s32)&work->pos4);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
                }
            }
            if (frame > 0x30) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_080/nonmatchings/fx_080", fx_080_headerPtr);
