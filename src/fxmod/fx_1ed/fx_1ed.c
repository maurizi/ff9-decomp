#include "fxmod/fx_1ed.h"

INCLUDE_ASM("asm/us/fx_1ed/nonmatchings/fx_1ed", func_801E7704);

INCLUDE_ASM("asm/us/fx_1ed/nonmatchings/fx_1ed", func_801E7738);

INCLUDE_ASM("asm/us/fx_1ed/nonmatchings/fx_1ed", func_801E7788);

INCLUDE_ASM("asm/us/fx_1ed/nonmatchings/fx_1ed", func_801E792C);

INCLUDE_ASM("asm/us/fx_1ed/nonmatchings/fx_1ed", fx_1ed_op80);

s32 fx_1ed_op81(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 i;

    if (mode == 0) {
        *arg4 = 0;
    } else if (mode == 1) {
        D_801E8910 = env;
        for (i = 0; i < env->unkC->unk24; i++) {
            ex->btlfx_getUnitBodyPos(i, 0, (s16 *)&D_801EA7E0[i]);
            ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&D_801EA7E0[i]);
        }
    } else {
        return entry->unk0 >= 0x30;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1ed/nonmatchings/fx_1ed", fx_1ed_headerPtr);
