#ifndef FX_0EE_H
#define FX_0EE_H

/* Effect module fx_0ee (dir 13 file 0x0EE): enemy Mirror. Emitters in front of the caster. Evidence: [data] the
 * party ability / item tables and enemy sequences that play this file id (tools/effect_labels.py;
 * docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR pos0;
    SVECTOR rot8;
} Fx0eeWork;

extern Unk800F78A4 *fx_0ee_env;

#endif
