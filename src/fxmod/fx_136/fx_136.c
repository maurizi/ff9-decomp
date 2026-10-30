#include "fxmod/fx_136.h"

s32 fx_136_op80(s32 mode, Fx136Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    MATRIX m;
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x14;
    } else if (mode == 1) {
        D_801E79C8 = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk4);
        ex->btlfx_getUnitBoneMatrix(0x10, 2, (BtlBone *)&m);
        ex->btlfx_matrixPointAhead(&m, 0x190, (s32)&work->unkC);
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->unk4);
    } else {
        frame = entry->frame;
        if (work->unk0 != NULL) {
            if (work->unk0->state == -1) {
                work->unk0 = NULL;
            }
            if (work->unk0 != NULL) {
                work->unk0->unk2C = (s32)&work->unkC;
            }
        }
        return frame >= 0x28;
    }
    return 0;
}

s32 fx_136_op81(s32 mode, Fx136Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x14;
    } else if (mode == 1) {
        D_801E79C8 = env;
        ex->btlfx_getUnitBonePos(0x10, 0xF, (u16 *)&work->unkC);
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->unkC);
        if (work->unk0 != NULL) {
            work->unk0->unk22 = ex->func_800E7CC4(0x10, 0x20);
        }
    } else {
        frame = entry->frame;
        ex->btlfx_getUnitRot(0x10, (s16 *)&work->unk4);
        if (work->unk0 != NULL) {
            if (work->unk0->state == -1) {
                work->unk0 = NULL;
            }
            if (work->unk0 != NULL) {
                work->unk0->unk5C.vx = work->unk4.vx;
                work->unk0->unk5C.vy = work->unk4.vy;
                work->unk0->unk5C.vz = work->unk4.vz;
            }
        }
        return frame >= 0x28;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_136/nonmatchings/fx_136", fx_136_headerPtr);
