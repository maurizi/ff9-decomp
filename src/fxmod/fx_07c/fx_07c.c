#include "fxmod/fx_07c.h"

s32 fx_07c_op80(s32 mode, Fx07cWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0xC;
    } else if (mode == 1) {
        work->unk4.vx = env->unkC->unk38;
        work->unk4.vy = env->unkC->unk3A;
        work->unk4.vz = env->unkC->unk3C;
        D_801E7818 = env;
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 6), (s32)&work->unk4);
    } else {
        return entry->unk0 >= 0x28;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_07c/nonmatchings/fx_07c", fx_07c_headerPtr);
