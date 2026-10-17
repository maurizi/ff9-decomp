/* STATUS (pass 25): 13 rows, all of them one register swap: cnt (0x1F801DAA) is a3 here and a2 in the original, &g_akaoChannels / the first loop's giv a2 here and a3 in the original. Everything else, including the late in-place giv init (`addiu a3,a3,0x88` before the D_80080A2C store) and g_akaoSeq = &g_akaoMainSeq as the first store, now matches. Behaviourally equivalent: tools/equiv.py 300/300 trials agree, 100% of the original's instructions ran (needs the func_8005C86C prototype below; without it equiv.py compares a3 at that call, which is just the swapped leftover). Pass-17 notes below describe the older 10-row draft (giv init early). 12 rows (pass 17; 14 before): SPU init. The original reads *cnt (0x1F801DAA) early into s0, the u16 counter i (i = *cnt; ...; *cnt = (i & ~5) | 1); `g_akaoSeq = &g_akaoMainSeq` after the D_8007F8D8 stores (51 -> 14, found by build/mvall.py). Remaining: the original materialises &g_akaoChannels in a3 at the D_800809EC store and does a3 += 0x88 (the first loop's giv) just before the SPU register stores, with &g_akaoMainSeq in v1 from the first instruction; here &g_akaoChannels and its +0x88 giv are hoisted to the top. In the second loop the original increments i right after reading it for unk118. Tried: moving the D_800809EC/g_akaoSeq/D_8007F790/i = *cnt lines anywhere, c = g_akaoChannels forms, i++ placement, dup sweep. Pass 14: in the pre-sched RTL the giv init (`281 = 88 + 136`, 88 = &g_akaoChannels) is the last insn of block 0; sched1 moves it up next to 88's definition because it kills 88 (2.8 defers scheduling insns with REG_DEAD notes), so 88 dies early and v1 is reused for &g_akaoMainSeq. In the original the init stays late and is in place (addiu a3,a3,0x88), as if 88 were still live after it. A 20-min permuter run found only junk (an uninitialised `c->unk94 = 0` store before the SPU stores gives the in-place, late init but 27 rows). A separate pointer for the second loop and `c = D_800809EC` change nothing. Pass 16: none of the 17 scalar globals written here changes anything as a struct field (g_akaoSeq as a field: 37). Pass 17: second loop as `for (i = 8; i < 24;) { c->unk34 = 0; c->unk118 = i; i++; ... }` gives the original's `andi v0,s0; addiu s0,s0,1` order (14 -> 12; `c->unk118 = i++` gives a move + andi instead). Volatile cnt / SPU registers and moving `i = *cnt` before the SPU stores change nothing. sched.c note: a giv init is set twice (init + loop increment), so it never gets adjust_priority's birthing boost and sched1 always places it right after its input's definition; something has to make it depend on a late insn. Pass 18: loop.c emits the giv init from the biv's REG_EQUAL constant initial value (a fresh lui/addiu g_akaoChannels + 136 in the preheader), and cse2 turns it into `88 + 136`. `D_800809EC = c = g_akaoChannels;` at the top with an init-less for loop: 19 (same early init, and the second loop gets worse). `c = D_800809EC` before the loop: 12. Index forms g_akaoChannels[i] / c[i] / c = &g_akaoChannels[i]: 27-32. Coordinator tip (loop invariants written inside the loop shift move_movables' threshold): the 0x18 store constant is already hoisted as in the original, and putting `cnt = 0x1F801DAA` or the g_akaoSeq store inside loop 1 gives 12 and 67. The early giv init comes from strength reduction plus sched1, not from move_movables. Pass 19: not re-tested. Pass 20 (sched1 dump, -dS): the giv init (insn 630, `281 = 88 + 136`) is on the initial ready list with priority 1, the same as almost every store, and rank_for_schedule would pick it first (latest LUID, class 3). What keeps it out is schedule_select's function-unit check: at every step the dump says "insn N has a greater potential hazard, now N 630 ...", so each remaining store goes ahead of the ALU giv init until only 88's definition is left above it. In the original it was placed just above the `sh D_80080A2C` / `lhu *cnt` group, i.e. at a point where the potential-hazard test let it win. The place to look is therefore the memory-unit hazard state (the order and number of loads and stores just below that point), not data dependencies. Pass 24: not changed. The loop.c threshold effect that fixed 8001DE50 (each hoisted movable lowers move_movables' threshold by 3) doesn't apply here: the early giv init comes from strength reduction plus sched1, not from move_movables. Pass 25: what pins the giv init is sched1's list scheduler, not loop.c. Three mechanisms (sched.c 2.8.1): (1) when an insn is scheduled, a predecessor that becomes ready gets LAUNCH priority (adjust_priority) only if it is "birthing", i.e. its dest has REG_N_SETS == 1; (2) within one priority group schedule_select takes the insn with the largest potential hazard, so loads/stores always win over ALU insns, and a load is blocked for a cycle right after a store; (3) the 34th memory reference in a block (sched_analyze_1, pending_lists_length > 32) flushes the pending lists and becomes a barrier for all memory ops before and after it (here the D_80080A2C store). The giv init is ALU, not birthing (set in the preheader and in the loop) and has no successor in the block, so it only gets scheduled at a "gap": a cycle where no unblocked memory insn and no LAUNCH insn is ready. With the SPU stores and D_800809B8/AC4/831C4 written after the D_80080A70/g_akaoMainSeq field stores, the only post-barrier memory insn left at the end of the bottom-up pass is the lhu of *cnt, which is blocked by the field store before it, and the previous store (sw zero,0x1C(a1)) releases no birthing %hi. That gap takes the giv init, so it lands right after the lhu, &g_akaoChannels lives across the block and the giv is in place; sched2 then moves it above the D_80080A2C store exactly like the original (12 rows -> structure match; g_akaoSeq first now works too). Remaining a2/a3 swap is local-alloc order: the &g_akaoChannels qty is tied to its %hi (5 refs, about 116 half-insns) and cnt has 3 refs at flow time (the lui/ori split happens after flow) over about 212, so QTY_CMP_PRI ranks &g_akaoChannels first and it takes a2. The original needs cnt allocated first: cnt with 8+ refs, &g_akaoChannels untied and cnt with 5+, cnt's life under 0.3x the other's, or &g_akaoChannels global (then local-alloc gives cnt a2 and global-alloc gives the pointer a3, its giv following by preference). Tried without success: i = *cnt and cnt = ... at every position (no effect, the scheduler places them), loop forms (c = g_akaoChannels at the top, c[i], index forms, c = D_800809EC, do-while: 13-24), a `do { } while (0);` barrier at every boundary (loop notes are full sched1/sched2 barriers and double REG_N_REFS inside them; 19+), and a 30-min hill climb over statement order from this draft. */
#include "ff9.h"

