#ifndef FX_0C7_H
#define FX_0C7_H

/* Effect module fx_0c7 (dir 13 file 0xC7): Focus. Evidence: [data] the party ability / item tables and enemy
 * sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0c7 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_FOCUS_GLOW_RING 0x1 /* emitter: orange gouraud ring, fans */

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4; /* unit 0x10 body position */
} Fx0c7Work;

extern Unk800F78A4 *fx_0c7_env;

#endif
