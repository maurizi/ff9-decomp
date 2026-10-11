/* STATUS: far, 47 rows (was 51, 57, 89) (mostly register naming). ovl_3a func_801F459C (0x1AC, name-entry cursor). Shape and constants right; the original keeps unk3 in a0 and the quotient in v1 (col computed in place), ours swaps them; the side-menu block uses v0/v1 the other way round. Permuter (20 min) did not improve it. Pass 4: five do { } while (0) wraps from build/dwgreedy.py take it to 57 rows; they probably stand in for a different statement shape (e.g. a helper inline or temporaries), so treat them as hints, not as the answer. Pass 15: 57 -> 51 with the group x offset as an inline (`grp_x(grp)` = grp * 72 + 0x3E; written flat, fold reassociates 0x3E onto r->x and the lhu moves up). The original masks col (andi) right after the % 15 and issues the / 5 multu before the y store; statement order alone (40 orders, no wraps) never gets below 80, so the wraps still stand in for something. Pass 15b: 51 -> 47 with the remainders spelled out (`col = unk3 - row * 15`, `sub = col - grp * 5`, no wraps in that block). The original's order in that block is col, the /5 multu, then row's andi and the y store, i.e. RTL order col, grp, row, y, sub, x; but every no-wrap statement order of those six (120 variants with the other blocks' wraps on/off) scores 78+. */
#include "ovl_3a.h"

extern s32 D_80077DEC;

static inline s32 grp_x(u8 c) {
    return c * 72 + 0x3E;
}
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
        do {
            D_80077DE8.unk2 = D_801F4BBC.unk1 >= 2 ? y + 0x15 : y + 0xD;
        } while (0);
    } else {
        r = &D_801F4BBC.unk134[4];
        if (D_801F4BBC.unk2 != 0) {
            row = D_801F4BBC.unk3 / 15;
            col = D_801F4BBC.unk3 - row * 15;
            D_80077DE8.unk2 = r->y + row * 21 + 0x26;
            grp = col / 5;
            sub = col - grp * 5;
            D_80077DE8.unk0 = r->x + grp_x(grp) + sub * 13 - 0x17;
        } else {
            row = D_801F4BBC.unk3 / 15;
            col = D_801F4BBC.unk3 % 15;
            do {
                D_80077DE8.unk0 = r->x + col * 14 + 0x27;
            } while (0);
            D_80077DE8.unk2 = r->y + row * 21 + 0x26;
        }
    }
    D_80077DEC = 3;
    func_800355E4();
}
