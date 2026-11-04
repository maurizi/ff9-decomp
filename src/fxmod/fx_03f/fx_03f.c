#include "fxmod/fx_03f.h"

s32 fx_03f_op80(s32 mode, Fx03fWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = 0x30;
    } else if (mode == 1) {
        fx_03f_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos20);
        work->pos18.vx = work->pos20.vx;
        work->pos18.vy = work->pos20.vy;
        work->pos18.vz = work->pos20.vz;
        work->pos28.vx = work->pos20.vx;
        work->pos28.vy = work->pos20.vy;
        work->pos28.vz = work->pos20.vz;
        work->pos18.vy -= 0xFA;
        work->pos28.vy += 0xFA;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SHELL_BARRIER_RING), (s32)&work->pos18);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SHELL_BARRIER_RING), (s32)&work->pos28);
        work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SHELL_BLUE_STREAK), (s32)&work->pos20);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
        }
        if (work->emit4 != NULL) {
            work->emit4->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
        }
        if (work->emit8 != NULL) {
            work->emit8->otZ = ex->btlfx_addRadiusSixth(0, 0xC8);
        }
    } else {
        frame = entry->frame;
        if (frame == 0xA) {
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SHELL_BARRIER_RING_2), (s32)&work->pos18);
            work->emit10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SHELL_BARRIER_RING_2), (s32)&work->pos28);
            if (work->emitC != NULL) {
                work->emitC->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
            }
            if (work->emit10 != NULL) {
                work->emit10->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
            }
        }
        if (frame == 0x14) {
            work->emit14 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SHELL_BLUE_RINGS), (s32)&work->pos20);
        }
        if (frame == 0xB) {
            ex->btlfx_stopEmitter(work->emit0);
            ex->btlfx_stopEmitter(work->emit4);
        }
        return frame >= 0x2D;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_03f/nonmatchings/fx_03f", fx_03f_headerPtr);
