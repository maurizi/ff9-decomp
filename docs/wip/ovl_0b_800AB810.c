/* WIP func_800AB810: 8 differing rows (build/cmp.py-style aligned diff). New in pass 7. Left: the empty count loop over unk8->unk8->unk3: the original's entry test is sltu v0,v1(=0),a0 (i = 0 not folded), ours folds it to beqz n; also arg0 is stored from s3 (ours from the a1 argument copy). Uses a local Unk8E0Ld because ff9.h's Unk8E0 has pad4 at 4. */
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
    e->unk8->unk8 = (BtlUnit9C8 *)func_8001C7FC(4, arg0, 0);
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
