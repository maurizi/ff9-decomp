#ifndef FX_0F9_H
#define FX_0F9_H

/* Effect module fx_0f9 (dir 13 file 0x0F9): Flee. One emitter at the caster. Evidence: [data] the party ability /
 * item tables and enemy sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round
 * 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0f9 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s4.md; "(look pending)" = by kind, appearance not settled). */
#define FX_FLEE_SPARKLE_BURST 0x1 /* emitter: pale gouraud rings, pale sparkle puffs and orange sparks */

/* fx_0f9 choreography (docs/fxdata.md, "Choreography literals"). */
#define FX_FLEE_FRAMES 0x40 /* op80's length (the op ends) */

extern Unk800F78A4 *fx_0f9_env;

#endif
