/* Draft of func_801E7894 (module fx_17d_0). Status: 75/85 aligned instructions with ccprobe; pts and i registers differ.
 * Declarations it needs that are not yet in include/fxmod/fx_17d_0.h are repeated below. */
#include "fxmod/fx_17d_0.h"

void func_801E79E8(u32 *ot, s32 n, s32 h, s32 dz, SVECTOR *rot, SVECTOR *pts, CVECTOR *rgb, s32 p, s32 abr);
/* the PsyQ gte_ReadGeomScreen: store the projection distance H */
#define gte_ReadGeomScreen(r0) __asm__ volatile( \
    "cfc2\t$12, $26;" \
    "nop;" \
    "sw\t$12, 0(%0)" \
    : : "r"(r0) : "$12", "memory")

void func_801E7894(s32 n, s32 r, SVECTOR *pts) {
    u32 *col = (u32 *)(pts + n);
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E9470);
    s32 i;
    s32 rad;
    s32 a;

    for (i = 0; i < n; i++, col++, pts++) {
        *col = 0x7F7F7F7F;
        rad = ex->btlfx_randRange(r >> 2, r >> 1);
        a = ex->btlfx_rand() & 0xFFF;
        pts->vx = (ex->math_cos(a) * rad) >> 12;
        pts->vy = (ex->math_sin(a) * rad) >> 12;
        pts->vz = ex->btlfx_rand() % 640 - 0x80;
    }
}
