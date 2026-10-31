#ifndef FX_0BB_H
#define FX_0BB_H

/* Effect module fx_0bb (dir 13 file 0xBB): Mustard Bomb; enemy Mustard Bomb (Drakan, ...). Evidence: [data] the
 * party ability / item tables and enemy sequences that play this file id (tools/effect_labels.py;
 * docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4; /* caster body position */
} Fx0bbWork80;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* caster body position */
} Fx0bbWork81;

extern Unk800F78A4 *fx_0bb_env;

#endif
