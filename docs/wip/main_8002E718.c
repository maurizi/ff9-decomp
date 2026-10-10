/* 23 rows differ: m->unk5 lands in a1 here (a3 in the original), which forces the x/y argument temps through v1. The otz param copy (move s3,a1) is also scheduled earlier than in the original. */
#include "ff9.h"

void func_8002E718(RECT *r, s32 otz, MenuList *m) {
    s32 blink = ((u32)D_80077150 >> 4) & 1;
    u8 more = m->unk5 < m->unk2 - m->unk3;

    func_800275DC(r->x + r->w - 6, r->y + 5, otz, 0xB8, m->unk5 != 0 ? blink : 1);
    func_800275DC(r->x + r->w - 6, r->y + r->h - 14, otz, 0xB9, more ? blink : 1);
}
