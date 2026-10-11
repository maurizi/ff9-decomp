/* 12 rows (pass 17; 14 before): SPU init. The original reads *cnt (0x1F801DAA) early into s0, the u16 counter i (i = *cnt; ...; *cnt = (i & ~5) | 1); `D_80080A10 = &D_8007F858` after the D_8007F8D8 stores (51 -> 14, found by build/mvall.py). Remaining: the original materialises &D_8007BD48 in a3 at the D_800809EC store and does a3 += 0x88 (the first loop's giv) just before the SPU register stores, with &D_8007F858 in v1 from the first instruction; here &D_8007BD48 and its +0x88 giv are hoisted to the top. In the second loop the original increments i right after reading it for unk118. Tried: moving the D_800809EC/D_80080A10/D_8007F790/i = *cnt lines anywhere, c = D_8007BD48 forms, i++ placement, dup sweep. Pass 14: in the pre-sched RTL the giv init (`281 = 88 + 136`, 88 = &D_8007BD48) is the last insn of block 0; sched1 moves it up next to 88's definition because it kills 88 (2.8 defers scheduling insns with REG_DEAD notes), so 88 dies early and v1 is reused for &D_8007F858. In the original the init stays late and is in place (addiu a3,a3,0x88), as if 88 were still live after it. A 20-min permuter run found only junk (an uninitialised `c->unk94 = 0` store before the SPU stores gives the in-place, late init but 27 rows). A separate pointer for the second loop and `c = D_800809EC` change nothing. Pass 16: none of the 17 scalar globals written here changes anything as a struct field (D_80080A10 as a field: 37). Pass 17: second loop as `for (i = 8; i < 24;) { c->unk34 = 0; c->unk118 = i; i++; ... }` gives the original's `andi v0,s0; addiu s0,s0,1` order (14 -> 12; `c->unk118 = i++` gives a move + andi instead). Volatile cnt / SPU registers and moving `i = *cnt` before the SPU stores change nothing. sched.c note: a giv init is set twice (init + loop increment), so it never gets adjust_priority's birthing boost and sched1 always places it right after its input's definition; something has to make it depend on a late insn. Pass 18: loop.c emits the giv init from the biv's REG_EQUAL constant initial value (a fresh lui/addiu D_8007BD48 + 136 in the preheader), and cse2 turns it into `88 + 136`. `D_800809EC = c = D_8007BD48;` at the top with an init-less for loop: 19 (same early init, and the second loop gets worse). `c = D_800809EC` before the loop: 12. Index forms D_8007BD48[i] / c[i] / c = &D_8007BD48[i]: 27-32. Coordinator tip (loop invariants written inside the loop shift move_movables' threshold): the 0x18 store constant is already hoisted as in the original, and putting `cnt = 0x1F801DAA` or the D_80080A10 store inside loop 1 gives 12 and 67. The early giv init comes from strength reduction plus sched1, not from move_movables. Pass 19: not re-tested. Pass 20 (sched1 dump, -dS): the giv init (insn 630, `281 = 88 + 136`) is on the initial ready list with priority 1, the same as almost every store, and rank_for_schedule would pick it first (latest LUID, class 3). What keeps it out is schedule_select's function-unit check: at every step the dump says "insn N has a greater potential hazard, now N 630 ...", so each remaining store goes ahead of the ALU giv init until only 88's definition is left above it. In the original it was placed just above the `sh D_80080A2C` / `lhu *cnt` group, i.e. at a point where the potential-hazard test let it win. The place to look is therefore the memory-unit hazard state (the order and number of loads and stores just below that point), not data dependencies. Pass 24: not changed. The loop.c threshold effect that fixed 8001DE50 (each hoisted movable lowers move_movables' threshold by 3) doesn't apply here: the early giv init comes from strength reduction plus sched1, not from move_movables. */
#include "ff9.h"

void func_8005681C(void) {
    u16 i;
    u16 *cnt;
    AkaoChannel *c;

    cnt = (u16 *)0x1F801DAA;
    D_8007F790 = NULL;
    D_800809EC = D_8007BD48;
    D_8007F71C = NULL;
    D_8007F8D8[1] = 0;
    D_80080A10 = &D_8007F858;
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
    D_8007F858.unk4 = 0;
    D_8007F858.unk8 = 0;
    D_8007F858.unk6A = 0;
    D_80080A70.unk10 = 0;
    D_8007F858.unk1C = 0;
    D_8007F8F0.unk6A = 0;
    D_8007F8F0.unk4 = 0;
    D_8007F858.unk50 = 0x7F0000;
    D_8007F858.unk5C = 0x400000;
    D_80080AC0 = 0x7FFF0000;
    D_8007F96C = 0;
    D_80083150 = 0;
    D_8007F858.unk58 = 0;
    D_8007F858.unk64 = 0;
    D_8007F96E = 0;
    D_80083154 = 0;
    D_80080A2C = 0;
    *(u16 *)0x1F801D80 = 0x3FFF;
    *(u16 *)0x1F801D82 = 0x3FFF;
    *(u16 *)0x1F801DB0 = 0x7FFF;
    *(u16 *)0x1F801DB2 = 0x7FFF;
    D_800809B8 = 0;
    D_80080AC4 = 0;
    D_800831C4 = 0;
    i = *cnt;
    D_80080A70.unk1C = 0;
    D_8007F858.unk3C = 0;
    D_80080A70.unk20 = 0;
    D_8007F858.unk40 = 0;
    D_80080A70.unk24 = 0;
    D_8007F858.unk44 = 0;
    D_8007F858.unk74 = 0;
    D_8007F858.unk72 = 0;
    D_8007F858.unk70 = 0;
    D_8007F858.unk78 = 0;
    D_80083168.unk40 = 0x7F00;
    D_80083168.unk48 = 0;
    D_80083238[2] = 0;
    D_80083238[1] = 0;
    D_80083238[0] = 0;
    D_80080A20 = 0;
    *cnt = (i & ~5) | 1;
    for (c = D_8007BD48, i = 0; i < 32; i++, c++) {
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
    D_80080A10->unk18 = 0;
    D_80080A10->unk14 = 0;
    D_80080A10->unk10 = 0;
    D_80080A70.unk18 = 1;
    D_80080A70.unk14.w = 0x66A80000;
    D_80080A70.unkC = 0;
    D_80080A70.unk8 = 0;
    D_80080A70.unk4 = 0;
    D_80080A10->unk48 = 0x3FFF000;
    D_80080A10->unk4C = 0;
    D_80080A10->unk66 = 0;
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
