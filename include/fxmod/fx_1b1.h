#ifndef FX_1B1_H
#define FX_1B1_H

/* Effect module fx_1b1 (dir 13 file 0x1b1): enemy Golem Death (Core/Sand Golem). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_1b1 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s6.md; "(look pending)" = by kind, appearance not settled). */
#define FX_GOLEM_DEATH_DUST_CLOUD   0x1 /* emitter: tan dust cloud quads, 17 frames (sprites render black) */
#define FX_GOLEM_DEATH_DUST_CLOUD_2 0x5 /* emitter: orbiting tan dust cloud quads, 17 frames (sprites render black) */

/* fx_1b1 choreography (docs/fxdata.md, "Choreography literals"): the dust clouds' keyframes and op80's length. */
#define FX_GOLEM_DEATH_CLOUD_HEIGHT  0xC8 /* DUST_CLOUD starts this far below the target's body position (+vy) */
#define FX_GOLEM_DEATH_CLOUD_2_FRAME 0x18 /* DUST_CLOUD_2 starts at the target's feet */
#define FX_GOLEM_DEATH_FRAMES        0x50 /* op80's length (the op ends) */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8;
    SVECTOR pos10;
} Fx1b1Work; /* 0x18 */

extern Unk800F78A4 *fx_1b1_env;

#endif
