/* WIP func_800EFD1C: 18 differing rows (build/cmp.py-style aligned diff). first loop: found path should set cur in the beq delay slot (cur = 0 in the bnez slot); side = !k should reuse k (andi a0,0xFF sits in the unk8 beqz delay slot); second loop u/list-pointer regs a0/a1 swapped. */
#define func_800EFD1C func_800EFD1C_hdr
#include "ovl_0b.h"
#undef func_800EFD1C

/* switches the target cursor to the other side (arg0: side currently targeted); returns 1 if it moved */
s32 func_800EFD1C(u8 arg0) {
    BtlUnit *cur;
    BtlUnit *u;
    BtlUnit *list[4];
    s32 n;
    s32 i;
    u8 side;

    cur = NULL;
    for (u = D_8006794C->unk1C->unk8E4.battle->unk6F8; u != NULL; u = u->unk0) {
        if ((u->unk68 & 0x30000000) == 0x30000000) {
            cur = u;
            break;
        }
    }
    if (D_80104FF0.unk8 != 0) {
        if (arg0 == (cur->unk68 & 1)) {
            return 0;
        }
        side = !(cur->unk68 & 1);
        D_80104FF0.unkA = side;
        func_8001D898(list, 0, sizeof(list));
        n = 0;
        for (u = D_8006794C->unk1C->unk8E4.battle->unk6F8; u != NULL; u = u->unk0) {
            if (((u->unk68 >> 28) & 1) && (u->unk68 & 1) == side) {
                if (!(u->unk3C & 0x100) || D_80104FF0.unkB) {
                    list[n++] = u;
                }
            }
        }
        if (n != 0) {
            cur->unk68 &= ~0x20000000;
            if (D_80104FF0.unk6 == 0) {
                list[0]->unk68 |= 0x20000000;
            } else {
                func_800F142C();
                for (i = 0; i < n; i++) {
                    list[i]->unk68 |= 0x20000000;
                }
            }
            return 1;
        }
    }
    return 0;
}
