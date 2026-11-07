#ifndef FX_0B9_H
#define FX_0B9_H

/* Effect module fx_0b9 (dir 13 file 0xB9): enemy Sandstorm (Core/Sand Golem, ...). Evidence: [data] the party
 * ability / item tables and enemy sequences that play this file id (tools/effect_labels.py;
 * docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0b9 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_SANDSTORM_SAND_TRAIL_2 0x1 /* emitter: screen trail with a grey sand streak strip (byte-identical to fx_04b entry 0x3) */
#define FX_SANDSTORM_SAND_TRAIL 0x2 /* emitter: homing trail with a grey sand streak strip (byte-identical to fx_04b entry 0x2) */
#define FX_SANDSTORM_SAND_CLOUD    0x3 /* emitter: grainy grey sand clouds */

/* fx_0b9 choreography (docs/fxdata.md, "Choreography literals"): the ops' keyframes, lengths and trail buffers. */
#define FX_SANDSTORM_SAND_TRAIL_FRAME   0xA  /* op80: FX_SANDSTORM_SAND_TRAIL starts and follows unit 0x10's bone from here on */
#define FX_SANDSTORM_SAND_TRAIL_2_FRAME 0x28 /* op80: FX_SANDSTORM_SAND_TRAIL_2 starts, aimed from the bone at the targets */
#define FX_SANDSTORM_FRAMES             0x64 /* op80: the task ends at this frame */
#define FX_SANDSTORM_CLOUD_FRAMES       0x3C /* op81 (one target's FX_SANDSTORM_SAND_CLOUD): the task ends at this frame */
#define FX_SANDSTORM_CLOUD_HEIGHT       0xC8 /* op81: the cloud starts this far above the target's feet */
#define FX_SANDSTORM_SAND_TRAIL_TRAIL_SLOTS    0x82 /* FX_SANDSTORM_SAND_TRAIL's trail buffer fx_0b9_trails: slots of */
#define FX_SANDSTORM_SAND_TRAIL_TRAIL_STRIDE   0x18 /* this many bytes (Pool6CEntry trailSlots / trailStride) */
#define FX_SANDSTORM_SAND_TRAIL_2_TRAIL_SLOTS  0xA0 /* FX_SANDSTORM_SAND_TRAIL_2's trail buffer fx_0b9_trails2: slots of */
#define FX_SANDSTORM_SAND_TRAIL_2_TRAIL_STRIDE 0x18 /* this many bytes */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* position: env->targets->centerX..Z */
    SVECTOR pos10; /* btlfx_getUnitArgBonePos(0x10, 0) */
    SVECTOR pos18; /* btlfx_getUnitArgBonePos(0x10, 0), every frame */
    SVECTOR rot20; /* angles from pos10 to pos8 */
} Fx0b9Work80;

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4; /* unit position, raised by 0xC8 */
    SVECTOR rotC; /* angles from fx_0b9_srcPos to pos4 */
    s32 unit14; /* unit */
} Fx0b9Work81;

extern Unk800F78A4 *fx_0b9_env;
extern SVECTOR fx_0b9_srcPos;
extern s32 fx_0b9_trails[];
extern s32 fx_0b9_trails2[];

#endif
