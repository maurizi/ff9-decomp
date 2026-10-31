#ifndef FX_0C4_H
#define FX_0C4_H

/* Effect module fx_0c4 (dir 13 file 0xC4): Curse (Elan). Evidence: [data] the party ability / item tables and enemy
 * sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* position: env->targets->centerX..Z */
} Fx0c4Work;

extern Unk800F78A4 *fx_0c4_env;

#endif
