/*
 * ovl_0d func_800AC060 (0x468 bytes): the card-flip/capture animation of a battle: positions the
 * two fighting cards, draws the clash effect sprite for frames 0x23..0x42, then picks the loser
 * (random rolls) and hands over to the result state.
 * WIP (pass 10): 283 vs 282 instructions, 19 aligned diff rows (was ~91).
 * Fixed: the loser card variable is the same one as the first card: `a = b;` in the else arm of
 * the final if, and `switch (a->unk3)` after it (the original copies s4 into s0 there); the effect
 * branch computes `o = ot + 30` after the (s->unk2E8 - 0x23) / 16 division, so o is block-local
 * (local-alloc gives it s0, which pushes s/p to s1/s2 like the original).
 * Left: a (first card) should be s0 and ot s3; here ot gets s0 and a s3. ot gets a hard-reg
 * preference for s0 from `o = ot + 30` (o is s0, ot dies there), so global-alloc keeps a off s0
 * (regs_someone_prefers) and a lands in s3. In the original that preference evidently doesn't
 * exist. Tried: o as a copy/&ot[30]/byte-offset form, AddPrim(ot + 30) without o, h/new temps.
 * Pass 11 (new build/search/cmp.py, same 19 rows): one `a = b;` is enough (the second copy did nothing).
 * Allocation dump (cc1 -dg): e (5 refs / 28 insns) outranks a (6 / 107), takes s0 first, and a ends up
 * in s3. In the original e shares s3 with ot (e is born after ot dies) and a gets s0, so a must outrank
 * e there. Assigning e right before the if (with `s->unk350[0].unk0 = 3` direct) gives a s0 but e no
 * longer crosses func_800ACFC8 and ot/b swap s3/s4 (18 rows). dup.py: `if (e)` around the effect
 * branch's `s->unk574 = pos.unk0` gets 13 rows (fake). Permuter 25 min: nothing.
 */
#include "ovl_0d.h"

void *func_800AC060(u32 *ot, void *vp, TmState *s) {
    SPRT *p = vp;
    TmPos pos;
    TmCard *a;
    TmCard *b;
    TmUnk350 *e;
    s32 t;
    s16 x;
    s16 y;
    s32 h;
    u8 c;
    u32 *o;

    a = func_800AEE08(s, s->unk424, s->unk428);
    b = func_800AEE08(s, s->unk42C, s->unk430);
    func_800AC4C8((s32)s, &pos, a, b);
    s->unk57C = 0x3C;
    if (s->unk2E8 == 0x23) {
        func_800B01E8(0xB);
    }
    if (s->unk2E8 < 0x23) {
        s->unk574 = pos.unk0;
        s->unk578 = pos.unk4;
    } else if (s->unk2E8 < 0x43) {
        x = (s->unk424 + s->unk42C) * 43 / 2 + 0x48;
        y = (s->unk428 + s->unk430) * 26 + 9;
        c = s->unk2EC;
        h = 0x30;
        if (s->unk2E8 - 0x23 < 0xF) {
            h = 0x28;
        }
        setlen(p, 4);
        p->code = 0x66;
        p->x0 = x;
        p->y0 = y;
        p->r0 = c;
        p->g0 = c;
        p->b0 = c;
        p->w = 0x28;
        p->h = h;
        p->u0 = (((s->unk2E8 - 0x23) & 0xF) >> 2) * 40;
        p->v0 = ((s->unk2E8 - 0x23) / 16) * 40;
        o = ot + 30;
        p->clut = 0x3B00;
        AddPrim(o, p);
        p++;
        setlen((DR_TPAGE *)p, 1);
        ((DR_TPAGE *)p)->code[0] = (GetTPage(1, 1, 0x200, 0x100) & 0x9FF) | 0xE1000000;
        AddPrim(o, p);
        s->unk574 = pos.unk0;
        p = (SPRT *)((DR_TPAGE *)p + 1);
        s->unk578 = pos.unk4;
    } else {
        if (s->unk2E8 == 0x43) {
            s->unk5BC = func_80062B3C() % pos.unk0;
            s->unk5C0 = func_80062B3C() % pos.unk4;
        }
        s->unk574 = pos.unk0 - (pos.unk0 - s->unk5BC) * (s->unk2E8 - 0x43) / 23;
        s->unk578 = pos.unk4 - (pos.unk4 - s->unk5C0) * (s->unk2E8 - 0x43) / 23;
        if (s->unk2E8 >= 0x5A) {
            e = s->unk350;
            func_800ACFC8(s);
            e->unk0 = 3;
            s->unk504 = 3;
            s->unk0 = 5;
            func_800B01E8(9);
            if (s->unk5C0 < s->unk5BC) {
                e->unk4 = s->unk42C;
                e->unk8 = s->unk430;
                s->unk420 = 9;
                s->unk41C = s->unk434;
            } else {
                if (s->unk434 == 0) {
                    s->unk41C = 1;
                } else {
                    s->unk41C = 0;
                }
                e->unk4 = s->unk424;
                e->unk8 = s->unk428;
                a = b;
                s->unk420 = 0xA;
            }
            switch (a->unk3) {
            case 0:
            case 1:
                if ((func_80062B3C() & 0x3F) == 0) {
                    a->unk3 = 2;
                }
                break;
            case 2:
                if ((func_80062B3C() & 0x7F) == 0) {
                    a->unk3 = 3;
                }
                break;
            }
            s->unk574 = s->unk5BC;
            s->unk578 = s->unk5C0;
        }
    }
    return p;
}
