#include "fxmod/fx_0fb_0.h"

INCLUDE_ASM("asm/us/fx_0fb_0/nonmatchings/fx_0fb_0", func_801E7720);

INCLUDE_ASM("asm/us/fx_0fb_0/nonmatchings/fx_0fb_0", func_801E7B90);

INCLUDE_ASM("asm/us/fx_0fb_0/nonmatchings/fx_0fb_0", func_801E809C);

INCLUDE_ASM("asm/us/fx_0fb_0/nonmatchings/fx_0fb_0", func_801E8540);

INCLUDE_RODATA("asm/us/fx_0fb_0/nonmatchings/fx_0fb_0", fx_0fb_0_headerPtr);

INCLUDE_ASM("asm/us/fx_0fb_0/nonmatchings/fx_0fb_0", fx_0fb_0_op80);

INCLUDE_ASM("asm/us/fx_0fb_0/nonmatchings/fx_0fb_0", fx_0fb_0_op81);

s32 fx_0fb_0_op87(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    SVECTOR v;

    if (mode == 0) {
        *arg4 = 4;
    } else if (mode == 1) {
        v.vx = env->unkC->unk38;
        v.vy = env->unkC->unk3A;
        v.vz = env->unkC->unk3C;
        v.vy -= 0x230;
        env->unk2C->unk0[2].vx = v.vx;
        env->unk2C->unk0[2].vy = v.vy;
        env->unk2C->unk0[2].vz = v.vz;
        D_801EB3AC = env;
        return 1;
    } else {
        return 0;
    }
    return 0;
}
