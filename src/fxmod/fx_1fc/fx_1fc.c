#include "fxmod/fx_1fc.h"

INCLUDE_ASM("asm/us/fx_1fc/nonmatchings/fx_1fc", fx_1fc_op80);

INCLUDE_ASM("asm/us/fx_1fc/nonmatchings/fx_1fc", fx_1fc_op81);

INCLUDE_ASM("asm/us/fx_1fc/nonmatchings/fx_1fc", fx_1fc_op82);

INCLUDE_RODATA("asm/us/fx_1fc/nonmatchings/fx_1fc", fx_1fc_headerPtr);

INCLUDE_ASM("asm/us/fx_1fc/nonmatchings/fx_1fc", fx_1fc_op83);

INCLUDE_ASM("asm/us/fx_1fc/nonmatchings/fx_1fc", fx_1fc_op84);

INCLUDE_ASM("asm/us/fx_1fc/nonmatchings/fx_1fc", func_801E86A0);

void func_801E86B8(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801EC14C)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801E86FC(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

INCLUDE_ASM("asm/us/fx_1fc/nonmatchings/fx_1fc", func_801E8754);

INCLUDE_ASM("asm/us/fx_1fc/nonmatchings/fx_1fc", func_801E876C);

INCLUDE_ASM("asm/us/fx_1fc/nonmatchings/fx_1fc", func_801E87D0);

INCLUDE_ASM("asm/us/fx_1fc/nonmatchings/fx_1fc", func_801E87E8);

INCLUDE_ASM("asm/us/fx_1fc/nonmatchings/fx_1fc", func_801E887C);

INCLUDE_ASM("asm/us/fx_1fc/nonmatchings/fx_1fc", func_801E8894);

INCLUDE_ASM("asm/us/fx_1fc/nonmatchings/fx_1fc", func_801E88D8);

INCLUDE_ASM("asm/us/fx_1fc/nonmatchings/fx_1fc", func_801E88F0);

INCLUDE_ASM("asm/us/fx_1fc/nonmatchings/fx_1fc", func_801E8954);

INCLUDE_ASM("asm/us/fx_1fc/nonmatchings/fx_1fc", func_801E89EC);

INCLUDE_ASM("asm/us/fx_1fc/nonmatchings/fx_1fc", func_801E8AB8);

INCLUDE_ASM("asm/us/fx_1fc/nonmatchings/fx_1fc", func_801E8B04);

INCLUDE_ASM("asm/us/fx_1fc/nonmatchings/fx_1fc", func_801E8B60);
