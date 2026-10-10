/* WIP func_800E1534: 4 differing rows (build/cmp.py-style aligned diff). New in pass 7. The loop giv base (t0 = e + 0x24 in the original) follows the last e-> field written in the loop, so e->unk24 is written last here; the original stores it before the tail check (sw a1,0(t0) before lw 0(a1)) and copies the tail pointer to v1 for the unk3C store. Pass 8: storing unk28/unk2C first, then unk24, then the tail check gives the right giv base (6 rows); a second e->unk24 = t after the check gets 3 rows but adds a store; t = e->unk24 reloads, 9 min of permuter: no better. */
#include "ovl_0b.h"

Pool6CEntry *func_800E1534(Pool30 *arg0, Pool6C *arg1, u8 *arg2, s32 arg3) {
    Pool6CEntry *e;
    s32 i;
    u32 k;
    u8 *p;
    EffElemHdr *h;
    EffElem *el;
    EffTail *t;

    e = arg1->unkC;
    for (i = 0; i < arg1->unk0; i++, e++) {
        if (e->unk30 == 0) {
            if (arg1->unk8 < i) {
                arg1->unk8 = i;
            }
            p = arg2;
            for (k = 0; k < 0x6C; k += 4) {
                *(s32 *)((u8 *)e + k) = 0;
            }
            e->unk30 = 1;
            e->unkE = 0;
            e->unk10 = 0;
            e->unk32 = 0;
            e->unk34.s.x = 0;
            e->unk34.s.y = 0;
            e->unk14 = 0;
            e->unk3A = 0xFF;
            e->unk22 = 0;
            e->unk0 = (EffFileHdr *)p;
            if (*p != 0xFF) {
                p += 0x28;
            } else {
                p += 0x10;
            }
            if (((u8 *)e->unk0)[1] != 0) {
                p += 0x38;
                e->unk4 = (EffSub *)p;
                p += 0x28;
            } else {
                e->unk4 = NULL;
            }
            h = (EffElemHdr *)p;
            p += 0x14;
            el = (EffElem *)p;
            e->unk18 = el;
            i = 0;
            p += h->unk4 * 0x28;
            for (; i < h->unk4; i++, el++) {
                if (el->unk0 != 9) {
                    if (el->unk1C != -1 && !(el->unk1C & 0x80000000)) {
                        el->unk1C = (s32)arg2 + el->unk1C;
                    }
                }
                if (el->unk1 != 0xFF) {
                    if (el->unk20 != -1 && !(el->unk20 & 0x80000000)) {
                        el->unk20 = (s32)arg2 + el->unk20;
                    }
                }
            }
            t = (EffTail *)p;
            if (t->unk0 == -1) {
                *(s32 *)&t->unk3C = 0;
            }
            e->unk28 = (SVECTOR *)arg3;
            e->unk2C = arg3;
            e->unk24 = t;
            return e;
        }
    }
    return NULL;
}
