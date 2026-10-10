/* WIP func_800C0C00: 5 differing rows (build/cmp.py-style aligned diff). regs/scheduling: v*4 must be computed at the join (sll a2,v0,2 right after the if chain) with v in v0; per-branch *4 cross-jumps but reload_cse turns the k=2 branch's sll into sllv a3. Pass 7: `v = (expr) * 4` per branch gives the v0/a2 registers but no cross-jump (7 rows); t = v * 4 / v <<= 2 / operand order / v types / do-while insertion / 9 min permuter: no change. sched1 keeps the sll next to the mult because it kills v (priority halved). Pass 8: t = v * 4 / v *= 4 as their own statements, casts on v * 4: the sll still lands next to the mult (5-7 rows). */
#include "ovl_0b.h"

void func_800C0C00(BtlUnit *u, s32 n) {
    s32 k;
    s32 v;
    s32 t;

    if (n == 1) {
        v = u->unk33;
        k = 0;
    } else if (n == 0x10) {
        v = u->unk33;
        k = 1;
    } else {
        v = 0x3C - u->unk33;
        k = 2;
    }
    u->unk40[k] = D_8006794C->unk1C->unk8E4.battle->unk154C[n].unk1 * (v * 4);
}
