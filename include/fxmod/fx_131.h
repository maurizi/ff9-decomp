#ifndef FX_131_H
#define FX_131_H

/* Effect module fx_131 (dir 13 file 0x131): enemy Golem Revival (Core/Sand Golem). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR pos0;
    SVECTOR pos8;
    SVECTOR pos10;
    Pool6CEntry *emit18;
} Fx131Work;

extern Unk800F78A4 *fx_131_env;

#endif
