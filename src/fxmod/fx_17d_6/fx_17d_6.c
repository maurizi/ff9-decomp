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

s32 func_801E8ED4(s32 mode, Fx17d6Bolt *b, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 t;
    s32 a;
    s32 c;
    s32 k;
    s32 i;
    Fx17d6Trail *tr;
    Fx17d6Trail *tr2;
    s32 u;

    if (mode != 1 && b->unk11 == 0) {
        t = (frame << 12) / 6;
        c = ex->math_cos(t >> 2);
        k = frame < 3;
        a = 0x1000 - t;
        i = b->unk10;
        u = b->unk15;
        tr = (Fx17d6Trail *)&D_801EB124[i];
        tr2 = (Fx17d6Trail *)&D_801EB474[i];
        b->unk14 += 3;
        func_801E8B10(entry, 8, 0, tr, 0x8C, b->unk18 + frame * k, &b->unk8, &b->unk0, 0, 0xDC, b->unk14 * 16, 0xF0,
                      frame, c, a * u / 1500, 0);
        func_801E8DBC(entry, 7, 0, tr, tr2, 0x2C, b->unk18, b->unk12, frame, c, (a * u) >> 12);
        if (frame >= 6) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM("asm/us/fx_17d_6/nonmatchings/fx_17d_6", fx_17d_6_op83);

INCLUDE_RODATA("asm/us/fx_17d_6/nonmatchings/fx_17d_6", fx_17d_6_headerPtr);
