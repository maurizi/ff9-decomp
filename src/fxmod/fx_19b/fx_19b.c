#include "fxmod/fx_19b.h"

s32 fx_19b_op80(s32 mode, Fx19bWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0x10;
    } else if (mode == 1) {
        D_801E793C = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk8);
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->unk8);
        work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->unk8);
        if (work->unk0 != NULL) {
            work->unk0->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
        }
        if (work->unk4 != NULL) {
            work->unk4->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
        }
    } else {
        return entry->frame >= 0;
    }
    return 0;
}

s32 fx_19b_op81(s32 mode, SVECTOR *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 8;
    } else if (mode == 1) {
        D_801E793C = env;
    } else {
        frame = entry->frame;
        ex->btlfx_getUnitPos(0x10, (s16 *)work);
        env->camera->points[0].vx = work->vx;
        env->camera->points[0].vy = work->vy;
        env->camera->points[0].vz = work->vz;
        return frame >= 7;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_19b/nonmatchings/fx_19b", fx_19b_headerPtr);
