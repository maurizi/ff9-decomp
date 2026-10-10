/* WIP func_800AE4FC: 5 differing rows (build/cmp.py-style aligned diff). the table base must end up in a0 (index in v1) and be loaded after the NULL check (lui a0 in the beqz delay slot); a local table pointer gets a0 but hoists the lui above the branch. */
#include "ovl_0b.h"

s32 func_800AE4FC(s32 arg0) {
    Character *c = D_8006794C->unk1C->unk54C[arg0];
    Unk8006E0E8 *w = D_8006E0E8;

    if (c == NULL) {
        return -1;
    }
    return ((FileEntry16 *)func_8001E218(8, w[c->unk39[0]].unk2))->unk2 | 0x90000;
}
