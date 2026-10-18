#include "fxmod/fx_17d_2.h"

s32 func_801E7704(s32 mode, Fx17d2Elem *e, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, Unk800DA1B8 *pool,
                  Fx17d2Work *work) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR sc;
    s32 t;
    s32 s;

    if (mode == 1) {
        e->unk0 = 0;
    } else {
    t = frame << 6;
    s = (((ex->math_sin(t >> 2) >> 1) + 0x1000) * e->unk4) >> 12;
    sc.vz = 0x400;
    sc.vx = sc.vy = s;
    e->unk10.vz -= e->unk2;
    e->unk8.vy += 0x3E;
    pos.vx = e->unk8.vx;
    pos.vy = e->unk8.vy;
    pos.vz = e->unk8.vz;
    ex->Hi_DrawEffModel(&e->unk10, &pos, (s32)&sc, work->unk74);
    ex->Hi_ModifyEffModelRGB(work->unk74, (((ex->math_cos(t >> 1) >> 2) + 0xC00) >> 5) - 0x80,
                             (((ex->math_cos(t >> 1) >> 2) + 0xC00) >> 5) - 0x80,
                             (((ex->math_cos(t >> 1) >> 2) + 0xC00) >> 5) - 0x80);
    if (frame >= 0x40) {
        return 1;
    }
    }
    return 0;
}

INCLUDE_ASM("asm/us/fx_17d_2/nonmatchings/fx_17d_2", fx_17d_2_op80);

INCLUDE_RODATA("asm/us/fx_17d_2/nonmatchings/fx_17d_2", fx_17d_2_headerPtr);
