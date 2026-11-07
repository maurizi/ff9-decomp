#ifndef FX_046_H
#define FX_046_H

/* Effect module fx_046 (dir 13 file 0x46). */
#include "btlfx_mod.h"

/* fx_046 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_FIRE_FLAME_BURST 0x2 /* emitter: flame explosion (burst, smoke, embers) over the targets */
#define FX_FIRE_HEAT_RING   0x3 /* emitter: white-red gouraud rings on the ground */

/* fx_046 choreography (docs/fxdata.md, "Choreography literals"): op80's length. */
#define FX_FIRE_FRAMES 0x3C /* op80: the task ends at this frame (the burst and heat ring start at once) */

typedef struct {
    s32 hold0; /* nonzero: the op skips its frame logic and never ends itself, so the task stays alive until the
                * effect script ends (runtime: Fire with it poked to 1 ran the op 165 times instead of 63). Zeroed
                * in mode 1 and nothing else writes it, so it is always 0 in the game (same in 21 other modules). */
    SVECTOR pos4;
} Fx046Work;

extern Unk800F78A4 *fx_046_env;

#endif
