#ifndef FX_033_H
#define FX_033_H

/* Effect module fx_033 (dir 13 file 0x33). */
#include "btlfx_mod.h"

/* fx_033 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_SNORT_SMOKE_STREAKS 0x5 /* emitter: two spiralling screen trails, the grey smoke streaks blowing past the target; runtime (docs/runtime-findings.md effect-data pass 2, medium) */

/* fx_033 choreography (docs/fxdata.md, "Choreography literals"): op80's knockback and length; fxm_easeLerp's curves. */
#define FX_SNORT_KNOCKBACK_FRAME  0x18 /* op80: the target is pushed back from this frame (timer0 = frame), */
#define FX_SNORT_KNOCKBACK_FRAMES 0x18 /* over this many (fxm_easeLerp n) */
#define FX_SNORT_PUSH_DIST        0x2000 /* how far back (and up, / 64) the target is pushed at most */
#define FX_SNORT_FRAMES           0x32 /* op80: the task ends at this frame */
#define FX_SNORT_EASE_LINEAR      0    /* fxm_easeLerp ratio 1 - t / n */
#define FX_SNORT_EASE_SQUARED     1    /* (1 - t / n)^2 */
#define FX_SNORT_EASE_INV_SQUARED 2    /* 1 - (t / n)^2 */
#define FX_SNORT_SMOKE_STREAKS_TRAIL_SLOTS  0x2E /* FX_SNORT_SMOKE_STREAKS's trail buffer fx_033_trails: slots of */
#define FX_SNORT_SMOKE_STREAKS_TRAIL_STRIDE 0x4C /* this many bytes (Pool6CEntry trailSlots / trailStride) */

typedef struct {
    s32 timer0; /* knockback countdown */
    SVECTOR pos4;
    SVECTOR posC; /* target body position */
    SVECTOR pos14; /* target home position */
    SVECTOR pos1C; /* target position while pushed back */
    SVECTOR aim24; /* angles from pos4 to posC */
    Pool6CEntry *emit2C;
} Fx033Work; /* 0x30 */

extern Unk800F78A4 *fx_033_env;
extern s32 fx_033_trails[];

static s32 fxm_easeLerp(s32 curve, s32 n, s32 t, s32 from, s32 to);

#endif
