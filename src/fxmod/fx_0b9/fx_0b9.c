#include "fxmod/fx_0b9.h"

s32 fx_0b9_op80(s32 mode, Fx0b9Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = 0x28;
    } else if (mode == 1) {
        work->pos8.vx = env->targets->centerX;
        work->pos8.vy = env->targets->centerY;
        work->pos8.vz = env->targets->centerZ;
        fx_0b9_env = env;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0xA:
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SANDSTORM_SAND_TRAIL), (s32)&work->pos8);
            if (work->emit0 != NULL) {
                work->emit0->trails = (s32)fx_0b9_trails;
                work->emit0->trailSlots = 0x82;
                work->emit0->trailStride = 0x18;
            }
            break;
        case 0x28:
            ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->pos10);
            fx_0b9_srcPos.vx = work->pos10.vx;
            fx_0b9_srcPos.vy = work->pos10.vy;
            fx_0b9_srcPos.vz = work->pos10.vz;
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SANDSTORM_SAND_TRAIL_2), (s32)&work->pos18);
            if (work->emit4 != NULL) {
                work->emit4->trails = (s32)fx_0b9_trails2;
                work->emit4->trailSlots = 0xA0;
                work->emit4->trailStride = 0x18;
            }
            ex->btlfx_aimAngles((s16 *)&work->pos10, (s16 *)&work->pos8, (s16 *)&work->rot20);
            break;
        }
        if (frame >= 0xA) {
            ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->pos18);
            if (work->emit0 != NULL) {
                if (work->emit0->state == BTLFX_EMIT_DONE) {
                    work->emit0 = NULL;
                }
                if (work->emit0 != NULL) {
                    work->emit0->target = (s32)&work->pos18;
                }
            }
        }
        if (frame >= 0x28) {
            if (work->emit4 != NULL) {
                if (work->emit4->state == BTLFX_EMIT_DONE) {
                    work->emit4 = NULL;
                }
                if (work->emit4 != NULL) {
                    work->emit4->rot.vx = work->rot20.vx;
                    work->emit4->rot.vy = work->rot20.vy;
                    work->emit4->rot.vz = work->rot20.vz;
                }
            }
        }
        return frame >= 0x64;
    }
    return 0;
}

s32 fx_0b9_op81(s32 mode, Fx0b9Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = 0x18;
    } else if (mode == 1) {
        s32 unit = *args;
        work->unit14 = unit;
        fx_0b9_env = env;
        if (unit >= env->targets->targetCount) {
            return 1;
        }
        ex->btlfx_getUnitPos(unit, (s16 *)&work->pos4);
        work->pos4.vy -= 0xC8;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SANDSTORM_SAND_CLOUD), (s32)&work->pos4);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(work->unit14, 0x80);
        }
        ex->btlfx_aimAngles((s16 *)&fx_0b9_srcPos, (s16 *)&work->pos4, (s16 *)&work->rotC);
    } else {
        frame = entry->frame;
        if (work->emit0 != NULL) {
            if (work->emit0->state == BTLFX_EMIT_DONE) {
                work->emit0 = NULL;
            }
            if (work->emit0 != NULL) {
                work->emit0->rot.vx = work->rotC.vx;
                work->emit0->rot.vy = work->rotC.vy;
                work->emit0->rot.vz = work->rotC.vz;
            }
        }
        return frame >= 0x3C;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0b9/nonmatchings/fx_0b9", fx_0b9_headerPtr);
