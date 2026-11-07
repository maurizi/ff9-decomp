#ifndef FX_017_H
#define FX_017_H

/* Effect module fx_017 (dir 13 file 0x17). */
#include "btlfx_mod.h"

/* fx_017 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_DEVILS_CANDLE_BLUE_STARBURST 0x1 /* emitter: grey flower flash, blue starburst, blue-violet streak sprites */
#define FX_DEVILS_CANDLE_FLAME          0x2 /* emitter: smoky yellow-grey flame quads */
#define FX_DEVILS_CANDLE_FLAME_2        0x7 /* emitter: smoky yellow-grey flame quads */

/* fx_017 choreography (docs/fxdata.md, "Choreography literals"): op80's keyframe and length. */
#define FX_DEVILS_CANDLE_FLAME_FRAME 0x14 /* op80: FX_DEVILS_CANDLE_FLAME (on the ground) and the blue starburst start */
#define FX_DEVILS_CANDLE_FRAMES      0x28 /* op80: the task ends at this frame */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    SVECTOR posC; /* target body position */
    SVECTOR pos14;
    SVECTOR pos1C; /* posC on the ground */
} Fx017Work;

extern Unk800F78A4 *fx_017_env;

#endif
