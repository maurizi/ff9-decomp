#include "fxmod/fx_0d0.h"

s32 fx_0d0_op80(s32 mode, Fx0d0Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x20;
    } else if (mode == 1) {
        fx_0d0_env = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->pos10);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos18);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xC), (s32)&work->pos10);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xD), (s32)&work->pos10);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
        }
        if (work->emit4 != NULL) {
            work->emit4->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
        }
    } else {
        frame = entry->frame;
        if (frame == 0x24) {
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->pos18);
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->pos18);
            if (work->emitC != NULL) {
                work->emitC->trails = (s32)fx_0d0_trails;
                work->emitC->trailSlots = 8;
                work->emitC->trailStride = 0x60;
            }
            if (work->emit8 != NULL) {
                work->emit8->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
            }
            if (work->emitC != NULL) {
                work->emitC->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
            }
        }
        return frame >= 0x46;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0d0/nonmatchings/fx_0d0", fx_0d0_headerPtr);
