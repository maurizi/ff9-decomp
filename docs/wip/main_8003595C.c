/* 12 rows differ, registers only: the raw products and the D_80077170 hi land in t9/t5/t6 here instead of t5/t9/t7. Needed: all AnimCell loads before setlen (the sb blocks them otherwise), dx/dy temps so -0x80 is not reassociated, and the tpage bits in a temp. Pass 15: D_8007716C/D_80077170 as one struct { ot; prim; } makes it much worse (the base address is materialised). */
#include "ff9.h"

void func_8003595C(s32 x, s32 y, s32 otz, Unk80035B90 *unused, AnimCell *c) {
    u32 t = c->unk0.unk4;
    s32 sw;
    s32 dx, dy;
    u32 tp;
    s32 sh;
    PolyFT4Words *p;

    sw = (c->unk0.unk8 * c->unk10) >> 12;
    dx = ((c->unk0.unk8 - sw) >> 1) - 0x80;
    x = x + c->unk0.unk2 + dx;
    sh = (c->unk0.unkA * c->unk12) >> 12;
    dy = ((c->unk0.unkA - sh) >> 1) - 0x80;
    y = y + c->unk0.unk3 + dy;
    setlen(D_80077170, 9);
    p = (PolyFT4Words *)D_80077170;
    p->rgbc = 0x2C808080;
    p->xy0 = (x & 0xFFFF) | (y << 16);
    p->xy1 = ((x + sw) & 0xFFFF) | (y << 16);
    p->xy2 = (x & 0xFFFF) | ((y + sh) << 16);
    p->xy3 = ((x + sw) & 0xFFFF) | ((y + sh) << 16);
    p->uv0clut = c->unk0.unk0 | (c->unk0.unk1 << 8) | ((u16)c->unk0.unk4 << 16);
    tp = ((t >> 16) & 0x1F) | (((t >> 21) & 3) << 5) | (((t >> 23) & 3) << 7);
    p->uv1tpage = (c->unk0.unk0 + c->unk0.unk8 - 1) | (c->unk0.unk1 << 8) | (tp << 16);
    p->uv2 = c->unk0.unk0 | ((c->unk0.unk1 + c->unk0.unkA - 1) << 8);
    p->uv3 = (c->unk0.unk0 + c->unk0.unk8 - 1) | ((c->unk0.unk1 + c->unk0.unkA - 1) << 8);
    addPrim(&D_8007716C[otz], p);
    D_80077170 = (u8 *)(p + 1);
}
