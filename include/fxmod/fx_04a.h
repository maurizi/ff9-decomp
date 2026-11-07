#ifndef FX_04A_H
#define FX_04A_H

/* Effect module fx_04a (dir 13 file 0x4A). */
#include "btlfx_mod.h"

/* fx_04a effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_GOBLIN_PUNCH_GOLD_RINGS 0x7 /* emitter: gold-white and yellow-white gouraud rings + blue sparkles */

/* fx_04a choreography (docs/fxdata.md, "Choreography literals"): op80's length. */
#define FX_GOBLIN_PUNCH_FRAMES 0x20 /* op80: the task ends at this frame */

typedef struct {
    s32 hold0; /* nonzero: the op idles and never ends by itself (runtime); always 0 in the game (see Fx046Work.hold0) */
    SVECTOR pos4; /* target body position */
} Fx04aWork;

extern Unk800F78A4 *fx_04a_env;

#endif
