#ifndef FX_0E4_H
#define FX_0E4_H

/* Effect module fx_0e4 (dir 13 file 0x0E4): Frog Drop. A model dropped in front of the target (morphed, then
 * slice-drawn as it sinks), rings (btlfx_drawRings) and emitters. Evidence: [data] the party ability / item tables
 * and enemy sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime
 * pass"). */
#include "btlfx_mod.h"

/* fx_0e4 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s3.md; "(look pending)" = by kind, appearance not settled). */
#define FX_FROG_DROP_STARS         0x5 /* emitter: yellow stars orbiting (dizzy stars) */
#define FX_FROG_DROP_DUST_RING     0x6 /* rings: white-grey gouraud rings */
#define FX_FROG_DROP_IMPACT_DEBRIS 0x7 /* emitter: ring of white debris sprites + white-cyan-blue rings */

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
extern BtlModelBuf fx_0e4_effModels[];

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);

#endif
