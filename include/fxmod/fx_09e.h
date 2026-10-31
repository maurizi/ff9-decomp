#ifndef FX_09E_H
#define FX_09E_H

/* Effect module fx_09e (dir 13 file 0x9E): Blind, one target (party; enemy Blind of Quale and others). Emitters
 * only. Evidence: [data] the party ability / item tables and enemy sequences that play this file id
 * (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR pos0; /* caster body position */
} Fx09eWork;

extern Unk800F78A4 *fx_09e_env;

#endif
