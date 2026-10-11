/* WIP func_800DA138: 6 differing rows (build/cmp.py-style aligned diff). only regs: stride v1 / counter a1 in the original, swapped here (permuter found nothing in 6 min). Pass 7: decl order, i/sz types, arg2/arg4 reused for sz or the counter, loop forms and do-while insertion: no better. Pass 8: sz split into two statements, i * sz indexing, unk2/unk0 reloads, 9 min of permuter: no better. Pass 13 (build/gsim.py): the original order needs sz allocated before p and i (sz 4 refs/11 = 0.73, p 7/13 = 1.08, i 8/12 = 2.0), or i skipping v1/a0 in pass 0 because sz full-prefers v1 and p a0; no source form found for either (types, while/for forms, i * sz indexing, sz += 4). Pass 14: arg1 reused as the counter (with `t = arg1` copied at entry for the unk8 store, `arg1 = 0` after the call so CSE keeps t) gets the counter out of the s-reg, but the entry copy becomes t = a1 directly, so the counter pseudo has no a1 preference (10 rows); copying arg0 for p: worse. Pass 15: goto loop (no loop notes, unweighted refs: i 5/12, p 4/13, sz 3/11) keeps the same order (6 rows); do { } while (0) around the sz statements or the p += sz raises sz/p refs but also changes live lengths (8-17 rows). Loop forms (for with comma increment, pre-tested do-while, while with i++ first): 6-7. Pass 17: loop moved into a static inline helper clr(p, n, sz), chained `arg0->unk0 = sz = ...`, while loop with i++ before p += sz: 6 rows. In find_reg pass 0, i (allocated first) takes v1 unless v1 is in regs_someone_prefers[i]. That means sz needs a full preference for v1, or i must sort after sz. Nothing in the source gives sz a v1 preference. Pass 18: the DA1B8 neighbour as an inline (clr(arg0), reloading unk0/unk2 each iteration): 20 rows; clr(arg0, sz, arg3), clr(arg0->unkC, sz, arg3) and the whole body as pool_init(arg0, arg1, func_800CFC0C(arg2) + 4, arg3, arg4): 6. 25 min of permuter on the inline form: nothing below the base. Priorities from the .lreg dump: sz 4 refs / 11 insns, i 8 / 12, p 7 / 13. For i to skip v1, sz would need a v1 preference (nothing in the source gives one), or sz would have to sort before i. Pass 19 (read global.c find_reg/prune_preferences/set_preference): every call-used register is in regs_used_so_far from the start, so pass 0 only skips regs_someone_prefers (pruned full preferences of lower-priority conflicting allocnos), and preferences only point at hard registers or local-alloc'd pseudos (set_preference goes through reg_renumber). p = arg0 + 12 gives p no preference, sz = v0 + 4 only v0 (pruned, conflicts). Live-length shifts can't put sz above i (2.0; sz would need len <= 4). So the original needs either i to skip v1 and a0 (sz preferring v1 and p preferring a0) or i with a copy preference for a1 plus sz above p (len_p > 1.75 len_sz). An a1 copy preference needs a reused param whose range doesn't cross the call: arg1 crosses it (unk8 store after the call), arg2 would give a2. Not tested further. */
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
