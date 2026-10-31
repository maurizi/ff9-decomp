#ifndef FX_119_H
#define FX_119_H

/* Effect module fx_119 (dir 13 file 0x119): item Magic Tag. Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4;
} Fx119Work;

extern Unk800F78A4 *fx_119_env;

#endif
