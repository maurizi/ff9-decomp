#ifndef FX_199_H
#define FX_199_H

/* Effect module fx_199 (dir 13 file 0x199): Darkside (alt). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4;
} Fx199Work;

extern Unk800F78A4 *fx_199_env;

#endif
