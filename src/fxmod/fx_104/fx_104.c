#include "fxmod/fx_104.h"

s32 fx_104_op80(s32 mode, Fx104Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    MATRIX nearMat;
    MATRIX farMat;

    if (mode == 0) {
        *args = 0x20;
    } else if (mode == 1) {
        fx_104_env = env;
        ex->btlfx_getUnitBoneMatrix(0x10, 0x12, (BtlBone *)&nearMat);
        ex->btlfx_matrixPointAhead(&nearMat, 0xC8, (s32)&work->pos8);
        ex->btlfx_getUnitBoneMatrix(0x10, 0x12, (BtlBone *)&farMat);
        ex->btlfx_matrixPointAhead(&farMat, 0x320, (s32)&work->pos10);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_EVERYONES_GRUDGE_PINK_RINGS), (s32)&work->pos8);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
        }
    } else {
        return entry->frame >= 0x28;
    }
    return 0;
}

s32 fx_104_op81(s32 mode, Fx104Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *args = 0x14;
    } else if (mode == 1) {
        work->posC.vx = env->targets->centerX;
        work->posC.vy = env->targets->centerY;
        work->posC.vz = env->targets->centerZ;
        fx_104_env = env;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_EVERYONES_GRUDGE_SWIRL_TRAIL), (s32)&work->posC);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_EVERYONES_GRUDGE_SMOKE), (s32)&work->posC);
        work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_EVERYONES_GRUDGE_MAGIC_CIRCLE), (s32)&work->posC);
        if (work->emit0 != NULL) {
            work->emit0->trails = (s32)fx_104_op81Trails;
            work->emit0->trailSlots = 0x10;
            work->emit0->trailStride = 0x60;
        }
    } else {
        return entry->frame >= 0x28;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_104/nonmatchings/fx_104", fx_104_headerPtr);
