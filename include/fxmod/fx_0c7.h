#ifndef FX_0C7_H
#define FX_0C7_H

/* Effect module fx_0c7 (dir 13 file 0xC7): Focus. Evidence: [data] the party ability / item tables and enemy
 * sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4; /* unit 0x10 body position */
} Fx0c7Work;

extern Unk800F78A4 *fx_0c7_env;

#endif
