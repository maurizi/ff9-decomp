/*
 * ovl_0a func_800C31F8 (0x410 bytes): averages the queued collision pushes (D_800CA9D8) and
 * re-runs func_800C2AF0.
 * WIP: 258 of 260 instructions, ~19 aligned diff rows:
 *  - else branch: the original copies `i = any` from the register (t2 = t6) where CSE folds it
 *    to 0 here (any = 0 sits in the delay slot of `if (D_800C9DDC == 1)`); moving `any = 0`
 *    anywhere else is worse;
 *  - tail: the original stores arg0->unk24->unk14.vx/vz right after their loads (two load-delay
 *    nops, hence 260), we interleave them; arg0 is t7 there, t8 here.
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
    arg0->unk24->unk14.vx = arg1->vx;
    arg0->unk24->unk14.vz = arg1->vz;
    D_800C9DDC = 0;
    return func_800C2AF0(arg0, arg2, &prev, arg1, 0, 0, 0, 0) == 0;
}
