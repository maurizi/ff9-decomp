#ifndef FX_00D_H
#define FX_00D_H

/* Effect module fx_00d (dir 13 file 0x0D). */
#include "btlfx_mod.h"

/* fx_00d effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_STONA_LIGHT_TRAIL 0x2 /* emitter: flat orbiting trail, yellow-red strip texture */
#define FX_STONA_BLUE_ORBS   0x3 /* emitter: blue light orbs dissolving into golden specks */
#define FX_STONA_GOLD_RINGS  0x9 /* emitter: gold-brown gouraud rings + yellow light streak */

/* fx_00d choreography (docs/fxdata.md, "Choreography literals"): op80's keyframe and length. */
#define FX_STONA_BLUE_ORBS_FRAME 0xF  /* op80: FX_STONA_BLUE_ORBS and FX_STONA_GOLD_RINGS start */
#define FX_STONA_FRAMES          0x32 /* op80: the task ends at this frame */
#define FX_STONA_LIGHT_TRAIL_TRAIL_SLOTS  0x13 /* FX_STONA_LIGHT_TRAIL's trail buffer fx_00d_trails: slots of */
#define FX_STONA_LIGHT_TRAIL_TRAIL_STRIDE 0x34 /* this many bytes (Pool6CEntry trailSlots / trailStride) */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    SVECTOR posC; /* target body position */
} Fx00dWork;

extern Unk800F78A4 *fx_00d_env;
extern s32 fx_00d_trails[];

#endif
