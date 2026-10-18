#include "fxmod/fx_180.h"

INCLUDE_ASM("asm/us/fx_180/nonmatchings/fx_180", func_801E78E0);

INCLUDE_ASM("asm/us/fx_180/nonmatchings/fx_180", func_801E79DC);

INCLUDE_ASM("asm/us/fx_180/nonmatchings/fx_180", fx_180_op80);

INCLUDE_RODATA("asm/us/fx_180/nonmatchings/fx_180", fx_180_headerPtr);

INCLUDE_ASM("asm/us/fx_180/nonmatchings/fx_180", fx_180_op81);

INCLUDE_ASM("asm/us/fx_180/nonmatchings/fx_180", fx_180_op82);

INCLUDE_ASM("asm/us/fx_180/nonmatchings/fx_180", fx_180_op83);

INCLUDE_ASM("asm/us/fx_180/nonmatchings/fx_180", func_801E8DA4);

void func_801E8DBC(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801E9258)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801E8E00(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

INCLUDE_ASM("asm/us/fx_180/nonmatchings/fx_180", func_801E8E58);

INCLUDE_ASM("asm/us/fx_180/nonmatchings/fx_180", func_801E8EF0);

INCLUDE_ASM("asm/us/fx_180/nonmatchings/fx_180", func_801E8F08);

INCLUDE_ASM("asm/us/fx_180/nonmatchings/fx_180", func_801E8F6C);

INCLUDE_ASM("asm/us/fx_180/nonmatchings/fx_180", func_801E8FD8);

INCLUDE_ASM("asm/us/fx_180/nonmatchings/fx_180", func_801E905C);

INCLUDE_ASM("asm/us/fx_180/nonmatchings/fx_180", func_801E90A8);

INCLUDE_ASM("asm/us/fx_180/nonmatchings/fx_180", func_801E90F4);
