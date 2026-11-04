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

typedef struct {
    SVECTOR pos0; /* caster */
    SVECTOR pos8; /* target */
    SVECTOR pos10;
    SVECTOR rot18; /* model rotation */
    VECTOR pos20; /* model position */
    VECTOR scale30; /* model scale */
    SVECTOR rot40[3]; /* rotation trail */
    VECTOR pos58[3]; /* position trail */
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
