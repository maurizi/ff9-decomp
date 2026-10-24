#include "fxmod/fx_0bc.h"

s32 fx_0bc_op80(s32 mode, Fx0bcWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0xC;
    } else if (mode == 1) {
        D_801E78DC = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk4);
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk4);
        if (work->unk0 != NULL) {
            work->unk0->unk22 = ex->func_800E7CC4(0, 0x80);
        }
    } else {
        return entry->unk0 >= 0x32;
    }
    return 0;
}

s32 fx_0bc_op81(s32 mode, Fx0bcWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 8;
    } else if (mode == 1) {
        D_801E78DC = env;
    } else {
        frame = entry->unk0;
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->unk0);
        env->camera->points[0].vx = work->unk0.vx;
        env->camera->points[0].vy = work->unk0.vy;
        env->camera->points[0].vz = work->unk0.vz;
        return frame >= 0xC;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0bc/nonmatchings/fx_0bc", fx_0bc_headerPtr);
