/* WIP func_800DA1F4: 14 differing rows (build/cmp.py), now the same length. Pass 10: the for loop tests `i < arg0->unk2` (a reload each time, which loop.c hoists: that is the count copy into a2 in the original) and d = p + 2 is set before the after-loop check; left: the blez still goes to the after-loop check instead of straight to return NULL, the arg0 copy (v0) for the header reads, and the p/e registers. An explicit `if (arg0->unk2 <= 0) return NULL;` first: 16-17 rows. Earlier notes: the original copies arg0 to v0 and a3 (inline parameter?), keeps p in a0 and only moves p+4 into s0, copies the count to a2 for the loop, and the blez skips straight to return 0. Pass 7: the blez a1 -> return NULL needs the after-loop `i < unk2` test skipped; `if (n <= 0) return NULL;` before the for loop gets that (20 rows) but keeps the for loop's own second blez; an if-wrapped do-while gets one blez (21 rows) but the wrong loop body shape. Pass 13: an h = arg0 copy for the header reads, `n = arg0->unk2` as the loop bound (13 rows, one insn short), == / <= forms of the after-loop check, 20 min of permuter: no better than 13. Pass 14: `n = h->unk2; stride = h->unk0; if (n <= 0) return NULL;` before the for loop gives the blez straight to return NULL, but the for loop keeps its own entry blez (cse does not fold the repeated `n <= 0`), 16 rows; with a do-while instead the loop body is reshaped (20). Pass 15: the blez straight to `return NULL` is jump.c's thread_jumps (run after cse): the duplicated loop entry test `i < unk2` and the after-loop `i < unk2` must be identical blocks. With `d = p + 2` moved after the after-loop check (and nothing else between the loop and the check) the blez is threaded to return NULL. But then d is local (one block, crosses the call, local-alloc gives it s0 first), and global.c set_preference((set d (plus p 4))) gives p a preference for s0, so p (or e, if d is computed from e) lands in s0: 21 rows. In the original p is a0 (from arg0 + 12 with arg0 dying there), the long-lived arg0 copy is a3 and the header reads use a third copy in v0. `p = arg0->unkC; k = arg0;` (k for all later uses) gives k = a3 and p computed from a0, but p still in s0 (24). Pass 16: the order after the loop decides which of p/e is the CSE class head for the in-loop *p load (make_regs_eqv: the copy e = p becomes head when e lives longer). With d = p + 2 before the e stores (as here, or right after the check) e is head and the load reads e; with the check, then e[0]/e[1], then d = p + 2 the load reads p as in the original, but p dies into d and is tied to it in s0 (21 rows, 45 insns). An inline pool_find(h) returning e or NULL plus the caller's NULL test: 25-28 rows. Pass 17: not re-tested. Pass 18: DA2AC-style `pool = arg0` copy with a comma-increment for loop: 21-24 rows. The whole body as a static inline called from func_800DA1F4: 19. pool_find inline returning e or NULL: 32. */
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
