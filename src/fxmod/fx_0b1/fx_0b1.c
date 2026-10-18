#include "fxmod/fx_0b1.h"

void func_801E7704(void) {
    s32 i;
    s32 m;

    D_801E8B90 = 0;
    m = -1;
    for (i = 15; i >= 0; i--) {
        D_801E8B9C[i].unk4 = m;
    }
}

void func_801E7738(s32 start, u32 dst, u32 src, s16 *data) {
    s32 i;

    for (i = 0; i < 16; i++) {
        if (D_801E8B9C[i].unk4 == -1) {
            D_801E8B9C[i].unk4 = 0;
            D_801E8B9C[i].unk10 = data + 1;
            D_801E8B9C[i].unk0 = start;
            D_801E8B9C[i].unk8 = dst;
            D_801E8B9C[i].unkC = src;
            return;
        }
    }
}

INCLUDE_ASM("asm/us/fx_0b1/nonmatchings/fx_0b1", func_801E7788);

s32 func_801E792C(s32 curve, s32 n, s32 t, s32 from, s32 to) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E8B8C);
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

INCLUDE_ASM("asm/us/fx_0b1/nonmatchings/fx_0b1", fx_0b1_op80);

INCLUDE_ASM("asm/us/fx_0b1/nonmatchings/fx_0b1", fx_0b1_op81);

INCLUDE_RODATA("asm/us/fx_0b1/nonmatchings/fx_0b1", fx_0b1_headerPtr);
