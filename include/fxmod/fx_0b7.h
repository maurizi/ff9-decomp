#ifndef FX_0B7_H
#define FX_0B7_H

/* Effect module fx_0b7 (dir 13 file 0xB7): enemy Heavy (Jabberwock). Evidence: [data] the party ability / item
 * tables and enemy sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31
 * runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR pos0; /* caster position */
} Fx0b7Work;

extern Unk800F78A4 *fx_0b7_env;

#endif
