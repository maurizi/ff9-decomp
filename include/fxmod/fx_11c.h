#ifndef FX_11C_H
#define FX_11C_H

/* Effect module fx_11c (dir 13 file 0x11C): enemy High Wind (Abadon). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_11c effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s4.md; "(look pending)" = by kind, appearance not settled). */
#define FX_HIGH_WIND_WIND_TRAIL   0x2 /* emitter: 3D trail spiralling up, grey wind-swirl strip */
#define FX_HIGH_WIND_TEAL_SWIRL   0x3 /* emitter: teal swirl and crescent particles */
#define FX_HIGH_WIND_WIND_TRAIL_2 0x6 /* emitter: 3D trail spiralling up, grey wind-swirl strip */
#define FX_HIGH_WIND_CLOUD        0x9 /* emitter: teal-white cloud puffs spiralling, 22 animated frames */
#define FX_HIGH_WIND_TEAL_RINGS   0xA /* emitter: teal gouraud rings, 32 frames */
#define FX_HIGH_WIND_WIND_TRAIL_3 0xB /* emitter: 3D trail spiralling up, grey wind-swirl strip */
#define FX_HIGH_WIND_WIND_STREAKS 0xC /* emitter: teal jagged wind-streak particles spiralling */
#define FX_HIGH_WIND_CLOUD_2      0xD /* emitter: teal-white cloud puffs spiralling, 22 animated frames */
#define FX_HIGH_WIND_GREY_SWIRL   0xE /* emitter: grey swirl particle */

/* fx_11c choreography (docs/fxdata.md, "Choreography literals"): the three ops' keys, trail buffers and op81's
 * burst table. */
#define FX_HIGH_WIND_WIND_TRAIL_SLOTS   0xD  /* op80: WIND_TRAIL_2's and WIND_TRAIL's trail buffers (fx_11c_op80Trails0 / 1): slots */
#define FX_HIGH_WIND_WIND_TRAIL_STRIDE  0x90 /* bytes per slot (also WIND_TRAIL_3's) */
#define FX_HIGH_WIND_GUST_FRAMES        0x28 /* op80's length */
#define FX_HIGH_WIND_BURST_COUNT        5    /* op81: fx_11c_op81Bursts, emitters round the target */
#define FX_HIGH_WIND_BURST_INTERVAL     2    /* burst i starts on frame i * this */
#define FX_HIGH_WIND_BURSTS_FRAMES      0x32 /* op81's length */
#define FX_HIGH_WIND_WIND_TRAIL_3_SLOTS 8    /* op82: WIND_TRAIL_3's trail buffer (fx_11c_op82Trails): slots */
#define FX_HIGH_WIND_STREAKS_FRAME      5    /* WIND_STREAKS and CLOUD_2 start at the target */
#define FX_HIGH_WIND_SIDE_CLOUDS_FRAME  0xD  /* two more CLOUD_2s start, left and right */
#define FX_HIGH_WIND_SIDE_CLOUD_OFFSET  0x190 /* this far from the target */
#define FX_HIGH_WIND_STORM_FRAMES       0x32 /* op82's length */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* caster body position */
    SVECTOR pos10;
} Fx11cWork80; /* 0x18 */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4[FX_HIGH_WIND_BURST_COUNT];
    Pool6CEntry *emit18;
    Pool6CEntry *emit1C;
    SVECTOR pos20; /* target position */
} Fx11cWork81; /* 0x28 */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    Pool6CEntry *emit14;
    SVECTOR pos18;
    SVECTOR pos20;
    SVECTOR pos28;
} Fx11cWork82; /* 0x30 */

typedef struct {
    SVECTOR pos0; /* spawn position */
    SVECTOR pos8;
    u16 active10; /* started */
    u16 frame12; /* age */
    u16 startFrame14; /* start frame */
    u16 fileId16; /* file */
} Fx11cBurst;

extern Fx11cBurst fx_11c_op81Bursts[FX_HIGH_WIND_BURST_COUNT];
extern Unk800F78A4 *fx_11c_env;
extern s32 fx_11c_op80Trails0[];
extern s32 fx_11c_op80Trails1[];
extern s32 fx_11c_op82Trails[];

#endif
