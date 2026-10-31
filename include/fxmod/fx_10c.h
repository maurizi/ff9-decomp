#ifndef FX_10C_H
#define FX_10C_H

/* Effect module fx_10c (dir 13 file 0x10C): Throw spear. Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR pos0; /* caster */
    SVECTOR pos8; /* target */
    SVECTOR pos10;
    SVECTOR rot18; /* model rotation */
    VECTOR pos20; /* model position */
    VECTOR scale30; /* model scale */
    SVECTOR unk40[2]; /* rotation trail */
    VECTOR unk50[2]; /* position trail */
    u16 model70; /* models */
    u16 model72;
} Fx10cWork80;

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR unk4;
} Fx10cWork81;

extern Unk800F78A4 *fx_10c_env;
extern BtlModel fx_10c_effModels[];

/* propagated */

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4; /* caster body position */
} Fx10cWork80_0bd;
s32 fx_10c_op81(s32 mode, Fx10cWork80_0bd *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

#endif
