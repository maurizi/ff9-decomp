/* STATUS: near miss, 10 rows (pass 16; was 38, 39, 46-48) (same function as ovl_39 func_801F7FD4: one solution gives both).
 ovl_34 func_801EFC20 (0x338, slider gauge: three LINE_F2 rows, two end-cap sprites, a DR_TWIN-repeated middle
 sprite and a DR_TPAGE). Logic and stores right; instruction count now matches (206).
 Pass 13 (build/search/galloc.py + the greg conflict lists): the original keeps otz*4 s3, w+8 s4, x s5, y s6, ic s7,
 mid fp and rematerialises &D_80068998 and %hi(D_8007716C) through t6. Ours had block-local pseudos that cross
 the calls (the &D_80068998 address, the %hi(D_8007716C) part) taking s4/s1 in local-alloc, which gives every
 global a hard conflict with s0-s5 and shifts the whole s-register assignment. `ec = &D_80068998;` at the top
 makes that address a global pseudo (4 refs / 206 insns, lowest priority, spilled and rematerialised like the
 original). Left: the post-loop %hi(D_8007716C) pseudo (4 uses after the calls) is still a block-local qty that
 wins s4; in the original it is a spilled global (lui t6 at each use, also inside the loop). The global
 allocation order is otherwise the original's: otz > w > x > y > ic > mid needs the ic/otz refs of the
 do { } while (0) around the first func_80027710 call (ic 6 refs > mid 7 / 260; otz 7 refs > w), but with
 the block-local %hi in s4 that version scores 49 rows and the plain call 39. Dead reads of D_8007716C at the
 top, in an else arm or after the loop do not make the post-loop %hi global (cse's extended block ends at the
 post-loop label).
 Pass 14: D_8007716C/D_80077170 are real scalars: declaring them as fields of one struct (or either as a
 1-element array via __asm__ names) gives 114-194 rows.
 Pass 16: 39 -> 38. The line loop now matches the original exactly: `k = i + 2; yy = (y + k) << 16;` (a
 separate variable keeps i + 2 in the loop and y first in the addu; `yy = i + 2; yy = y + yy` swaps the
 addu operands because expand_binop puts the target-equal operand first, and `y + i + 2` / `i + 2 + y` get
 reassociated so y + 2 is hoisted) and `pos + x + 3` (x + 4 and pos + x + 3 hoisted into t4/t3). What is
 left is only the global allocation after the loop (ic spilled/rematerialised in t6 and the block-local
 %hi(D_8007716C) in s4, where the original keeps ic in s7 and rematerialises the %hi). The permuter
 results for ovl_37 func_801F13F8 and ovl_33 func_801EF644 point at duplicated statement bodies that
 jump2 cross-jumps after allocation (a pseudo used in two copies is global until then); the same might
 explain the spilled %hi here (untested).
 Pass 16b: 38 -> 18 with `width = w + 8;` in its own local after `ec = ...` (instead of `w += 8`; permuter
 hint `new_var = w`): the %hi(D_8007716C) is now spilled and rematerialised in t6 like the original.
 18 -> 10 with the first func_80027710 call in `do { } while (0)` again (ic s7, mid fp). Left: the prologue
 loads w into v0 (original v1, %hi(D_80068988) in v0), the second call's x + width is computed right after
 the first call into v0 (original: last, into a0), and `x + ic->unk4` operand order in the sprite xy. */
#include "ovl_34.h"

typedef struct {
    u8 unk0;
    u8 unk1;
    u16 unk2;
    u8 unk4;
    u8 unk5;
    u8 pad6[2];
} IconT;
extern IconT D_80068988[2];
extern IconT D_80068998;
extern u32 D_801F154C[3];
extern RECT D_801F1544;
void SetTexWindow(void *p, RECT *r);

typedef struct {
    u32 tag;
    u32 rgbc;
    u32 xy0;
    u32 xy1;
} LineWords;

void func_801EFC20(s32 x, s32 y, s32 otz, s32 pos, s32 w) {
    IconT *ic;
    IconT *mid;
    IconT *ec;
    s32 width;
    SprtWords *p;
    LineWords *l;
    RECT r;
    s32 i;

    ic = D_80068988;
    mid = &ic[1];
    ec = &D_80068998;
    width = w + 8;
    if (pos != 0) {
        for (i = 0; i < 3; i++) {
            s32 k = i + 2;
            s32 yy = (y + k) << 16;
            setlen(D_80077170, 3);
            l = (LineWords *)D_80077170;
            l->rgbc = D_801F154C[i];
            l->xy0 = (x + 4) | yy;
            l->xy1 = (pos + x + 3) | yy;
            addPrim(&D_8007716C[otz], l);
            D_80077170 = (u8 *)(l + 1);
        }
    }
    do {
        func_80027710(x, y, otz, ic, 0);
    } while (0);
    func_80027710(x + width - ec->unk4, y, otz, ec, 0);
    SetTexWindow(D_80077170, &D_801F1544);
    addPrim(&D_8007716C[otz], D_80077170);
    D_80077170 += 0xC;
    setlen(D_80077170, 4);
    p = (SprtWords *)D_80077170;
    p->rgbc = 0x64808080;
    p->xy = ((u16)(x + ic->unk4)) | (y << 16);
    p->uvclut = mid->unk2 << 16;
    p->wh = (width - (ic->unk4 + ec->unk4)) | (mid->unk5 << 16);
    addPrim(&D_8007716C[otz], p);
    D_80077170 = (u8 *)(p + 1);
    r.x = mid->unk0;
    r.y = mid->unk1;
    r.w = mid->unk4;
    r.h = mid->unk5;
    SetTexWindow(D_80077170, &r);
    addPrim(&D_8007716C[otz], D_80077170);
    D_80077170 += 0xC;
    SetDrawTPage(D_80077170, 1, 1, 0x1F);
    addPrim(&D_8007716C[otz], D_80077170);
    D_80077170 += 8;
}
