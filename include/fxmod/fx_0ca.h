#ifndef FX_0CA_H
#define FX_0CA_H

/* Effect module fx_0ca (dir 13 file 0xCA): Poison. Evidence: the file btlfx_loadFile loads for the spell (docs/runtime-findings.md, btlfx_loadFile). */
#include "btlfx_mod.h"

/* fx_0ca effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_POISON_BUBBLES 0x1 /* emitter: purple poison blobs and bubbles */
#define FX_POISON_SWIRL   0x2 /* emitter: orbiting purple poison trail */

/* fx_0ca choreography (docs/fxdata.md, "Choreography literals"): op80's keyframe, length and trail buffer. */
#define FX_POISON_SWIRL_HEIGHT  0x190 /* op80: FX_POISON_SWIRL circles this far above the targets' centre */
#define FX_POISON_BUBBLES_FRAME 0x19  /* op80: FX_POISON_BUBBLES start at the targets' centre */
#define FX_POISON_FRAMES        0x46  /* op80: the task ends at this frame */
#define FX_POISON_SWIRL_TRAIL_SLOTS  0x1E /* FX_POISON_SWIRL's trail buffer fx_0ca_trails: slots of */
#define FX_POISON_SWIRL_TRAIL_STRIDE 0x30 /* this many bytes (Pool6CEntry trailSlots / trailStride) */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* pos10 raised by 0x190 */
    SVECTOR pos10; /* position: env->targets->centerX..Z */
} Fx0caWork;

extern Unk800F78A4 *fx_0ca_env;
extern s32 fx_0ca_trails[];

#endif
