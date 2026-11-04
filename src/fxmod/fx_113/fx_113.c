#include "fxmod/fx_113.h"

s32 fx_113_op80(s32 mode, Fx113Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *args = 0x1C;
    } else if (mode == 1) {
        work->index18 = *args;
        fx_113_env = env;
        if (work->index18 >= 3) {
            return 1;
        }
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos8);
        work->pos10.vx = work->pos8.vx;
        work->pos10.vy = work->pos8.vy;
        work->pos10.vz = work->pos8.vz;
        switch (work->index18) {
        case 0:
            work->pos10.vx += 0x64;
            work->pos10.vy -= 0x190;
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_VIRUS_COMBO_BURST), (s32)&work->pos10);
            break;
        case 1:
            work->pos10.vx -= 0x32;
            work->pos10.vy -= 0x32;
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_VIRUS_COMBO_BURST_2), (s32)&work->pos10);
            break;
        case 2:
            work->pos10.vy -= 0xC8;
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_VIRUS_COMBO_BURST_3), (s32)&work->pos10);
            break;
        }
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_VIRUS_COMBO_PUFF), (s32)&work->pos10);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
        }
        if (work->emit4 != NULL) {
            work->emit4->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
        }
        return 0;
    } else {
        return entry->frame >= 0x28;
    }
    return 0;
}

s32 fx_113_op81(s32 mode, Fx113Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *args = 0xC;
    } else if (mode == 1) {
        fx_113_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos4);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_VIRUS_COMBO_STREAK_TRAIL), (s32)&work->pos4);
        if (work->emit0 != NULL) {
            work->emit0->trails = (s32)fx_113_op81Trails;
            work->emit0->trailSlots = 0xD;
            work->emit0->trailStride = 0x50;
            if (work->emit0 != NULL) {
                work->emit0->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
        }
        return 0;
    } else {
        return entry->frame >= 0x28;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_113/nonmatchings/fx_113", fx_113_headerPtr);
