#include "fxmod/fx_0b1.h"

INCLUDE_ASM("asm/us/fx_0b1/nonmatchings/fx_0b1", func_801E7704);

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

INCLUDE_ASM("asm/us/fx_0b1/nonmatchings/fx_0b1", func_801E792C);

INCLUDE_ASM("asm/us/fx_0b1/nonmatchings/fx_0b1", fx_0b1_op80);

INCLUDE_ASM("asm/us/fx_0b1/nonmatchings/fx_0b1", fx_0b1_op81);

INCLUDE_RODATA("asm/us/fx_0b1/nonmatchings/fx_0b1", fx_0b1_headerPtr);
