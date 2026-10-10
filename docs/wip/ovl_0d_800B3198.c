/*
 * ovl_0d func_800B3198 (0x234 bytes): draws the two battle power numbers (func_800B33CC digits).
 * WIP: 141 of 141 instructions, 2 aligned diff rows. The permuter found `y = b->unkC; x = y + 8;`
 * in the second card's branch, which fixes most registers; left: that unkC load goes to v0 in
 * the original but straight into y's s-register (s4) here.
 */
#include "ovl_0d.h"
void *func_800B3198(u32 *ot, void *vp, TmState *s) {
    SPRT *p = vp;
    TmCard *a;
    TmCard *b;
    s32 i;
    s32 n;
    s32 x;
    s32 y;
    s32 t;

    if (s->unk57C != 0) {
        a = func_800AEE08(s, s->unk424, s->unk428);
        b = func_800AEE08(s, s->unk42C, s->unk430);
        for (i = 0; i < 2; i++) {
            if (i == 0) {
                n = s->unk574;
                t = a->unk10;
                x = a->unkC + 8;
            } else {
                n = s->unk578;
                t = b->unk10;
                y = b->unkC;
                x = y + 8;
            }
            y = t + 0x12;
            if (n < 10) {
                x += 9;
            } else if (n < 100) {
                x += 4;
            }
            if (n >= 100) {
                p = func_800B33CC(ot, p, x, y, n / 100);
                n %= 100;
                x += 9;
                p = func_800B33CC(ot, p, x, y, n / 10);
                x += 9;
                n %= 10;
            } else if (n >= 10) {
                p = func_800B33CC(ot, p, x, y, n / 10);
                x += 9;
                n %= 10;
            }
            p = func_800B33CC(ot, p, x, y, n);
        }
        setlen((DR_TPAGE *)p, 1);
        ((DR_TPAGE *)p)->code[0] = (GetTPage(0, 0, 0x340, 0x1E0) & 0x9FF) | 0xE1000000;
        AddPrim(ot + 11, p);
        p = (SPRT *)((DR_TPAGE *)p + 1);
        s->unk57C--;
    }
    return p;
}
