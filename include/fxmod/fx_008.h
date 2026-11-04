#ifndef FX_008_H
#define FX_008_H

/* Effect module fx_008 (dir 13 file 0x08). */
#include "btlfx_mod.h"

/* fx_008 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_CURE_STAR_FLARES   0x3 /* sprite line: blue star flares along a path */
#define FX_CURE_SPARKLE_SWIRL 0x4 /* emitter: star-sparkle trails orbiting the target, sparkles and bubbles */

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4; /* target position */
    SVECTOR posC; /* target body position */
    u16 frame14;
    u16 alpha16;
} Fx008Work;

extern Unk800F78A4 *fx_008_env;
extern s32 fx_008_trails[];

#endif
