#ifndef FX_19B_H
#define FX_19B_H

/* Effect module fx_19b (dir 13 file 0x19b): Iai Strike (alt). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8;
} Fx19bWork;

extern Unk800F78A4 *fx_19b_env;

#endif
