#ifndef FX_0F1_H
#define FX_0F1_H

/* Effect module fx_0f1 (dir 13 file 0x0F1): Free Energy. One emitter at the target body. Evidence: [data] the party
 * ability / item tables and enemy sequences that play this file id (tools/effect_labels.py;
 * docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4;
} Fx0f1Work;

extern Unk800F78A4 *fx_0f1_env;

#endif
