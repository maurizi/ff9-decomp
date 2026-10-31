#ifndef FX_135_H
#define FX_135_H

/* Effect module fx_135 (dir 13 file 0x135): enemy Bad Breath, Curse (Hades, ...). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8;
    SVECTOR pos10;
    SVECTOR rot18;
} Fx135Work;

extern Unk800F78A4 *fx_135_env;

#endif
