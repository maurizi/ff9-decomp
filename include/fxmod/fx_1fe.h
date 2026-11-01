#ifndef FX_1FE_H
#define FX_1FE_H

/* Effect module fx_1fe (dir 13 file 0x1fe): Phoenix, short summon. Fire / ice drawn as a grid of texel quads
 * (fxm_drawTexelQuads). Evidence: [data] the party ability / item tables and enemy sequences that play this file id
 * (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    SVECTOR unk8; /* write-only (copy of fx_1fe_basePos0) */
    SVECTOR pos10;
    SVECTOR pos18;
    SVECTOR camTarget20; /* camera target */
    u8 pad28[0x34 - 0x28];
    s32 model34; /* model */
    s32 model38; /* model */
    s32 model3C; /* model */
} Fx1feWork81; /* 0x40 */

extern BtlModelBuf fx_1fe_effModels[];
extern SVECTOR fx_1fe_basePos0;
extern SVECTOR fx_1fe_basePos1;
extern CVECTOR fx_1fe_rippleColor;
extern u8 fx_1fe_op81Trails[];
s32 fx_1fe_op81(s32 mode, Fx1feWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

/* propagated */

extern Unk800F78A4 *fx_1fe_env;
extern s32 fx_1fe_waveAmp;
extern s32 fx_1fe_wavePhase;
static void fxm_waveVertexX(s16 *vert);
s32 fx_1fe_op85(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

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
