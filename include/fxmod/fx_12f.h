#ifndef FX_12F_H
#define FX_12F_H

/* Effect module fx_12f (dir 13 file 0x12F): enemy Paper Storm (Agares/Gargoyle, ...). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR rot0;
    SVECTOR pos8;
    SVECTOR pos10;
    SVECTOR pos18;
    SVECTOR pos20[4];
    Pool6CEntry *emit40;
} Fx12fWork;

extern Unk800F78A4 *fx_12f_env;

#endif
