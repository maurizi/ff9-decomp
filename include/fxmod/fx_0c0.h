#ifndef FX_0C0_H
#define FX_0C0_H

/* Effect module fx_0c0 (dir 13 file 0xC0): Shock; Shock (Seiken); enemy Shock (Beatrix). Evidence: [data] the party
 * ability / item tables and enemy sequences that play this file id (tools/effect_labels.py;
 * docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    SVECTOR pos10; /* unit 0x10 body position */
    SVECTOR pos18; /* caster body position */
} Fx0c0Work;

extern Unk800F78A4 *fx_0c0_env;

#endif
