#ifndef FX_035_H
#define FX_035_H

/* Effect module fx_035 (dir 13 file 0x35). */
#include "btlfx_mod.h"

/* fx_035 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_DEVILS_CANDLE_FLAME   0x3 /* emitter: teal / tan flame quads */
#define FX_DEVILS_CANDLE_FLAME_2 0x8 /* emitter: teal / tan flame quads */
#define FX_DEVILS_CANDLE_FIRE    0x9 /* emitter: red-orange-yellow flame tongues + red glow */

/* fx_035 choreography (docs/fxdata.md, "Choreography literals"): op80's keyframe and length (the names of fx_017, the other
 * Devil's Candle module). */
#define FX_DEVILS_CANDLE_FLAME_FRAME 0x14 /* op80: FX_DEVILS_CANDLE_FLAME and FX_DEVILS_CANDLE_FIRE start on the ground */
#define FX_DEVILS_CANDLE_FRAMES      0x28 /* op80: the task ends at this frame */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    SVECTOR posC; /* target body position */
    SVECTOR pos14;
    SVECTOR pos1C; /* posC on the ground */
} Fx035Work;

extern Unk800F78A4 *fx_035_env;

#endif
