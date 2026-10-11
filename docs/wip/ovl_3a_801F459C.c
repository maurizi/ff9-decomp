/* STATUS: far, 44 rows (pass 17; was 45, 47, 51, 57, 89), no wraps (mostly register naming). ovl_3a func_801F459C (0x1AC, name-entry cursor). Shape and constants right; the original keeps unk3 in a0 and the quotient in v1 (col computed in place), ours swaps them; the side-menu block uses v0/v1 the other way round. Permuter (20 min) did not improve it. Pass 4: five do { } while (0) wraps from build/dwgreedy.py take it to 57 rows; they probably stand in for a different statement shape (e.g. a helper inline or temporaries), so treat them as hints, not as the answer. Pass 15: 57 -> 51 with the group x offset as an inline (`grp_x(grp)` = grp * 72 + 0x3E; written flat, fold reassociates 0x3E onto r->x and the lhu moves up). The original masks col (andi) right after the % 15 and issues the / 5 multu before the y store; statement order alone (40 orders, no wraps) never gets below 80, so the wraps still stand in for something. Pass 15b: 51 -> 47 with the remainders spelled out (`col = unk3 - row * 15`, `sub = col - grp * 5`, no wraps in that block). The original's order in that block is col, the /5 multu, then row's andi and the y store, i.e. RTL order col, grp, row, y, sub, x; but every no-wrap statement order of those six (120 variants with the other blocks' wraps on/off) scores 78+.
 Pass 16: 47 -> 45 with the first block's unk2 store as `if (unk1 >= 2) unk2 = y + 0x15; else unk2 = y + 0xD;`
 (no do-while): the block's registers now match (hi of D_80077DE8 in v1, &D_80077DE8 in a1, unk1 in v0), but
 the two stores are not cross-jumped as in the original (there the 0x15 arm jumps to the 0xD arm's store, which
 reorg puts in the `j` delay slot); the ternary keeps one store but allocates like before (51). Sibling
 functions ovl_37 F13F8 / ovl_33 EF644 point at duplicated statement bodies merged by jump2.
 Pass 17: block 1 matches exactly with a block-local `Unk80077DE8 *h` assigned after the unk0 store and a ternary
 (`h->unk2 = D_801F4BBC.unk1 >= 2 ? y + 0x15 : y + 0xD;`): the 0x15 arm jumps to the shared store like the original.
 The block-3 do-while is gone (block-local h after the unk0 store, as in block 1). 44 rows, all in blocks 2/3:
 the original keeps unk3 in a0 and q = unk3 / 15 in v1, masks row (andi a3,v1) right after the srl and before
 q * 15, so q stays live; ours sinks the row mask after the last q use and ties row to q. In block 2 the
 original computes col in place (unk3's register), sub from the raw col / 5 quotient (= `sub = col % 5`), and
 masks grp right after its srl. Block-local u8 row/col/grp/sub with `col = unk3 % 15; ... sub = col % 5;` get
 col in place and the sub/grp shape (build/w/f45b.c in the pass-17 worktree) but swap unk3/q in both blocks
 (75 rows); all 140 orders of block 2's statements (build/w/topo.py) give 50+ with that form. Permuter 14 min
 on f45b: only `if (h) A else A` duplicates (45 rows).
 */
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
        Unk80077DE8 *h;
        D_80077DE8.unk0 = D_801F4BBC.unk134[2].x - 0xE;
        h = &D_80077DE8;
        y = D_801F4BBC.unk134[2].y + D_801F4BBC.unk1 * 16;
        h->unk2 = D_801F4BBC.unk1 >= 2 ? y + 0x15 : y + 0xD;
    } else {
        r = &D_801F4BBC.unk134[4];
        if (D_801F4BBC.unk2 != 0) {
            Unk80077DE8 *h;
            row = D_801F4BBC.unk3 / 15;
            col = D_801F4BBC.unk3 - row * 15;
            h = &D_80077DE8;
            h->unk2 = r->y + row * 21 + 0x26;
            grp = col / 5;
            sub = col - grp * 5;
            D_80077DE8.unk0 = r->x + grp_x(grp) + sub * 13 - 0x17;
        } else {
            Unk80077DE8 *h;
            row = D_801F4BBC.unk3 / 15;
            col = D_801F4BBC.unk3 % 15;
            D_80077DE8.unk0 = r->x + col * 14 + 0x27;
            h = &D_80077DE8;
            h->unk2 = r->y + row * 21 + 0x26;
        }
    }
    D_80077DEC = 3;
    func_800355E4();
}
