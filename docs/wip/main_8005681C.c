/* 12 rows (pass 17; 14 before): SPU init. The original reads *cnt (0x1F801DAA) early into s0, the u16 counter i (i = *cnt; ...; *cnt = (i & ~5) | 1); `D_80080A10 = &D_8007F858` after the D_8007F8D8 stores (51 -> 14, found by build/mvall.py). Remaining: the original materialises &D_8007BD48 in a3 at the D_800809EC store and does a3 += 0x88 (the first loop's giv) just before the SPU register stores, with &D_8007F858 in v1 from the first instruction; here &D_8007BD48 and its +0x88 giv are hoisted to the top. In the second loop the original increments i right after reading it for unk118. Tried: moving the D_800809EC/D_80080A10/D_8007F790/i = *cnt lines anywhere, c = D_8007BD48 forms, i++ placement, dup sweep. Pass 14: in the pre-sched RTL the giv init (`281 = 88 + 136`, 88 = &D_8007BD48) is the last insn of block 0; sched1 moves it up next to 88's definition because it kills 88 (2.8 defers scheduling insns with REG_DEAD notes), so 88 dies early and v1 is reused for &D_8007F858. In the original the init stays late and is in place (addiu a3,a3,0x88), as if 88 were still live after it. A 20-min permuter run found only junk (an uninitialised `c->unk94 = 0` store before the SPU stores gives the in-place, late init but 27 rows). A separate pointer for the second loop and `c = D_800809EC` change nothing. Pass 16: none of the 17 scalar globals written here changes anything as a struct field (D_80080A10 as a field: 37). Pass 17: second loop as `for (i = 8; i < 24;) { c->unk34 = 0; c->unk118 = i; i++; ... }` gives the original's `andi v0,s0; addiu s0,s0,1` order (14 -> 12; `c->unk118 = i++` gives a move + andi instead). Volatile cnt / SPU registers and moving `i = *cnt` before the SPU stores change nothing. sched.c note: a giv init is set twice (init + loop increment), so it never gets adjust_priority's birthing boost and sched1 always places it right after its input's definition; something has to make it depend on a late insn. */
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
