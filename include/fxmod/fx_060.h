#ifndef FX_060_H
#define FX_060_H

/* Effect module fx_060 (dir 13 file 0x60). */
#include "btlfx_mod.h"

/* fx_060 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_CONFUSE_STAFF_WAVES  0x1 /* emitter: flat trails of the music-staff strips (the page's staves are the real texture) and a smoke wisp; runtime (docs/runtime-findings.md effect-data pass) */
#define FX_CONFUSE_SWIRLS       0x2 /* emitter: teal and pink spiral swirl particles */
#define FX_CONFUSE_STAFF_SWIRL  0x9 /* emitter: 3D and screen trails of treble / bass staves with notes, smoke swirl and haze band, orbiting the target; runtime (docs/runtime-findings.md effect-data pass) */

/* fx_060 choreography (docs/fxdata.md, "Choreography literals"): op80's keyframes and trail buffers. */
#define FX_CONFUSE_SWIRLS_FRAME 0x10 /* FX_CONFUSE_SWIRLS starts */
#define FX_CONFUSE_FRAMES       0x31 /* op80's length (the op ends) */
#define FX_CONFUSE_STAFF_WAVES_TRAIL_SLOTS  0x13 /* FX_CONFUSE_STAFF_WAVES' trail buffer fx_060_trails: slots of */
#define FX_CONFUSE_STAFF_WAVES_TRAIL_STRIDE 0x6C /* this many bytes */
#define FX_CONFUSE_STAFF_SWIRL_TRAIL_SLOTS  0xC  /* FX_CONFUSE_STAFF_SWIRL's trail buffer fx_060_trails2: slots of */
#define FX_CONFUSE_STAFF_SWIRL_TRAIL_STRIDE 0x7C /* this many bytes */

typedef struct {
    s32 hold0; /* nonzero: the op idles and never ends by itself (runtime); always 0 in the game (see Fx046Work.hold0) */
    SVECTOR pos4; /* target body position */
} Fx060Work;

extern Unk800F78A4 *fx_060_env;
extern s32 fx_060_trails2[];
extern s32 fx_060_trails[];

#endif
