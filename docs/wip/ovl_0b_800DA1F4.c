/* WIP func_800DA1F4: 14 differing rows (build/cmp.py), now the same length. Pass 10: the for loop tests `i < arg0->unk2` (a reload each time, which loop.c hoists: that is the count copy into a2 in the original) and d = p + 2 is set before the after-loop check; left: the blez still goes to the after-loop check instead of straight to return NULL, the arg0 copy (v0) for the header reads, and the p/e registers. An explicit `if (arg0->unk2 <= 0) return NULL;` first: 16-17 rows. Earlier notes: the original copies arg0 to v0 and a3 (inline parameter?), keeps p in a0 and only moves p+4 into s0, copies the count to a2 for the loop, and the blez skips straight to return 0. Pass 7: the blez a1 -> return NULL needs the after-loop `i < unk2` test skipped; `if (n <= 0) return NULL;` before the for loop gets that (20 rows) but keeps the for loop's own second blez; an if-wrapped do-while gets one blez (21 rows) but the wrong loop body shape. Pass 13: an h = arg0 copy for the header reads, `n = arg0->unk2` as the loop bound (13 rows, one insn short), == / <= forms of the after-loop check, 20 min of permuter: no better than 13. Pass 14: `n = h->unk2; stride = h->unk0; if (n <= 0) return NULL;` before the for loop gives the blez straight to return NULL, but the for loop keeps its own entry blez (cse does not fold the repeated `n <= 0`), 16 rows; with a do-while instead the loop body is reshaped (20). */
#include "ovl_0b.h"

void *func_800DA1F4(Unk800DA1B8 *arg0) {
    s16 *p;
    s16 *e;
    s32 i;
    s32 n;
    s32 stride;
    u8 *d;

    p = (s16 *)arg0->unkC;
    stride = arg0->unk0;
    for (i = 0; i < arg0->unk2; i++) {
        e = p;
        if (*p == 0) {
            break;
        }
        p = (s16 *)((u8 *)p + stride);
    }
    d = (u8 *)(p + 2);
    if (i >= arg0->unk2) {
        return NULL;
    }
    e[0] = 1;
    e[1] = 0;
    arg0->unk4(1, d, D_800F78A4, arg0->unk8, 0, arg0, arg0->unk8->unkC);
    return d;
}
