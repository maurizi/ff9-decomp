#ifndef FX_037_H
#define FX_037_H

/* Effect module fx_037 (dir 13 file 0x37). */
#include "btlfx_mod.h"

/* fx_037 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_DARKNESS_DARK_CRESCENTS 0x1 /* emitter: pale crescent quads tinted violet / green + coloured specks */
#define FX_DARKNESS_GREEN_HAZE     0x2 /* emitter: orbiting screen trail, a strip of green glow dots on long streaks at run time: the dark-green haze at the frame edges; runtime (docs/runtime-findings.md effect-data pass 2, low) (look pending: low) */
#define FX_DARKNESS_WHITE_GLOW     0x3 /* emitter: white radial glow + one particle (texture not in the file) */
#define FX_DARKNESS_DARK_ORBS      0x4 /* emitter: blue orbs turning violet then red */
#define FX_DARKNESS_MAGENTA_STREAK 0x5 /* emitter: fan drawn as a thin magenta horizontal light streak */

/* fx_037 choreography (docs/fxdata.md, "Choreography literals"): op80's keyframes and length. */
#define FX_DARKNESS_STREAK_FRAME 8    /* op80: FX_DARKNESS_MAGENTA_STREAK starts */
#define FX_DARKNESS_ORBS_FRAME   10   /* op80: FX_DARKNESS_DARK_ORBS and DARK_CRESCENTS start */
#define FX_DARKNESS_GLOW_FRAME   20   /* op80: FX_DARKNESS_WHITE_GLOW starts */
#define FX_DARKNESS_FRAMES       0x3C /* op80: the task ends at this frame */
#define FX_DARKNESS_GREEN_HAZE_TRAIL_SLOTS  0x15 /* FX_DARKNESS_GREEN_HAZE's trail buffer fx_037_trails: slots of */
#define FX_DARKNESS_GREEN_HAZE_TRAIL_STRIDE 0x60 /* this many bytes (Pool6CEntry trailSlots / trailStride) */
#define FX_DARKNESS_MAGENTA_STREAK_TRAIL_SLOTS  0x2C /* FX_DARKNESS_MAGENTA_STREAK's trail buffer fx_037_trails2: slots of */
#define FX_DARKNESS_MAGENTA_STREAK_TRAIL_STRIDE 0x8C /* this many bytes (Pool6CEntry trailSlots / trailStride) */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    SVECTOR pos14; /* target body position */
} Fx037Work;

extern Unk800F78A4 *fx_037_env;
extern s32 fx_037_trails[];
extern s32 fx_037_trails2[];

#endif
