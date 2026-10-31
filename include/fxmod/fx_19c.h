#ifndef FX_19C_H
#define FX_19C_H

/* Effect module fx_19c (dir 13 file 0x19c): Thunder Slash (Seiken). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4;
    s32 colorC;
} Fx19cWork;

extern Unk800F78A4 *fx_19c_env;

#endif
