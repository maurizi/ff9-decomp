#ifndef FX_048_H
#define FX_048_H

/* Effect module fx_048 (dir 13 file 0x48): Firaga, all targets (and one large target). Fire / ice drawn as a grid
 * of texel quads (fxm_drawTexelQuads). Evidence: [data] the party ability / item tables and enemy sequences that
 * play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR unk0; /* caster position */
    SVECTOR pos8; /* caster position, y = 0 */
    s32 unk10; /* state */
    s32 model14[4]; /* models */
    s32 model24; /* model */
    s32 model28; /* model */
    Pool6CEntry *emit2C;
    Pool6CEntry *emit30;
    s32 unk34; /* frame counter */
} Fx048Work; /* 0x38 */

extern Unk800F78A4 *fx_048_env;
extern BtlModel fx_048_effModels[];
extern s32 D_801E93FC[];
extern u16 D_801E968C[];

s32 fx_048_op80(s32 mode, Fx048Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

/* propagated */

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
static void fxm_drawTexelQuads(SVECTOR *pos, SVECTOR *rot, s32 tp, s32 x, s32 y, s32 div, s32 idx, s32 size, s32 w, s32 h,
                   s32 scale, s32 bright, s32 abr, u16 *colors, s32 skip);

#endif
