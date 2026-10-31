#ifndef FX_0C3_H
#define FX_0C3_H

/* Effect module fx_0c3 (dir 13 file 0xC3): Curse. Evidence: [data] the party ability / item tables and enemy
 * sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* caster position */
} Fx0c3Work;

extern Unk800F78A4 *fx_0c3_env;

#endif
