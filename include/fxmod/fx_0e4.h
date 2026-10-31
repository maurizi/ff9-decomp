#ifndef FX_0E4_H
#define FX_0E4_H

/* Effect module fx_0e4 (dir 13 file 0x0E4): Frog Drop. A model dropped in front of the target (morphed, then
 * slice-drawn as it sinks), rings (btlfx_drawRings) and emitters. Evidence: [data] the party ability / item tables
 * and enemy sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime
 * pass"). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR rot8; /* model rotation */
    VECTOR pos10; /* model position */
    VECTOR scale20; /* model scale */
    s32 rgb30; /* model brightness */
    SVECTOR pos34; /* target position */
    SVECTOR rot3C; /* target rotation */
    SVECTOR pos44;
    SVECTOR ringPos4C;
    SVECTOR ringRot54;
    s16 ringScale5C;
    s16 ringAlpha5E;
    s16 dropTimer60; /* drop countdown */
    s16 fadeTimer62; /* fade countdown */
    s32 state64; /* state */
    u16 model68[3]; /* models */
} Fx0e4Work;

extern u16 fx_0e4_modelBlocks[3];
extern Unk800F78A4 *fx_0e4_env;
extern BtlModel fx_0e4_effModels[];

static void fxm_lerpSvec(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);

#endif
