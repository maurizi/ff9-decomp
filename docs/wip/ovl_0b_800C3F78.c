/* WIP func_800C3F78: 3 differing rows (build/cmp.py, -O1 -G0), of which only one is real: the case-5 test (beq->case5 + j other here, bne->other + j case5 in the original). The other two rows are the D_800FE36C vs D_800FE368+4 relocation names, which link identically. Pass 17: the table-base order is fixed by `idx = ...; tb = D_800F5758; t = &tb[idx];` (base lui/addiu before the sll, sum in idx's register a0), the same with `t = tb + idx` / `idx + tb`; 2D-array forms (D[][19]) and pointer-arith casts are worse (8-34). Reorg: fill_eager_delay_slots tries the target thread even when prediction <= 0 (reorg.c 3944), and mark_target_live_regs for the `j other` fallthrough follows that jump and finds v0 dead at other, so the case-5 lui is always fillable in pass 1 unless something else makes it unmovable; with the beq filled, relax_delay_slots never swaps. Re-tested with the table fix: do { case 5: } while (0), for (;;) { case 5: ... break; }, while (1) { ...; break; }, goto five into an if (0) do-while: all 3 rows. A pass-1-unfilled beq would need its target to be a jump (loop entry jump that jump.c's follow_jumps refuses to thread before reload), but the post-reload jump pass threads it again. Older notes: Pass 7: no change from default-first, case 5 before case 1 (6 rows: gives the bne->other but body order flips), if/else chain, switch index casts (s16 worse), found-label gotos. gcc emits leaf tests as beq + final j default, so the bne + j c5 must come from jump/reorg with case 5's label right after `j other` at that time. Pass 8: case 5 as `goto five` with its body after case 1, default placed first/middle: no change (jump threading removes the extra block). Pass 14: the bne + j case5 is reorg's relax_delay_slots swap (`if (x) goto A; goto B;` becomes `if (!x) goto B; goto A;`), which needs mostly_true_jump on the case-5 beq to predict it very unlikely; here it does not. Switch/default placements, if/else-if, `default: return`, switch on an idx temp: no change. The table-base order is fixed by `idx = (s32)&t[idx]` with t = D_800F5758 (base loaded before the sll) but then idx's registers differ (7 rows); `t = D_800F5758; t = &t[idx]` gets the order with the sum in the base's register (10). Pass 15 (read gcc 2.8.1 reorg.c): the swap in relax_delay_slots fires only when `0 < mostly_true_jump (beq)`, i.e. the case-5 beq must be predicted *likely*, not unlikely, and it must still be unfilled when relax runs. For an EQ test whose fallthrough is `j other`, mostly_true_jump can only return > 0 through loop notes (NOTE_INSN_LOOP_BEG among the notes before the target label: 2; LOOP_VTOP right after it: 1); rare_destination is 0 for any target that starts with a label, so noreturn calls or a goto to the end can't help. A case label inside `do { case 5: ... } while (0)` does give prediction 2 (checked in the dbr dump), but then fill_eager_delay_slots fills the beq from its target first (steals the lui) and relax no longer sees a plain condjump, so no swap: 5 rows. The original's final shape (bne other [lui from case 5]; j L4004 [nop]) also can't come from pass-2 fill_simple/eager after a pass-1 swap as far as I can trace (the j would get the lui). Unresolved. Pass 16 (reading reorg.c, not tested): if a pass-1 relax swap gives `bne other; j C5`, pass 2 fill_simple_delay_slots fills `j C5` from its target (steals the lui, retargets to the lw, i.e. L4004), and fill_eager_delay_slots for the bne can then take the lui from the j's delay slot with steal_delay_list_from_fallthrough, which leaves `j L4004; nop` as in the original. So the open question is only how the pass-1 swap happens while the beq is unfilled. Pass 18: not re-tested. Note from reading mostly_true_jump: besides loop notes it returns >0 when rare_destination(fallthrough) > rare_destination(target). The target always starts with a label (0), so the beq fallthrough (`j other`) would have to reach a RETURN, a barrier after a noreturn call, or the end of the function without passing a label. Pass 19: not re-tested. Pass 20: not re-tested at the source level. For the pass-1 relax swap, mostly_true_jump only gives > 0 to the EQ case-5 test through the loop notes (reorg.c 1396-1410). The rare_destination path can't, because both threads reach a label. The NE/LE/GE rules don't apply to the decision tree's do_jump_if_equal. With prediction 2, fill_eager_delay_slots tries the target first and steals the lui unless the lui sets something the fallthrough (`j other`) needs. So a variant whose case-5 first insn writes a register that is live at `other` would leave the beq unfilled and let relax swap it. Pass 23: extra switch cases that also go to other (case 0, 2-4, 6, with and without default): 5-12 rows. */
#include "ovl_0b.h"

void func_800C3F78(Unk800C412C *arg0, Unk800C3F78 *arg1) {
    BtlUnit *u;
    Unk800F5758 *t;
    s32 idx;
    Unk800F5758 *tb;

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
            idx = ((D_8006794C->unk1C->unk3C[(u->unk68 >> 1) & 0xF].unk34.w >> 4) & 0x1F) +
                  (arg1->unk2_14 - 1) * 19;
            tb = D_800F5758;
            t = &tb[idx];
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
