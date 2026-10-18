#include "fxmod/fx_183.h"

INCLUDE_ASM("asm/us/fx_183/nonmatchings/fx_183", fx_183_op80);

INCLUDE_ASM("asm/us/fx_183/nonmatchings/fx_183", fx_183_op81);

INCLUDE_ASM("asm/us/fx_183/nonmatchings/fx_183", fx_183_op82);

INCLUDE_ASM("asm/us/fx_183/nonmatchings/fx_183", fx_183_op83);

void func_801E8140(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801E82E8)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801E8184(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

INCLUDE_ASM("asm/us/fx_183/nonmatchings/fx_183", func_801E81DC);

INCLUDE_ASM("asm/us/fx_183/nonmatchings/fx_183", func_801E81F4);

INCLUDE_RODATA("asm/us/fx_183/nonmatchings/fx_183", fx_183_headerPtr);
