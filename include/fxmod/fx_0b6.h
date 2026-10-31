#ifndef FX_0B6_H
#define FX_0B6_H

/* Effect module fx_0b6 (dir 13 file 0xB6): enemy Scorch (Grenade/Vepal). Evidence: [data] the party ability / item
 * tables and enemy sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31
 * runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR pos0;
} Fx0b6Work;

extern Unk800F78A4 *fx_0b6_env;

#endif
