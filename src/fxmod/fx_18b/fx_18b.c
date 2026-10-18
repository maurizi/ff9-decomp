#include "fxmod/fx_18b.h"

INCLUDE_ASM("asm/us/fx_18b/nonmatchings/fx_18b", func_801E7704);

void func_801E7EB8(s32 r, s32 i, s32 ang, s32 arg3, s16 *out) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E827C);
    s32 s = (ex->math_sin((i << 11) >> 4) * D_801E8284) >> 12;

    r += (s * ex->math_sin(D_801E8280 + (i << 10))) >> 12;
    out[0] = (ex->math_cos(ang) * r) >> 12;
    out[1] = (ex->math_sin(ang) * r) >> 12;
}

INCLUDE_ASM("asm/us/fx_18b/nonmatchings/fx_18b", fx_18b_op80);

INCLUDE_RODATA("asm/us/fx_18b/nonmatchings/fx_18b", fx_18b_headerPtr);
