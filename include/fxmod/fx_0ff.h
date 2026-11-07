#ifndef FX_0FF_H
#define FX_0FF_H

/* Effect module fx_0ff (dir 13 file 0x0FF): Cook. Six models orbiting the target (fx_0ff_orbits). Evidence: [data]
 * the party ability / item tables and enemy sequences that play this file id (tools/effect_labels.py;
 * docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0ff effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s4.md; "(look pending)" = by kind, appearance not settled). */
#define FX_COOK_WHITE_RINGS 0x4 /* emitter: white-lilac and white-cyan gouraud rings in a cone + a fan element (byte-identical to fx_0fe entry 0x4) */
#define FX_COOK_FLAMES      0x8 /* emitter: flame tongue particles + white-orange gouraud ring */

/* fx_0ff choreography (docs/fxdata.md, "Choreography literals"): op80's keys, the orbiting models and their fades
 * (shared names with the Eat / Cook twin, fx_0fe / fx_0ff). */
#define FX_COOK_MODEL_COUNT       3    /* btlfx_initEffModels slots: the models of fx_0ff_modelBlocks, cycled over the orbits */
#define FX_COOK_ORBIT_COUNT       6    /* fx_0ff_orbits: models evenly spaced round the target */
#define FX_COOK_MIN_HEIGHT        0x118 /* the orbit centre (target body position) stays at least this high */
#define FX_COOK_MAX_RADIUS        0x898 /* orbit radius = the target's model radius, at most this */
#define FX_COOK_FADE_IN_FRAMES    4    /* the models fade in from frame 0 over this many frames */
#define FX_COOK_FADE_OUT_FRAME    0x14 /* and out from this frame */
#define FX_COOK_FADE_OUT_FRAMES   8    /* over this many */
#define FX_COOK_FLAMES_FRAME      2    /* FLAMES start at the target position */
#define FX_COOK_WHITE_RINGS_FRAME 0x16 /* WHITE_RINGS start */
#define FX_COOK_ORBIT_FRAMES      0x1C /* the models draw for this many frames */
#define FX_COOK_FRAMES            0x28 /* op80's length (the op ends) */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR rot8[FX_COOK_ORBIT_COUNT]; /* model rotations */
    VECTOR pos38[FX_COOK_ORBIT_COUNT]; /* model positions */
    VECTOR scale98; /* model scale */
    s32 rgbA8; /* model brightness */
    SVECTOR posAC; /* target body position */
    SVECTOR posB4; /* target position */
    s32 fadeInTimerBC; /* fade-in countdown */
    s32 fadeOutTimerC0; /* fade-out countdown */
    u16 modelC4[FX_COOK_MODEL_COUNT]; /* models */
} Fx0ffWork; /* 0xCC */

typedef struct {
    s16 radius0; /* radius */
    s16 angle2; /* angle */
    s16 spin4; /* spin */
} Fx0ffOrbit;

extern u16 fx_0ff_modelBlocks[FX_COOK_MODEL_COUNT];
extern Unk800F78A4 *fx_0ff_env;
extern BtlModelBuf fx_0ff_effModels[];
extern Fx0ffOrbit fx_0ff_orbits[FX_COOK_ORBIT_COUNT];
static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);

#endif
