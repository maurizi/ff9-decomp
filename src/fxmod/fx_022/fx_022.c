#include "fxmod/fx_022.h"

s32 fx_022_op80(s32 mode, Fx022Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x14;
    } else if (mode == 1) {
        D_801E7898 = env;
        ex->func_800DC858(0x10, 0, (u16 *)&work->unk4);
        ex->func_800DC858(0x10, 1, (u16 *)&work->unkC);
        env->unk2C->unk0[0].vx = work->unk4.vx;
        env->unk2C->unk0[0].vy = work->unk4.vy;
        env->unk2C->unk0[0].vz = work->unk4.vz;
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk4);
    } else {
        frame = entry->unk0;
        if (work->unk0 != NULL) {
            if (work->unk0->unk30 == -1) {
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

INCLUDE_RODATA("asm/us/fx_022/nonmatchings/fx_022", fx_022_headerPtr);
