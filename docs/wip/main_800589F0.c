/* STATUS: 3 rows off (5 with cmp.py; 2 are the D_8008323C symbol rows that link identically). Only the else-branch keep mask lands in s1 instead of s2. Needs func_80058990/func_800589E0 as K&R definitions (called with two args). The if (b1) identical-branch around func_800589E0 is needed (6 -> 5). */
#include "ff9.h"

long SpuSetNoiseClock(long n_clock);
void SpuSetReverbModeDepth(short depth_left, short depth_right);
void func_800585E0(AkaoChannel *, u32, u32, u32 *);
void func_80058118(AkaoChannel *, u32);
void func_80057898(u32, u32 *, u32);
void func_80058990();
void func_800589E0();
void func_800588B0(u32);
void func_80059140(u32 *out, u32 m1, u32 m2, u32 extra);
extern s32 D_80080A74;

void func_800589F0(void) {
    u32 keyon;
    u32 ex;
    u32 m1;
    u32 m2;
    u32 b1;
    u32 b2;
    u32 bit;
    AkaoChannel *ch;
    Unk80080A10 *s;
    Unk80080A10 *t;

    ex = D_80080A70.unk0 | D_80080A70.unk10 | D_80083174;
    keyon = 0;
    if ((D_80080A10->unk4 & D_80080A10->unk10) ||
        (D_8007F790 != NULL && (D_8007F790->unk4 & D_8007F790->unk10))) {
        func_800588B0(ex);
    }
    if (D_8007F790 != NULL) {
        if (D_80083158.unk4 & 0x100) {
            func_80058990(D_8007F790, D_8007F71C);
        }
        t = D_8007F790;
        m1 = t->unk4 & t->unk14 & ~(t->unkC & ex);
        b1 = m1 & t->unkC & ~ex;
        D_80080A10 = t;
        if (m1 & D_80080A10->unk8) {
            func_800585E0(D_8007F71C, m1 & D_80080A10->unk8, b1, &keyon);
            D_80080A10->unk10 &= ~D_80080A10->unk8;
            m1 &= ~D_80080A10->unk8;
        }
        s = &D_8007F858;
        D_80080A10 = s;
        m2 = s->unk4 & s->unk14 & ~(s->unkC & (b1 | ex));
        b2 = m2 & s->unkC & ~(b1 | ex);
        if (m2 & D_80080A10->unk8) {
            func_800585E0(D_8007BD48, m2 & D_80080A10->unk8, b2, &keyon);
            D_80080A10->unk10 &= ~D_80080A10->unk8;
            m2 &= ~D_80080A10->unk8;
        }
        if (m1) {
            D_80080A10 = D_8007F790;
            func_800585E0(D_8007F71C, m1, b1 & ~b2, &keyon);
            D_80080A10->unk10 = 0;
            D_80080A10 = s;
        }
        if (m2) {
            func_800585E0(D_8007BD48, m2, b2, &keyon);
            D_80080A10->unk10 = 0;
        }
        if (D_80083158.unk4 & 0x100) {
            if (b1) {
                func_800589E0(D_8007F790, D_8007F71C);
            } else {
                func_800589E0(D_8007F790, D_8007F71C);
            }
        }
    } else {
        m2 = D_80080A10->unk4 & D_80080A10->unk14 & ~(D_80080A10->unkC & ex);
        b1 = m2 & D_80080A10->unkC & ~ex;
        if (m2 & D_80080A10->unk8) {
            func_800585E0(D_8007BD48, m2 & D_80080A10->unk8, b1, &keyon);
            D_80080A10->unk10 &= ~D_80080A10->unk8;
            m2 &= ~D_80080A10->unk8;
        }
        if (m2) {
            func_800585E0(D_8007BD48, m2, b1, &keyon);
            D_80080A10->unk10 = 0;
        }
    }
    m2 = D_80080A70.unk0 & D_80080A70.unk8;
    if (m2) {
        keyon |= D_80080A70.unk4;
        for (bit = 0x100, ch = D_8007E3D8; m2 != 0; bit <<= 1, ch++) {
            if (m2 & bit) {
                func_80058118(ch, bit);
                if (ch->unk11C) {
                    do { func_80057898(ch->unk118, &ch->unk118, ch->unk34); } while (0);
                }
                m2 &= ~bit;
            }
        }
        D_80080A74 = 0;
    }
    m2 = D_80083158.unk8;
    if (m2 & 0x80) {
        SpuSetReverbModeDepth(D_80080A10->unk48 >> 12, D_80080A10->unk48 >> 12);
        D_80083158.unk8 &= ~0x80;
    }
    if (m2 & 0x10) {
        SpuSetNoiseClock(D_80080A70.unk0 ? D_80080A70.unk28 : D_80080A10->unk6E);
        D_80083158.unk8 &= ~0x10;
    }
    if (m2 & 0x100) {
        func_80059140((u32 *)&D_80083238[1], D_8007F790->unk3C, D_80080A10->unk3C, D_80080A70.unk1C);
        func_80059140((u32 *)&D_80083238[0], D_8007F790->unk40, D_80080A10->unk40, D_80080A70.unk20);
        func_80059140((u32 *)&D_80083238[2], D_8007F790->unk44, D_80080A10->unk44, D_80080A70.unk24);
        func_800575EC(D_80083238[0]);
        func_80057608(D_80083238[1]);
        func_80057624(D_80083238[2]);
        D_80083158.unk8 &= ~0x100;
    }
    if (keyon) {
        func_800575B4(keyon);
    }
}
