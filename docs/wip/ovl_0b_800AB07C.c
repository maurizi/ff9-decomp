/* WIP func_800AB07C: 51 differing rows (build/cmp.py-style aligned diff). loop shape right (texture list walked backwards); left: y + 0xE5 reassociation and s-register assignment. An index-based do/while (a[n], r[n]) gets the givs but hoists invariants and spills. */
#include "ovl_0b.h"

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
} Pos8;

s32 func_800AB07C(Unk800AA958 *arg0, Unk800AA958Req *arg1, s32 arg2, s32 arg3) {
    s32 sp18;
    s32 dst;
    Unk800AA504Tbl *tbl;
    Unk800AA504Ent *te;
    s32 n;
    Unk800AA504Pos *a;
    Unk800AA504Pos *b;
    Pos8 *r;
    s32 x;
    s32 y;
    s32 yy;
    s32 h;
    s32 m;

    sp18 = func_8001C8B0(arg0->unk24, 2, 0x1B0000, 0);
    dst = arg1->unkC;
    tbl = (Unk800AA504Tbl *)func_8001C8B0(sp18, 2, 0x120000, 0);
    te = tbl->unk4;
    n = te->unk4.b[3];
    a = (Unk800AA504Pos *)((u8 *)te + (te->unk4.w & 0xFFFFFF));
    b = a + n;
    r = (Pos8 *)(b + n);
    if (n > 0) {
        x = arg2 * 64 + 0xC0;
        y = arg2 * 3;
        r += n;
        b += n;
        a += n;
    loop:
            r--;
            b--;
            a--;
            n--;
            m = func_8001C8B0(dst, 2, (n & 0xFFFF) | 0x40000, 0);
            if (arg3) {
                yy = r->unk2 + 0xE5 + y;
                h = (r->unk2 << 7) + 0x80;
            } else {
                yy = y + 0xE8;
                h = 0x1D0;
            }
            func_8001D9CC(m, 0x100, yy, x, h);
            func_8001D8A8(m);
            a->unk0 = ((h & 0x100) >> 4) | (((x & 0x3FF) >> 6) | 0x80) | ((h & 0x200) << 2);
            a->unk2 = (yy << 6) | 0x10;
            b->unk0 = 0;
            b->unk2 = h % 256 - h % 128;
            r->unk4 = x;
            r->unk0 = x;
            r->unk6 = h;
            r->unk2 = h;
            if (n > 0) {
                goto loop;
            }
    }
    return sp18;
}
