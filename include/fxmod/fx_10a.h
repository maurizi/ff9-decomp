#ifndef FX_10A_H
#define FX_10A_H

/* Effect module fx_10a (dir 13 file 0x10A): Throw thief sword. Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_10a effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s4.md; "(look pending)" = by kind, appearance not settled). */
#define FX_THROW_THIEF_SWORD            0x1 /* 'so' model: the thrown thief sword, drawn on its path (the same model as entry 0xC, 'so' form) */
#define FX_THROW_THIEF_SWORD_IMPACT     0x5 /* emitter: white-lilac gouraud ring + sprites (render black) at the hit (byte-identical in every Throw module) */
#define FX_THROW_THIEF_SWORD_AFTERIMAGE 0xC /* model: thief sword (60 polys, weapon texture), drawn as fading afterimages along the path */

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
} Fx10aWork80;

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4;
} Fx10aWork81;

extern Unk800F78A4 *fx_10a_env;
extern BtlModelBuf fx_10a_effModels[];

#endif
