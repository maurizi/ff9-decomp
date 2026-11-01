#include "fxmod/fx_11f.h"

s32 fx_11f_op80(s32 mode, Fx11fWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = 0x18;
    } else if (mode == 1) {
        fx_11f_env = env;
        env->camera->paused = 0;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos10);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos10);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
        }
    } else {
        frame = entry->frame;
        if (frame == 0xC) {
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos10);
            if (work->emit4 != NULL) {
                work->emit4->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
        }
        if (frame == 0x12) {
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->pos10);
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->pos10);
            if (work->emit8 != NULL) {
                work->emit8->trails = (s32)fx_11f_op80Trails;
                work->emit8->trailSlots = 0x14;
                work->emit8->trailStride = 0x90;
                if (work->emit8 != NULL) {
                    work->emit8->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
                }
            }
            if (work->emitC != NULL) {
                work->emitC->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
        }
        return frame >= 0x32;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_11f/nonmatchings/fx_11f", fx_11f_headerPtr);
