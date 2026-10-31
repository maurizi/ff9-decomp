#include "fxmod/fx_137.h"

s32 fx_137_op80(s32 mode, Fx137Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    SVECTOR pos;
    MATRIX m;
    Pool6CEntry *p;
    s32 frame;
    s32 i;

    if (mode == 0) {
        *arg4 = 0x24;
    } else if (mode == 1) {
        fx_137_env = env;
        for (i = 0; i < env->targets->targetCount; i++) {
            ex->btlfx_getUnitBodyPos(i, 0, (s16 *)&fx_137_targetPos[i]);
        }
        work->pos18.vx = env->targets->centerX;
        work->pos18.vz = env->targets->centerZ;
        work->pos18.vy = 0;
        ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->pos8);
        ex->btlfx_aimAngles((s16 *)&work->pos8, (s16 *)&work->pos18, (s16 *)&work->rot0);
        work->emit20 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->pos8);
    } else {
        frame = entry->frame;
        if (work->emit20 != NULL) {
            if (work->emit20->state == BTLFX_EMIT_DONE) {
                work->emit20 = NULL;
            }
        }
        ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->pos10);
        if ((s16)env->targets->fxParam != 0) {
            ex->btlfx_moveEmitterTo((s32)work->emit20, (s32)&work->pos10);
        } else {
            ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->pos8);
        }
        ex->btlfx_getUnitBoneMatrix(0x10, env->targets->fxArgs[0], (BtlBone *)&m);
        ex->btlfx_matrixPointAhead(&m, 0x1000, (s32)&pos);
        ex->btlfx_aimAngles((s16 *)&work->pos10, (s16 *)&pos, (s16 *)&work->rot0);
        ex->btlfx_aimAngles((s16 *)&work->pos10, (s16 *)&work->pos18, (s16 *)&rot);
        work->rot0.vx = rot.vx;
        ex->btlfx_getUnitRot(0x10, (s16 *)&rot);
        work->rot0.vy += (((work->rot0.vy + 0x1000) & 0xFFF) - ((rot.vy + 0x1000) & 0xFFF)) * env->targets->fxArgs[1];
        if (work->emit20 != NULL) {
            work->emit20->rot.vx = work->rot0.vx;
            work->emit20->rot.vy = work->rot0.vy;
            work->emit20->rot.vz = work->rot0.vz;
        }
        if (frame == 0x10) {
            for (i = 0; i < env->targets->targetCount; i++) {
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&fx_137_targetPos[i]);
                if (p != NULL) {
                    p->otZ = ex->btlfx_addRadiusSixth(i, 0x40);
                }
            }
        }
        return frame >= 0x30;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_137/nonmatchings/fx_137", fx_137_headerPtr);
