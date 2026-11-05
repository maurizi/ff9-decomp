#ifndef FX_01F_H
#define FX_01F_H

/* Effect module fx_01f (dir 13 file 0x1F). */
#include "btlfx_mod.h"

/* fx_01f effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_01F_PINK_VORTEX 0x1 /* emitter: white-violet-maroon gouraud rings + pink swirling orb */
#define FX_01F_WHITE_FLASH 0x3 /* emitter: textured rings + white gouraud rings, the huge white flash that swallows the target; runtime (docs/runtime-findings.md effect-data pass 2) */
#define FX_01F_RED_GLOW    0x4 /* emitter: dark red glow with a white spot */
#define FX_01F_PINK_RINGS  0xB /* emitter: white-lilac-maroon and white-cream-pink gouraud rings */
#define FX_01F_RED_GLOW_2  0xC /* emitter: dark red glow with a white spot (orbiting) */
#define FX_01F_BURST_TEXTURE 0xD /* layer table: render-to-texture (btlfx_drawLayerTexture), drawn from the start; skipping it changes the red / white burst rays round the target; runtime (docs/runtime-findings.md effect-data pass 2, medium) */

typedef struct {
    s32 hold0; /* nonzero: the op idles and never ends by itself (runtime); always 0 in the game (see Fx046Work.hold0) */
    SVECTOR pos4;
} Fx01fWork;

extern Unk800F78A4 *fx_01f_env;

#endif
