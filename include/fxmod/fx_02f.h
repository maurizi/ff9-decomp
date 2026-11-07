#ifndef FX_02F_H
#define FX_02F_H

/* Effect module fx_02f (dir 13 file 0x2F). */
#include "btlfx_mod.h"

/* fx_02f effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_FLAME_FLAME_BURST 0x5 /* emitter: flame explosion with expanding red-orange fireballs and flame tongues, at each target (byte-identical to fx_137 entry 0x5) */
#define FX_FLAME_FLAME_STREAK 0x6 /* emitter: screen trail with a flame-streak strip */

/* fx_02f choreography (docs/fxdata.md, "Choreography literals"): op80's keyframe and length. */
#define FX_FLAME_BURST_FRAME 0x10 /* op80: FX_FLAME_FLAME_BURST starts at each target */
#define FX_FLAME_FRAMES      0x30 /* op80: the task ends at this frame */
#define FX_FLAME_FLAME_STREAK_TRAIL_SLOTS  0x40 /* FX_FLAME_FLAME_STREAK's trail buffer fx_02f_trails: slots of */
#define FX_FLAME_FLAME_STREAK_TRAIL_STRIDE 0x60 /* this many bytes (Pool6CEntry trailSlots / trailStride) */

typedef struct {
    SVECTOR aim0; /* beam angles */
    SVECTOR pos8;
    SVECTOR pos10;
    SVECTOR pos18; /* target position, on the ground */
    Pool6CEntry *emit20;
} Fx02fWork; /* 0x24 */

extern Unk800F78A4 *fx_02f_env;
extern SVECTOR fx_02f_targetPos[]; /* target body positions */
extern s32 fx_02f_trails[];

#endif
