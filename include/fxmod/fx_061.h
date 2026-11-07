#ifndef FX_061_H
#define FX_061_H

/* Effect module fx_061 (dir 13 file 0x61: Confuse, from runtime traces of btlfx_loadFile). */
#include "btlfx_mod.h"

/* fx_061 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_CONFUSE_SWIRLS       0x1 /* emitter: teal and pink spiral swirl particles */
#define FX_CONFUSE_STAFF_SWIRL  0x2 /* emitter: 3D and screen trails of treble / bass staves with notes, smoke swirl and haze band, orbiting the target; runtime (docs/runtime-findings.md effect-data pass) */
#define FX_CONFUSE_SWIRLS_2     0x3 /* emitter: teal and pink spiral swirl particles, rising in a cylinder */
#define FX_CONFUSE_STAFF_WAVES  0x7 /* emitter: flat trails of the music-staff strips (the page's staves are the real texture) and a smoke wisp; runtime (docs/runtime-findings.md effect-data pass) */

/* fx_061 choreography (docs/fxdata.md, "Choreography literals"): op80's length, height and trail buffers. */
#define FX_CONFUSE_FRAMES       0x31  /* op80's length (the op ends) */
#define FX_CONFUSE_HEIGHT       0x180 /* the emitters start this far above the targets' centre */
#define FX_CONFUSE_STAFF_WAVES_TRAIL_SLOTS  0x13 /* FX_CONFUSE_STAFF_WAVES' trail buffer fx_061_trails: slots of */
#define FX_CONFUSE_STAFF_WAVES_TRAIL_STRIDE 0x6C /* this many bytes */
#define FX_CONFUSE_STAFF_SWIRL_TRAIL_SLOTS  0xC  /* FX_CONFUSE_STAFF_SWIRL's trail buffer fx_061_trails2: slots of */
#define FX_CONFUSE_STAFF_SWIRL_TRAIL_STRIDE 0x7C /* this many bytes */

typedef struct {
    s32 hold0; /* nonzero: the op idles and never ends by itself (runtime); always 0 in the game (see Fx046Work.hold0) */
    SVECTOR pos4;
} Fx061Work;

extern Unk800F78A4 *fx_061_env;
extern s32 fx_061_trails2[];
extern s32 fx_061_trails[];

#endif
