#include "fxmod/fx_1b7.h"

INCLUDE_ASM("asm/us/fx_1b7/nonmatchings/fx_1b7", func_801E7704);

s32 func_801E774C(s32 model) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E87E0);
    Fx1b7Split *s = D_801E87E8;
    SVECTOR (*f)[3] = s->unk28;
    s32 n;
    s32 i;
    s32 k;

    n = (u32)ex->Hi_GetSplitMdlVertex(model, -1) / 24;
    ex->Hi_SplitMdlVertex(model, -1, (s32)f);
    k = s->unk0;
    s->unk2C[k] = f;
    s->unk4[k] = n;
    s->unk28 = f + n;
    s->unk2 += n;
    s->unk0++;
    for (i = 0; i < n; i++) {
        (*f)[0].pad = 0;
        f++;
    }
    return s->unk2;
}

INCLUDE_ASM("asm/us/fx_1b7/nonmatchings/fx_1b7", func_801E7830);

INCLUDE_ASM("asm/us/fx_1b7/nonmatchings/fx_1b7", func_801E7868);

INCLUDE_ASM("asm/us/fx_1b7/nonmatchings/fx_1b7", func_801E7ACC);

INCLUDE_ASM("asm/us/fx_1b7/nonmatchings/fx_1b7", fx_1b7_op80);

s32 fx_1b7_op83(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0;
    } else if (mode == 1) {
        D_801E87E0 = env;
        ex->func_800E8274(0x10);
    } else {
        return 1;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1b7/nonmatchings/fx_1b7", fx_1b7_headerPtr);
