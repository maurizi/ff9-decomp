/* Draft of fx_034 func_801E7744. Status: 17 aligned mismatches: the original hoists &D_801E82D0 out of the outer
 * loop (s6) but recomputes &D_801E82B0 every outer iteration; here both are hoisted (one more saved register). */
#include "fxmod/fx_034.h"

void func_801E7744(s32 model, s32 scale, u8 *buf) {
    SVECTOR **p;
    u16 *q;
    s32 n;
    s32 i;
    s32 j = 0;

    n = BTLFX_EXPORTS(D_801E82A0)->func_800DD0DC(model & 0xFFFF);
    for (; j < n; j++) {
        for (i = 0; i < 8; i++) {
            func_801E7810(i, D_801E82D0[i], scale, D_801E82B0[i]);
        }
    }
}
