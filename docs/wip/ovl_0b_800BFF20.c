/* WIP func_800BFF20: 4 differing rows (cmp.py prints 6: two are the .rodata vs D_800A7948 symbol of the table, which go away when integrated), same length. Pass 18: the tail flag is the existing BtlUnitHasStatus inline: `n = BtlUnitHasStatus(arg0, 0x200000); if ((u8)n)` (n reused, as before) gives the tail exactly (copy before beqz, no andi) and keeps all registers. `if (BtlUnitHasStatus(...))` directly drops n from the tail and shifts registers (49-57); `if (n)`: 9. Left: only the c block (original: c in v1, arm 2 computes into v0 and copies to v1; ours puts c in v0). Tried on the new base: c merged into v (u8 or u32 v, `if (v)` / `if ((u8)v)`, arm 2 direct / via w, c or t temp): 6-10, and with v as c the copy is merged (c lands in v1 as in the original, the copy is lost); ?: for the whole c or v: 5-9; arm 2 through an inline (nb(arg0), nb(x, m), u8/s32 returns): 5-8; temp T in `T = X; c = T;` / `T = U = X; c = T;` over all locals: 5-39; the whole c as an inline ph(arg0, n) (u8 local + return, two returns, s32): 5-10. ~20 min of permuter on this base (score 25): nothing better. Pass 10: `if ((u8)n)` on the end flag gives the copy-before-beqz shape (the original's copy is what is left of a zero-extend that combine dropped), but ours keeps andi 0xff; a separate flag variable shifts all registers (60+ rows). New in pass 9 (status tint / float bob of a battle unit). The 24-byte colour table is the local initialiser (replaces INCLUDE_RODATA D_800A7948 when integrated). Fixed: n (the D7C counter) reused as the float flag, m = n then m %= 24, t dropped (permuter), c tested as u8. Left: two "copy before test" spots: the original computes the 0x800000 test into v0 and copies it to c (v1), and the float flag a0 is copied to v0 before beqz; v = w = ... gets the first copy but in the wrong direction. Pass 13: a separate u32/s32/u8 flag for the tail (declared anywhere) changes the code (68+ rows). Pass 14: the tail's copy needs combine to drop the zero-extend, i.e. nonzero_bits(flag) <= 0xFF over every set of the flag pseudo; n also holds the D7C counter, so that never happens. w (only ever set to a comparison) as the tail flag gives the copy shape at the tail but moves registers everywhere (68 rows); `if ((t = n) != 0)` / `t = (u8)n` forms: 8. Pass 15: the c/temp copy: all single-variable forms of the 0x800000 arm (`c = (x & 0x800000) == 0`, `!(...)`, ?: for the whole c, swapped arms, w temp) lose the copy (10 rows). A separate tail flag f (u8 or s32 with `(u8)f`) keeps the andi because combine has no LOG_LINK from `f = 1` (previous block) to the zero_extend at the join; with n reused, n's D7C set also blocks it. With a separate flag the allocation changes because CSE then makes m (not n) the head of {n, m} (`m = n; m %= 24`); `m = n % 24` brings that back to 60 rows. An inline `static u8 floating(BtlUnit *)` for the tail: 15-16 rows (i2/i3) or 66. Pass 16: in the original the join block before reorg is [v0 = n (plain copy); beqz v0] (reorg copies it into the second test's delay slot and retargets past it), so combine left a register copy unmerged with the branch, which it only does when the copy's destination is still live after the branch or the copy is created after combine. `n = (a & 0x200000) || (b & 0x200000); if ((u8)n)`: 8 rows; with c/v/w as the flag: 66-69. Pass 17: not re-tested. Pass 19: not re-tested. */
#include "ovl_0b.h"

void func_800BFF20(BtlUnit *arg0) {
    Unk800CEE74 *e;
    u8 *p;
    s32 t;
    u32 n;
    u32 m;
    u8 c;
    u32 v;
    u32 w;
    s16 r, g, b;
    s32 rr, gg, bb;

    if (!((arg0->unk68 >> 22) & 1) && !(arg0->unk3C & 1)) {
        e = func_800CEE74();
        if (arg0->unk3C & 0x3000840) {
            func_800C8A3C(arg0->unk9C, arg0->unkC1[0], arg0->unkC1[1], arg0->unkC1[2]);
            if (arg0->unkA8 != NULL) {
                func_800C8A3C(arg0->unkA8, arg0->unkC1[0], arg0->unkC1[1], arg0->unkC1[2]);
            }
        } else {
            if (arg0->unk3C & 0xC00000) {
                n = D_8006794C->unk1C->unk8E4.battle->unkD7C;
                m = n;
                m = m % 24;
                if ((arg0->unk3C & 0xC00000) == 0xC00000) {
                    c = (n % 48) >= 24;
                } else {
                    v = w = (arg0->unk3C & 0x800000) < 0x800000;
                    c = v;
                }
                if (c) {
                    r = e->unkC - 0x40;
                    g = e->unkD + 0x18;
                    b = e->unkE + 0x48;
                } else {
                    r = e->unkC + 0x28;
                    g = e->unkD + 0x28;
                    b = e->unkE - 0x50;
                }
                if ((u8)m < 8) {
                    v = m;
                } else if ((u8)m < 16) {
                    v = 8;
                } else {
                    v = 24 - m;
                }
            } else if ((arg0->unk3C & 0x4100) == 0x4000) {
            u8 tbl[8][3] = {
                { 0xFF, 0x60, 0x60 }, { 0x68, 0x78, 0xFF }, { 0xFF, 0xB8, 0x78 }, { 0x80, 0xC0, 0xD0 },
                { 0xC0, 0x68, 0x90 }, { 0xD0, 0x70, 0x78 }, { 0xC8, 0xD8, 0x68 }, { 0xD0, 0xB8, 0x68 },
            };
                p = tbl[(arg0->unk68 >> 1) & 0xF];
                r = e->unkC + (p[0] - 0x80);
                g = e->unkD + (p[1] - 0x80);
                b = e->unkE + (p[2] - 0x80);
                v = D_8006794C->unk1C->unk8E4.battle->unkD7C & 0xF;
                if (v >= 8) {
                    v = 16 - v;
                }
            } else {
                goto end;
            }
            rr = (r * (u8)v) << 13 >> 16;
            gg = (g * (u8)v) << 13 >> 16;
            bb = (b * (u8)v) << 13 >> 16;
            func_800C86C8(arg0->unk9C, rr, gg, bb);
            if (arg0->unkA8 != NULL) {
                func_800C86C8(arg0->unkA8, rr, gg, bb);
            }
        }
    }
end:
    n = BtlUnitHasStatus(arg0, 0x200000);
    if ((u8)n) {
        arg0->unk8C[1] = t = 0xFF380000 - func_8004BB80((D_8006794C->unk1C->unk8E4.battle->unkD7C & 0xF) << 8) * 480;
        arg0->unk98->unk0[1] = t;
    }
}
