#ifndef FX_18E_H
#define FX_18E_H

/* Effect module fx_18e (dir 13 file 0x18e): Charge!. Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4;
    SVECTOR posC;
    SVECTOR pos14;
} Fx18eWork;

extern Unk800F78A4 *fx_18e_env;

#endif
