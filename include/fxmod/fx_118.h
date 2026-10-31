#ifndef FX_118_H
#define FX_118_H

/* Effect module fx_118 (dir 13 file 0x118): item Eye Drops. Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4;
} Fx118Work;

extern Unk800F78A4 *fx_118_env;

#endif
