#ifndef FX_04C_H
#define FX_04C_H

/* Effect module fx_04c (dir 13 file 0x4C). */
#include "btlfx_mod.h"

/* fx_04c effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_FLOAT_GLOW_2     0x1 /* emitter: one particle (magenta: texture still outside the file and the common pages) (look pending) */
#define FX_FLOAT_GLOW       0x2 /* emitter: one particle (magenta: texture still outside the file and the common pages) (look pending) */
#define FX_FLOAT_STAR_FLASH 0x3 /* emitter: speckled ring, blue star flashes, two fans (per target) */
#define FX_FLOAT_SPECK_RING 0x4 /* emitter: ring textured with brown specks (per target) */
#define FX_FLOAT_BACKDROP   0x5 /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture; not drawn by the tool) (look pending) */

typedef struct {
    s32 hold0; /* nonzero: the op idles and never ends by itself (runtime); always 0 in the game (see Fx046Work.hold0) */
    SVECTOR pos4;
} Fx04cWork80;

typedef struct {
    s32 state0; /* state */
    SVECTOR pos4[4]; /* target positions */
    Pool6CEntry *emit24[4];
    s32 count34; /* target count */
    s32 target38; /* current target */
} Fx04cWork81; /* 0x3C */

extern Unk800F78A4 *fx_04c_env;

#endif
