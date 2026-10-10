/* 9 rows: a (D_8007F790) lands in v1 instead of a3 (the original also has t and the loop counter v in a3), and D_80080A10 is loaded before a->unk8. Pass 8: v reused for t (11 -> 9). new_var2 is a permuter leftover; a fresh draft without the temps scores 39. */
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