void func_8005C86C(s32 mode);

void func_8005681C(void) {
    u16 i;
    u16 *cnt;
    AkaoChannel *c;

    g_akaoSeq = &g_akaoMainSeq;
    cnt = (u16 *)0x1F801DAA;
    D_8007F790 = NULL;
    D_800809EC = g_akaoChannels;
    D_8007F71C = NULL;
    D_8007F8D8[1] = 0;
    D_8007F8D8[0] = 0;
    D_800809C0[5] = 0;
    D_800809C0[4] = 0;
    D_800809C0[3] = 0;
    D_800809C0[2] = 0;
    D_800809C0[1] = 0;
    D_8008314C = 0;
    D_800809C0[0] = 0;
    D_8007F8E0 = 1;
    D_80083158.unk0 = 0;
    D_80083158.unk4 = 1;
    D_80080A70.unk0 = 0;
    g_akaoMainSeq.unk4 = 0;
    g_akaoMainSeq.unk8 = 0;
    g_akaoMainSeq.unk6A = 0;
    D_80080A70.unk10 = 0;
    g_akaoMainSeq.unk1C = 0;
    D_8007F8F0.unk6A = 0;
    D_8007F8F0.unk4 = 0;
    g_akaoMainSeq.unk50 = 0x7F0000;
    g_akaoMainSeq.unk5C = 0x400000;
    D_80080AC0 = 0x7FFF0000;
    D_8007F96C = 0;
    D_80083150 = 0;
    g_akaoMainSeq.unk58 = 0;
    g_akaoMainSeq.unk64 = 0;
    D_8007F96E = 0;
    D_80083154 = 0;
    D_80080A2C = 0;
    i = *cnt;
    D_80080A70.unk1C = 0;
    g_akaoMainSeq.unk3C = 0;
    D_80080A70.unk20 = 0;
    g_akaoMainSeq.unk40 = 0;
    D_80080A70.unk24 = 0;
    g_akaoMainSeq.unk44 = 0;
    g_akaoMainSeq.unk74 = 0;
    g_akaoMainSeq.unk72 = 0;
    g_akaoMainSeq.unk70 = 0;
    g_akaoMainSeq.unk78 = 0;
    *(u16 *)0x1F801D80 = 0x3FFF;
    *(u16 *)0x1F801D82 = 0x3FFF;
    *(u16 *)0x1F801DB0 = 0x7FFF;
    *(u16 *)0x1F801DB2 = 0x7FFF;
    D_800809B8 = 0;
    D_80080AC4 = 0;
    D_800831C4 = 0;
    D_80083168.unk40 = 0x7F00;
    D_80083168.unk48 = 0;
    D_80083238[2] = 0;
    D_80083238[1] = 0;
    D_80083238[0] = 0;
    D_80080A20 = 0;
    *cnt = (i & ~5) | 1;
    for (c = g_akaoChannels, i = 0; i < 32; i++, c++) {
        c->unk34 = 0;
        c->unk118 = 0x18;
        c->unk94 = 0;
        c->unk88 = 0;
    }
    c = D_8007E3D8;
    for (i = 8; i < 24;) {
        c->unk34 = 0;
        c->unk118 = i;
        i++;
        c->unk94 = 1;
        c->unk88 = 0;
        c->unk100 = 0x7F00;
        c->unkBC = 0;
        c->unkB6 = 0;
        c->unk70 = 0;
        c->unkBA = 0;
        c++;
    }
    g_akaoSeq->unk18 = 0;
    g_akaoSeq->unk14 = 0;
    g_akaoSeq->unk10 = 0;
    D_80080A70.unk18 = 1;
    D_80080A70.unk14.w = 0x66A80000;
    D_80080A70.unkC = 0;
    D_80080A70.unk8 = 0;
    D_80080A70.unk4 = 0;
    g_akaoSeq->unk48 = 0x3FFF000;
    g_akaoSeq->unk4C = 0;
    g_akaoSeq->unk66 = 0;
    D_80083158.unk8 |= 0x80;
    func_8005C86C(4);
    SpuSetReverb(1);
    for (i = 0; i < 24; i++) {
        func_800576BC(i, 0x1030);
    }
    D_8007E3C8[1] = 0;
    D_8007E3C8[0] = 0;
    D_8007E3D0 = 0;
}
