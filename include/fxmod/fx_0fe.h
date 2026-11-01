#ifndef FX_0FE_H
#define FX_0FE_H

/* Effect module fx_0fe (dir 13 file 0x0FE): Eat. Six models orbiting the target (fx_0fe_orbits). Evidence: [data]
 * the party ability / item tables and enemy sequences that play this file id (tools/effect_labels.py;
 * docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR rot4[6]; /* model rotations */
    VECTOR pos34[6]; /* model positions */
    VECTOR scale94; /* model scale */
    s32 rgbA4; /* model brightness */
    SVECTOR posA8; /* target body position */
    s32 fadeInTimerB0; /* fade-in countdown */
    s32 fadeOutTimerB4; /* fade-out countdown */
    u16 modelB8[3]; /* models */
} Fx0feWork;

typedef struct {
    s16 radius0; /* radius */
    s16 angle2; /* angle */
    s16 spin4; /* spin */
} Fx0feOrbit;

extern u16 fx_0fe_modelBlocks[3];
extern Unk800F78A4 *fx_0fe_env;
extern BtlModelBuf fx_0fe_effModels[];
extern Fx0feOrbit fx_0fe_orbits[6];

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);

#endif
