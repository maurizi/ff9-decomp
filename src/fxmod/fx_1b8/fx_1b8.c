#include "fxmod/fx_1b8.h"

s32 fx_1b8_op80(s32 mode, Fx1b8Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *args = sizeof(Fx1b8Work80);
    } else if (mode == 1) {
        fx_1b8_env = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->pos4);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DEATH_SWIRL), (s32)&work->pos4);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
        }
    } else {
        return entry->frame >= FX_DEATH_SWIRL_FRAMES;
    }
    return 0;
}

s32 fx_1b8_op81(s32 mode, Fx1b8Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx1b8Work81);
    } else if (mode == 1) {
        fx_1b8_env = env;
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->pos10);
        work->pos18.vx = work->pos10.vx;
        work->pos18.vy = work->pos10.vy;
        work->pos18.vz = work->pos10.vz;
        work->pos20.vx = work->pos10.vx;
        work->pos20.vy = work->pos10.vy;
        work->pos20.vz = work->pos10.vz;
    } else {
        frame = entry->frame;
        switch (frame) {
        case FX_DEATH_SPOUT_FRAME:
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DEATH_TEAL_GLOWS), (s32)&work->pos20);
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DEATH_WATER_SPOUT), (s32)&work->pos20);
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DEATH_WATER_WAVE), (s32)&work->pos18);
            if (work->emit4 != NULL) {
                work->emit4->otZ = ex->btlfx_addRadiusSixth(0x10, 0x20);
            }
            if (work->emit0 != NULL) {
                work->emit0->otZ = ex->btlfx_addRadiusSixth(0x10, 0x20);
            }
            break;
        case FX_DEATH_RIPPLES_FRAME:
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DEATH_RIPPLES), (s32)&work->pos18);
            break;
        }
        return frame >= FX_DEATH_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1b8/nonmatchings/fx_1b8", fx_1b8_headerPtr);
