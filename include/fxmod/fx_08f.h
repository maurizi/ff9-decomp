#ifndef FX_08F_H
#define FX_08F_H

/* Effect module fx_08f (dir 13 file 0x8F). */
#include "btlfx_mod.h"

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
extern Unk800F78A4 *D_801E8A04;
void func_801E7858(u32 *ot, s32 n, s32 h, s32 dz, void *src, SVECTOR *pts, CVECTOR *col, s32 p, s32 abr);

#endif
