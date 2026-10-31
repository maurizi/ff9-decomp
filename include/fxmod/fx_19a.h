#ifndef FX_19A_H
#define FX_19A_H

/* Effect module fx_19a (dir 13 file 0x19a): Minus Strike (alt). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

extern Unk800F78A4 *fx_19a_env;

/* propagated */

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4; /* caster body position */
} Fx19aWork80;
s32 fx_19a_op80(s32 mode, Fx19aWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

#endif
