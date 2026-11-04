#include "fxmod/fx_0a8.h"

s32 fx_0a8_op80(s32 mode, Fx0a8Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = 0x18;
    } else if (mode == 1) {
        fx_0a8_env = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->pos8);
        ex->btlfx_getUnitRot(0x10, (s16 *)&work->rot10);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_REIS_WIND_LIGHT_STREAK), (s32)&work->pos8);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_REIS_WIND_SPARKLE_CIRCLE), (s32)&work->pos8);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
        }
        if (work->emit4 != NULL) {
            work->emit4->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
        }
    } else {
        frame = entry->frame;
        if (work->emit0 != NULL) {
            if (work->emit0->state == BTLFX_EMIT_DONE) {
                work->emit0 = NULL;
            }
            if (work->emit0 != NULL) {
                work->emit0->rot.vx = work->rot10.vx;
                work->emit0->rot.vy = work->rot10.vy;
                work->emit0->rot.vz = work->rot10.vz;
            }
        }
        if (work->emit4 != NULL) {
            if (work->emit4->state == BTLFX_EMIT_DONE) {
                work->emit4 = NULL;
            }
            if (work->emit4 != NULL) {
                work->emit4->rot.vx = work->rot10.vx;
                work->emit4->rot.vy = work->rot10.vy;
                work->emit4->rot.vz = work->rot10.vz;
            }
        }
        return frame >= 0x28;
    }
    return 0;
}

s32 fx_0a8_op81(s32 mode, Fx0a8Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = 0x28;
    } else if (mode == 1) {
        work->pos20.vx = env->targets->centerX;
        work->pos20.vy = env->targets->centerY;
        work->pos20.vz = env->targets->centerZ;
        fx_0a8_env = env;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_REIS_WIND_VIOLET_SPARKLES), (s32)&work->pos20);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_REIS_WIND_WIND_TRAIL), (s32)&work->pos20);
        work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_REIS_WIND_GREEN_RINGS), (s32)&work->pos20);
        if (work->emit4 != NULL) {
            work->emit4->trails = (s32)fx_0a8_trails;
            work->emit4->trailSlots = 6;
            work->emit4->trailStride = 0x60;
        }
    } else {
        frame = entry->frame;
        if (frame == 8) {
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_REIS_WIND_SPARKLES), (s32)&work->pos20);
            work->emit10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_REIS_WIND_GREEN_WISP), (s32)&work->pos20);
            work->emit18 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_REIS_WIND_GREEN_CRESCENT), (s32)&work->pos20);
        }
        return frame >= 0x28;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0a8/nonmatchings/fx_0a8", fx_0a8_headerPtr);
