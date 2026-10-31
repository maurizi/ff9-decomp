#ifndef FX_080_H
#define FX_080_H

/* Effect module fx_080 (dir 13 file 0x80): Dispel. Emitters only. Evidence: [data] the party ability / item
 * tables and enemy sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31
 * runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    s32 state0; /* state */
    SVECTOR pos4; /* caster body position */
} Fx080Work;

extern Unk800F78A4 *fx_080_env;

#endif
