/* 3 rows differ: the slti/xori registers are swapped (lt in v0 here, v1 in the original) and the slot < 8 branch sense is inverted. With a kind local (vk_a form) the branch sense matches but the registers stay swapped (4 rows). */
#include "ff9.h"

static inline s32 key_inline(s32 slot) {
    s32 key = slot;
    if (slot >= 9) {
        key = slot - 4;
    }
    return key;
}

static inline s32 find(s32 key, s32 slot) {
    Character **p;
    Character *c;
    s32 kind;
    s32 ge;
    s32 lt;

    for (p = D_8006794C->unk1C->unk54C; p < &D_8006794C->unk1C->unk54C[4]; p++) {
        c = *p;
        if (c == NULL) {
            continue;
        }
        if (c->unk34.b.id != key) {
            continue;
        }
        kind = c->unk34.b.unk4;
        ge = kind >= 14;
        if (slot < 8) {
            if (ge) {
                continue;
            }
        } else {
            if (kind < 14) {
                continue;
            }
        }
        break;
    }
    return p < &D_8006794C->unk1C->unk54C[4];
}

s32 func_8004B85C(s32 slot) {
    return find(key_inline(slot), slot);
}
