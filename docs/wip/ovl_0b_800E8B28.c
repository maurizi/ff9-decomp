/* WIP func_800E8B28: 17 differing rows (build/cmp.py-style aligned diff). the original materialises `arg2 ? 0x14 : 0` as a value (beqz over addiu) for every access and recomputes the store and load addresses separately; gcc folds the ternary into the address here. */
#include "ovl_0b.h"

/* weapon texture animation: arg1 0 = start anim (arg2 ? 1 : 0), 1 = stop anim 2, 2 = start anim 2 */
void func_800E8B28(s32 arg0, s32 arg1, s32 arg2) {
    BtlUnit *u = func_800E716C(arg0);

    switch (arg1) {
    case 0:
        u->unk9C->unk50[arg2 ? 1 : 0].unk4 |= 3;
        u->unk9C->unk50[arg2 ? 1 : 0].unkC = 0;
        u->unk9C->unk50[arg2 ? 1 : 0].unk12 = 0x1000;
        break;
    case 1:
        u->unk9C->unk50[2].unk4 &= ~3;
        break;
    case 2:
        u->unk9C->unk50[2].unk4 |= 1;
        u->unk9C->unk50[2].unkC = 0;
        u->unk9C->unk50[2].unk12 = 0x1000;
        break;
    }
}
