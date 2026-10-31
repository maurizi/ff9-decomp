#ifndef FX_0ED_H
#define FX_0ED_H

/* Effect module fx_0ed (dir 13 file 0x0ED): Roulette (party and enemy). Ribbon trails (fxm_drawRibbonTrail).
 * Evidence: [data] the party ability / item tables and enemy sequences that play this file id
 * (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    s32 state0; /* state */
    s32 endFrame4; /* end frame of the current target */
    s32 frame8; /* frame counter */
    BtlUnit *unitC; /* target */
    SVECTOR pos10; /* target position */
} Fx0edWork81; /* 0x18 */


extern Unk800F78A4 *fx_0ed_env;

s32 fx_0ed_op81(s32 mode, Fx0edWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

typedef struct {
    SVECTOR a0;
    SVECTOR b8;
} Fx0edSeg; /* 0x10 */
static void fxm_drawRibbonTrail(SVECTOR *a, SVECTOR *b, u32 n, CVECTOR *ca, CVECTOR *cb, s32 p, Fx0edSeg *buf, s32 abr);
static void fxm_getSummonBonePoint(s32 model, s32 bone, SVECTOR *in, SVECTOR *out);

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    SVECTOR pos8; /* unit position */
    SVECTOR rot10; /* unit rotation */
    SVECTOR pos18; /* model position */
    SVECTOR unk20; /* unused (runtime watch, round 33) */
    s32 model28; /* model */
    SVECTOR rot2C; /* model rotation */
    s32 scale34[3]; /* model scale */
} Fx0edWork80; /* 0x40 */

typedef struct {
    u8 pad0[0x54];
    struct {
        u8 pad0[0x180];
        BtlMotion *motion180;
    } *summon54; /* g_btlfxLoader.summon */
    u8 pad58[0x60 - 0x58];
    s32 summonModel60; /* summon model */
} Fx0edSummon;

extern s32 fx_0ed_facingBlocks[2];
extern CVECTOR fx_0ed_ribbonRgb0;
extern CVECTOR fx_0ed_ribbonRgb1;
extern CVECTOR fx_0ed_ribbonRgb2;
extern SVECTOR fx_0ed_ribbonPt0;
extern SVECTOR fx_0ed_ribbonPt1;
extern SVECTOR fx_0ed_ribbonPt2;
extern u8 fx_0ed_op80Trails0[];
extern Unk80100DC054 fx_0ed_summonModel;
extern u8 fx_0ed_op80Trails1[];
extern Fx0edSeg fx_0ed_ribbon0[17];
extern Fx0edSeg fx_0ed_ribbon1[17];
extern BtlModelBuf fx_0ed_effModels[];

#endif
