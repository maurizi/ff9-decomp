#ifndef FX_0C5_H
#define FX_0C5_H

/* Effect module fx_0c5 (dir 13 file 0xC5): Aura. Evidence: [data] the party ability / item tables and enemy
 * sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0c5 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_AURA_SWIRL       0x7 /* emitter: yellow / white / lilac swirl particles, blue star flash, sparkles, fans */
#define FX_AURA_FAN_EMITTER 0x8 /* emitter: three orbiting fan elements (not rendered) (look pending) */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* caster position */
} Fx0c5Work;

extern Unk800F78A4 *fx_0c5_env;

#endif
