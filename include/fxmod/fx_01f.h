#ifndef FX_01F_H
#define FX_01F_H

/* Effect module fx_01f (dir 13 file 0x1F). */
#include "btlfx_mod.h"

/* fx_01f effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_01F_PINK_VORTEX 0x1 /* emitter: white-violet-maroon gouraud rings + pink swirling orb */
#define FX_01F_WHITE_RINGS 0x3 /* emitter: textured rings (texture not in the file) + white gouraud rings (look pending) */
#define FX_01F_RED_GLOW    0x4 /* emitter: dark red glow with a white spot */
#define FX_01F_PINK_RINGS  0xB /* emitter: white-lilac-maroon and white-cream-pink gouraud rings */
#define FX_01F_RED_GLOW_2  0xC /* emitter: dark red glow with a white spot (orbiting) */
#define FX_01F_BACKDROP    0xD /* backdrop: btlfx_drawBackdrop layer table (screen tint/backdrop; not drawn by the tool) (look pending) */

typedef struct {
    s32 hold0; /* nonzero: the op idles and never ends by itself (runtime); always 0 in the game (see Fx046Work.hold0) */
    SVECTOR pos4;
} Fx01fWork;

extern Unk800F78A4 *fx_01f_env;

#endif
