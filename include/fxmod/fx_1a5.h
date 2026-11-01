#ifndef FX_1A5_H
#define FX_1A5_H

/* Effect module fx_1a5 (dir 13 file 0x1a5): Revive (Elan). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8;
    s32 unit10; /* unit */
} Fx1a5Work;

extern Unk800F78A4 *fx_1a5_env;

static void fxm_setSignal(Unk800F78A4 *env, s32 signal, s32 value);

#endif
