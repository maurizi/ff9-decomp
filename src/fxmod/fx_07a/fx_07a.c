#include "fxmod/fx_07a.h"

INCLUDE_ASM("asm/us/fx_07a/nonmatchings/fx_07a", func_801E77A8);

s32 func_801E78E4(s32 curve, s32 n, s32 t, s32 from, s32 to) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E94F4);
    s32 k = 0x20000000 / (n * n);
    s32 r = 0;

    switch (curve) {
    case 0:
        r = 0x1000 - (((0x10000000 / n) * t) >> 16);
        break;
    case 1:
        r = (k * (n - t) * (n - t)) >> 17;
        break;
    case 2:
        r = 0x1000 - ((k * t * t) >> 17);
        break;
    }
    return ex->btlfx_lerp(from, to, r);
}

INCLUDE_RODATA("asm/us/fx_07a/nonmatchings/fx_07a", fx_07a_headerPtr);

INCLUDE_ASM("asm/us/fx_07a/nonmatchings/fx_07a", fx_07a_op80);

INCLUDE_ASM("asm/us/fx_07a/nonmatchings/fx_07a", func_801E8A88);
