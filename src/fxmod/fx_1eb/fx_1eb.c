#include "fxmod/fx_1eb.h"

INCLUDE_ASM("asm/us/fx_1eb/nonmatchings/fx_1eb", fx_1eb_op80);

INCLUDE_ASM("asm/us/fx_1eb/nonmatchings/fx_1eb", fx_1eb_op81);

INCLUDE_RODATA("asm/us/fx_1eb/nonmatchings/fx_1eb", fx_1eb_headerPtr);

INCLUDE_ASM("asm/us/fx_1eb/nonmatchings/fx_1eb", fx_1eb_op82);

INCLUDE_ASM("asm/us/fx_1eb/nonmatchings/fx_1eb", fx_1eb_op83);

void func_801E806C(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801E8268)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801E80B0(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

INCLUDE_ASM("asm/us/fx_1eb/nonmatchings/fx_1eb", func_801E8108);

INCLUDE_ASM("asm/us/fx_1eb/nonmatchings/fx_1eb", func_801E8120);

INCLUDE_ASM("asm/us/fx_1eb/nonmatchings/fx_1eb", func_801E81B4);

INCLUDE_ASM("asm/us/fx_1eb/nonmatchings/fx_1eb", func_801E81CC);
