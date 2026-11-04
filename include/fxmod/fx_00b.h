#ifndef FX_00B_H
#define FX_00B_H

/* Effect module fx_00b (dir 13 file 0x0B). */
#include "btlfx_mod.h"

/* fx_00b effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_ESUNA_BACKDROP     0x7 /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture; not drawn by the tool) (look pending) */
#define FX_ESUNA_LIGHT_SHOWER 0x8 /* emitter: rainbow halo + blue ripple, falling golden streaks with white glints, lens flare */

typedef struct {
    s32 hold0; /* nonzero: the op idles and never ends by itself (runtime); always 0 in the game (see Fx046Work.hold0) */
    SVECTOR pos4; /* target body position */
    s32 unkC; /* unused (runtime watch, round 33) */
} Fx00bWork;

extern Unk800F78A4 *fx_00b_env;

#endif
