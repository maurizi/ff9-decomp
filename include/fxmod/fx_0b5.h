#ifndef FX_0B5_H
#define FX_0B5_H

/* Effect module fx_0b5 (dir 13 file 0xB5): enemy Rainbow Wind (Zemzelett). Evidence: [data] the party ability /
 * item tables and enemy sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round
 * 31 runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR pos0; /* caster position */
    SVECTOR pos8; /* caster body position */
} Fx0b5Work;

extern Unk800F78A4 *fx_0b5_env;
extern s32 fx_0b5_trails[];

#endif
