#ifndef FX_04B_H
#define FX_04B_H

/* Effect module fx_04b (dir 13 file 0x4B). */
#include "btlfx_mod.h"

/* fx_04b effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_SANDSTORM_SAND_CLOUD   0x1 /* emitter: beige sand particle quads */
#define FX_SANDSTORM_SAND_TRAIL   0x2 /* emitter: homing trail with a grey streak strip */
#define FX_SANDSTORM_SAND_TRAIL_2 0x3 /* emitter: screen trail with a grey streak strip */

/* fx_04b choreography (docs/fxdata.md, "Choreography literals"): op80's keyframes and length; op81's height and length. */
#define FX_SANDSTORM_TRAIL_FRAME   10  /* op80: FX_SANDSTORM_SAND_TRAIL starts at the targets, homing on the caster from then on */
#define FX_SANDSTORM_TRAIL_2_FRAME 40  /* op80: FX_SANDSTORM_SAND_TRAIL_2 starts at the caster, aimed at the targets */
#define FX_SANDSTORM_FRAMES        100 /* op80: the task ends at this frame */
#define FX_SANDSTORM_CLOUD_HEIGHT  0xC8 /* op81: FX_SANDSTORM_SAND_CLOUD's position (pos4) above the target */
#define FX_SANDSTORM_CLOUD_FRAMES  0x3C /* op81: the task ends at this frame */
#define FX_SANDSTORM_SAND_TRAIL_TRAIL_SLOTS  0x82 /* FX_SANDSTORM_SAND_TRAIL's trail buffer fx_04b_trails: slots of */
#define FX_SANDSTORM_SAND_TRAIL_TRAIL_STRIDE 0x18 /* this many bytes (Pool6CEntry trailSlots / trailStride) */
#define FX_SANDSTORM_SAND_TRAIL_2_TRAIL_SLOTS  0xA0 /* FX_SANDSTORM_SAND_TRAIL_2's trail buffer fx_04b_trails2: slots of */
#define FX_SANDSTORM_SAND_TRAIL_2_TRAIL_STRIDE 0x18 /* this many bytes (Pool6CEntry trailSlots / trailStride) */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* target position */
    SVECTOR pos10;
    SVECTOR pos18;
    SVECTOR aim20; /* angles from pos10 to pos8 */
} Fx04bWork80; /* 0x28 */

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4; /* above the target */
    SVECTOR aimC; /* angles from fx_04b_casterPos to pos4 */
    s32 target14; /* target index */
} Fx04bWork81; /* 0x18 */

extern Unk800F78A4 *fx_04b_env;
extern SVECTOR fx_04b_casterPos;
extern s32 fx_04b_trails[];
extern s32 fx_04b_trails2[];

#endif
