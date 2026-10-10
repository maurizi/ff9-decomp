/* STATUS: near miss, 5 rows. ovl_36 func_801F1A48 (0xA0, start the CD read of a card picture). The u32 x = c->unk3 temp gives the original's andi; left: the original sets a1 = 0x800 and a2 early and keeps x in t0, ours puts x in a1 and loads 0x800 last. Statement permutations and permuter (2 x 10 min, plus 20 min in pass 4) did not find it; pass 4 also tried a size local (at the top, before/after each store; constant propagation removes it), argument spellings, do { } while (0) around every statement, and static inline wrappers for the read. Pass 5 also tried the stores folded into the first call argument and a buf local before the stores (both worse). */
#include "ovl_36.h"

void func_801F1A48(void) {
    CardCache *c = &D_801F347C.unk1900;
    s32 id;
    u32 x;

    if (c->unkA != 0) {
        if (func_80022B18() != 0) {
            return;
        }
        c->unkA = 0;
        func_801F1778();
    }
    id = c->unk4;
    if (id != 0xFF) {
        c->unk4 = 0xFF;
        c->unk3 = id;
        D_801F347C.unk1900.unkA = 1;
        x = c->unk3;
        D_801F347C.unk4 = x;
        func_80022C60(D_8006794C->unk20 + D_801F347C.unk18EC->unk4 + x, 0x800, D_801F347C.unk10D0, NULL);
    }
}
