#ifndef FX_124_H
#define FX_124_H

/* Effect module fx_124 (dir 13 file 0x124): items Amethyst, Aquamarine, Diamond, Emerald, Garnet, Lapis Lazuli, Moonstone, Opal and others. Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4;
} Fx124Work;

extern Unk800F78A4 *fx_124_env;

#endif
