#ifndef FX_061_H
#define FX_061_H

/* Effect module fx_061 (dir 13 file 0x61: Confuse, from runtime traces of btlfx_loadFile). */
#include "btlfx_mod.h"

/* fx_061 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_CONFUSE_SWIRLS       0x1 /* emitter: teal and pink spiral swirl particles */
#define FX_CONFUSE_ORBIT_TRAILS 0x2 /* emitter: orbiting 3D + screen trails, scribble strip texture (look pending) */
#define FX_CONFUSE_SWIRLS_2     0x3 /* emitter: teal and pink spiral swirl particles, rising in a cylinder */
#define FX_CONFUSE_WAVE_TRAILS  0x7 /* emitter: two wobbling flat trails, strip texture with wave / scribble marks (look pending) */

typedef struct {
    s32 hold0; /* nonzero: the op idles and never ends by itself (runtime); always 0 in the game (see Fx046Work.hold0) */
    SVECTOR pos4;
} Fx061Work;

extern Unk800F78A4 *fx_061_env;
extern s32 fx_061_trails2[];
extern s32 fx_061_trails[];

#endif
