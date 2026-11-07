#ifndef FX_136_H
#define FX_136_H

/* Effect module fx_136 (dir 13 file 0x136): enemy Absorb (Malboro). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_136 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_ABSORB_SPARKLE     0x1 /* emitter: cyan swirl collapsing into a star sparkle */
#define FX_ABSORB_SMOKE_SWIRL 0x3 /* emitter: curling white smoke */

/* fx_136 choreography (docs/fxdata.md, "Choreography literals"): the caster bones the pieces head for, the ops'
 * lengths. */
#define FX_ABSORB_SPARKLE_REACH  0x190 /* op80: SPARKLE flies from the target to this far ahead of the caster's bone 2 */
#define FX_ABSORB_SPARKLE_FRAMES 0x28  /* op80's length */
#define FX_ABSORB_SMOKE_BONE     0xF   /* op81: SMOKE_SWIRL starts at this bone of the caster */
#define FX_ABSORB_SMOKE_FRAMES   0x28  /* op81's length */

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4;
    SVECTOR posC;
} Fx136Work;

extern Unk800F78A4 *fx_136_env;

#endif
