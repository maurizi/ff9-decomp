#ifndef FX_0A7_H
#define FX_0A7_H

/* Effect module fx_0a7 (dir 13 file 0xA7): enemy Water-gun (Sahagin). Emitters only. Evidence: [data] the party
 * ability / item tables and enemy sequences that play this file id (tools/effect_labels.py;
 * docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0a7 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_WATER_GUN_SPRAY_ARC     0x1 /* emitter: fan + blue water spray arc */
#define FX_WATER_GUN_WATER_TRAIL   0x4 /* emitter: water strip trail + droplet sprites */
#define FX_WATER_GUN_SPRAY_ARC_2   0x5 /* emitter: blue water spray arc */
#define FX_WATER_GUN_WATER_TRAIL_2 0x6 /* emitter: water strip trail */
#define FX_WATER_GUN_BUBBLES       0x7 /* emitter: bubble rings */

/* fx_0a7 choreography (docs/fxdata.md, "Choreography literals"). */
#define FX_WATER_GUN_TRAIL_REACH  0x12C /* FX_WATER_GUN_WATER_TRAIL_2 starts this far from the unit's bone towards the target */
#define FX_WATER_GUN_SPRAY_REACH  0xC8  /* FX_WATER_GUN_SPRAY_ARC starts this far */
#define FX_WATER_GUN_HIT_FRAME    12    /* WATER_TRAIL and SPRAY_ARC_2 start at the target (and are aimed from then on) */
#define FX_WATER_GUN_BUBBLES_FRAME 30   /* FX_WATER_GUN_BUBBLES start */
#define FX_WATER_GUN_FRAMES       0x32  /* op80's length (the op ends) */
#define FX_WATER_GUN_WATER_TRAIL_2_TRAIL_SLOTS  0xB  /* FX_WATER_GUN_WATER_TRAIL_2's trail buffer fx_0a7_trails: slots of */
#define FX_WATER_GUN_WATER_TRAIL_2_TRAIL_STRIDE 0x40 /* this many bytes */
#define FX_WATER_GUN_SPRAY_ARC_TRAIL_SLOTS      0x2D /* FX_WATER_GUN_SPRAY_ARC's trail buffer fx_0a7_trails2: slots of */
#define FX_WATER_GUN_SPRAY_ARC_TRAIL_STRIDE     0x18 /* this many bytes */
#define FX_WATER_GUN_WATER_TRAIL_TRAIL_SLOTS    0x51 /* FX_WATER_GUN_WATER_TRAIL's trail buffer fx_0a7_trails3: slots of */
#define FX_WATER_GUN_WATER_TRAIL_TRAIL_STRIDE   0x40 /* this many bytes */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    s32 unk8; /* unused */
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    Pool6CEntry *emit14;
    SVECTOR pos18; /* unit 0x10 body position */
    SVECTOR pos20; /* caster body position */
    SVECTOR pos28; /* caster position */
    SVECTOR pos30; /* btlfx_getUnitArgBonePos(0x10, 0) */
    SVECTOR pos38; /* pos30 moved 0x12C along aim48 */
    SVECTOR pos40; /* pos30 moved 0xC8 along aim48 */
    SVECTOR aim48; /* angles from pos30 to pos20 */
    SVECTOR aim50; /* angles from pos20 to pos30 */
} Fx0a7Work;

extern Unk800F78A4 *fx_0a7_env;
extern s32 fx_0a7_trails[];
extern s32 fx_0a7_trails2[];
extern s32 fx_0a7_trails3[];

#endif
