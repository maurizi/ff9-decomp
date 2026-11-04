#ifndef FX_110_H
#define FX_110_H

/* Effect module fx_110 (dir 13 file 0x110): Throw dagger. Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_110 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s4.md; "(look pending)" = by kind, appearance not settled). */
#define FX_THROW_DAGGER_IMPACT     0x5 /* emitter: white-lilac gouraud ring + sprites (render black) at the hit (byte-identical in every Throw module) */
#define FX_THROW_DAGGER            0x6 /* 'so' model: the thrown dagger, drawn on its path (the same model as entry 0xC, 'so' form) */
#define FX_THROW_DAGGER_AFTERIMAGE 0xC /* model: dagger (48 polys, weapon texture), drawn as fading afterimages along the path */

typedef struct {
    SVECTOR pos0; /* caster */
    SVECTOR pos8; /* target */
    SVECTOR pos10;
    SVECTOR rot18; /* model rotation */
    VECTOR pos20; /* model position */
    VECTOR scale30; /* model scale */
    SVECTOR rot40[4]; /* rotation trail */
    VECTOR pos60[4]; /* position trail */
    u16 modelA0; /* models */
    u16 modelA2;
} Fx110Work80;

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4;
} Fx110Work81;

extern Unk800F78A4 *fx_110_env;
extern BtlModelBuf fx_110_effModels[];

#endif
