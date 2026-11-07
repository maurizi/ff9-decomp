#ifndef FX_10C_H
#define FX_10C_H

/* Effect module fx_10c (dir 13 file 0x10C): Throw spear. Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_10c effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s4.md; "(look pending)" = by kind, appearance not settled). */
#define FX_THROW_IMPACT 0x4 /* emitter: white-lilac gouraud ring + sprites (render black) at the hit (byte-identical in every Throw module) */
#define FX_THROW_SPEAR            0x5 /* 'so' model: the thrown spear, drawn on its path (the same model as entry 0xC, 'so' form) */
#define FX_THROW_SPEAR_AFTERIMAGE 0xC /* model: spear (67 polys, weapon texture), drawn as fading afterimages along the path */

/* fx_10c choreography (docs/fxdata.md, "Choreography literals"): op80 throws the weapon model from the caster's
 * hand (bone 0x17) to the target with fading afterimages, op81 plays the impact. The Throw modules (fx_109 ..
 * fx_110, fx_115) share these names with their own values. */
#define FX_THROW_MODEL_SLOTS      2    /* btlfx_initEffModels slots: the weapon and its afterimage model */
#define FX_THROW_FLIGHT_FRAMES     6    /* the weapon flies for this many frames (the (frame << 12) / N path ramp); op80 ends */
#define FX_THROW_PATH_FRAMES       8    /* the path and spin update while frame < this */
#define FX_THROW_AFTERIMAGES       2    /* afterimages drawn behind the weapon (rot40 / pos50 history, one frame apart) */
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
    VECTOR pos50[FX_THROW_AFTERIMAGES]; /* position trail */
    u16 model70; /* models */
    u16 model72;
} Fx10cWork80;

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR unk4; /* unused */
} Fx10cWork81;

extern Unk800F78A4 *fx_10c_env;
extern BtlModelBuf fx_10c_effModels[];

/* propagated */

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4; /* caster body position */
} Fx10cWork80_0bd;
s32 fx_10c_op81(s32 mode, Fx10cWork80_0bd *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

#endif
