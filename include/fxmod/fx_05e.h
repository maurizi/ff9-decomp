#ifndef FX_05E_H
#define FX_05E_H

/* Effect module fx_05e (dir 13 file 0x5E). */
#include "btlfx_mod.h"

/* ribbon history: two edge points per step, unk0.pad = live */
typedef struct {
    SVECTOR unk0;
    SVECTOR unk8;
} Fx05eSeg; /* 0x10 */

#define gte_stszotz(r0) __asm__ volatile( \
    "mfc2\t$12, $19;" \
    "nop;" \
    "sra\t$12, $12, 2;" \
    "sw\t$12, 0(%0)" \
    : : "r"(r0) : "$12", "memory")

extern Unk800F78A4 *D_801E8E74;

void func_801E7704(SVECTOR *a, SVECTOR *b, u32 n, CVECTOR *ca, CVECTOR *cb, s32 p, Fx05eSeg *buf, s32 abr);
void func_801E7B74(s32 model, s32 bone, SVECTOR *in, SVECTOR *out);

#endif
