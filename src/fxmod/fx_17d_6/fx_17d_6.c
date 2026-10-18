#include "fxmod/fx_17d_6.h"

INCLUDE_ASM("asm/us/fx_17d_6/nonmatchings/fx_17d_6", fx_17d_6_op80);

INCLUDE_ASM("asm/us/fx_17d_6/nonmatchings/fx_17d_6", func_801E8B10);

void func_801E8DBC(Unk80100C78Entry *entry, s32 file, s32 ot, Fx17d6Trail *path, Fx17d6Trail *trail, s32 n, s32 *noise,
                   s32 i, s32 type, s32 alpha, s32 pad) {
    SVECTOR *pt = path->unk4;
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E93B4);
    SVECTOR mid;

    pt += i;
    ex->btlfx_lerpVec((s16 *)(pt + 1), (s16 *)pt, 0x4000, (s16 *)&mid);
    func_801E8B10(entry, file, ot, trail, n, noise, &mid, pt, 0, 0x20, ex->btlfx_rand(), 0x33, type, alpha, pad, 0);
}

INCLUDE_ASM("asm/us/fx_17d_6/nonmatchings/fx_17d_6", func_801E8ED4);

INCLUDE_ASM("asm/us/fx_17d_6/nonmatchings/fx_17d_6", fx_17d_6_op83);

INCLUDE_RODATA("asm/us/fx_17d_6/nonmatchings/fx_17d_6", fx_17d_6_headerPtr);
