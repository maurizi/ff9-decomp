/*
 * ovl_0d func_800AEE2C (0x2D4 bytes): marks which hand cards are shown and lays out the enemy's
 * card stack ("NO" + count, up to five stacked cards from the deck via func_800625A0).
 * WIP (pass 7, first draft): 182 vs 181 instructions, ~77 aligned diff rows.
 *  - loop 1: the original's giv is the card pointer (v1 = s + 0x10, stores at 0x34(v1)); with
 *    `c = &s->unk10[i]` we get an index giv plus an addu each iteration, with a pointer biv
 *    (`c++`) loop.c folds 0x34 into the giv (s + 0x44);
 *  - `5` for `s->unk47C != 5` lives in s1 (i's register) in the original, hence `i = 5`;
 *  - n should stay in fp (we spill it), one s-register too many (frame 0x68 vs 0x60);
 *  - the dx block: the original threads the `i == 0` test into the unk418 == 0 arm.
 */
#include "ovl_0d.h"

s32 func_8002BA10(u8 *);

void func_800AEE2C(TmState *s) {
    u8 buf[0x20];
    TmCard *c;
    TmCard *c0;
    Unk6 *d;
    s32 i;
    s32 n;
    s32 m;
    s32 k;
    s32 dx;
    s32 w;

    for (i = 0; i < 5; i++) {
        c = &s->unk10[i];
        if (i < s->unk47C) {
            c->unk34 = 1;
        } else {
            c->unk34 = 0;
        }
    }
    for (i = 4; i >= 0; i--) {
        c = &s->unk10[i + 5];
        c->unk34 = 0;
    }
    i = 5;
    if (s->unk47C != i && s->unk0 != 0x1B) {
        n = func_80062560(func_800AF268(s));
        if (n != 0) {
            func_8002BAE4(func_800AF268(s) + 1, buf, -1, 3);
            w = (func_8002BA10(buf) + 0xE) / 2;
            func_8002B56C(0xD8 - w, 0x78, 0xFFA, "NO", 0);
            func_8002B5B4(0xE6 - w, 0x76, 0xFFA, buf, 0);
            m = n;
            k = s->unk4B4;
            c0 = &s->unk10[5];
            if (m >= 6) {
                m = 5;
            }
            for (i = 0, c = c0; i < m; i++, c++) {
                dx = 0;
                if (s->unk568 != 0) {
                    if (s->unk418 == 0) {
                        dx = s->unk568 - 4;
                    } else {
                        dx = -s->unk568 + 1;
                    }
                    if (i == 0) {
                        dx = 0;
                    }
                }
                d = func_800625A0(func_800AF268(s), k);
                c->unkC = 0xC2 + i * 4 + dx;
                c->unk10 = i + 0x81;
                c->unk30 = i + 0x28;
                c->unk34 = 1;
                c->unk1C = 0;
                c->unk18 = 1;
                c->unk14 = 0;
                c->unk0 = d->unk0;
                if (i == 0) {
                    c0->unk1 = d->unk1;
                } else {
                    c->unk1 = 0;
                }
                c->unk2 = d->unk2;
                c->unk3 = d->unk3;
                c->unk4 = d->unk4;
                k++;
                c->unk34 = 1;
                c->unk5 = d->unk5;
                if (k == n) {
                    k = 0;
                }
            }
            w = func_80062794(func_800AF268(s));
            func_8002B5FC(0xD9 - w / 2, 0xC7, 0xFFA, (u8 *)func_8006276C(func_800AF268(s)), 0);
        }
    } else {
        s->unk10[5].unk34 = 0;
    }
}
