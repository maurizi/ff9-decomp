/* 12 rows differ: a (D_8007F790) lands in v1 instead of a3, and D_80080A10 is loaded after a->unk8 here. The early lim = 0xFFFFFF temp (lui a2 in the beqz delay slot) fixed the mask constant. */
#include "ff9.h"

void func_80059414(void) {
    s32 new_var2;
    Unk80080A10 *a = D_8007F790;
    Unk80080A10 *new_var;
    u32 mask;
    u32 found;
    u32 bit;
    s32 v;
    s32 n;
    AkaoChannel *ch;
    u32 t;
    u32 lim;

    if (a != NULL) {
    lim = 0xFFFFFF;
    found = 0;
    new_var2 = a->unk8 & a->unkC;
    t = ~(D_80080A10->unk8 & D_80080A10->unkC) | new_var2;
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
