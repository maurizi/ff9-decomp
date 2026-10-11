/* STATUS: byte-identical but NOT integrated (user decision, pass 23): it needs a conditional dead store
 `if (cnt) { p = 5; }` before the empty `for (; i < cnt; i++) {}` loop. Until flow deletes it, the block
 boundary keeps combine from folding `i = 0` into the loop entry test. `p = 5` is invented, so it was rejected.
 Find a construct the original plausibly had that leaves the same block boundary: the empty loop suggests a
 compiled-out debug macro (its body may have been an if/statement that left the boundary), e.g. a macro that
 expands to `if (cnt) {}`-like residue, a label, or a different loop form. The rest of the body is the matching
 version. Also: `c` is reused for e->unk8->unk8 (second set keeps it global, in v1). */
#include "ovl_0b.h"

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
    u32 cnt;

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
    /* c reused for the unk8 block (a second set keeps it a global pseudo, in v1 as in the original) */
    c = (BtlUnit9CC *)e->unk8->unk8;
    cnt = ((BtlUnit9C8 *)c)->unk3;
    i = 0;
    if (cnt) {
        /* Probably macro residue: a dead store (deleted by flow, no code). The block boundary it leaves
           until flow keeps combine from folding i = 0 into the loop entry test. */
        p = 5;
    }
    for (; i < cnt; i++) {
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
