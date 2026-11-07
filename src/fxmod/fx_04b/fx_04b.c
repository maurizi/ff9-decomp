#include "fxmod/fx_04b.h"

s32 fx_04b_op80(s32 mode, Fx04bWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx04bWork80);
    } else if (mode == 1) {
        work->pos8.vx = env->targets->centerX;
        work->pos8.vy = env->targets->centerY;
        fx_04b_env = env;
        work->pos8.vz = env->targets->centerZ;
    } else {
        frame = entry->frame;
        switch (frame) {
        case FX_SANDSTORM_TRAIL_FRAME:
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SANDSTORM_SAND_TRAIL), (s32)&work->pos8);
            if (work->emit0 != NULL) {
                work->emit0->trails = (s32)fx_04b_trails;
                work->emit0->trailSlots = FX_SANDSTORM_SAND_TRAIL_TRAIL_SLOTS;
                work->emit0->trailStride = FX_SANDSTORM_SAND_TRAIL_TRAIL_STRIDE;
            }
            break;
        case FX_SANDSTORM_TRAIL_2_FRAME:
            ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->pos10);
            fx_04b_casterPos.vx = work->pos10.vx;
            fx_04b_casterPos.vy = work->pos10.vy;
            fx_04b_casterPos.vz = work->pos10.vz;
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SANDSTORM_SAND_TRAIL_2), (s32)&work->pos18);
            if (work->emit4 != NULL) {
                work->emit4->trails = (s32)fx_04b_trails2;
                work->emit4->trailSlots = FX_SANDSTORM_SAND_TRAIL_2_TRAIL_SLOTS;
                work->emit4->trailStride = FX_SANDSTORM_SAND_TRAIL_2_TRAIL_STRIDE;
            }
            ex->btlfx_aimAngles((s16 *)&work->pos10, (s16 *)&work->pos8, (s16 *)&work->aim20);
            break;
        }
        if (frame >= FX_SANDSTORM_TRAIL_FRAME) {
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
        if (frame >= FX_SANDSTORM_TRAIL_2_FRAME) {
            if (work->emit4 != NULL) {
                if (work->emit4->state == BTLFX_EMIT_DONE) {
                    work->emit4 = NULL;
                }
                if (work->emit4 != NULL) {
                    work->emit4->rot.vx = work->aim20.vx;
                    work->emit4->rot.vy = work->aim20.vy;
                    work->emit4->rot.vz = work->aim20.vz;
                }
            }
        }
        return frame >= FX_SANDSTORM_FRAMES;
    }
    return 0;
}

s32 fx_04b_op81(s32 mode, Fx04bWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx04bWork81);
    } else if (mode == 1) {
        s32 target = *args;

        work->target14 = target;
        fx_04b_env = env;
        if (target >= env->targets->targetCount) {
            return 1;
        }
        ex->btlfx_getUnitPos(target, (s16 *)&work->pos4);
        work->pos4.vy -= FX_SANDSTORM_CLOUD_HEIGHT;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SANDSTORM_SAND_CLOUD), (s32)&work->pos4);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(work->target14, 0x80);
        }
        ex->btlfx_aimAngles((s16 *)&fx_04b_casterPos, (s16 *)&work->pos4, (s16 *)&work->aimC);
    } else {
        frame = entry->frame;
        if (work->emit0 != NULL) {
            if (work->emit0->state == BTLFX_EMIT_DONE) {
                work->emit0 = NULL;
            }
            if (work->emit0 != NULL) {
                work->emit0->rot.vx = work->aimC.vx;
                work->emit0->rot.vy = work->aimC.vy;
                work->emit0->rot.vz = work->aimC.vz;
            }
        }
        return frame >= FX_SANDSTORM_CLOUD_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_04b/nonmatchings/fx_04b", fx_04b_headerPtr);
