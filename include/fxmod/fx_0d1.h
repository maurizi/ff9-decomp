#ifndef FX_0D1_H
#define FX_0D1_H

/* Effect module fx_0d1 (dir 13 file 0xD1): Mental Break. Evidence: [data] the party ability / item tables and enemy
 * sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    SVECTOR pos14; /* unit 0x10 body position */
    SVECTOR pos1C; /* caster body position */
} Fx0d1Work;

extern Unk800F78A4 *fx_0d1_env;

#endif
