#include "fxmod/fx_104.h"

s32 fx_104_op80(s32 mode, Fx104Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    MATRIX m1;
    MATRIX m2;

    if (mode == 0) {
        *arg4 = 0x20;
    } else if (mode == 1) {
        D_801E79F4 = env;
        ex->btlfx_getUnitBoneMatrix(0x10, 0x12, (BtlBone *)&m1);
        ex->btlfx_matrixPointAhead(&m1, 0xC8, (s32)&work->unk8);
        ex->btlfx_getUnitBoneMatrix(0x10, 0x12, (BtlBone *)&m2);
        ex->btlfx_matrixPointAhead(&m2, 0x320, (s32)&work->unk10);
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk8);
        if (work->unk0 != NULL) {
            work->unk0->unk22 = ex->func_800E7CC4(0x10, 0x80);
        }
    } else {
        return entry->unk0 >= 0x28;
    }
    return 0;
}

s32 fx_104_op81(s32 mode, Fx104Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0x14;
    } else if (mode == 1) {
        work->unkC.vx = env->targets->centerX;
        work->unkC.vy = env->targets->centerY;
        work->unkC.vz = env->targets->centerZ;
        D_801E79F4 = env;
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 3), (s32)&work->unkC);
        work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unkC);
        work->unk8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unkC);
        if (work->unk0 != NULL) {
            work->unk0->unk14 = (s32)D_801E7A04;
            work->unk0->unk12 = 0x10;
            work->unk0->unkC = 0x60;
        }
    } else {
        return entry->unk0 >= 0x28;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_104/nonmatchings/fx_104", fx_104_headerPtr);
