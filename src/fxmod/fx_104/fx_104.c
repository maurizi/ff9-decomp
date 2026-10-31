#include "fxmod/fx_104.h"

s32 fx_104_op80(s32 mode, Fx104Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    MATRIX m1;
    MATRIX m2;

    if (mode == 0) {
        *arg4 = 0x20;
    } else if (mode == 1) {
        fx_104_env = env;
        ex->btlfx_getUnitBoneMatrix(0x10, 0x12, (BtlBone *)&m1);
        ex->btlfx_matrixPointAhead(&m1, 0xC8, (s32)&work->pos8);
        ex->btlfx_getUnitBoneMatrix(0x10, 0x12, (BtlBone *)&m2);
        ex->btlfx_matrixPointAhead(&m2, 0x320, (s32)&work->unk10);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos8);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
        }
    } else {
        return entry->frame >= 0x28;
    }
    return 0;
}

s32 fx_104_op81(s32 mode, Fx104Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0x14;
    } else if (mode == 1) {
        work->posC.vx = env->targets->centerX;
        work->posC.vy = env->targets->centerY;
        work->posC.vz = env->targets->centerZ;
        fx_104_env = env;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->posC);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->posC);
        work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->posC);
        if (work->emit0 != NULL) {
            work->emit0->trails = (s32)D_801E7A04;
            work->emit0->trailSlots = 0x10;
            work->emit0->trailStride = 0x60;
        }
    } else {
        return entry->frame >= 0x28;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_104/nonmatchings/fx_104", fx_104_headerPtr);
