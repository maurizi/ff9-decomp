/* STATUS: near miss, 30 rows. ovl_36 func_801F1778 (0x2D0, decompress a card picture into the VRAM cache; the second half is an inlined copy of func_801F2D88, now func_801F2D88_inline in ovl_36.h). Left: s-register assignment only (original: s3 = &D_801F347C derived from the cache pointer, s4 = D_801F0274 table, then s4-s7 reused for the RECT constants 1/0x100/0x13/0x2F and s1 for 0xAAAAAAAB); ours keeps the table in s3 and D_801F347C in s6. Permuter (25 min, manual-inline copy) found only the s->unk1/unk0 store swap. */
#include "ovl_36.h"

void func_801F1778(void) {
    RECT a, b;
    CardCache *cc;
    CardSlot *s;
    s32 *ofs;
    s32 n;
    u32 k;

    cc = &D_801F347C.unk1900;
    ofs = D_801F0274;
    if (cc->unk2 >= 12) {
        cc->unkD[cc->unk0].unk0 = 0;
        cc->unk2++;
        cc->unk0 = (cc->unk0 + 1) % 12;
    }
    s = &cc->unkD[cc->unk1];
    s->unk1 = D_801F347C.unk4;
    s->unk0 = 1;
    cc->unk2++;
    cc->unk1 = (cc->unk1 + 1) % 12;
    n = func_801F3034(D_801F347C.unk10D0, D_801F347C.unkD0);
    if (ofs[D_801F347C.unk4 + 1] != ofs[D_801F347C.unk4]) {
        func_801F3034((u8 *)D_801F0274 - 4 + ofs[D_801F347C.unk4], D_801F347C.unkD0 + n);
    }
    k = s->unk2;
    a.x = 0;
    a.w = 0x100;
    a.h = 1;
    b.w = 0x13;
    b.h = 0x2F;
    a.y = k + 0x1F4;
    b.x = (u8)(k % 3) * 0x13;
    b.y = (u8)(k / 3) * 0x2F + 0x100;
    LoadImage(&a, (u32 *)D_801F347C.unkD0);
    LoadImage(&b, (u32 *)D_801F347C.unk2D0);
    DrawSync(0);
    if (s->unk1 == cc->unk5) {
        func_801F2D88_inline(s);
    }
}
