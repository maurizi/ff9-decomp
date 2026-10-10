/* WIP func_800DA1F4: 14 differing rows (build/cmp.py-style aligned diff). the original copies arg0 to v0 and a3 (inline parameter?), keeps p in a0 and only moves p+4 into s0, copies the count to a2 for the loop, and the blez skips straight to return 0. Pass 7: the blez a1 -> return NULL needs the after-loop `i < unk2` test skipped; `if (n <= 0) return NULL;` before the for loop gets that (20 rows) but keeps the for loop's own second blez; an if-wrapped do-while gets one blez (21 rows) but the wrong loop body shape. */
#include "ovl_0b.h"

void *func_800DA1F4(Unk800DA1B8 *arg0) {
    s16 *p;
    s16 *e;
    s32 i;
    s32 n;
    s32 stride;
    u8 *d;

    p = (s16 *)arg0->unkC;
    n = arg0->unk2;
    stride = arg0->unk0;
    for (i = 0; i < n; i++) {
        e = p;
        if (*p == 0) {
            break;
        }
        p = (s16 *)((u8 *)p + stride);
    }
    if (i >= arg0->unk2) {
        return NULL;
    }
    d = (u8 *)(p + 2);
    e[0] = 1;
    e[1] = 0;
    arg0->unk4(1, d, D_800F78A4, arg0->unk8, 0, arg0, arg0->unk8->unkC);
    return d;
}
