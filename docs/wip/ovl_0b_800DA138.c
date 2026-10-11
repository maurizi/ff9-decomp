/* WIP func_800DA138: 6 differing rows (build/cmp.py-style aligned diff). only regs: stride v1 / counter a1 in the original, swapped here (permuter found nothing in 6 min). Pass 7: decl order, i/sz types, arg2/arg4 reused for sz or the counter, loop forms and do-while insertion: no better. Pass 8: sz split into two statements, i * sz indexing, unk2/unk0 reloads, 9 min of permuter: no better. Pass 13 (build/gsim.py): the original order needs sz allocated before p and i (sz 4 refs/11 = 0.73, p 7/13 = 1.08, i 8/12 = 2.0), or i skipping v1/a0 in pass 0 because sz full-prefers v1 and p a0; no source form found for either (types, while/for forms, i * sz indexing, sz += 4). */
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
