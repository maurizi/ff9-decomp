/* 5 rows (cmp with symbol names): only the scheduling of lui %hi(D_80083174) (the original hoists it above sw zero,0x18(a3)) and ori 0xFFFFFF. Pass 8: `D_80080A10->unk18 = 0; x = 0xFFFFFF; do { } while (0); D_8007F95A = 0;` (6 -> 5); expression orders, dup sweep no change. Pass 14: the do { } while (0) is a sched2 region barrier (loop notes); without it every order of the three tail statements gives 9 rows. The original has lui %hi(D_8007F95A) and lui %hi(D_80083174) both before the sw zero,0x18(a3), i.e. in the same region as that store, while the D_80083174 lw stays after the stores. All 120 placements of the four tail statements around the D_8007F790 if (with and without the barrier) and 10 spellings of the mask expression: best 5. */
#include "ff9.h"

void func_8005A320(s32 addr, s32 restore) {
    AkaoSeqHeader *seq = (AkaoSeqHeader *)addr;
    u32 flags;
    u32 x;
    u8 *old;
    s32 d;
    AkaoChannel *c;
    s32 n;
    u32 bit;
    u32 active;

    if (restore == 0) {
        func_8005CE04(&D_8007F8F0, D_80080A10, 0x7C);
        func_8005CE04(D_80080AC8, D_8007BD48, 0x2680);
    } else {
        func_8005CE8C(&D_8007F8F0, D_80080A10, 0x7C);
        func_8005CE8C(D_80080AC8, D_8007BD48, 0x2680);
    }
    flags = D_80080A10->unk0 & ~0x30;
    D_80080A10->unk0 = flags;
    D_80080A10->unk0 = seq->unk14 == D_8007F8DC ? flags | 0x20 : flags | 0x10;
    c = D_8007BD48;
    n = 32;
    old = D_80080A10->unk2C;
    D_80080A10->unk2C = (u8 *)seq;
    D_80080A10->unk10 = 0;
    bit = 1;
    D_80083158.unk8 |= 0x90;
    d = (u8 *)seq - old;
    active = D_80080A10->unk4;
    D_80080A10->unk30 = (u16 *)((u8 *)D_80080A10->unk30 + d);
    D_80080A10->unk34 += d;
    D_80080A10->unk10 = D_80080A10->unk14;
    do {
        if (active & bit) {
            c->unk0 += d;
            c->unk18 += d;
            c->unk4[0] += d;
            c->unk4[1] += d;
            c->unk4[2] += d;
            c->unk4[3] += d;
            c->unk96 += 2;
            c->unk98 += 2;
            c->unk11C |= 0x1FF93;
            func_8005936C(D_80080A10->unk0, c);
        } else {
            c->unk96 = 4;
            c->unk98 = 2;
            c->unk0 = D_8006F40C;
        }
        c->unk118 = 0x18;
        n--;
        c++;
        bit <<= 1;
    } while (n != 0);
    if (D_8007F790 != NULL) {
        active = func_800595F0(D_8007F71C, D_8007F790->unk4 & D_8007F790->unk8);
    } else {
        active = 0;
    }
    D_80080A10->unk18 = 0;
    x = 0xFFFFFF;
    do { } while (0);
    D_8007F95A = 0;
    D_80080A70.unkC |= ~active & (~(D_80080A70.unk0 | D_80083174) & x);
    D_80083158.unk8 |= 0x100;
    if (D_800831C4 & 1) {
        D_80080A10->unk1C = D_80080A10->unk4;
        D_80080A10->unk4 = 0;
    }
}
