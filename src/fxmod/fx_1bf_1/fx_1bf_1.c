#include "fxmod/fx_1bf_1.h"

s32 func_801EC704(SVECTOR *v, SVECTOR *rot) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EF2D8);
    s32 a = ex->btlfx_rand();

    v->pad = 0;
    v->vy = 0;
    v->vx = (ex->math_cos(a) * D_801EFDD8) >> 12;
    v->vz = (ex->math_sin(a) * D_801EFDD8) >> 12;
    rot->vy = a;
    return 0;
}

INCLUDE_ASM("asm/us/fx_1bf_1/nonmatchings/fx_1bf_1", func_801EC7C8);

INCLUDE_ASM("asm/us/fx_1bf_1/nonmatchings/fx_1bf_1", fx_1bf_1_op80);

INCLUDE_ASM("asm/us/fx_1bf_1/nonmatchings/fx_1bf_1", func_801EE6BC);

void func_801EE968(Unk80100C78Entry *entry, s32 file, s32 ot, Fx1bf1Trail *path, Fx1bf1Trail *trail, s32 n, s32 *noise,
                   s32 i, s32 type, s32 alpha, s32 pad) {
    SVECTOR *pt = path->unk4;
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EF2D8);
    SVECTOR mid;

    pt += i;
    ex->btlfx_lerpVec((s16 *)(pt + 1), (s16 *)pt, 0x4000, (s16 *)&mid);
    func_801EE6BC(entry, file, ot, trail, n, noise, &mid, pt, 0, 0x20, ex->btlfx_rand(), 0x33, type, alpha, pad, 0);
}

INCLUDE_ASM("asm/us/fx_1bf_1/nonmatchings/fx_1bf_1", func_801EEA80);

INCLUDE_ASM("asm/us/fx_1bf_1/nonmatchings/fx_1bf_1", fx_1bf_1_op83);

INCLUDE_RODATA("asm/us/fx_1bf_1/nonmatchings/fx_1bf_1", fx_1bf_1_headerPtr);
