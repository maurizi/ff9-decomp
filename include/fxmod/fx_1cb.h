#ifndef FX_1CB_H
#define FX_1CB_H

/* Effect module fx_1cb (dir 13 file 0x1cb): enemy Stock Break (Beatrix). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4;
} Fx1cbWork;

extern Unk800F78A4 *fx_1cb_env;

#endif
