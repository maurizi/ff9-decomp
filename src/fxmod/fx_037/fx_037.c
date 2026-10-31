#include "fxmod/fx_037.h"

s32 fx_037_op80(s32 mode, Fx037Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x1C;
    } else if (mode == 1) {
        fx_037_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos14);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos14);
        if (work->emit0 != NULL) {
            work->emit0->trails = (s32)fx_037_trails;
            work->emit0->trailSlots = 0x15;
            work->emit0->trailStride = 0x60;
        }
    } else {
        frame = entry->frame;
        switch (frame) {
        case 8:
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->pos14);
            if (work->emit8 != NULL) {
                work->emit8->trails = (s32)fx_037_trails2;
                work->emit8->trailSlots = 0x2C;
                work->emit8->trailStride = 0x8C;
            }
            break;
        case 10:
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->pos14);
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos14);
            if (work->emit4 != NULL) {
                work->emit4->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
            }
            break;
        case 20:
            work->emit10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->pos14);
            if (work->emit10 != NULL) {
                work->emit10->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
            }
            break;
        }
        return frame >= 0x3C;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_037/nonmatchings/fx_037", fx_037_headerPtr);
