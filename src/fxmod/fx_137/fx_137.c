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
        D_801E7AC0 = env;
        for (i = 0; i < env->targets->targetCount; i++) {
            ex->btlfx_getUnitBodyPos(i, 0, (s16 *)&D_801E7AD0[i]);
        }
        work->unk18.vx = env->targets->centerX;
        work->unk18.vz = env->targets->centerZ;
        work->unk18.vy = 0;
        ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->unk8);
        ex->btlfx_aimAngles((s16 *)&work->unk8, (s16 *)&work->unk18, (s16 *)&work->unk0);
        work->unk20 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->unk8);
    } else {
        frame = entry->frame;
        if (work->unk20 != NULL) {
            if (work->unk20->state == -1) {
                work->unk20 = NULL;
            }
        }
        ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->unk10);
        if ((s16)env->targets->unk42 != 0) {
            ex->btlfx_moveEmitterTo((s32)work->unk20, (s32)&work->unk10);
        } else {
            ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->unk8);
        }
        ex->btlfx_getUnitBoneMatrix(0x10, env->targets->unk25[0], (BtlBone *)&m);
        ex->btlfx_matrixPointAhead(&m, 0x1000, (s32)&pos);
        ex->btlfx_aimAngles((s16 *)&work->unk10, (s16 *)&pos, (s16 *)&work->unk0);
        ex->btlfx_aimAngles((s16 *)&work->unk10, (s16 *)&work->unk18, (s16 *)&rot);
        work->unk0.vx = rot.vx;
        ex->btlfx_getUnitRot(0x10, (s16 *)&rot);
        work->unk0.vy += (((work->unk0.vy + 0x1000) & 0xFFF) - ((rot.vy + 0x1000) & 0xFFF)) * env->targets->unk25[1];
        if (work->unk20 != NULL) {
            work->unk20->unk5C.vx = work->unk0.vx;
            work->unk20->unk5C.vy = work->unk0.vy;
            work->unk20->unk5C.vz = work->unk0.vz;
        }
        if (frame == 0x10) {
            for (i = 0; i < env->targets->targetCount; i++) {
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&D_801E7AD0[i]);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(i, 0x40);
                }
            }
        }
        return frame >= 0x30;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_137/nonmatchings/fx_137", fx_137_headerPtr);
