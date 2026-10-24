#include "fxmod/fx_19c.h"

s32 fx_19c_op80(s32 mode, Fx19cWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x10;
    } else if (mode == 1) {
        D_801E7970 = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk4);
        work->unkC = 0x80;
    } else {
        frame = entry->unk0;
        env->unk28 = 0x80;
        ex->btlfx_drawSprite((u8 *)ex->btlfx_getFileData(entry->unk8, 2), &work->unk4, frame, 1, -1);
        if (frame == 3) {
            work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk4);
            if (work->unk0 != NULL) {
                work->unk0->unk22 = ex->func_800E7CC4(0, 0x80);
            }
        }
        if (frame == 7) {
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, work->unkC, work->unkC, work->unkC);
        }
        return frame >= 0x32;
    }
    return 0;
}

s32 fx_19c_op81(s32 mode, SVECTOR *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 8;
    } else if (mode == 1) {
        D_801E7970 = env;
    } else {
        frame = entry->unk0;
        ex->btlfx_getUnitPos(0x10, (s16 *)work);
        env->camera->points[0].vx = work->vx;
        env->camera->points[0].vy = work->vy;
        env->camera->points[0].vz = work->vz;
        return frame >= 7;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_19c/nonmatchings/fx_19c", fx_19c_headerPtr);
