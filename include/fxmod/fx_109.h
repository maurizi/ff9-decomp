#ifndef FX_109_H
#define FX_109_H

/* Effect module fx_109 (dir 13 file 0x109): Throw racket. Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_109 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s4.md; "(look pending)" = by kind, appearance not settled). */
#define FX_THROW_RACKET            0x1 /* 'so' model: the thrown racket, drawn on its path (the same model as entry 0xC, 'so' form) (look pending) */
#define FX_THROW_RACKET_IMPACT     0x5 /* emitter: white-lilac gouraud ring + sprites (render black) at the hit (byte-identical in every Throw module) */
#define FX_THROW_RACKET_AFTERIMAGE 0xC /* model: racket (76 polys, weapon texture), drawn as fading afterimages along the path */

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
} Fx109Work80;

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4;
} Fx109Work81;

extern Unk800F78A4 *fx_109_env;
extern BtlModelBuf fx_109_effModels[];

#endif
