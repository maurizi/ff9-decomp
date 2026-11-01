#ifndef FX_05E_H
#define FX_05E_H

/* Effect module fx_05e (dir 13 file 0x5E): Death (party and enemy). Ribbon trails (fxm_drawRibbonTrail). Evidence:
 * [data] the party ability / item tables and enemy sequences that play this file id (tools/effect_labels.py;
 * docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* ribbon history: two edge points per step, edge0.pad = live */
typedef struct {
    SVECTOR edge0;
    SVECTOR edge8;
} Fx05eSeg; /* 0x10 */

#define gte_stszotz(r0) __asm__ volatile( \
    "mfc2\t$12, $19;" \
    "nop;" \
    "sra\t$12, $12, 2;" \
    "sw\t$12, 0(%0)" \
    : : "r"(r0) : "$12", "memory")

typedef struct {
    s32 state0;
    s32 frame4; /* frame */
    SVECTOR pos8; /* summoner position */
    SVECTOR rot10; /* summoner rotation */
    SVECTOR pos18; /* summon position */
    SVECTOR pos20; /* ribbon head */
    s32 model28; /* slice model */
    SVECTOR rot2C; /* summon rotation */
    s32 scale34[3]; /* summon scale */
} Fx05eWork; /* 0x40 */

/* the summon slot behind env->loader */
typedef struct {
    u8 pad0[0x54];
    struct {
        u8 pad0[0x180];
        BtlMotion *motion180;
    } *summon54; /* g_btlfxLoader.summon */
    u8 pad58[8];
    s32 model60; /* summon model */
} Fx05eSummon;

extern s32 fx_05e_facingBlocks[2];
extern SVECTOR fx_05e_ribbonRgb0;
extern SVECTOR fx_05e_ribbonRgb1;
extern SVECTOR fx_05e_ribbonRgb2;
extern SVECTOR fx_05e_bonePt0;
extern SVECTOR fx_05e_bonePt1;
extern SVECTOR fx_05e_bonePt2;
extern u8 fx_05e_trails[];
extern Unk80100DC054 fx_05e_summon;
extern BtlModelBuf fx_05e_effModels[];
extern u8 fx_05e_trails2[];
extern Fx05eSeg fx_05e_ribbonA[];
extern Fx05eSeg fx_05e_ribbonB[];
extern Unk800F78A4 *fx_05e_env;

static void fxm_drawRibbonTrail(SVECTOR *posA, SVECTOR *posB, u32 n, CVECTOR *colorA, CVECTOR *colorB, s32 fade, Fx05eSeg *buf, s32 abr);
static void fxm_getSummonBonePoint(s32 model, s32 bone, SVECTOR *in, SVECTOR *out);

#endif
