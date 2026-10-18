#include "fxmod/fx_1be.h"

s32 func_801E7704(s32 curve, s32 n, s32 t, SVECTOR *from, SVECTOR *to, SVECTOR *out) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E8FD8);
    s32 k = 0x20000000 / (n * n);
    s32 r = 0;

    switch (curve) {
    case 0:
        r = ((0x10000000 / n) * t) >> 16;
        break;
    case 1:
        r = 0x1000 - ((k * (n - t) * (n - t)) >> 17);
        break;
    case 2:
        r = (k * t * t) >> 17;
        break;
    }
    return ex->btlfx_lerpVec((s16 *)from, (s16 *)to, 0x1000 - r, (s16 *)out);
}

s32 func_801E7840(s32 curve, s32 n, s32 t, s32 from, s32 to) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E8FD8);
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

INCLUDE_ASM("asm/us/fx_1be/nonmatchings/fx_1be", fx_1be_op80);

INCLUDE_ASM("asm/us/fx_1be/nonmatchings/fx_1be", func_801E89A8);

INCLUDE_ASM("asm/us/fx_1be/nonmatchings/fx_1be", func_801E8E10);

INCLUDE_RODATA("asm/us/fx_1be/nonmatchings/fx_1be", fx_1be_headerPtr);
