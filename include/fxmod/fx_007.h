#ifndef FX_007_H
#define FX_007_H

/* Effect module fx_007 (dir 13 file 0x07). */
#include "btlfx_mod.h"

/* PsyQ gte_stszotz: SZ3 / 4 */
#define gte_stszotz(r0) __asm__ volatile( \
    "mfc2\t$12, $19;" \
    "nop;" \
    "sra\t$12, $12, 2;" \
    "sw\t$12, 0(%0)" \
    : : "r"(r0) : "$12", "memory")
/* VZ0 = VZ1 = VZ2 = 0 */
#define gte_ldVZ012_0() __asm__ volatile( \
    "mtc2\t$0, $1;" \
    "mtc2\t$0, $3;" \
    "mtc2\t$0, $5")

/* V0..V2 x and y only (VZ0..VZ2 are loaded once with gte_ldVZ012_0) */
#define gte_ldv3xy(r0, r1, r2) __asm__ volatile( \
    "lwc2\t$0, 0(%0);" \
    "lwc2\t$2, 0(%1);" \
    "lwc2\t$4, 0(%2)" \
    : : "r"(r0), "r"(r1), "r"(r2))
#define gte_ldv0xy(r0) __asm__ volatile( \
    "lwc2\t$0, 0(%0)" \
    : : "r"(r0))

extern Unk800F78A4 *fx_007_env;

static void fxm_drawTexelQuads(SVECTOR *pos, SVECTOR *rot, s32 tp, s32 x, s32 y, s32 div, s32 idx, s32 size, s32 w, s32 h,
                   s32 scale, s32 bright, s32 abr, u16 *colors, s32 skip);

typedef struct {
    SVECTOR pos0; /* target body position */
    SVECTOR pos8; /* target position */
    s32 unk10; /* state */
    s32 model14[4]; /* models */
    s32 model24;
    s32 model28;
    Pool6CEntry *emit2C;
    Pool6CEntry *emit30;
    s32 unk34; /* frame */
} Fx007Work80; /* 0x38 */

extern BtlModel fx_007_effModels[];
extern u8 D_801E93A8[];
extern u16 D_801E9638[];

#endif
