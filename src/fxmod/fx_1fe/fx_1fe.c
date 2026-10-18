#include "fxmod/fx_1fe.h"

INCLUDE_ASM("asm/us/fx_1fe/nonmatchings/fx_1fe", func_801E7704);

void func_801E7CA0(s16 *v) {
    s32 t = (BTLFX_EXPORTS(D_801E9060)->math_sin(D_801E95EC + (v[1] << 8)) * D_801E95E8) >> 12;
    s32 x = v[0];

    v[0] = x + t;
}

INCLUDE_ASM("asm/us/fx_1fe/nonmatchings/fx_1fe", fx_1fe_op81);

s32 fx_1fe_op85(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    if (mode == 0) {
        *arg4 = 0;
    } else if (mode == 1) {
        D_801E9060 = env;
        env->unk2C->unk52 = 0;
    } else {
        return 1;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1fe/nonmatchings/fx_1fe", fx_1fe_headerPtr);
