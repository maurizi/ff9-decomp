/* 4 rows differ: only the scheduling of bit=1 vs i=0 in the prologue. Writing bit=1 first flips the s4/s5 allocation of mask/i (11 rows). The do{}while(0) around *used |= 1<<i fixed the s7/s8 swap. func_80057898 is called with a 3rd arg (ch->unk34), hence the cast. */
#include "ff9.h"

void func_80057D80(AkaoChannel *, u32);
s32 func_800584C8(s32);
s32 func_80058578(s32);
void func_800577EC(s32 v, Unk800577EC *p, s32 vol);
void func_80057898(s32 v, Unk800577EC *p);

void func_800585E0(AkaoChannel *ch, u32 mask, u32 keep, u32 *used) {
    u32 bit;
    s32 i;
    u32 on;
    s32 f;
    s32 v;

    i = 0;
    bit = 1;
    on = mask & D_80080A10->unk10;

    do {
        if (mask & bit) {
            func_80057D80(ch, bit);
            if (ch->unk11C != 0) {
                if (D_80080AC4 & bit) {
                    ch->unk132 = 0;
                    ch->unk130 = 0;
                }
                if (on & bit) {
                    if (keep & bit) {
                        do {
                            *used |= 1 << i;
                        } while (0);
                        ch->unk118 = i;
                    } else {
                        f = (D_80080A10->unk8 & bit) != 0;
                        v = func_80058578(f);
                        if (v == 24) {
                            D_80080A10->unk0 |= 2;
                            v = func_800584C8(f);
                        }
                        if (v == 24) {
                            ch->unk118 = v;
                            D_80080A10->unk0 |= 1;
                        } else {
                            *used |= 1 << v;
                            ch->unk118 = v;
                            D_8007F798[v].unk4 = 0x7FFF;
                        }
                    }
                    if (ch->unk118 < 24) {
                        func_800577EC(ch->unk118, (Unk800577EC *)&ch->unk118, (u16)ch->unk12E);
                        D_800831C8[ch->unk118] = D_80080A10;
                        D_80083158.unk8 |= 0x100;
                    }
                } else if (ch->unk118 < 24) {
                    ((void (*)(s32, Unk800577EC *, u32))func_80057898)(ch->unk118, (Unk800577EC *)&ch->unk118, ch->unk34);
                }
            }
            mask &= ~bit;
        }
        bit <<= 1;
        ch++;
        i++;
    } while (mask != 0);
}
