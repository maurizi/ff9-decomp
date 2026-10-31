#ifndef FX_115_H
#define FX_115_H

/* Effect module fx_115 (dir 13 file 0x115): Throw pinwheel/rising sun. Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR pos0; /* caster */
    SVECTOR pos8; /* target */
    SVECTOR pos10;
    SVECTOR rot18; /* model rotation */
    VECTOR pos20; /* model position */
    VECTOR scale30; /* model scale */
    SVECTOR unk40[4]; /* rotation trail */
    VECTOR unk60[4]; /* position trail */
    u16 modelA0; /* models */
    u16 modelA2;
} Fx115Work80;

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR unk4;
} Fx115Work81;

extern Unk800F78A4 *fx_115_env;
extern BtlModel fx_115_effModels[];

/* propagated */

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4; /* caster body position */
} Fx115Work80_0bd;
s32 fx_115_op81(s32 mode, Fx115Work80_0bd *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

#endif
