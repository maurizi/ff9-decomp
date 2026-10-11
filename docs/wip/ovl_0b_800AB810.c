/* WIP func_800AB810: 6 differing rows (build/cmp.py-style aligned diff). Pass 13: passing e->unk4 (not arg0) to the first func_8001C7FC fixes the arg0 store (now from s3, before the a1 copy). Still left: the loop entry test (tried while, explicit if + do-while, reversed compare, i as u32/s32/u8/s16: all fold the entry test). New in pass 7. Left: the empty count loop over unk8->unk8->unk3: the original's entry test is sltu v0,v1(=0),a0 (i = 0 not folded), ours folds it to beqz n; also arg0 is stored from s3 (ours from the a1 argument copy). Uses a local Unk8E0Ld because ff9.h's Unk8E0 has pad4 at 4. Pass 14: the fold is not CSE: the .cse/.cse2 dumps still have `sltu (reg i) (reg n)`; combine substitutes i = 0 into the entry test (LOG_LINKS, same basic block) and makes it beqz. So in the original something kept combine from linking i = 0 to the test (a block boundary between them at combine time, or i not a plain constant set). Tried: do { for } while (0), for inside for (;;) { ...; break; }, i = 0 outside a do-while(0), goto loop (15 rows), while (i++ < n) (8), if + do-while, an inline count() in the condition (stops loop rotation: no duplicated test, 9 rows). Pass 15: an empty do { } while (0) inside the loop body, `i = 0; do {} while (0);` before the for, `continue;` body, while-with-do-while: all still fold (6 rows). For combine not to substitute i = 0 the set must be in another basic block than the entry test (flow only makes LOG_LINKS inside a block), and a label between them that is not jumped to gets deleted; putting i = 0 before func_800AC280 would make i cross the call. Pass 17: not re-tested. combine can split the folded test from `i = 0` (two-insn result), so a later use of i doesn't block the fold. Only a missing LOG_LINK prevents it, i.e. a basic-block boundary that is still there at flow/combine time and gone by the final code. */
#include "ovl_0b.h"

/* GameData.unk8E0 entry as func_800AB810 fills it (ff9.h's Unk8E0 has pad4) */
typedef struct {
    s32 unk0; /* flags */
    s32 unk4; /* model id */
    BtlUnit9C *unk8;
    s32 unkC;
} Unk8E0Ld;

u32 func_800AB810(s32 arg0, s32 arg1) {
    s16 *pa;
    s16 *pb;
    s32 pc;
    u32 n;
    Unk8E0Ld *e;
    BtlUnit9CC *c;
    s32 p;
    u32 i;
    u32 r;

    r = func_8004AB64(arg1);
    if (r < 9) {
        n = r;
        e = (Unk8E0Ld *)func_8001E350(r);
    } else {
        n = D_8006794C->unk1C->unk39 + 9;
        e = (Unk8E0Ld *)func_8001E350(n);
        D_8006794C->unk1C->unk39++;
    }
    e->unk4 = arg0;
    e->unkC = arg1;
    e->unk8->unk8 = (BtlUnit9C8 *)func_8001C7FC(4, e->unk4, 0);
    e->unk8->unkC = (BtlUnit9CC *)func_8001C7FC(4, func_800AC244(e->unk4), 0);
    c = e->unk8->unkC;
    if (c->unkC >= 0) {
        c->unkC = (s32)c + c->unkC;
        if (c->unk10 != 0) {
            c->unk10 = (s32)e->unk8->unkC + c->unk10;
        }
    }
    func_800AC280((e->unk4 & 0xFFFFFF) | (n << 24), (s32 **)&pa, (s32 **)&pb, (s32 **)&pc);
    for (i = 0; i < e->unk8->unk8->unk3; i++) {
    }
    func_800C7730(e->unk8, pa, pb);
    p = (s32)func_8001C7FC(9, e->unk4, 0x190000);
    if (p != -1) {
        e->unk8->unk50 = (BtlUnit9C50 *)func_8001C7FC(4, p, 0);
        func_800C9374(e->unk8->unk50, pc);
    }
    e->unk0 |= 1;
    return n;
}
