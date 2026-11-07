#ifndef FX_0FE_H
#define FX_0FE_H

/* Effect module fx_0fe (dir 13 file 0x0FE): Eat. Six models orbiting the target (fx_0fe_orbits). Evidence: [data]
 * the party ability / item tables and enemy sequences that play this file id (tools/effect_labels.py;
 * docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0fe effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s4.md; "(look pending)" = by kind, appearance not settled). */
#define FX_EAT_WHITE_RINGS 0x4 /* emitter: white-lilac and white-cyan gouraud rings in a cone + a fan element (byte-identical to fx_0ff entry 0x4) */

/* fx_0fe choreography (docs/fxdata.md, "Choreography literals"): op80's keys, the orbiting models and their fades
 * (shared names with the Eat / Cook twin, fx_0fe / fx_0ff). */
#define FX_EAT_MODEL_COUNT       3    /* btlfx_initEffModels slots: the models of fx_0fe_modelBlocks, cycled over the orbits */
#define FX_EAT_ORBIT_COUNT       6    /* fx_0fe_orbits: models evenly spaced round the target */
#define FX_EAT_MIN_HEIGHT        0x118 /* the orbit centre (target body position) stays at least this high */
#define FX_EAT_MAX_RADIUS        0x898 /* orbit radius = the target's model radius, at most this */
#define FX_EAT_FADE_IN_FRAMES    4    /* the models fade in from frame 0 over this many frames */
#define FX_EAT_FADE_OUT_FRAME    0x14 /* and out from this frame */
#define FX_EAT_FADE_OUT_FRAMES   8    /* over this many */
#define FX_EAT_WHITE_RINGS_FRAME 0x16 /* WHITE_RINGS start */
#define FX_EAT_ORBIT_FRAMES      0x1C /* the models draw for this many frames */
#define FX_EAT_FRAMES            0x28 /* op80's length (the op ends) */

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR rot4[FX_EAT_ORBIT_COUNT]; /* model rotations */
    VECTOR pos34[FX_EAT_ORBIT_COUNT]; /* model positions */
    VECTOR scale94; /* model scale */
    s32 rgbA4; /* model brightness */
    SVECTOR posA8; /* target body position */
    s32 fadeInTimerB0; /* fade-in countdown */
    s32 fadeOutTimerB4; /* fade-out countdown */
    u16 modelB8[FX_EAT_MODEL_COUNT]; /* models */
} Fx0feWork; /* 0xC0 */

typedef struct {
    s16 radius0; /* radius */
    s16 angle2; /* angle */
    s16 spin4; /* spin */
} Fx0feOrbit;

extern u16 fx_0fe_modelBlocks[FX_EAT_MODEL_COUNT];
extern Unk800F78A4 *fx_0fe_env;
extern BtlModelBuf fx_0fe_effModels[];
extern Fx0feOrbit fx_0fe_orbits[FX_EAT_ORBIT_COUNT];

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);

#endif
