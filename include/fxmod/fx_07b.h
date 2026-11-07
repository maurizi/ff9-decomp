#ifndef FX_07B_H
#define FX_07B_H

/* Effect module fx_07b (dir 13 file 0x7B: Protect, from runtime traces of btlfx_loadFile). */
#include "btlfx_mod.h"

/* fx_07b effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_PROTECT_BLUE_HELIX 0x1 /* emitter: blue flat trail spiralling up + sprite */
#define FX_PROTECT_GOLD_RING  0x3 /* emitter: golden ring particles with blue glints */

/* fx_07b choreography (docs/fxdata.md, "Choreography literals"): op80's keyframes, the emitter offset and the
 * trail buffer. */
#define FX_PROTECT_GOLD_RING_FRAME 0xE  /* FX_PROTECT_GOLD_RING starts (and then turns with the target) */
#define FX_PROTECT_FRAMES          0x28 /* op80's length (the op ends) */
#define FX_PROTECT_BACK_OFFSET     0x190 /* the emitters stand this far behind the target (along its facing) */
#define FX_PROTECT_BLUE_HELIX_TRAIL_SLOTS  6    /* FX_PROTECT_BLUE_HELIX's trail buffer fx_07b_trails: slots of */
#define FX_PROTECT_BLUE_HELIX_TRAIL_STRIDE 0xA0 /* this many bytes */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* target body position, moved back */
    SVECTOR rot10; /* target rotation */
} Fx07bWork;

extern SVECTOR fx_07b_emitRot;
extern Unk800F78A4 *fx_07b_env;
extern s32 fx_07b_trails[];

#endif
