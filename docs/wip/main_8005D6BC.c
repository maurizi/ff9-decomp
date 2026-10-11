/* STATUS: 24 rows (cmp.py). The whole loop matches once the GetRCnt start value t0 is spilled. Here the tail uses a separate d (t0's refs drop, so t0 loses its s-reg to the hoisted &D_80083158 (fp), as in the original), but the original also keeps the tail value in the 0x10(sp) slot (reloads into t0/v0), i.e. d and t0 look like one spilled pseudo. Reusing t0 for the tail raises its priority above the hoisted address (t0 8 refs/207 insns vs 9/314). Wrapping the D_80083158 unk8 test and the unk4 &= ~0x100 block in do { } while (0) (loop depth weights refs) gets t0 reused and spilled (45 rows): then only the spill register is a3 instead of t0 and the tail registers are shifted by one (the original reloads into v0). Pass 13 (galloc numbers): with t0 reused for the tail (`t0 = GetRCnt(0xF2000002) - t0; if (t0 <= 0) t0 += 0x44E8;`) t0 is pseudo 80 with 8 refs / 207 insns (pri 0.116) against the hoisted &D_80083158 (182) at 9 / 314 (0.086). 182 has to win the last s-register (fp) so that t0 is spilled. That needs t0 at 7 refs or fewer (2*7/207 = 0.068) or a live length of 280+. Every tail spelling tried keeps 8 refs: if ((t0 = ...) <= 0), a ternary, the sum via D_8006F324[3], a d = t0 copy, and rotating the array through memory. */
#include "ff9.h"

void func_8005D6BC(void) {
    s32 t0;
    s32 n;
    u32 now;
    Unk80080A10 *tmp;
    AkaoChannel *c;
    u32 mask;
    u32 bit;
    u32 acc;
    s32 a;
    s32 b;
    s32 e;
    s32 d;

    t0 = GetRCnt(0xF2000002);
    D_8007E3C8[1] = D_8007E3C8[0];
    now = VSync(-1);
    n = 1;
    mask = now - D_8007E3C8[1];
    D_8007E3C8[0] = now;
    if (mask != 0) {
        if (mask < 9) {
            bit = mask * 4;
            if (D_8007E3D0 < bit) {
                n = bit - D_8007E3D0 + 1;
            }
            D_8007E3D0 %= bit;
        }
    }
    do {
        n--;
        D_8007E3D0++;
        D_8006F320++;
        if ((D_80080A10->unk18 | D_80080A70.unkC) != 0 || (D_8007F790 != NULL && D_8007F790->unk18 != 0)) {
            func_80058F94();
        }
        if (D_8007F790 != NULL) {
            if (D_8007F790->unk4 == 0) {
                D_8007F790 = NULL;
            } else if ((D_80080A10->unk4 | D_80080A10->unk1C) == 0) {
                func_8005CE04(D_8007F790, D_80080A10, 0x7C);
                func_8005CE04(D_8007F71C, D_8007BD48, 0x2680);
                tmp = D_8007F790;
                D_8007F790 = NULL;
                tmp->unk6A = 0;
                tmp->unk4 = 0;
            }
        }
        if ((D_80083158.unk8 | D_80080A10->unk14 | D_80080A70.unk8) != 0 || (D_8007F790 != NULL && D_8007F790->unk14 != 0)) {
            func_800589F0();
        }
        if (D_80080A10->unk4 != 0) {
            func_8005D384(D_8007BD48, 0);
        }
        if (D_8007F790 != NULL) {
            if (D_8007F790->unk4 != 0) {
                D_80080A10 = D_8007F790;
                func_8005D384(D_8007F71C, 1);
                D_80080A10 = &D_8007F858;
            }
            if ((D_80083158.unk4 & 0x100) && !(D_8006F320 & 3)) {
                func_8005D628(D_8007F790, D_8007F71C);
                if (D_8007F790->unk4 == 0) {
                    D_80083158.unk4 &= ~0x100;
                }
            }
        }
        if (D_80080A70.unk0 != 0) {
            mask = D_80080A70.unk0;
            acc = D_80080A70.unk18 + D_80080A70.unk14.h[1];
            D_80080A70.unk18 = acc;
            if ((acc & 0xFFFF0000) || (D_800831C4 & 4)) {
                D_80080A70.unk18 = acc & 0xFFFF;
                bit = 0x100;
                c = D_8007E3D8;
                do {
                    if (mask & bit) {
                        if (!(D_800831C4 & 2) || (c->unk28 & 0x2000000)) {
                            c->unk88++;
                            c->unk96--;
                            c->unk98--;
                            if (c->unk96 == 0) {
                                func_8005E3FC(c, bit);
                            } else if (c->unk98 == 0) {
                                D_80080A70.unkC |= bit;
                                D_80080A70.unk8 &= ~bit;
                            }
                            func_800579B4(c, bit, 1);
                        }
                        mask ^= bit;
                    }
                    c++;
                    bit <<= 1;
                } while (mask != 0);
            }
        }
        if (!(D_8006F320 & 3)) {
            func_8005CF04();
        }
    } while (n != 0);
    d = GetRCnt(0xF2000002) - t0;
    if (d <= 0) {
        d += 0x44E8;
    }
    a = D_8006F324[1];
    b = D_8006F324[2];
    e = D_8006F324[3];
    D_8006F324[3] = d;
    D_8006F324[0] = a;
    D_8006F324[1] = b;
    D_8006F324[2] = e;
    D_8006F31C = a + b + e + d;
}
