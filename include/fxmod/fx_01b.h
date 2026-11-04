#ifndef FX_01B_H
#define FX_01B_H

/* Effect module fx_01b (dir 13 file 0x1B: Float, from runtime traces of btlfx_loadFile). */
#include "btlfx_mod.h"

/* fx_01b effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_FLOAT_SPECK_RING 0x1 /* emitter: particle (magenta: texture still outside the file and the common pages) + ring textured with brown specks (look pending) */
#define FX_FLOAT_LIFT_RINGS 0x2 /* emitter: speckled ring, white-lilac gouraud rings, two fans */
#define FX_FLOAT_GLOW       0x3 /* emitter: one particle (magenta: texture still outside the file and the common pages) (look pending) */
#define FX_FLOAT_BACKDROP   0x4 /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture; not drawn by the tool) (look pending) */

typedef struct {
    s32 hold0; /* nonzero: the op idles and never ends by itself (runtime); always 0 in the game (see Fx046Work.hold0) */
    SVECTOR pos4; /* target position */
} Fx01bWork;

extern Unk800F78A4 *fx_01b_env;

#endif
