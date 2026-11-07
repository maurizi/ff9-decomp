#ifndef FX_0F0_H
#define FX_0F0_H

/* Effect module fx_0f0 (dir 13 file 0x0F0): enemy Zombie Powder (Whale Zombie). Emitters from a caster bone with
 * trail buffers, one homing on the target. Evidence: [data] the party ability / item tables and enemy sequences
 * that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0f0 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s3.md; "(look pending)" = by kind, appearance not settled). */
#define FX_ZOMBIE_POWDER_BLUE_SPECKS  0x1 /* emitter: animated quads, blue specks at run time: the blue glowing dust drifting round the caster; runtime (docs/runtime-findings.md effect-data pass 2, medium) */
#define FX_ZOMBIE_POWDER_SPORE_TRAIL  0x2 /* emitter: homing green spores, yellow puffs, sparkles */
#define FX_ZOMBIE_POWDER_YELLOW_PUFFS 0x3 /* emitter: yellow powder puffs */
#define FX_ZOMBIE_POWDER_YELLOW_CLOUD 0x4 /* emitter: yellow powder cloud */
#define FX_ZOMBIE_POWDER_VIOLET_CLOUD 0x5 /* emitter: violet powder cloud */
#define FX_ZOMBIE_POWDER_MIST_TRAIL   0x6 /* emitter: green-grey mist screen trails */
#define FX_ZOMBIE_POWDER_MIST_TRAIL_2 0x7 /* emitter: green-grey mist screen trails */
#define FX_ZOMBIE_POWDER_DUST_WAVE    0x15 /* emitter: yellow-green dust half ring */

/* fx_0f0 choreography (docs/fxdata.md, "Choreography literals"): op80's / op81's keyframes, the trail buffers and
 * the op lengths. */
#define FX_ZOMBIE_POWDER_SPRAY_FRAME 0x26 /* op80: the mists, VIOLET_CLOUD and SPORE_TRAIL start at the caster bone; the spores home on the target */
#define FX_ZOMBIE_POWDER_FRAMES      0x50 /* op80's length (the op ends) */
#define FX_ZOMBIE_POWDER_MIST_TRAIL_TRAIL_SLOTS   0x1C /* MIST_TRAIL / MIST_TRAIL_2 trail buffers fx_0f0_op80Trails0 / 1: slots of */
#define FX_ZOMBIE_POWDER_MIST_TRAIL_TRAIL_STRIDE  0x9C /* this many bytes (Pool6CEntry trailSlots / trailStride) */
#define FX_ZOMBIE_POWDER_SPORE_TRAIL_TRAIL_SLOTS  0x3A /* SPORE_TRAIL's trail buffer fx_0f0_op80Trails2: slots of */
#define FX_ZOMBIE_POWDER_SPORE_TRAIL_TRAIL_STRIDE 0x70 /* this many bytes */
#define FX_ZOMBIE_POWDER_WAVE_FRAME  4    /* op81: FX_ZOMBIE_POWDER_DUST_WAVE at the target (the yellow puffs and cloud at init) */
#define FX_ZOMBIE_POWDER_HIT_FRAMES  0x3C /* op81's length (the op ends) */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    SVECTOR pos14;
    SVECTOR pos1C;
    SVECTOR pos24;
} Fx0f0Work80; /* 0x2C */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    SVECTOR posC;
} Fx0f0Work81; /* 0x14 */

extern Unk800F78A4 *fx_0f0_env;
extern s32 fx_0f0_op80Trails0[];
extern s32 fx_0f0_op80Trails1[];
extern s32 fx_0f0_op80Trails2[];

#endif
