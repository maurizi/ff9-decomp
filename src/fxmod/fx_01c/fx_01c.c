#include "fxmod/fx_01c.h"

s32 fx_01c_op80(s32 mode, Fx01cWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = 0x2C;
    } else if (mode == 1) {
        fx_01c_env = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->pos14);
        if (env->targets->targetCount == 1) {
            ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos1C);
        } else {
            work->pos1C.vx = env->targets->centerX;
            work->pos1C.vy = env->targets->centerY;
            work->pos1C.vz = env->targets->centerZ;
        }
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_AQUA_BREATH_BUBBLE), (s32)&work->pos14);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_AQUA_BREATH_BLUE_RING), (s32)&work->pos14);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
        }
        if (work->emit4 != NULL) {
            work->emit4->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
        }
        ex->btlfx_aimAngles((s16 *)&work->pos14, (s16 *)&work->pos1C, (s16 *)&work->aim24);
    } else {
        frame = entry->frame;
        switch (frame) {
        case 30:
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_AQUA_BREATH_WAVE_RING), (s32)&work->pos14);
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_AQUA_BREATH_BUBBLES), (s32)&work->pos14);
            if (work->emitC != NULL) {
                work->emitC->trails = (s32)fx_01c_trails;
                work->emitC->trailSlots = 8;
                work->emitC->trailStride = 0x90;
            }
            break;
        case 50:
            work->emit10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_AQUA_BREATH_WATER_DROPS), (s32)&work->pos1C);
            if (work->emit10 != NULL) {
                work->emit10->otZ = 0x80;
            }
            break;
        }
        if (work->emit0 != NULL) {
            if (work->emit0->state == BTLFX_EMIT_DONE) {
                work->emit0 = NULL;
            }
            if (work->emit0 != NULL) {
                work->emit0->rot.vx = work->aim24.vx;
                work->emit0->rot.vy = work->aim24.vy;
                work->emit0->rot.vz = work->aim24.vz;
            }
        }
        if (frame >= 0x1E) {
            if (work->emit8 != NULL) {
                if (work->emit8->state == BTLFX_EMIT_DONE) {
                    work->emit8 = NULL;
                }
                if (work->emit8 != NULL) {
                    work->emit8->rot.vx = work->aim24.vx;
                    work->emit8->rot.vy = work->aim24.vy;
                    work->emit8->rot.vz = work->aim24.vz;
                }
            }
            if (work->emitC != NULL) {
                if (work->emitC->state == BTLFX_EMIT_DONE) {
                    work->emitC = NULL;
                }
                if (work->emitC != NULL) {
                    work->emitC->rot.vx = work->aim24.vx;
                    work->emitC->rot.vy = work->aim24.vy;
                    work->emitC->rot.vz = work->aim24.vz;
                }
            }
        }
        return frame >= 0x50;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_01c/nonmatchings/fx_01c", fx_01c_headerPtr);
