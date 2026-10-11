/* STATUS: draft, 22 rows (pass 17; was 26, 31), no wraps (pass 12: block 1 as D_80077DE8.unk0 = r->x + k; h = &D_80077DE8; h->unk4 = 0x17; h->unk2 = r->y + 8; the x-first add gives the original's addu v1,v1,v0 and the plain h assignment after the store scores 31; the same for h->unk0 = r->x + k through h. Still left in block 1: the original keeps h's lo_sum addiu right after the unk0 store (an anti-dependence, as if lui/addiu were in place in one register) while ours hoists it to the block top; block-local h declarations in each block score 43). Pass 11: 33 rows. ovl_33 func_801EF644 (0x2BC, equipment menu cursor; sibling of ovl_37 func_801F13F8 and ovl_3a func_801F459C). Control flow, constants and the party loop are right (ours is 3 instructions longer). Wrapping each 'h = &D_80077DE8;' in do { } while (0) gets the original's 'sh v0,%lo(D_80077DE8)(a0); addiu a0,a0,%lo' pairing in the later blocks (47 -> 38 rows). Left: block 1 reads r->x through %lo(D_801FBFAC) and D_801FBF84.unk0 as -0x28(r) and schedules the table load first; block 3 load order. The permuter's 'ry = r->y;' temp before the last block's stores took it from 36 to 33.
 Pass 14: func_800355E4 is unprototyped and ovl_32 passes &D_80077DE8; passing nothing/h/&D_80077DE8 in
 each of the three calls (27 combinations) gives 30-31 rows, so the argument is not the missing piece.
 Pass 16: 31 -> 28 with the last block's pointer as its own local (h3, no do-while): a block-local pointer
 can be tied to the %hi register by local-alloc, which gives the original's `sh ..%lo(D_80077DE8)(a0); addiu a0,a0,%lo`.
 Making h local in blocks 1/2 as well (with or without the do-while) scores 33-44, so those still differ
 (sched1 puts the addiu before the unk0 store there). Block 1 reading r->y + 8 into ry before the unk4
 store (as block 3 does with ry = r->y; permuter hint): 28 -> 26.
 Pass 17: 26 -> 22 with no do-while: blocks 1 and 2 get their own block-local cursor pointers (h1, h2), block 1
 in the order r, h1 = &D_80077DE8, k, unk0 store, ry, unk4, unk2 (build/w/topo.py, all 7-statement orders):
 block 1 now matches (its 3 rows are cmp.py symbol-name false positives: %lo(D_801FBF84)+0x28 vs D_801FBFAC);
 block 2 in the order l, k, unk0 store, h2 = &D_80077DE8, k2, unk4, unk2 has the right instruction count (175).
 Left in block 2: the original issues `lui a2,%hi(D_80077DE8)` early (right after the l address) and the
 base pointer (cse's &D_801FBF84) is a3, ours issues the lui just before the unk0 store and the base is a2;
 also `addiu v0,a3,8` before the unk0 lbu. Block 3's base pointer is a1 there, a2 here. Moving h2 before the
 store gives an early lui but then the addiu is not in place (24-25). Block-local k/l/ry change nothing.
 With h2 = &D_80077DE8 right after l (L H K X J F Z; build/w/q33_0.c, 31 rows) the instruction order of block 2
 is exactly the original's (early lui, in-place addiu after the store) and only the register names differ:
 the original has l in a1 and the unk7 byte in a0 (so local-alloc saw a0 busy there), ours l in a0. Permuter
 (12k iterations on that base) found nothing; `r = &D_801FBF84.unk48`, a block-local state pointer, condition
 spellings and duplicated bodies do not change it.
 */
#include "ovl_33.h"
extern u16 D_801F33C8[][2];

void func_801EF644(void) {
    RECT *r;
    MenuList *l;
    Unk80077DE8 *h;
    Unk80077DE8 *h3;
    s32 i;
    s32 y;
    u16 k;
    u8 blink;
    u8 done;
    s16 ry;

    done = 0;
    blink = (D_80077E54 & 3) != 0;
    func_80035674(D_801FBF84.unk1 != 1);
    if (func_8002F810()) {
        return;
    }
    if (D_801FBF84.unk1 == 1 || D_801FBF84.unk1 == 5 || blink) {
        Unk80077DE8 *h1;
        r = &D_801FBF84.unk28;
        h1 = &D_80077DE8;
        k = D_801F33C8[D_801FBF84.unk0][0] - 0x18;
        D_80077DE8.unk0 = r->x + k;
        ry = r->y + 8;
        h1->unk4 = 0x17;
        h1->unk2 = ry;
        func_800355E4();
    }
    if (D_801FBF84.unk1 == 2 || (D_801FBF84.unk1 == 3 && blink)) {
        Unk80077DE8 *h2;
        l = &D_801FBF84.unk8[D_801FBF84.unk0];
        k = (l->unk7 ^ ((l->unk8 >> 3) & 1)) * 150 - 10;
        D_80077DE8.unk0 = D_801FBF84.unk48.x + k;
        h2 = &D_80077DE8;
        k = l->unk6 * 15 + 10;
        h2->unk4 = 0x17;
        h2->unk2 = D_801FBF84.unk48.y + k;
        func_800355E4();
    }
    if (D_801FBF84.unk1 == 3) {
        r = &D_801FBF84.unk58.unk4;
        if (D_801FBF84.unk3 & 0x80) {
            D_80077DE8.unk4 = 0x12;
            if (blink) {
                for (i = 0; i < 4; i++) {
                    y = i * 0x2B + 0x12;
                    if (D_8006794C->unk1C->unk54C[i] != NULL) {
                        func_800270EC(r->x - 10, r->y + y, D_80077DE8.unk4);
                        if (!done) {
                            D_80077DE8.unk0 = r->x - 10;
                            done = 1;
                            D_80077DE8.unk2 = r->y + y;
                        }
                    }
                }
            }
        } else {
            D_80077DE8.unk0 = D_801FBF84.unk58.unk4.x - 10;
            h3 = &D_80077DE8;
            ry = r->y;
            h3->unk4 = 0x12;
            k = D_801FBF84.unk3 * 0x2B + 0x12;
            h3->unk2 = ry + k;
            func_800355E4();
        }
    }
}
