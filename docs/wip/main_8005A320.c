/* 6 rows differ (cmp with symbol names; rows 0x23C-0x25C): only the order of lui %hi(D_80080A10)/ori 0xFFFFFF and of lui %hi(D_80083174) vs sw zero,0x18(a3) after the loop. Needed: the x = 0xFFFFFF temp (mask association, as in func_8005963C), ~(D_80080A70.unk0 | D_80083174) operand order (the old D_80083174 | ... order swapped the two loads' registers, which the old relocation-masking cmp hid), and the empty do { } while (0) after x = 0xFFFFFF. Statement-order permutations, do-while wraps and the permuter (25 min) did not help. */
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
    D_8007F95A = 0;
    x = 0xFFFFFF;
    do { } while (0);
    D_80080A70.unkC |= ~active & (~(D_80080A70.unk0 | D_80083174) & x);
    D_80083158.unk8 |= 0x100;
    if (D_800831C4 & 1) {
        D_80080A10->unk1C = D_80080A10->unk4;
        D_80080A10->unk4 = 0;
    }
}
