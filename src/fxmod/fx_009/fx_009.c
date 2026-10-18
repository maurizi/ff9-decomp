#include "fxmod/fx_009.h"

s32 fx_009_op80(s32 mode, Fx009Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 4;
    } else if (mode == 1) {
        D_801E7818 = env;
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 3), (s32)&env->unkC->unk38);
        if (work->unk0 != NULL) {
            work->unk0->unk14 = (s32)D_801E7828;
            work->unk0->unk12 = 0x17;
            work->unk0->unkC = 0x70;
        }
    } else {
        return entry->unk0 >= 0x32;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_009/nonmatchings/fx_009", fx_009_headerPtr);
