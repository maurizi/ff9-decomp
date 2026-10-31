#ifndef FX_137_H
#define FX_137_H

/* Effect module fx_137 (dir 13 file 0x137): enemy Flame (Cerberus, ...). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR rot0;
    SVECTOR pos8;
    SVECTOR pos10;
    SVECTOR pos18;
    Pool6CEntry *emit20;
} Fx137Work;

extern Unk800F78A4 *fx_137_env;
extern SVECTOR fx_137_targetPos[];

#endif
