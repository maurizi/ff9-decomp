#ifndef FX_0BF_H
#define FX_0BF_H

/* Effect module fx_0bf (dir 13 file 0xBF): Thunder Slash. Evidence: [data] the party ability / item tables and
 * enemy sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime
 * pass"). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4; /* caster body position */
    s32 tintC; /* screen tint */
} Fx0bfWork80;

typedef struct {
    SVECTOR pos0; /* unit 0x10 position */
} Fx0bfWork81;

extern Unk800F78A4 *fx_0bf_env;

#endif
