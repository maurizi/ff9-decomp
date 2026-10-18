#include "fxmod/fx_1cb.h"

s32 fx_1cb_op80(s32 mode, Fx1cbWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0xC;
    } else if (mode == 1) {
        work->unk4.vx = env->unkC->unk38;
        work->unk4.vy = env->unkC->unk3A;
        work->unk4.vz = env->unkC->unk3C;
        D_801E7828 = env;
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk4);
        if (work->unk0 != NULL) {
            work->unk0->unk22 = 0x80;
        }
    } else {
        return entry->unk0 >= 0x28;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1cb/nonmatchings/fx_1cb", fx_1cb_headerPtr);
