#ifndef FX_060_H
#define FX_060_H

/* Effect module fx_060 (dir 13 file 0x60). */
#include "btlfx_mod.h"

/* fx_060 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_CONFUSE_WAVE_TRAILS  0x1 /* emitter: two wobbling flat trails, strip texture with wave / scribble marks (look pending) */
#define FX_CONFUSE_SWIRLS       0x2 /* emitter: teal and pink spiral swirl particles */
#define FX_CONFUSE_ORBIT_TRAILS 0x9 /* emitter: orbiting 3D + screen trails, scribble strip texture (look pending) */

typedef struct {
    s32 hold0; /* nonzero: the op idles and never ends by itself (runtime); always 0 in the game (see Fx046Work.hold0) */
    SVECTOR pos4; /* target body position */
} Fx060Work;

extern Unk800F78A4 *fx_060_env;
extern s32 fx_060_trails2[];
extern s32 fx_060_trails[];

#endif
