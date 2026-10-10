/* WIP func_800D9D58: 5 differing rows (build/cmp.py-style aligned diff). New in pass 7, modelled on func_800D9AB0. The else path reads u->unkA0 twice (statement 1 reloads it, a = u->unkA0 is taken after the first store and hoisted by sched) and ends in return D_800FEC56 = 1 (v1 copy of the 1). Left: the last unkA0 field load goes to a1 instead of reusing a's v1. */
#include "ovl_0b.h"

s32 func_800D9D58(Unk800D8124 *arg0, s32 arg1, s32 *arg2) {
    s16 rot[4];
    BtlUnit *u;
    s32 v = *arg2;
    BtlUnitA0 *a;

    u = func_800E716C(arg0->unk0);
    if (arg0->unk2 == 0 || func_800D8A64(arg0->unk1) == 0) {
        if (v == 0) {
            arg0->unk8[0] = u->unkA0->unk20 - u->unk86[0];
            arg0->unk8[1] = u->unkA0->unk22 - u->unk86[1];
            arg0->unk8[2] = u->unkA0->unk24 - u->unk86[2];
        }
        func_800E77D8(arg0->unk0, rot);
        rot[0] += arg0->unk8[0] / arg0->unk4;
        rot[1] += arg0->unk8[1] / arg0->unk4;
        rot[2] += arg0->unk8[2] / arg0->unk4;
        func_800E7824(arg0->unk0, rot);
        if (v != arg0->unk4 - 1) {
            goto end;
        }
        D_800FEC56 = 1;
        return 1;
    }
    u->unk86[0] = u->unkA0->unk20;
    a = u->unkA0;
    u->unk86[1] = a->unk22;
    do {
    } while (0);
    u->unk86[2] = a->unk24;
    return D_800FEC56 = 1;
end:
    return 0;
}
