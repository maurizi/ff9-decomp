/*
 * ovl_0d func_800AEE2C (0x2D4 bytes): marks which hand cards are shown and lays out the enemy's
 * card stack ("NO" + count, up to five stacked cards from the deck via func_800625A0).
 * WIP (pass 10): 181 of 181 instructions, 25 aligned diff rows, all one s-register rotation:
 * the original has s in s5, c0 in s4, m in s6 (ours s4 / s6 / s5). Everything else matches.
 * Fixed this pass:
 *  - loop 1 `q = s->unk10; p = &q[i]; p->unk34 = ...` keeps the card base (s + 0x10) as the
 *    giv with the 0x34 offset in the stores; loop 2 is a pointer biv `q = &s->unk10[9]; q--`;
 *    both in temps p/q (v1/v0 in the original);
 *  - card loop `c = &c0[i]` (giv starting at c0, s0) instead of a biv, which puts n in fp;
 *  - `i = func_8002BA10(buf) + 0xE; w = i / 2;` and `i = func_80062794(...)` (the original
 *    keeps both in s1, i's register);
 *  - the dx block: the unk418 == 0 arm has its own `if (i == 0) dx = 0;` (jump threading);
 *  - `c->unk5 = d->unk5` before `k++`.
 * Left: global-alloc priorities are s 0.305, m 0.185, c0 0.127, so c0 can never win s4 over
 * s unless its refs/live length differ in the original (it is only used for the giv init and
 * the i == 0 unk1 store there too).
 */
#include "ovl_0d.h"

s32 func_8002BA10(u8 *);

void func_800AEE2C(TmState *s) {
    u8 buf[0x20];
    TmCard *c;
    TmCard *c0;
    TmCard *p;
    TmCard *q;
    Unk6 *d;
    s32 i;
    s32 n;
    s32 m;
    s32 k;
    s32 dx;
    s32 w;

    q = s->unk10;
    for (i = 0; i < 5; i++) {
        p = &q[i];
        if (i < s->unk47C) {
            p->unk34 = 1;
        } else {
            p->unk34 = 0;
        }
    }
    for (i = 4, q = &s->unk10[9]; i >= 0; i--, q--) {
        q->unk34 = 0;
    }
    i = 5;
    if (s->unk47C != i && s->unk0 != 0x1B) {
        n = func_80062560(func_800AF268(s));
        if (n != 0) {
            func_8002BAE4(func_800AF268(s) + 1, buf, -1, 3);
            i = func_8002BA10(buf) + 0xE;
            w = i / 2;
            func_8002B56C(0xD8 - w, 0x78, 0xFFA, "NO", 0);
            func_8002B5B4(0xE6 - w, 0x76, 0xFFA, buf, 0);
            m = n;
            k = s->unk4B4;
            c0 = &s->unk10[5];
            if (m >= 6) {
                m = 5;
            }
            for (i = 0; i < m; i++) {
                c = &c0[i];
                dx = 0;
                if (s->unk568 != 0) {
                    if (s->unk418 == 0) {
                        dx = s->unk568 - 4;
                        if (i == 0) {
                            dx = 0;
                        }
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
                c->unk5 = d->unk5;
                k++;
                c->unk34 = 1;
                if (k == n) {
                    k = 0;
                }
            }
            i = func_80062794(func_800AF268(s));
            func_8002B5FC(0xD9 - i / 2, 0xC7, 0xFFA, (u8 *)func_8006276C(func_800AF268(s)), 0);
        }
    } else {
        s->unk10[5].unk34 = 0;
    }
}
