/* STATUS: 11 rows (cmp.py), all in the tail block (local-alloc of the D_8006F324 rotation). Pass 14: the start value t0 is reused for the tail (`t0 = GetRCnt(0xF2000002) - t0; if (t0 <= 0) t0 += 0x44E8;`) and then copied into d (`d = t0;`), which the tail uses. The copy survives CSE only if d is mentioned before the tail's CSE block (cse make_regs_eqv makes d the canonical reg only when it lives outside the block); the dead `s32 d = 0` initialiser does that and flow deletes it, so d is block-local. With the copy t0 has 7 refs / 200 insns (0.070) and loses to the hoisted &D_80083158 (182, 0.086), which now gets fp; t0 is spilled to 0x10(sp) and reloaded through t0 exactly like the original, so the whole loop and the GetRCnt/bgtz/sw sequence match without any do-while (reusing acc or now for the copy also spills t0: 19 rows). Remaining: the original's tail has d in v0, a in v1, &D_8006F324 in a0, its hi in a1, b in a2, e in a3 (and hi(D_8006F31C) a0); here d and the address swap (a0/v0) with `d + (a + b + e)`, 13-15 rows with the other sums. Tried: all 4! load orders with the copy first/last x 6 sums, and all 120 sum trees with two copy positions (best 11). The original allocates the address after d and a, which a block-local address pseudo (7 refs, highest local priority) never is. */
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
    s32 d = 0;

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
    t0 = GetRCnt(0xF2000002) - t0;
    if (t0 <= 0) {
        t0 += 0x44E8;
    }
    d = t0;
    a = D_8006F324[1];
    b = D_8006F324[2];
    e = D_8006F324[3];
    D_8006F324[3] = d;
    D_8006F324[0] = a;
    D_8006F324[1] = b;
    D_8006F324[2] = e;
    D_8006F31C = d + (a + b + e);
}
