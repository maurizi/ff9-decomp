#ifndef FX_055_H
#define FX_055_H

/* Effect module fx_055 (dir 13 file 0x55). */
#include "btlfx_mod.h"

/* fx_055 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_CHAKRA_DOT_RINGS   0x6 /* emitter: fans, rings of violet / yellow dots, teal-white rings */
#define FX_CHAKRA_STAR_GLINTS 0x7 /* emitter: blue star glints + light streak, small glints */

/* fx_055 choreography (docs/fxdata.md, "Choreography literals"): op80's keyframe and length. */
#define FX_CHAKRA_DOT_RINGS_FRAME 0xF  /* op80: FX_CHAKRA_DOT_RINGS start */
#define FX_CHAKRA_FRAMES          0x32 /* op80: the task ends at this frame */
#define FX_CHAKRA_STAR_GLINTS_TRAIL_SLOTS  0xA /* FX_CHAKRA_STAR_GLINTS's trail buffer fx_055_trails: slots of */
#define FX_CHAKRA_STAR_GLINTS_TRAIL_STRIDE 0x90 /* this many bytes (Pool6CEntry trailSlots / trailStride) */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* target position */
} Fx055Work;

extern Unk800F78A4 *fx_055_env;
extern s32 fx_055_trails[];

#endif
