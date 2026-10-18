/* Draft of fx_086 func_801E7E18. Status: 84/85, one instruction differs: the loop giv for &p->vz is initialised from
 * the incoming $a2 instead of its $s4 copy (addiu s3,a2,4 vs addiu s3,s4,4). Pointer/index rewrites and dropping the
 * do-while(0) are worse. */
#include "btlfx_mod.h"

extern Unk800F78A4 *D_801E9D6C;

void func_801E7E18(s32 n, s32 r, SVECTOR *pts);
#define gte_stszotz(r0) __asm__ volatile( \
    "mfc2\t$12, $19;" \
    "nop;" \
    "sra\t$12, $12, 2;" \
    "sw\t$12, 0(%0)" \
    : : "r"(r0) : "$12", "memory")
#define gte_ReadGeomScreen(r0) __asm__ volatile( \
    "cfc2\t$12, $26;" \
    "nop;" \
    "sw\t$12, 0(%0)" \
    : : "r"(r0) : "$12", "memory")
void func_801E7F6C(u32 *ot, s32 n, s32 h, s32 dz, void *src, SVECTOR *pts, CVECTOR *col, s32 p, s32 abr);

void func_801E7E18(s32 n, s32 r, SVECTOR *pts) {
    SVECTOR *p = pts;
    s32 *prev = (s32 *)&p[n];
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E9D6C);
    s32 i;
    s32 rad;
    s32 a;

    for (i = 0; i < n; i++, prev++, p++) {
        *prev = 0x7F7F7F7F;
        rad = ex->btlfx_randRange(r >> 2, r >> 1);
        a = ex->btlfx_rand() & 0xFFF;
        do {
            p->vx = (ex->math_cos(a) * rad) >> 12;
            p->vy = (ex->math_sin(a) * rad) >> 12;
        } while (0);
        p->vz = ex->btlfx_rand() % 1280 - 0x100;
    }
}
