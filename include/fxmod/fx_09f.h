#ifndef FX_09F_H
#define FX_09F_H

/* Effect module fx_09f (dir 13 file 0x9F): Pyro (party; enemy Pyro of Benero/King Leo/Zenero). Emitters only.
 * Evidence: [data] the party ability / item tables and enemy sequences that play this file id
 * (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    s32 state0; /* state */
    SVECTOR pos4; /* caster position */
    SVECTOR posC; /* caster body position */
} Fx09fWork;

extern Unk800F78A4 *fx_09f_env;

#endif
