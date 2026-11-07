#ifndef FX_115_H
#define FX_115_H

/* Effect module fx_115 (dir 13 file 0x115): Throw pinwheel/rising sun. Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_115 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s4.md; "(look pending)" = by kind, appearance not settled). */
#define FX_THROW_PINWHEEL_IMPACT 0x4 /* emitter: white-lilac gouraud ring + sprites (render black) at the hit */
#define FX_THROW_PINWHEEL        0x5 /* 'so' model: the thrown pinwheel / rising sun (a ring blade), registered twice */

/* fx_115 choreography (docs/fxdata.md, "Choreography literals"): op80 throws the weapon model from the caster's
 * hand (bone 0x17) to the target with fading afterimages, op81 plays the impact. The Throw modules (fx_109 ..
 * fx_110, fx_115) share these names with their own values. */
#define FX_THROW_MODEL_SLOTS      2    /* btlfx_initEffModels slots: the weapon and its afterimage model */
#define FX_THROW_FLIGHT_FRAMES     6    /* the weapon flies for this many frames (the (frame << 12) / N path ramp); op80 ends */
#define FX_THROW_PATH_FRAMES       8    /* the path and spin update while frame < this */
#define FX_THROW_AFTERIMAGES       4    /* afterimages drawn behind the weapon (rot40 / pos60 history, one frame apart) */
#define FX_THROW_AFTERIMAGE_DIM    20   /* each older afterimage is this much darker */
#define FX_THROW_IMPACT_FRAMES     0x1E /* op81's length: IMPACT at the target */

typedef struct {
    SVECTOR pos0; /* caster */
    SVECTOR pos8; /* target */
    SVECTOR pos10;
    SVECTOR rot18; /* model rotation */
    VECTOR pos20; /* model position */
    VECTOR scale30; /* model scale */
    SVECTOR rot40[FX_THROW_AFTERIMAGES]; /* rotation trail */
    VECTOR pos60[FX_THROW_AFTERIMAGES]; /* position trail */
    u16 modelA0; /* models */
    u16 modelA2;
} Fx115Work80;

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR unk4; /* unused */
} Fx115Work81;

extern Unk800F78A4 *fx_115_env;
extern BtlModelBuf fx_115_effModels[];

/* propagated */

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4; /* caster body position */
} Fx115Work80_0bd;
s32 fx_115_op81(s32 mode, Fx115Work80_0bd *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

#endif
