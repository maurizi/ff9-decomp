#ifndef FX_015_H
#define FX_015_H

/* Effect module fx_015 (dir 13 file 0x15). */
#include "btlfx_mod.h"

/* fx_015 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_POISON_PURPLE_SMOKE 0x1 /* emitter: orbiting trail with purple smoke clouds */
#define FX_POISON_TOXIC_BURST  0x8 /* emitter: purple clouds + purple droplet sprites */

/* fx_015 choreography (docs/fxdata.md, "Choreography literals"): op80's keyframe and length. */
#define FX_POISON_TOXIC_BURST_FRAME 0x19 /* op80: FX_POISON_TOXIC_BURST starts (and the smoke is drawn further back) */
#define FX_POISON_FRAMES            0x46 /* op80: the task ends at this frame */
#define FX_POISON_SMOKE_HEIGHT      0x190 /* FX_POISON_PURPLE_SMOKE's position (pos8) above the target */
#define FX_POISON_PURPLE_SMOKE_TRAIL_SLOTS  0x1E /* FX_POISON_PURPLE_SMOKE's trail buffer fx_015_trails: slots of */
#define FX_POISON_PURPLE_SMOKE_TRAIL_STRIDE 0x30 /* this many bytes (Pool6CEntry trailSlots / trailStride) */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* above the target */
    SVECTOR pos10; /* target position */
} Fx015Work;

extern Unk800F78A4 *fx_015_env;
extern s32 fx_015_trails[];

#endif
