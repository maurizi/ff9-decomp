#include "fxmod/fx_1f5.h"

s32 fx_1f5_op80(s32 mode, Fx1f5Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0xC;
    } else if (mode == 1) {
        work->pos4.vy = -0x1B58;
        work->pos4.vx = 0;
        work->pos4.vz = 0x7D0;
        env->camera->points[0].vx = work->pos4.vx;
        env->camera->points[0].vy = work->pos4.vy;
        env->camera->points[0].vz = work->pos4.vz;
        fx_1f5_env = env;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos4);
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
        fx_1f5_env = env;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->pos4);
        if (work->emit0 != NULL) {
            work->emit0->trails = (s32)D_801E79F8;
            work->emit0->trailSlots = 0x1E;
            work->emit0->trailStride = 0x68;
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
        fx_1f5_env = env;
        if (unit >= env->targets->targetCount) {
            return 1;
        }
        ex->btlfx_getUnitPos(unit, (s16 *)&work->pos4);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->pos4);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(work->unkC, 0x20);
        }
    } else {
        return entry->frame >= 0x28;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1f5/nonmatchings/fx_1f5", fx_1f5_headerPtr);
