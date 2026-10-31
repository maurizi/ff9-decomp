#ifndef FX_08A_H
#define FX_08A_H

/* Effect module fx_08a (dir 13 file 0x8A): Auto-Life. Emitters only. Evidence: [data] the party ability / item
 * tables and enemy sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31
 * runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    s32 state0; /* state */
    SVECTOR pos4; /* caster body position */
} Fx08aWork;

extern Unk800F78A4 *fx_08a_env;
extern s32 fx_08a_trails[];

#endif
