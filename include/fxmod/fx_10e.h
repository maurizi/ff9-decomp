#ifndef FX_10E_H
#define FX_10E_H

/* Effect module fx_10e (dir 13 file 0x10E): Throw rod/flute/staff. Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_10e effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s4.md; "(look pending)" = by kind, appearance not settled). */
#define FX_THROW_ROD            0x4 /* 'so' model: the thrown rod / flute / staff, drawn on its path (the same model as entry 0xC, 'so' form) */
#define FX_THROW_IMPACT 0x5 /* emitter: white-lilac gouraud ring + sprites (render black) at the hit (byte-identical in every Throw module) */
#define FX_THROW_ROD_AFTERIMAGE 0xC /* model: rod / flute / staff (54 polys, weapon texture), drawn as fading afterimages along the path */

/* fx_10e choreography (docs/fxdata.md, "Choreography literals"): op80 throws the weapon model from the caster's
 * hand (bone 0x17) to the target with fading afterimages, op81 plays the impact. The Throw modules (fx_109 ..
 * fx_110, fx_115) share these names with their own values. */
#define FX_THROW_MODEL_SLOTS      2    /* btlfx_initEffModels slots: the weapon and its afterimage model */
#define FX_THROW_FLIGHT_FRAMES     8    /* op80's length: the path ramp (frame << 9) reaches the target on this frame */
#define FX_THROW_PATH_FRAMES       0xA  /* the path and spin update while frame < this */
#define FX_THROW_ARC_HEIGHT        400  /* the throw arc's peak height */
#define FX_THROW_AFTERIMAGES       3    /* afterimages drawn behind the weapon (rot40 / pos58 history, one frame apart) */
#define FX_THROW_AFTERIMAGE_DIM    16   /* each older afterimage is this much darker */
#define FX_THROW_AFTERIMAGE_TPAGE  0x97   /* Hi_RegisterTexEffModel texture of the afterimage: 8-bit page (0x1C0, 0x100), abr 0 */
#define FX_THROW_AFTERIMAGE_CLUT   0x3D40 /* CLUT at (0x0, 0xF5) */
#define FX_THROW_AFTERIMAGE_U      0      /* texture origin (u, v) in the page */
#define FX_THROW_AFTERIMAGE_V      0
#define FX_THROW_IMPACT_FRAMES     0x1E /* op81's length: IMPACT at the target */

typedef struct {
    SVECTOR pos0; /* caster */
    SVECTOR pos8; /* target */
    SVECTOR pos10;
    SVECTOR rot18; /* model rotation */
    VECTOR pos20; /* model position */
    VECTOR scale30; /* model scale */
    SVECTOR rot40[FX_THROW_AFTERIMAGES]; /* rotation trail */
    VECTOR pos58[FX_THROW_AFTERIMAGES]; /* position trail */
    u16 model88; /* models */
    u16 model8A;
} Fx10eWork80;

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4;
} Fx10eWork81;

extern Unk800F78A4 *fx_10e_env;
extern BtlModelBuf fx_10e_effModels[];

#endif
