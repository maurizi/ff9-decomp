#include "fxmod/fx_0ae.h"

s32 fx_0ae_op80(s32 mode, Fx0aeWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    BtlBone bone;
    Pool6CEntry *p;
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x28;
    } else if (mode == 1) {
        work->unk0 = 0;
        D_801E79BC = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk10);
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk18);
        ex->btlfx_getUnitBoneMatrix(0x10, env->targets->fxArgs[0], &bone);
        ex->btlfx_matrixPointAhead((MATRIX *)&bone, (s16)env->targets->unk42, (s32)&work->unk8);
        ex->btlfx_aimAngles((s16 *)&work->unk8, (s16 *)&work->unk10, (s16 *)&work->unk20);
        work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->unk8);
        if (work->unk4 != NULL) {
            work->unk4->unk14 = (s32)D_801E79CC;
            work->unk4->unk12 = 0x2D;
            work->unk4->unkC = 0x2C;
        }
        if (work->unk4 != NULL) {
            work->unk4->unk22 = 0x20;
        }
    } else {
        frame = entry->frame;
        switch (work->unk0) {
        case 0:
            ex->btlfx_getUnitBoneMatrix(0x10, env->targets->fxArgs[0], &bone);
            ex->btlfx_matrixPointAhead((MATRIX *)&bone, (s16)env->targets->unk42, (s32)&work->unk8);
            if (work->unk4 != NULL) {
                if (work->unk4->state == -1) {
                    work->unk4 = NULL;
                }
                if (work->unk4 != NULL) {
                    work->unk4->unk5C.vx = work->unk20.vx;
                    work->unk4->unk5C.vy = work->unk20.vy;
                    work->unk4->unk5C.vz = work->unk20.vz;
                }
            }
            if (frame == 0xC) {
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->unk18);
                if (p != NULL) {
                    p->unk22 = ex->btlfx_addRadiusSixth(0, 0x20);
                }
            }
            if (frame >= 0x3C) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0ae/nonmatchings/fx_0ae", fx_0ae_headerPtr);
