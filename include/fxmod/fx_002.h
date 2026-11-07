#ifndef FX_002_H
#define FX_002_H

/* Effect module fx_002 (dir 13 file 0x02). */
#include "btlfx_mod.h"

/* fx_002 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_FIRA_FLAME_BURST   0x4 /* emitter: flame explosion with ember sparks, at the target body */
#define FX_FIRA_SMOKE         0x5 /* emitter: grey smoke puffs */
#define FX_FIRA_FLAME_BURST_2 0x7 /* emitter: second flame explosion (embers, red-tinted burst) */
#define FX_FIRA_SMOKE_2       0x8 /* emitter: grey smoke puffs after the second burst */
#define FX_FIRA_FLAME_SWIRL   0x9 /* emitter: flame-strip trails orbiting the target */
#define FX_FIRA_GROUND_RING   0xD /* rings: red gouraud ring drawn flat on the ground */
#define FX_FIRA_HEAT_RING     0xE /* emitter: white-red gouraud rings at the target's feet */

/* fx_002 choreography (docs/fxdata.md, "Choreography literals"): op80's keyframes, phases and length. */
#define FX_FIRA_SMOKE_FRAME           5    /* op80: FX_FIRA_SMOKE starts (FX_FIRA_FLAME_BURST started in mode 1) */
#define FX_FIRA_FLAME_SWIRL_FRAME     0x12 /* op80: FX_FIRA_FLAME_SWIRL, FX_FIRA_FLAME_BURST_2 and FX_FIRA_HEAT_RING start */
#define FX_FIRA_SMOKE_2_FRAME         0x17 /* op80: FX_FIRA_SMOKE_2 starts */
#define FX_FIRA_RING_PULSE_FRAME      0x17 /* op80: FX_FIRA_GROUND_RING stops growing and swells a second time */
#define FX_FIRA_GROUND_RING_FRAMES    0x2C /* op80: FX_FIRA_GROUND_RING is drawn for this many frames */
#define FX_FIRA_FRAMES                0x32 /* op80: the task ends at this frame */
#define FX_FIRA_FLAME_SWIRL_TRAIL_SLOTS  0x15 /* FX_FIRA_FLAME_SWIRL's trail buffer fx_002_trails: slots of */
#define FX_FIRA_FLAME_SWIRL_TRAIL_STRIDE 0xAC /* this many bytes (Pool6CEntry trailSlots / trailStride) */

typedef struct {
    s32 hold0; /* nonzero: the op idles and never ends by itself (runtime); always 0 in the game (see Fx046Work.hold0) */
    SVECTOR pos4; /* target position */
    SVECTOR posC; /* target body position */
} Fx002Work;

extern Unk800F78A4 *fx_002_env;
extern s32 fx_002_trails[];

#endif
