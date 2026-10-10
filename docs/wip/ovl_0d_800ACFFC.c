/*
 * ovl_0d func_800ACFFC (0x404 bytes): lays out the hand cards and the two flying cards of a battle.
 * WIP: 257 of 257 instructions, 2 aligned diff rows: only the loop preheader order. The original
 * emits `li s5, 0x1A` (y = 0x1A) after the loop.c insns `li fp, 0x10; addu s0, s1, fp` (hoisted
 * 0x10 and the &cards[i] giv); ours puts it before them. Tried: every position of `y = 0x1A`
 * before the loop, for-init orders, x/cards/y permutations, 20 min of the permuter.
 * Pass 9: `cc1 -dS` shows all five preheader insns at equal priority, so sched1 keeps the RTL
 * order, and loop.c always inserts its movables after the source statements. In the original
 * y = 0x1A therefore comes from somewhere that ends up after the movables (not a plain
 * statement before the loop). 50 more permuter minutes found nothing.
 */
#include "ovl_0d.h"
void func_800ACFFC(TmState *s) {
    TmCard *a;
    TmCard *b;
    TmCard *c;
    TmCard *cards;
    s32 dx;
    s32 dy;
    s32 k;
    s32 x;
    s32 y;
    s32 i;
    s32 m;
    s32 t;

    dx = 0;
    dy = 0;
    a = NULL;
    b = NULL;
    k = 0;
    if (s->unk0 == 7) {
        a = func_800B19D4(s, s->unk424, s->unk428)->unk4;
        b = func_800B19D4(s, s->unk42C, s->unk430)->unk4;
        if ((u32)(s->unk2E8 - 0x23) < 0xF) {
            k = s->unk2E8 - 0x23;
            if (s->unk2E8 >= 0x2A) {
                k = 0x32 - s->unk2E8;
            }
        } else {
            k = 0;
        }
        dx = (s->unk424 - s->unk42C) * 0x2A00 / 270;
        dy = (s->unk428 - s->unk430) * 0x3300 / 270;
        k = k * k;
    }
    x = 8;
    cards = s->unk10;
    for (i = 0, y = 0x1A; i < 10; i++) {
        c = &cards[i];
        if (s->unk0 == 0x12 || s->unk0 == 0x23 || s->unk5B8 != 0) {
            c->unk30 = i + 0x28;
        } else {
            c->unk30 = 0x3C - i;
        }
        if (c->unk14 == 0) {
            if (c->unk18 == 0) {
                c->unk1C = 0;
                c->unk10 = x;
                if (func_800AE878(s, 0) == 5 || s->unk0 == 0x12 || s->unk0 == 0x23 || s->unk5B8 != 0) {
                    x += 0x2A;
                    c->unkC = 0x108;
                    c->unk1C = 1;
                } else {
                    x += 0x34;
                    c->unk1C = 0;
                    c->unkC = 0x104;
                }
            }
            if (s->unk0 == 0x22 && i >= 5) {
                if (func_800AE908(s, s->unk5B0) == c) {
                    c->unkC = 0x16;
                } else {
                    c->unkC = 0x10;
                }
            }
            if (s->unk0 == 0xF && i >= 5) {
                c->unkC = 0x10;
            }
        }
        if (c->unk14 == 1 && c->unk18 == 0) {
            c->unk10 = y;
            y += 0xE;
            c->unkC = 0x10;
        }
        if (s->unk0 == 7) {
            if (a == c) {
                t = s->unk424 * 43;
                m = dx * k / 256 - 0x48;
                c->unkC = t - m;
                t = s->unk428 * 52;
                m = dy * k / 256 - 8;
                c->unk10 = t - m;
                c->unk30 = 0x26;
            }
            if (b == c) {
                t = s->unk42C * 43;
                m = dx * k / 256 + 0x48;
                c->unkC = t + m;
                t = s->unk430 * 52;
                m = dy * k / 256 + 8;
                c->unk10 = t + m;
                c->unk30 = 0x27;
            }
        }
    }
}
