/*
 * ovl_0a func_800C31F8 (0x410 bytes): averages the queued collision pushes (D_800CA9D8) and
 * re-runs func_800C2AF0.
 * WIP (pass 10): 258 of 260 instructions, 10 aligned diff rows (was 19).
 * Fixed: the tail stores go through `*(s32 *)((u8 *)arg0->unk24 + 0x14)` (not a struct field), so
 * gcc 2.8's MEM_IN_STRUCT_P heuristic no longer lets the D_800C9DDC = 0 store move above them;
 * the two loads/stores now come out in source order with the original's load-delay nops.
 * Left: (1) sched2 fills those two nops with the call's `move a0, arg0` / `addiu a2, sp, 0x30`
 * (the original puts them after `lui v0, %hi(D_800C9DDC)`); (2) else branch: the original copies
 * `i = any` from t6 where CSE folds it to 0 here (any = 0 sits in the delay slot of
 * `if (D_800C9DDC == 1)`); (3) arg0 is t7 there, t8 here (D_800CAA38 base swaps with it).
 */
#include "ovl_0a.h"

s32 func_800C31F8(Unk800C9DF4 *arg0, SVECTOR *arg1, Unk800C9E00 **arg2, s16 *arg3) {
    VECTOR sum;
    SVECTOR prev;
    u8 i;
    u8 j;
    u8 dup;
    u8 any;
    s32 n;

    if (D_800C9DDC == 0) {
        return 1;
    }
    prev.vx = arg1->vx;
    prev.vy = arg1->vy;
    prev.vz = arg1->vz;
    any = 0;
    if (D_800C9DDC == 1) {
        arg1->vx += D_800CA9D8[0][0];
        arg1->vz += D_800CA9D8[0][2];
    } else {
        i = any;
        n = 0;
        sum.vx = 0;
        sum.vz = 0;
        for (; i < D_800C9DDC; i++) {
            if (D_800CAA38[i] != 0) {
                dup = 0;
                for (j = 0; j < i; j++) {
                    if (D_800CA9D8[j][0] == D_800CA9D8[i][0] && D_800CA9D8[j][2] == D_800CA9D8[i][2]) {
                        dup = 1;
                    }
                }
                if (!dup) {
                    any = 1;
                    sum.vx += D_800CA9D8[i][0];
                    n++;
                    sum.vz += D_800CA9D8[i][2];
                }
            }
        }
        if (any) {
            if (n >= 2) {
                sum.vx /= n;
                sum.vz /= n;
            }
        } else {
            n = 0;
            for (i = 0; i < D_800C9DDC; i++) {
                dup = 0;
                for (j = 0; j < i; j++) {
                    if (D_800CA9D8[j][0] == D_800CA9D8[i][0] && D_800CA9D8[j][2] == D_800CA9D8[i][2]) {
                        dup = 1;
                    }
                }
                if (!dup) {
                    sum.vx += D_800CA9D8[i][0];
                    n++;
                    sum.vz += D_800CA9D8[i][2];
                }
            }
            if (n > 0) {
                sum.vx /= n;
                sum.vy /= n;
            }
        }
        arg1->vx += sum.vx;
        arg1->vz += sum.vz;
    }
    *(s32 *)((u8 *)arg0->unk24 + 0x14) = arg1->vx;
    *(s32 *)((u8 *)arg0->unk24 + 0x1C) = arg1->vz;
    D_800C9DDC = 0;
    return func_800C2AF0(arg0, arg2, &prev, arg1, 0, 0, 0, 0) == 0;
}
