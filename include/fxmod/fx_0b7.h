#ifndef FX_0B7_H
#define FX_0B7_H

/* Effect module fx_0b7 (dir 13 file 0xB7): enemy Heavy (Jabberwock). Evidence: [data] the party ability / item
 * tables and enemy sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31
 * runtime pass"). */
#include "btlfx_mod.h"

/* fx_0b7 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_HEAVY_RED_STREAKS 0x2 /* emitter: two orbiting fans of small red streaks */
#define FX_HEAVY_GREEN_SWIRL 0x6 /* emitter: green swirl ring particle + pink-white gouraud rings */

/* fx_0b7 choreography (docs/fxdata.md, "Choreography literals"): op80's length. */
#define FX_HEAVY_FRAMES 0x45 /* op80: the task ends at this frame */

typedef struct {
    SVECTOR pos0; /* caster position */
} Fx0b7Work;

extern Unk800F78A4 *fx_0b7_env;

#endif
