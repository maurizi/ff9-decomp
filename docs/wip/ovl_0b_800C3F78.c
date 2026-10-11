/* WIP func_800C3F78: 3 differing rows (build/cmp.py-style aligned diff, -O1 -G0). -O1 unit. Left: case 5 test is bne->other + j case5 in the original (beq->case5 + j other here), and the table base lui/addiu comes before the index sll. Pass 7: no change from default-first, case 5 before case 1 (6 rows: gives the bne->other but body order flips), if/else chain, switch index casts (s16 worse), found-label gotos. gcc emits leaf tests as beq + final j default, so the bne + j c5 must come from jump/reorg with case 5's label right after `j other` at that time. Pass 8: case 5 as `goto five` with its body after case 1, default placed first/middle: no change (jump threading removes the extra block). Pass 14: the bne + j case5 is reorg's relax_delay_slots swap (`if (x) goto A; goto B;` becomes `if (!x) goto B; goto A;`), which needs mostly_true_jump on the case-5 beq to predict it very unlikely; here it does not. Switch/default placements, if/else-if, `default: return`, switch on an idx temp: no change. The table-base order is fixed by `idx = (s32)&t[idx]` with t = D_800F5758 (base loaded before the sll) but then idx's registers differ (7 rows); `t = D_800F5758; t = &t[idx]` gets the order with the sum in the base's register (10). Pass 15 (read gcc 2.8.1 reorg.c): the swap in relax_delay_slots fires only when `0 < mostly_true_jump (beq)`, i.e. the case-5 beq must be predicted *likely*, not unlikely, and it must still be unfilled when relax runs. For an EQ test whose fallthrough is `j other`, mostly_true_jump can only return > 0 through loop notes (NOTE_INSN_LOOP_BEG among the notes before the target label: 2; LOOP_VTOP right after it: 1); rare_destination is 0 for any target that starts with a label, so noreturn calls or a goto to the end can't help. A case label inside `do { case 5: ... } while (0)` does give prediction 2 (checked in the dbr dump), but then fill_eager_delay_slots fills the beq from its target first (steals the lui) and relax no longer sees a plain condjump, so no swap: 5 rows. The original's final shape (bne other [lui from case 5]; j L4004 [nop]) also can't come from pass-2 fill_simple/eager after a pass-1 swap as far as I can trace (the j would get the lui). Unresolved. */
#include "ovl_0b.h"

void func_800C3F78(Unk800C412C *arg0, Unk800C3F78 *arg1) {
    BtlUnit *u;
    Unk800F5758 *t;
    s32 idx;

    func_80063864(&arg0->unk0[1], &arg0->unk0[0], 0xC);
    arg0->unk0[1].unk8 = arg1->unk0;
    arg0->unk0[1].unk9 = arg1->unk1;
    if (arg1->unk2_14 != 0) {
        switch (arg1->unk0 & 0x1F) {
        case 1:
            u = D_800FE368.unk0;
            break;
        case 5:
            u = D_800FE368.unk4;
            break;
        default:
            goto other;
        }
        if (u->unk68 & 1) {
            t = &D_800F5758[((D_8006794C->unk1C->unk3C[(u->unk68 >> 1) & 0xF].unk34.w >> 4) & 0x1F) +
                            (arg1->unk2_14 - 1) * 19];
            arg0->unk0[1].unk0[3] = t->unk0 << 6;
            arg0->unk0[1].unk0[0] = t->unk1 << 4;
            arg0->unk0[1].unk0[1] = t->unk2 << 4;
            arg0->unk0[1].unk0[2] = t->unk3 << 4;
            return;
        }
    }
other:
    arg0->unk0[1].unk0[0] = arg1->unk2_0 << 5;
    arg0->unk0[1].unk0[1] = arg1->unk2_7 << 5;
    arg0->unk0[1].unk0[2] = arg1->unk4_0 << 5;
    arg0->unk0[1].unk0[3] = arg1->unk4_7 << 7;
}
