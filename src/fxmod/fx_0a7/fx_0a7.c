#include "fxmod/fx_0a7.h"

s32 fx_0a7_op80(s32 mode, Fx0a7Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx0a7Work);
    } else if (mode == 1) {
        fx_0a7_env = env;
        ex->btlfx_getUnitBodyPos(BTLFX_UNIT_CASTER, 0, (s16 *)&work->pos18);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos20);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos28);
        ex->btlfx_getUnitArgBonePos(BTLFX_UNIT_CASTER, 0, (u16 *)&work->pos30);
        ex->btlfx_aimAngles((s16 *)&work->pos30, (s16 *)&work->pos20, (s16 *)&work->aim48);
        ex->btlfx_aimAngles((s16 *)&work->pos20, (s16 *)&work->pos30, (s16 *)&work->aim50);
        ex->btlfx_pointAhead((s16 *)&work->pos30, (s16 *)&work->aim48, FX_WATER_GUN_TRAIL_REACH, (s16 *)&work->pos38);
        ex->btlfx_pointAhead((s16 *)&work->pos30, (s16 *)&work->aim48, FX_WATER_GUN_SPRAY_REACH, (s16 *)&work->pos40);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_WATER_GUN_WATER_TRAIL_2), (s32)&work->pos38);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_WATER_GUN_SPRAY_ARC), (s32)&work->pos40);
        if (work->emit0 != NULL) {
            work->emit0->trails = (s32)fx_0a7_trails;
            work->emit0->trailSlots = FX_WATER_GUN_WATER_TRAIL_2_TRAIL_SLOTS;
            work->emit0->trailStride = FX_WATER_GUN_WATER_TRAIL_2_TRAIL_STRIDE;
        }
        if (work->emit4 != NULL) {
            work->emit4->trails = (s32)fx_0a7_trails2;
            work->emit4->trailSlots = FX_WATER_GUN_SPRAY_ARC_TRAIL_SLOTS;
            work->emit4->trailStride = FX_WATER_GUN_SPRAY_ARC_TRAIL_STRIDE;
        }
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(BTLFX_UNIT_CASTER, 0x20);
        }
        if (work->emit4 != NULL) {
            work->emit4->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
        }
    } else {
        frame = entry->frame;
        switch (frame) {
        case FX_WATER_GUN_HIT_FRAME:
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_WATER_GUN_WATER_TRAIL), (s32)&work->pos20);
            work->emit10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_WATER_GUN_SPRAY_ARC_2), (s32)&work->pos20);
            if (work->emitC != NULL) {
                work->emitC->trails = (s32)fx_0a7_trails3;
                work->emitC->trailSlots = FX_WATER_GUN_WATER_TRAIL_TRAIL_SLOTS;
                work->emitC->trailStride = FX_WATER_GUN_WATER_TRAIL_TRAIL_STRIDE;
            }
            if (work->emitC != NULL) {
                work->emitC->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
            if (work->emit10 != NULL) {
                work->emit10->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
            break;
        case FX_WATER_GUN_BUBBLES_FRAME:
            work->emit14 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_WATER_GUN_BUBBLES), (s32)&work->pos28);
            break;
        }
        if (work->emit0 != NULL) {
            if (work->emit0->state == BTLFX_EMIT_DONE) {
                work->emit0 = NULL;
            }
            if (work->emit0 != NULL) {
                work->emit0->rot.vx = work->aim50.vx;
                work->emit0->rot.vy = work->aim50.vy;
                work->emit0->rot.vz = work->aim50.vz;
            }
        }
        if (work->emit4 != NULL) {
            if (work->emit4->state == BTLFX_EMIT_DONE) {
                work->emit4 = NULL;
            }
            if (work->emit4 != NULL) {
                work->emit4->rot.vx = work->aim48.vx;
                work->emit4->rot.vy = work->aim48.vy;
                work->emit4->rot.vz = work->aim48.vz;
            }
        }
        if (frame >= FX_WATER_GUN_HIT_FRAME) {
            if (work->emitC != NULL) {
                if (work->emitC->state == BTLFX_EMIT_DONE) {
                    work->emitC = NULL;
                }
                if (work->emitC != NULL) {
                    work->emitC->rot.vx = work->aim48.vx;
                    work->emitC->rot.vy = work->aim48.vy;
                    work->emitC->rot.vz = work->aim48.vz;
                }
            }
            if (work->emit10 != NULL) {
                if (work->emit10->state == BTLFX_EMIT_DONE) {
                    work->emit10 = NULL;
                }
                if (work->emit10 != NULL) {
                    work->emit10->rot.vx = work->aim48.vx;
                    work->emit10->rot.vy = work->aim48.vy;
                    work->emit10->rot.vz = work->aim48.vz;
                }
            }
        }
        return frame >= FX_WATER_GUN_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0a7/nonmatchings/fx_0a7", fx_0a7_headerPtr);
