#include "fxmod/fx_02f.h"

s32 fx_02f_op80(s32 mode, Fx02fWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;
    SVECTOR rot;
    SVECTOR aimPoint;
    MATRIX m;
    Pool6CEntry *emitter;

    if (mode == 0) {
        *args = sizeof(Fx02fWork);
    } else if (mode == 1) {
        fx_02f_env = env;
        for (i = 0; i < env->targets->targetCount; i++) {
            ex->btlfx_getUnitBodyPos(i, 0, (s16 *)&fx_02f_targetPos[i]);
        }
        work->pos18.vx = env->targets->centerX;
        work->pos18.vz = env->targets->centerZ;
        work->pos18.vy = 0;
        ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->pos8);
        ex->btlfx_aimAngles((s16 *)&work->pos8, (s16 *)&work->pos18, (s16 *)&work->aim0);
        work->emit20 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FLAME_FLAME_STREAK), (s32)&work->pos8);
        if (work->emit20 != NULL) {
            work->emit20->trails = (s32)fx_02f_trails;
            work->emit20->trailSlots = FX_FLAME_FLAME_STREAK_TRAIL_SLOTS;
            work->emit20->trailStride = FX_FLAME_FLAME_STREAK_TRAIL_STRIDE;
        }
    } else {
        frame = entry->frame;
        if (work->emit20 != NULL && work->emit20->state == BTLFX_EMIT_DONE) {
            work->emit20 = NULL;
        }
        ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->pos10);
        if ((s16)env->targets->fxParam != 0) {
            ex->btlfx_moveEmitterTo((s32)work->emit20, (s32)&work->pos10);
        } else {
            ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->pos8);
        }
        ex->btlfx_getUnitBoneMatrix(0x10, env->targets->fxArgs[0], (BtlBone *)&m);
        ex->btlfx_matrixPointAhead(&m, 0x1000, (s32)&aimPoint);
        ex->btlfx_aimAngles((s16 *)&work->pos10, (s16 *)&aimPoint, (s16 *)&work->aim0);
        ex->btlfx_aimAngles((s16 *)&work->pos10, (s16 *)&work->pos18, (s16 *)&rot);
        work->aim0.vx = rot.vx;
        ex->btlfx_getUnitRot(0x10, (s16 *)&rot);
        work->aim0.vy += (((work->aim0.vy + 0x1000) & 0xFFF) - ((rot.vy + 0x1000) & 0xFFF)) * env->targets->fxArgs[1];
        if (work->emit20 != NULL) {
            work->emit20->rot.vx = work->aim0.vx;
            work->emit20->rot.vy = work->aim0.vy;
            work->emit20->rot.vz = work->aim0.vz;
        }
        if (frame == FX_FLAME_BURST_FRAME) {
            for (i = 0; i < env->targets->targetCount; i++) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FLAME_FLAME_BURST), (s32)&fx_02f_targetPos[i]);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(i, 0x40);
                }
            }
        }
        return frame >= FX_FLAME_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_02f/nonmatchings/fx_02f", fx_02f_headerPtr);
