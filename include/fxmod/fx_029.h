#ifndef FX_029_H
#define FX_029_H

/* Effect module fx_029 (dir 13 file 0x29). */
#include "btlfx_mod.h"

/* fx_029 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_WHITE_WIND_GLOW 0x7 /* emitter: blue specks + blue-white star glow */

/* fx_029 choreography (docs/fxdata.md, "Choreography literals"): op80's length and height. */
#define FX_WHITE_WIND_FRAMES 0x3C /* op80: the task ends at this frame */
#define FX_WHITE_WIND_HEIGHT 0xFA /* FX_WHITE_WIND_GLOW's position (pos8) above the targets' centre */

typedef struct {
    s32 hold0; /* nonzero: the op idles and never ends by itself (runtime); always 0 in the game (see Fx046Work.hold0) */
    Pool6CEntry *emit4;
    SVECTOR pos8;
} Fx029Work;

extern Unk800F78A4 *fx_029_env;

#endif
