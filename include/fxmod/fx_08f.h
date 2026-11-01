#ifndef FX_08F_H
#define FX_08F_H

/* Effect module fx_08f (dir 13 file 0x8F): Meteor, alternate file. Streak field (fxm_drawStreakField). Evidence:
 * [data] the party ability / item tables and enemy sequences that play this file id (tools/effect_labels.py;
 * docs/runtime-findings.md, "Round 31 runtime pass"). */
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
extern Unk800F78A4 *fx_08f_env;
static void fxm_drawStreakField(u32 *ot, s32 n, s32 h, s32 dz, void *src, SVECTOR *pts, CVECTOR *col, s32 p, s32 abr);
typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame */
    s16 unk8[6]; /* write-only: three screen points (0xA0,0x78), (0,0x78), (0xC8,0x78) */
    SVECTOR pos14; /* summon position */
    SVECTOR camPos1C;
    s32 model24; /* models */
    s32 model28;
    s32 model2C;
    s32 model30;
    s32 model34;
    s32 model38;
    s32 model3C;
    s32 model40;
    Pool6CEntry *emit44;
} Fx08fWork80; /* 0x48 */

extern BtlModelBuf fx_08f_effModels[];
extern SVECTOR fx_08f_streakPts[];
extern u8 fx_08f_trail[];
static void fxm_randDiscPoints(s32 n, s32 r, SVECTOR *pts);
s32 fx_08f_op80(s32 mode, Fx08fWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

#endif
