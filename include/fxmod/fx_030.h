#ifndef FX_030_H
#define FX_030_H

/* Effect module fx_030 (dir 13 file 0x30). */
#include "btlfx_mod.h"

/* fx_030 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_POLLEN_POLLEN_DOTS 0x1 /* emitter: faint pollen dots (btlfx_emitDots) */
#define FX_POLLEN_DUST_PUFF   0x2 /* emitter: grey dust puff + dim specks */
#define FX_POLLEN_POLLEN_PUFF 0x3 /* emitter: yellow-brown pollen puff + grey puff */
#define FX_POLLEN_POLLEN_DOTS_2 0x8 /* emitter: faint pollen dots (btlfx_emitDots) */

/* fx_030 choreography (docs/fxdata.md, "Choreography literals"): op80's keyframe, length and aim point. */
#define FX_POLLEN_DUST_PUFF_FRAME 6     /* op80: FX_POLLEN_DUST_PUFF starts at each target */
#define FX_POLLEN_FRAMES          0x32  /* op80: the task ends at this frame */
#define FX_POLLEN_AIM_DEPTH       0x1F4 /* pos8, where the pollen is aimed: the targets' centre on the ground, this much further in z */

typedef struct {
    SVECTOR pos0;
    SVECTOR pos8;
    SVECTOR aim10; /* angles from pos0 to pos8 */
    SVECTOR aim18; /* angles from pos8 to pos0 */
    Pool6CEntry *emit20;
    Pool6CEntry *emit24;
    Pool6CEntry *emit28;
    SVECTOR pos2C[8]; /* target body positions */
} Fx030Work; /* 0x6C */

extern Unk800F78A4 *fx_030_env;

#endif
