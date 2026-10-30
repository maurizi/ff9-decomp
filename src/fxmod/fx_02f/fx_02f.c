#include "fxmod/fx_02f.h"

s32 fx_02f_op80(s32 mode, Fx02fWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;
    SVECTOR rot;
    SVECTOR v;
    MATRIX m;
    Pool6CEntry *p;

    if (mode == 0) {
        *arg4 = 0x24;
    } else if (mode == 1) {
        D_801E7AEC = env;
        for (i = 0; i < env->targets->targetCount; i++) {
            ex->btlfx_getUnitBodyPos(i, 0, (s16 *)&D_801E7AFC[i]);
        }
        work->unk18.vx = env->targets->centerX;
        work->unk18.vz = env->targets->centerZ;
        work->unk18.vy = 0;
        ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->unk8);
        ex->btlfx_aimAngles((s16 *)&work->unk8, (s16 *)&work->unk18, (s16 *)&work->unk0);
        work->unk20 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->unk8);
        if (work->unk20 != NULL) {
            work->unk20->unk14 = (s32)D_801E7B3C;
            work->unk20->unk12 = 0x40;
            work->unk20->unkC = 0x60;
        }
    } else {
        frame = entry->frame;
        if (work->unk20 != NULL && work->unk20->state == -1) {
            work->unk20 = NULL;
        }
        ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->unk10);
        if ((s16)env->targets->fxParam != 0) {
            ex->btlfx_moveEmitterTo((s32)work->unk20, (s32)&work->unk10);
        } else {
            ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->unk8);
        }
        ex->btlfx_getUnitBoneMatrix(0x10, env->targets->fxArgs[0], (BtlBone *)&m);
        ex->btlfx_matrixPointAhead(&m, 0x1000, (s32)&v);
        ex->btlfx_aimAngles((s16 *)&work->unk10, (s16 *)&v, (s16 *)&work->unk0);
        ex->btlfx_aimAngles((s16 *)&work->unk10, (s16 *)&work->unk18, (s16 *)&rot);
        work->unk0.vx = rot.vx;
        ex->btlfx_getUnitRot(0x10, (s16 *)&rot);
        work->unk0.vy += (((work->unk0.vy + 0x1000) & 0xFFF) - ((rot.vy + 0x1000) & 0xFFF)) * env->targets->fxArgs[1];
        if (work->unk20 != NULL) {
            work->unk20->rot.vx = work->unk0.vx;
            work->unk20->rot.vy = work->unk0.vy;
            work->unk20->rot.vz = work->unk0.vz;
        }
        if (frame == 0x10) {
            for (i = 0; i < env->targets->targetCount; i++) {
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&D_801E7AFC[i]);
                if (p != NULL) {
                    p->otZ = ex->btlfx_addRadiusSixth(i, 0x40);
                }
            }
        }
        return frame >= 0x30;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_02f/nonmatchings/fx_02f", fx_02f_headerPtr);
