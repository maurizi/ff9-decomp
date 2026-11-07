#ifndef FX_01E_H
#define FX_01E_H

/* Effect module fx_01e (dir 13 file 0x1E). */
#include "btlfx_mod.h"

/* fx_01e effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_1000_NEEDLES_NEEDLE_BURST  0x4 /* emitter: orange specks, pink starbursts, white shard sprites, sparks (texture not in the file) */
#define FX_1000_NEEDLES_NEEDLE_TRAILS 0x8 /* emitter: flat trails with a white needle-streak strip */

/* fx_01e choreography (docs/fxdata.md, "Choreography literals"): op80's keyframe and length. */
#define FX_1000_NEEDLES_BURST_FRAME 8    /* op80: FX_1000_NEEDLES_NEEDLE_BURST starts at the target */
#define FX_1000_NEEDLES_FRAMES      0x3C /* op80: the task ends at this frame */
#define FX_1000_NEEDLES_NEEDLE_TRAILS_TRAIL_SLOTS  0x64 /* FX_1000_NEEDLES_NEEDLE_TRAILS's trail buffer fx_01e_trails: slots of */
#define FX_1000_NEEDLES_NEEDLE_TRAILS_TRAIL_STRIDE 0x1C /* this many bytes (Pool6CEntry trailSlots / trailStride) */

typedef struct {
    s32 hold0; /* nonzero: the op idles and never ends by itself (runtime); always 0 in the game (see Fx046Work.hold0) */
    Pool6CEntry *emit4;
    SVECTOR pos8; /* caster body position */
    SVECTOR pos10; /* target body position */
    SVECTOR aim18; /* angles from caster to target */
} Fx01eWork;

extern Unk800F78A4 *fx_01e_env;
extern s32 fx_01e_trails[];

#endif
