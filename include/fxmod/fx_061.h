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

typedef struct {
    s32 hold0; /* nonzero: the op idles and never ends by itself (runtime); always 0 in the game (see Fx046Work.hold0) */
    SVECTOR pos4;
} Fx061Work;

extern Unk800F78A4 *fx_061_env;
extern s32 fx_061_trails2[];
extern s32 fx_061_trails[];

#endif
