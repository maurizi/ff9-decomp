/* WIP func_800DA138: 6 differing rows (build/cmp.py-style aligned diff). only regs: stride v1 / counter a1 in the original, swapped here (permuter found nothing in 6 min). */
#include "ovl_0b.h"

void func_800DA138(Unk800DA1B8 *arg0, Unk80100C78Entry *arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 sz;
    s32 i;
    u8 *p;

    sz = func_800CFC0C(arg2) + 4;
    arg0->unk0 = sz;
    arg0->unk2 = arg3;
    arg0->unk8 = (Unk800DA1B88 *)arg1;
    arg0->unk4 = (void *)arg4;
    p = (u8 *)arg0->unkC;
    for (i = 0; i < arg3; i++) {
        *(s32 *)p = 0;
        p += sz;
    }
}
