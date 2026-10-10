/*
 * ovl_0d func_800AEE2C (0x2D4 bytes): marks which hand cards are shown and lays out the enemy's
 * card stack ("NO" + count, up to five stacked cards from the deck via func_800625A0).
 * MATCHES (pass 10): a trial integration gave `build/us/ovl_0d.bin: OK`. Not integrated only
 * because the "NO" label is migrated rodata (D_800A7398, inside func_800AEE2C.s): integrating
 * needs `D_800A7398 = 0x800A7398; // force_not_migration:True` in config/symbols.ovl_0d.us.txt,
 * `make resplit`, then `extern char D_800A7398[];` + INCLUDE_RODATA instead of the literal below.
 * What made it match:
 *  - loop 1 `q = s->unk10; p = &q[i]` (card-base giv with the 0x34 store offset), loop 2 a
 *    pointer biv `q = &s->unk10[9]; q--`, both temps;
 *  - card loop `c = &c0[i]` (giv starting at c0) so n stays in fp;
 *  - `i = func_8002BA10(buf) + 0xE; w = i / 2;` and `i = func_80062794(...)` (both in s1);
 *  - the unk418 == 0 arm has its own `if (i == 0) dx = 0;` (jump threading), `unk5` before `k++`;
 *  - c0 also holds the two func_800AF268(s) results passed straight on (combine deletes those
 *    copies, but their refs still raise c0's global-alloc priority above s and m: c0 s4, s s5,
 *    m s6).
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
        c0 = (TmCard *)func_800AF268(s);
        n = func_80062560((s32)c0);
        if (n != 0) {
            c0 = (TmCard *)func_800AF268(s);
            func_8002BAE4((s32)c0 + 1, buf, -1, 3);
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
