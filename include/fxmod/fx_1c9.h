#ifndef FX_1C9_H
#define FX_1C9_H

/* Effect module fx_1c9 (dir 13 file 0x1c9): enemy Thunder Slash (Beatrix). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4;
    s32 colorC;
} Fx1c9Work;

extern Unk800F78A4 *fx_1c9_env;

#endif
