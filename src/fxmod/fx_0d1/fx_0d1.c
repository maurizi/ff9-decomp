#include "fxmod/fx_0d1.h"

s32 fx_0d1_op80(s32 mode, Fx0d1Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x24;
    } else if (mode == 1) {
        fx_0d1_env = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->pos14);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos1C);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xD), (s32)&work->pos14);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xE), (s32)&work->pos14);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
        }
        if (work->emit4 != NULL) {
            work->emit4->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
        }
    } else {
        frame = entry->frame;
        if (frame == 0x24) {
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->pos1C);
            if (work->emit8 != NULL) {
                work->emit8->otZ = 0x100;
            }
        }
        if (frame == 0x38) {
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 9), (s32)&work->pos1C);
            work->emit10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->pos1C);
            if (work->emitC != NULL) {
                work->emitC->otZ = 0x100;
            }
            if (work->emit10 != NULL) {
                work->emit10->otZ = 0x100;
            }
        }
        return frame >= 0x50;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0d1/nonmatchings/fx_0d1", fx_0d1_headerPtr);
