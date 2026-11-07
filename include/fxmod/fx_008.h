#ifndef FX_008_H
#define FX_008_H

/* Effect module fx_008 (dir 13 file 0x08). */
#include "btlfx_mod.h"

/* fx_008 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_CURE_STAR_FLARES   0x3 /* sprite line: blue star flares along a path */
#define FX_CURE_SPARKLE_SWIRL 0x4 /* emitter: star-sparkle trails orbiting the target, sparkles and bubbles */

/* fx_008 choreography (docs/fxdata.md, "Choreography literals"): op80's keyframes and length. */
#define FX_CURE_STAR_FLARES_FRAME      0x18 /* op80: FX_CURE_STAR_FLARES shows from this frame */
#define FX_CURE_STAR_FLARES_FRAMES     0x10 /* for this many frames */
#define FX_CURE_STAR_FLARES_FADE_FRAME 4    /* and fades out from this one of them on */
#define FX_CURE_STAR_FLARES_ANIM_FRAMES 6   /* sprite-line frames (frame14 stops on the last) */
#define FX_CURE_FRAMES                 0x3C /* op80: the task ends at this frame */
#define FX_CURE_SPARKLE_SWIRL_TRAIL_SLOTS  0xB  /* FX_CURE_SPARKLE_SWIRL's trail buffer fx_008_trails: slots of */
#define FX_CURE_SPARKLE_SWIRL_TRAIL_STRIDE 0x8C /* this many bytes (Pool6CEntry trailSlots / trailStride) */

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
