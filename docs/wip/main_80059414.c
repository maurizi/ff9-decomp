/* 9 rows: a (D_8007F790) lands in v1 instead of a3 (the original also has t and the loop counter v in a3), and D_80080A10 is loaded before a->unk8. Pass 8: v reused for t (11 -> 9). new_var2 is a permuter leftover; a fresh draft without the temps scores 39. Pass 13: in the original, a (a3) conflicts with the D_80080A10 value (v1, loaded while a is live), which is why a skips v1 and lands in a3 next to t/v (a dies at lw v1,0xC(a3), where t is born). Ours loads D_80080A10 into v0 (it shares the dying %hi qty), so a takes v1. The natural spelling `v = ~(D_80080A10->unk8 & D_80080A10->unkC) | (a->unk8 & a->unkC); mask = ~(a->unkC & v) & D_80080A10->unkC & lim;` gets a into a3 but shifts the loop registers (32 rows). A Unk80080A10 *b local for D_80080A10 does the same. Pass 14: in the original the ~(b8 & bC) | (a8 & aC) result is v (a3, `or a3,v0,a0`), with the nor result in v0 and a8 & aC in a0. global.c's set_preference takes the first operand of the ior, so in the natural spelling (where the first ior operand is local-allocated to a0) v prefers a0 and the loop registers shift (32 rows); in the original that operand is v0, which v conflicts with, so v gets no usable preference and lands in a3. Sweeps of operand orders, a b local, a temp for either half (including the loop counter n), and lim/found positions (288 variants) give 13 at best. Pass 15: a 25-min permuter run on this draft (base 85) found nothing; D_80083174 as D_80083168.unkC (the 8005A320 fix) changes nothing here; 8 more first-block spellings (b local, temps in mask/bit/n, the ~ split into statements) give 28-32. Pass 16: D_80080A10, D_8007F790 and D_8007F71C as struct fields change nothing (no store precedes the loads). Making b = D_80080A10 a global pseudo (a fake store of b at the end) puts a in a3 as in the original, confirming that a lands in a3 once v1 is taken during its life; in the original v1 is b (local, v0 held by b->unk8 while b is live). 88 more first-block spellings (mask/v/n/bit temps, b local at three positions, lim/found order): best 20. Pass 17: local-alloc qty priorities (floor_log2(refs)*refs/(death-birth)) decide it: the b8 chain {b8, b8&bC, ~} has 6 refs (12/len) and the aC chain {aC, aC&v, ~, &bC} 9 refs (27/len), so aC's chain is allocated first and takes v0 unless it lives more than 2.25x as long as b8's. In the original aC's chain sits in v1 (b8's chain holds v0), so the original's sched1 order must give the aC chain a much longer life, or the chain must be split. Tried: an AND8C inline or macro for both `p->unk8 & p->unkC` (28-36), De Morgan spellings `~(b8 & bC & ~(a8 & aC))` (28-40), and 51 variants reusing mask/bit/n/found/lim as temps for either half (best 23). */
#include "ff9.h"

void func_80059414(void) {
    s32 new_var2;
    Unk80080A10 *a = D_8007F790;
    u32 mask;
    u32 found;
    u32 bit;
    s32 v;
    s32 n;
    AkaoChannel *ch;
    u32 lim;

    if (a != NULL) {
    v = D_80080A10->unk8;
    lim = 0xFFFFFF;
    found = 0;
    new_var2 = a->unk8 & a->unkC;
    v = ~(v & D_80080A10->unkC) | new_var2;
    mask = ~(a->unkC & v) & D_80080A10->unkC & lim;
        for (v = 0; mask != 0; v++) {
            if (mask & (1 << v)) {
                n = 0x20;
                bit = 1 << v;
                ch = D_8007F71C;
                for (; n != 0; n--) {
                    if (ch->unk118 == v) {
                        ch->unk118 = 0x18;
                        found |= bit;
                    }
                    ch++;
                }
                mask &= ~(1 << v);
            }
        }
        D_80080A70.unkC |= found & ~(D_80080A70.unk0 | D_80083174);
    }
}
