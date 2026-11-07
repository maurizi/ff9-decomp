#ifndef FX_020_H
#define FX_020_H

/* Effect module fx_020 (dir 13 file 0x20). */
#include "btlfx_mod.h"

/* fx_020 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_REGEN_GLINTS      0x2 /* emitter: fan + blue glint, teal-white ring, blue / green star glints */
#define FX_REGEN_LIGHT_TRAIL 0x8 /* emitter: white-blue light trail + faint sparkle sprites */

/* fx_020 choreography (docs/fxdata.md, "Choreography literals"): op80's keyframe and length. */
#define FX_REGEN_GLINTS_FRAME 0x14 /* op80: FX_REGEN_GLINTS start (turned to fx_020_emitRot from then on) */
#define FX_REGEN_FRAMES       0x32 /* op80: the task ends at this frame */
#define FX_REGEN_LIGHT_TRAIL_TRAIL_SLOTS  0x14 /* FX_REGEN_LIGHT_TRAIL's trail buffer fx_020_trails: slots of */
#define FX_REGEN_LIGHT_TRAIL_TRAIL_STRIDE 0x90 /* this many bytes (Pool6CEntry trailSlots / trailStride) */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* target position */
} Fx020Work;

extern SVECTOR fx_020_emitRot;
extern Unk800F78A4 *fx_020_env;
extern s32 fx_020_trails[];

#endif
