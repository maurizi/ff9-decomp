/* 16 rows differ, all in the first call: m->unk5 lands in a1 here (a3 in the original) so y goes through v1, and the otz param copy (move s3,a1) is scheduled earlier than in the original. The second call matches once y and x are computed into locals (y first). The permuter got to 7 rows by putting the 6 of both x expressions in a variable (new_var = 6; r->x + r->w - new_var), which moves m->unk5 to a0 (still not a3). Pass 13: all of these score worse (24-51 rows): the first call's x/y in locals (either order), the ternary split into an if/else around two calls, and a local for m->unk5. */
#include "ff9.h"

void func_8002E718(RECT *r, s32 otz, MenuList *m) {
    s32 blink = ((u32)D_80077150 >> 4) & 1;
    u8 more = m->unk5 < m->unk2 - m->unk3;
    s32 x;
    s32 y;

    func_800275DC(r->x + r->w - 6, r->y + 5, otz, 0xB8, m->unk5 != 0 ? blink : 1);
    y = r->y + r->h - 14;
    x = r->x + r->w - 6;
    func_800275DC(x, y, otz, 0xB9, more ? blink : 1);
}
