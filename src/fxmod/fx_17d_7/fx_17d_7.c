#include "fxmod/fx_17d_7.h"

s32 func_801EC704(SVECTOR *v, SVECTOR *rot) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EF2DC);
    s32 a = ex->btlfx_rand();

    v->pad = 0;
    v->vy = 0;
    v->vx = (ex->math_cos(a) * D_801EFDDC) >> 12;
    v->vz = (ex->math_sin(a) * D_801EFDDC) >> 12;
    rot->vy = a;
    return 0;
}

INCLUDE_ASM("asm/us/fx_17d_7/nonmatchings/fx_17d_7", func_801EC7C8);

INCLUDE_ASM("asm/us/fx_17d_7/nonmatchings/fx_17d_7", fx_17d_7_op80);

INCLUDE_ASM("asm/us/fx_17d_7/nonmatchings/fx_17d_7", func_801EE6E4);

INCLUDE_ASM("asm/us/fx_17d_7/nonmatchings/fx_17d_7", func_801EE990);

INCLUDE_ASM("asm/us/fx_17d_7/nonmatchings/fx_17d_7", func_801EEAA8);

INCLUDE_ASM("asm/us/fx_17d_7/nonmatchings/fx_17d_7", fx_17d_7_op83);

INCLUDE_RODATA("asm/us/fx_17d_7/nonmatchings/fx_17d_7", fx_17d_7_headerPtr);
