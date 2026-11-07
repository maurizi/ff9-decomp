#ifndef FX_056_H
#define FX_056_H

/* Effect module fx_056 (dir 13 file 0x56). */
#include "btlfx_mod.h"

/* fx_056 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_CHAKRA_DOT_RINGS   0x6 /* emitter: fans, rings of violet / yellow dots, teal-white rings (byte-identical to fx_055 entry 0x6) */
#define FX_CHAKRA_STAR_GLINTS 0x8 /* emitter: blue star glints + light streak, small glints (byte-identical to fx_055 entry 0x7) */

/* fx_056 choreography (docs/fxdata.md, "Choreography literals"): op80's keyframes and length (the names of fx_055, the one-target Chakra). */
#define FX_CHAKRA_RELEASE_FRAME   0xA  /* op80: sets BTLFX_SIG_WAIT, releasing the script's wait (the next target's task starts) */
#define FX_CHAKRA_DOT_RINGS_FRAME 0xF  /* op80: FX_CHAKRA_DOT_RINGS start */
#define FX_CHAKRA_FRAMES          0x32 /* op80: the task ends at this frame */
#define FX_CHAKRA_STAR_GLINTS_TRAIL_SLOTS  0xA  /* FX_CHAKRA_STAR_GLINTS' trail buffer fx_056_trails[target]: slots of */
#define FX_CHAKRA_STAR_GLINTS_TRAIL_STRIDE 0x90 /* this many bytes (Pool6CEntry trailSlots / trailStride) */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* target position */
    s32 target10; /* target index */
} Fx056Work;

extern Unk800F78A4 *fx_056_env;
extern s32 fx_056_trails[][FX_CHAKRA_STAR_GLINTS_TRAIL_SLOTS * FX_CHAKRA_STAR_GLINTS_TRAIL_STRIDE / 4]; /* per target */

static void fxm_setSignal(Unk800F78A4 *env, s32 signal, s32 value);

#endif
