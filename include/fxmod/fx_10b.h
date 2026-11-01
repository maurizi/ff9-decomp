#ifndef FX_10B_H
#define FX_10B_H

/* Effect module fx_10b (dir 13 file 0x10B): Throw sword. Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR pos0; /* caster */
    SVECTOR pos8; /* target */
    SVECTOR pos10;
    SVECTOR rot18; /* model rotation */
    VECTOR pos20; /* model position */
    VECTOR scale30; /* model scale */
    SVECTOR rot40[2]; /* rotation trail */
    VECTOR pos50[2]; /* position trail */
    u16 model70; /* models */
    u16 model72;
} Fx10bWork80;

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR unk4; /* unused */
} Fx10bWork81;

extern Unk800F78A4 *fx_10b_env;
extern BtlModelBuf fx_10b_effModels[];

/* propagated */

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4; /* caster body position */
} Fx10bWork80_0bd;
s32 fx_10b_op81(s32 mode, Fx10bWork80_0bd *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

#endif
