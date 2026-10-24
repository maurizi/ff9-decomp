#include "fxmod/fx_1f5.h"

s32 fx_1f5_op80(s32 mode, Fx1f5Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0xC;
    } else if (mode == 1) {
        work->unk4.vy = -0x1B58;
        work->unk4.vx = 0;
        work->unk4.vz = 0x7D0;
        env->camera->points[0].vx = work->unk4.vx;
        env->camera->points[0].vy = work->unk4.vy;
        env->camera->points[0].vz = work->unk4.vz;
        D_801E79E8 = env;
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->unk4);
    } else {
        return entry->frame >= 0x32;
    }
    return 0;
}

s32 fx_1f5_op81(s32 mode, Fx1f5Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0xC;
    } else if (mode == 1) {
        D_801E79E8 = env;
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->unk4);
        if (work->unk0 != NULL) {
            work->unk0->unk14 = (s32)D_801E79F8;
            work->unk0->unk12 = 0x1E;
            work->unk0->unkC = 0x68;
        }
    } else {
        return entry->frame >= 0x28;
    }
    return 0;
}

s32 fx_1f5_op82(s32 mode, Fx1f5Work82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0x10;
    } else if (mode == 1) {
        s32 unit = *arg4;
        work->unkC = unit;
        D_801E79E8 = env;
        if (unit >= env->targets->targetCount) {
            return 1;
        }
        ex->btlfx_getUnitPos(unit, (s16 *)&work->unk4);
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->unk4);
        if (work->unk0 != NULL) {
            work->unk0->unk22 = ex->func_800E7CC4(work->unkC, 0x20);
        }
    } else {
        return entry->frame >= 0x28;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1f5/nonmatchings/fx_1f5", fx_1f5_headerPtr);
