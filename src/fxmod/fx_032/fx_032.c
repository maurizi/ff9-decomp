#include "fxmod/fx_032.h"

s32 fx_032_op80(s32 mode, Fx032Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    MATRIX m0;
    MATRIX m1;
    MATRIX m2;
    MATRIX m3;
    MATRIX m4;
    MATRIX m5;
    MATRIX m6;
    MATRIX m7;

    if (mode == 0) {
        *arg4 = 0x58;
    } else if (mode == 1) {
        D_801E7C60 = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk0);
        ex->func_800DC8AC(0x10, env->unkC->unk25[1], (BtlBone *)&m0);
        ex->func_800CFE88(&m0, (s16)env->unkC->unk42, (s32)env->unk2C);
    } else {
        frame = entry->unk0;
        if (frame == 4) {
            ex->func_800DC8AC(0x10, env->unkC->unk25[1], (BtlBone *)&m0);
            ex->func_800CFE88(&m0, (s16)env->unkC->unk42, (s32)&work->unk8);
            work->unk48 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk8);
            ex->func_800DC8AC(0x10, env->unkC->unk25[1], (BtlBone *)&m1);
            ex->func_800CFE88(&m1, (s16)env->unkC->unk42, (s32)&work->unk18);
            work->unk4C = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk18);
            ex->func_800DC8AC(0x10, env->unkC->unk25[0], (BtlBone *)&m2);
            ex->func_800CFE88(&m2, (s16)env->unkC->unk42, (s32)&work->unk28);
            work->unk50 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk28);
            ex->func_800DC8AC(0x10, env->unkC->unk25[0], (BtlBone *)&m3);
            ex->func_800CFE88(&m3, (s16)env->unkC->unk42, (s32)&work->unk38);
            work->unk54 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 3), (s32)&work->unk38);
        }
        if (frame >= 5) {
            if (work->unk48 != NULL) {
                if (work->unk48->unk30 == -1) {
                    work->unk48 = NULL;
                }
                if (work->unk48 != NULL) {
                    ex->func_800DC8AC(0x10, env->unkC->unk25[1], (BtlBone *)&m4);
                    ex->func_800CFE88(&m4, (s16)env->unkC->unk42, (s32)&work->unk10);
                    ex->func_800E3D20((s32)work->unk48, (s32)&work->unk10);
                }
            }
            if (work->unk4C != NULL) {
                if (work->unk4C->unk30 == -1) {
                    work->unk4C = NULL;
                }
                if (work->unk4C != NULL) {
                    ex->func_800DC8AC(0x10, env->unkC->unk25[1], (BtlBone *)&m5);
                    ex->func_800CFE88(&m5, (s16)env->unkC->unk42, (s32)&work->unk20);
                    ex->func_800E3D20((s32)work->unk4C, (s32)&work->unk20);
                }
            }
            if (work->unk50 != NULL) {
                if (work->unk50->unk30 == -1) {
                    work->unk50 = NULL;
                }
                if (work->unk50 != NULL) {
                    ex->func_800DC8AC(0x10, env->unkC->unk25[0], (BtlBone *)&m6);
                    ex->func_800CFE88(&m6, (s16)env->unkC->unk42, (s32)&work->unk30);
                    ex->func_800E3D20((s32)work->unk50, (s32)&work->unk30);
                }
            }
            if (work->unk54 != NULL) {
                if (work->unk54->unk30 == -1) {
                    work->unk54 = NULL;
                }
                if (work->unk54 != NULL) {
                    ex->func_800DC8AC(0x10, env->unkC->unk25[0], (BtlBone *)&m7);
                    ex->func_800CFE88(&m7, (s16)env->unkC->unk42, (s32)&work->unk40);
                    ex->func_800E3D20((s32)work->unk54, (s32)&work->unk40);
                }
            }
        }
        if (frame == 0x10) {
            ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk0);
            ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unk0);
        }
        return frame >= 0x28;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_032/nonmatchings/fx_032", fx_032_headerPtr);
