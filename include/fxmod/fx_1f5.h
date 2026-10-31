#ifndef FX_1F5_H
#define FX_1F5_H

/* Effect module fx_1f5 (dir 13 file 0x1f5): Night; enemy Night (Nymph, ...). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4;
} Fx1f5Work80;

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4;
    s32 unkC; /* unit */
} Fx1f5Work82;

extern Unk800F78A4 *fx_1f5_env;
extern s32 fx_1f5_op81Trails[];

#endif
