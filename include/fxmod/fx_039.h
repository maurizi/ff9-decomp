#ifndef FX_039_H
#define FX_039_H

/* Effect module fx_039 (dir 13 file 0x39). */
#include "btlfx_mod.h"

/* fx_039 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_OSMOSE_SPARKLES     0x7 /* emitter: blue glint + small blue / white sparkle dots */
#define FX_OSMOSE_DRAIN_STREAKS 0x8 /* emitter: two fans drawn as white-violet comet streaks */
#define FX_OSMOSE_DRAIN_ORB    0x9 /* emitter: white-violet-cyan gouraud rings + screen trail, flies between caster and target */
#define FX_OSMOSE_ABSORB_FLASH 0xA /* emitter: white-lilac / white-violet-cyan rings + purple light streaks */

/* fx_039 choreography (docs/fxdata.md, "Choreography literals"): op80's length; op81's projectiles, their flight and length. */
#define FX_OSMOSE_FRAMES            0x28 /* op80: the task ends at this frame */
#define FX_OSMOSE_PROJECTILES       3    /* op81 tasks (proj20 0..2); a higher index ends at once */
#define FX_OSMOSE_ORB_FRAME         0xA  /* op81: FX_OSMOSE_DRAIN_ORB starts and flies from the target to the caster, */
#define FX_OSMOSE_ORB_FLIGHT_FRAMES 0x10 /* for this many frames (t reaches 1 on the last) */
#define FX_OSMOSE_ARC_WIDTH         0x4B0 /* the sideways arc of projectiles 1 and 2 (sin) */
#define FX_OSMOSE_ARC_HEIGHT        0x258 /* the upward arc of every projectile */
#define FX_OSMOSE_ABSORB_FRAMES     0x28 /* op81: the task ends at this frame */
#define FX_OSMOSE_DRAIN_ORB_TRAIL_SLOTS  0x12 /* FX_OSMOSE_DRAIN_ORB's trail buffer fx_039_trails[proj20]: slots of */
#define FX_OSMOSE_DRAIN_ORB_TRAIL_STRIDE 0xB0 /* this many bytes (Pool6CEntry trailSlots / trailStride) */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* target position */
    SVECTOR pos10; /* target body position */
} Fx039Work80;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* caster body position */
    SVECTOR pos10; /* target position */
    SVECTOR pos18; /* projectile position */
    s32 proj20; /* projectile index (0..2) */
} Fx039Work81; /* 0x24 */
#define FX_039_PROJ_ARC_NEG_X 1 /* proj20: the shot arcs toward -x */
#define FX_039_PROJ_ARC_POS_X 2 /* proj20: the shot arcs toward +x (0 flies straight) */

extern Unk800F78A4 *fx_039_env;
extern s32 fx_039_trails[][FX_OSMOSE_DRAIN_ORB_TRAIL_SLOTS * FX_OSMOSE_DRAIN_ORB_TRAIL_STRIDE / 4];
extern SVECTOR fx_039_emitPos[];

#endif
