/* STATUS: far, 89 rows (mostly register naming). ovl_3a func_801F459C (0x1AC, name-entry cursor). Shape and constants right; the original keeps unk3 in a0 and the quotient in v1 (col computed in place), ours swaps them; the side-menu block uses v0/v1 the other way round. Permuter (20 min) did not improve it. */
#include "ovl_3a.h"

extern s32 D_80077DEC;

void func_801F459C(void) {
    RECT *r;
    s32 y;
    u8 row;
    u8 col;
    u8 grp;
    u8 sub;

    if (D_801F4BBC.unk0 == 0) {
        D_80077DE8.unk0 = D_801F4BBC.unk134[2].x - 0xE;
        y = D_801F4BBC.unk134[2].y + D_801F4BBC.unk1 * 16;
        D_80077DE8.unk2 = D_801F4BBC.unk1 >= 2 ? y + 0x15 : y + 0xD;
    } else {
        r = &D_801F4BBC.unk134[4];
        if (D_801F4BBC.unk2 != 0) {
            col = D_801F4BBC.unk3 % 15;
            row = D_801F4BBC.unk3 / 15;
            D_80077DE8.unk2 = r->y + row * 21 + 0x26;
            grp = col / 5;
            sub = col % 5;
            D_80077DE8.unk0 = r->x + (grp * 72 + 0x3E) + sub * 13 - 0x17;
        } else {
            row = D_801F4BBC.unk3 / 15;
            col = D_801F4BBC.unk3 % 15;
            D_80077DE8.unk0 = r->x + col * 14 + 0x27;
            D_80077DE8.unk2 = r->y + row * 21 + 0x26;
        }
    }
    D_80077DEC = 3;
    func_800355E4();
}
