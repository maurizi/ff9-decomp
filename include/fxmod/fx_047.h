#ifndef FX_047_H
#define FX_047_H

/* Effect module fx_047 (dir 13 file 0x47). */
#include "btlfx_mod.h"

/* fx_047 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_FIRA_FLAME_SWIRL   0x1 /* emitter: flame-strip trails orbiting the targets */
#define FX_FIRA_HEAT_RING     0x2 /* emitter: white-red gouraud rings on the ground */
#define FX_FIRA_FLAME_BURST   0x3 /* emitter: flame explosion with ember sparks, above the targets */
#define FX_FIRA_SMOKE         0x4 /* emitter: grey smoke puffs */
#define FX_FIRA_FLAME_BURST_2 0x5 /* emitter: second flame explosion */

/* fx_047 choreography (docs/fxdata.md, "Choreography literals"): op80's keyframes and length. */
#define FX_FIRA_BURST_HEIGHT          0x12C /* FX_FIRA_FLAME_BURST plays this far above the targets' centre (pos4) */
#define FX_FIRA_SMOKE_FRAME           5     /* op80: FX_FIRA_SMOKE starts */
#define FX_FIRA_FLAME_SWIRL_FRAME     0x11  /* op80: FX_FIRA_FLAME_SWIRL and FX_FIRA_FLAME_BURST_2 start */
#define FX_FIRA_FRAMES                0x32  /* op80: the task ends at this frame */
#define FX_FIRA_FLAME_SWIRL_TRAIL_SLOTS  0x1D /* FX_FIRA_FLAME_SWIRL's trail buffer fx_047_trails: slots of */
#define FX_FIRA_FLAME_SWIRL_TRAIL_STRIDE 0xAC /* this many bytes (Pool6CEntry trailSlots / trailStride) */

typedef struct {
    s32 hold0; /* nonzero: the op idles and never ends by itself (runtime); always 0 in the game (see Fx046Work.hold0) */
    SVECTOR pos4; /* above posC */
    SVECTOR posC; /* target on the ground */
    Pool6CEntry *emit14;
} Fx047Work;

extern Unk800F78A4 *fx_047_env;
extern s32 fx_047_trails[];

#endif
