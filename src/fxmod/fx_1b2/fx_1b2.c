#include "fxmod/fx_1b2.h"

INCLUDE_ASM("asm/us/fx_1b2/nonmatchings/fx_1b2", fx_1b2_op80);

INCLUDE_ASM("asm/us/fx_1b2/nonmatchings/fx_1b2", fx_1b2_op81);

INCLUDE_ASM("asm/us/fx_1b2/nonmatchings/fx_1b2", fx_1b2_op82);

INCLUDE_ASM("asm/us/fx_1b2/nonmatchings/fx_1b2", fx_1b2_op83);

void func_801E816C(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801E84EC)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801E81B0(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

INCLUDE_ASM("asm/us/fx_1b2/nonmatchings/fx_1b2", func_801E8208);

INCLUDE_ASM("asm/us/fx_1b2/nonmatchings/fx_1b2", func_801E8220);

INCLUDE_ASM("asm/us/fx_1b2/nonmatchings/fx_1b2", func_801E8284);

INCLUDE_ASM("asm/us/fx_1b2/nonmatchings/fx_1b2", func_801E831C);

INCLUDE_ASM("asm/us/fx_1b2/nonmatchings/fx_1b2", func_801E8388);

INCLUDE_ASM("asm/us/fx_1b2/nonmatchings/fx_1b2", func_801E840C);

INCLUDE_ASM("asm/us/fx_1b2/nonmatchings/fx_1b2", func_801E8458);

INCLUDE_RODATA("asm/us/fx_1b2/nonmatchings/fx_1b2", fx_1b2_headerPtr);
