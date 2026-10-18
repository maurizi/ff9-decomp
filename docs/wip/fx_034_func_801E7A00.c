/* Draft of fx_034 func_801E7A00. Status: only the prologue order differs (the original copies buf into s7 before
 * model into s6); register assignment and the rest match. */
#include "fxmod/fx_034.h"

void func_801E7A00(s32 model, u8 *buf) {
    BtlfxExports *ex;
    s32 n;
    s32 i;
    s32 j;
    s32 m;

    ex = BTLFX_EXPORTS(D_801E82A0);
    j = 0;
    m = model & 0xFFFF;
    ex->Hi_SplitMdlVertex(m, -1, (s32)buf);
    n = ex->func_800DD0DC(m);
    for (; j < n; j++) {
        for (i = 0; i < 8; i++) {
            D_801E82D0[i] = ex->Hi_GetMdlVertexPtr(model & 0xFFFF, j, (s32)buf, (s32 *)&D_801E82B0[i], i);
            func_801E7B1C(i, D_801E82D0[i], D_801E82B0[i]);
        }
    }
}
