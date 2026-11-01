#ifndef FX_0C6_H
#define FX_0C6_H

/* Effect module fx_0c6 (dir 13 file 0xC6): Aura (Elan). Evidence: [data] the party ability / item tables and enemy
 * sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* unit position */
    s32 unit10; /* unit */
} Fx0c6Work;

extern Unk800F78A4 *fx_0c6_env;

static void fxm_setSignal(Unk800F78A4 *env, s32 signal, s32 value);

#endif
