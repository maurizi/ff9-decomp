#ifndef FX_136_H
#define FX_136_H

/* Effect module fx_136 (dir 13 file 0x136): enemy Absorb (Malboro). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4;
    SVECTOR posC;
} Fx136Work;

extern Unk800F78A4 *fx_136_env;

#endif
