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

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR rot8[6]; /* model rotations */
    VECTOR pos38[6]; /* model positions */
    VECTOR scale98; /* model scale */
    s32 rgbA8; /* model brightness */
    SVECTOR posAC; /* target body position */
    SVECTOR posB4; /* target position */
    s32 fadeInTimerBC; /* fade-in countdown */
    s32 fadeOutTimerC0; /* fade-out countdown */
    u16 modelC4[3]; /* models */
} Fx0ffWork;

typedef struct {
    s16 radius0; /* radius */
    s16 angle2; /* angle */
    s16 spin4; /* spin */
} Fx0ffOrbit;

extern u16 fx_0ff_modelBlocks[3];
extern Unk800F78A4 *fx_0ff_env;
extern BtlModelBuf fx_0ff_effModels[];
extern Fx0ffOrbit fx_0ff_orbits[6];
static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);

#endif
