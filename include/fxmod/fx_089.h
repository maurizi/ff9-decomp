#ifndef FX_089_H
#define FX_089_H

/* Effect module fx_089 (dir 13 file 0x89): Revive. Emitters only. Evidence: [data] the party ability / item
 * tables and enemy sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31
 * runtime pass"). */
#include "btlfx_mod.h"

/* fx_089 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_REVIVE_LIGHT_SWIRL 0x4 /* emitter: blue-white light swirl + flare */
#define FX_REVIVE_FEATHERS    0x5 /* emitter: white wing / feather sprites + flare */

/* fx_089 choreography (docs/fxdata.md, "Choreography literals"). */
#define FX_REVIVE_FEATHERS_FRAME 0xF  /* FX_REVIVE_FEATHERS starts */
#define FX_REVIVE_FRAMES         0x28 /* op80's length (the op ends) */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* caster position */
} Fx089Work;

extern Unk800F78A4 *fx_089_env;

#endif
