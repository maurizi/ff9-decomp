/* 11 rows differ: a (D_8007F790) lands in v1 instead of a3 (the original also puts t in a3 once a is dead), and D_80080A10 is loaded after a->unk8 here. Needed: the early lim = 0xFFFFFF temp (lui a2 in the beqz delay slot), the s32 new_var2 = a->unk8 & a->unkC temp (a u32 temp or the inline expression gives 30+ rows), and v (the loop counter) reused for D_80080A10->unk8 (permuter, 12 -> 11). Using D_8007F790 directly instead of a, a D_80080A10 pointer local (39), reusing v for t (31), operand orders and the 120 orderings of the five setup statements did not help. */
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
    u32 t;
    u32 lim;

    if (a != NULL) {
    v = D_80080A10->unk8;
    lim = 0xFFFFFF;
    found = 0;
    new_var2 = a->unk8 & a->unkC;
    t = ~(v & D_80080A10->unkC) | new_var2;
    mask = ~(a->unkC & t) & D_80080A10->unkC & lim;
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
