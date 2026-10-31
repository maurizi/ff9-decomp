#ifndef FX_10D_H
#define FX_10D_H

/* Effect module fx_10d (dir 13 file 0x10D): Throw claws. Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR pos0; /* caster */
    SVECTOR pos8; /* target */
    SVECTOR pos10;
    SVECTOR rot18; /* model rotation */
    VECTOR pos20; /* model position */
    VECTOR scale30; /* model scale */
    SVECTOR unk40[3]; /* rotation trail */
    VECTOR unk58[3]; /* position trail */
    u16 model88; /* models */
    u16 model8A;
} Fx10dWork80;

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4;
} Fx10dWork81;

extern Unk800F78A4 *fx_10d_env;
extern BtlModel fx_10d_effModels[];

#endif
