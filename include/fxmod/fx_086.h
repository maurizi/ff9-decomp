#ifndef FX_086_H
#define FX_086_H

/* Effect module fx_086 (dir 13 file 0x86): Meteor (party and enemy). Streak field (fxm_drawStreakField). Evidence:
 * [data] the party ability / item tables and enemy sequences that play this file id (tools/effect_labels.py;
 * docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    u8 r;
    u8 g;
    u8 b;
    u8 code;
} Fx086Rgbc; /* primitive colour word */

extern Unk800F78A4 *fx_086_env;

static void fxm_randDiscPoints(s32 n, s32 r, SVECTOR *pts);
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
static void fxm_drawStreakField(u32 *ot, s32 n, s32 h, s32 dz, void *src, SVECTOR *pts, CVECTOR *col, s32 p, s32 abr);

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* counter */
    s16 unk8[6];
    SVECTOR pos14; /* position */
    SVECTOR pos1C; /* target position */
    s32 model24; /* models */
    s32 model28;
    s32 model2C;
    s32 model30;
    s32 model34;
    s32 model38;
    s32 model3C;
    s32 model40;
    Pool6CEntry *emit44;
} Fx086Work80; /* 0x48 */

extern BtlModelBuf fx_086_effModels[];
extern SVECTOR fx_086_streakPts[];
extern SVECTOR fx_086_spritePos[64];
static void fxm_drawTexCylinder(SVECTOR *pos, SVECTOR *rot, VECTOR *scale, s32 r0, s32 r1, s32 w0, s32 w1, s32 c, s32 tpage,
                   s32 clut, s32 u, s32 v, s32 ang, s32 dy, s32 abr, s32 dang);

#endif
