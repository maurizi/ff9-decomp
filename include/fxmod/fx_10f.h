#ifndef FX_10F_H
#define FX_10F_H

/* Effect module fx_10f (dir 13 file 0x10F): Throw fork. Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

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
} Fx10fWork80;

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4;
} Fx10fWork81;

extern Unk800F78A4 *fx_10f_env;
extern BtlModelBuf fx_10f_effModels[];

#endif
