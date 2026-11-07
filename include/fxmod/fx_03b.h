#ifndef FX_03B_H
#define FX_03B_H

/* Effect module fx_03b (dir 13 file 0x3B). */
#include "btlfx_mod.h"

/* fx_03b effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_TROUBLE_JUICE_SPLASH         0x1 /* emitter: orange burst of juice blobs */
#define FX_TROUBLE_JUICE_SCRIBBLE_RINGS 0x2 /* emitter: ring with an orange scribble texture + cream gouraud rings */
#define FX_TROUBLE_JUICE_SCREEN_TRAIL   0x3 /* emitter: screen trail, still renders all black; runtime (docs/runtime-findings.md effect-data pass 2): ablating it changes < 190 px (look pending) */
#define FX_TROUBLE_JUICE_SCREEN_TRAIL_2 0xA /* emitter: screen trail, still renders all black; runtime (docs/runtime-findings.md effect-data pass 2): started, not ablated (look pending) */

/* fx_03b choreography (docs/fxdata.md, "Choreography literals"): op80's keyframes and length. */
#define FX_TROUBLE_JUICE_TRAIL_2_FRAME 5    /* op80: FX_TROUBLE_JUICE_SCREEN_TRAIL_2 starts at the target body */
#define FX_TROUBLE_JUICE_RINGS_FRAME   8    /* op80: FX_TROUBLE_JUICE_SCRIBBLE_RINGS and SCREEN_TRAIL start */
#define FX_TROUBLE_JUICE_FRAMES        0x28 /* op80: the task ends at this frame */
#define FX_TROUBLE_JUICE_TRAIL_HEIGHT  0xB4 /* FX_TROUBLE_JUICE_SCREEN_TRAIL's position (pos18) above the target */
#define FX_TROUBLE_JUICE_SCREEN_TRAIL_2_TRAIL_SLOTS  0x28 /* FX_TROUBLE_JUICE_SCREEN_TRAIL_2's trail buffer fx_03b_trails: slots of */
#define FX_TROUBLE_JUICE_SCREEN_TRAIL_2_TRAIL_STRIDE 0x18 /* this many bytes (Pool6CEntry trailSlots / trailStride) */
#define FX_TROUBLE_JUICE_SCREEN_TRAIL_TRAIL_SLOTS  0x28 /* FX_TROUBLE_JUICE_SCREEN_TRAIL's trail buffer fx_03b_trails2: slots of */
#define FX_TROUBLE_JUICE_SCREEN_TRAIL_TRAIL_STRIDE 0x20 /* this many bytes (Pool6CEntry trailSlots / trailStride) */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    SVECTOR pos10; /* target position */
    SVECTOR pos18; /* above the target */
    SVECTOR pos20; /* target body position */
    SVECTOR pos28;
} Fx03bWork;

extern Unk800F78A4 *fx_03b_env;
extern s32 fx_03b_trails2[];
extern s32 fx_03b_trails[];

#endif
