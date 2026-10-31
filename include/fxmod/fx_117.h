#ifndef FX_117_H
#define FX_117_H

/* Effect module fx_117 (dir 13 file 0x117): item Echo Screen; item Dead Pepper; item Gysahl Greens. Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4;
} Fx117Work;

extern Unk800F78A4 *fx_117_env;

#endif
