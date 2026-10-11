/* WIP func_800AB07C: 24 differing rows (build/cmp.py-style aligned diff), same length. Pass 9: the loop walks separate pointers pa/pb/pr set from b, r and r + n (the original computes them in temps and copies), getTPage(1, 0, x, h) / getClut(0x100, yy), yy = y + 0xE5 + pr->unk2. Left: s-register numbering only (original: m s2, yy s1, pr s3, n s4; here n s2, yy s3, m s4, pr s1). The permuter reached 14 rows by wrapping everything after b = a + n in do { ... } while (0) (pr/yy/m then rotate by one). Earlier: the goto loop is needed (while/do-while: 54+). Pass 10: the do { } while (0) wrap works because flow.c weights REG_N_REFS by loop depth (the loop notes of the dummy loop double every ref inside it); the goto loop has no loop notes. With the wrap, `yy = y; yy = yy + 0xE5 + pr->unk2;` gives 10 rows (only pr s2 vs m s3 left: pr priority 16 refs/65 insns = 0.98 must drop below m's 0.71), and also wrapping `yy = y + 0xE8` in an identical-arm `if (m) {..} else {..}` gives 1 row. All three are hacks; the real source probably has a loop that has loop notes but that loop.c rejects (no invariant hoisting: the tpage bits of x are computed inside the loop in the original), e.g. a label inside the loop that is jumped to from outside. Pass 13: a plain do/while (loop notes) lets loop.c hoist x's tpage bits and reduce the walk pointers (different code), confirming that the original loop has loop-depth-2 refs but was not optimized by loop.c. Pass 15: loop.c treats a loop as invalid (no hoisting / reduction) only for setjmp, labels whose address is taken, or a jump into the loop from outside (find_and_verify_loops / mark_loop_jump). while (1) / for (;;) with continue / do-while forms all get optimized by loop.c (56 rows). */
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
    Unk800AA504Pos *pa;
    Unk800AA504Pos *pb;
    Pos8 *pr;
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
        pr = r + n;
        pb = (Unk800AA504Pos *)r;
        pa = b;
    loop:
            pr--;
            pb--;
            pa--;
            n--;
            m = func_8001C8B0(dst, 2, (n & 0xFFFF) | 0x40000, 0);
            if (arg3) {
                yy = y + 0xE5 + pr->unk2;
                h = (pr->unk2 << 7) + 0x80;
            } else {
                yy = y + 0xE8;
                h = 0x1D0;
            }
            func_8001D9CC(m, 0x100, yy, x, h);
            func_8001D8A8(m);
            pa->unk0 = getTPage(1, 0, x, h);
            pa->unk2 = getClut(0x100, yy);
            pb->unk0 = 0;
            pb->unk2 = h % 256 - h % 128;
            pr->unk4 = x;
            pr->unk0 = x;
            pr->unk6 = h;
            pr->unk2 = h;
            if (n > 0) {
                goto loop;
            }
    }
    return sp18;
}
