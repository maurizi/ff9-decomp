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

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* caster body position */
    SVECTOR pos10;
} Fx11cWork80;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4[5];
    Pool6CEntry *emit18;
    Pool6CEntry *emit1C;
    SVECTOR pos20; /* target position */
} Fx11cWork81;

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
} Fx11cWork82;

typedef struct {
    SVECTOR pos0; /* spawn position */
    SVECTOR pos8;
    u16 active10; /* started */
    u16 frame12; /* age */
    u16 startFrame14; /* start frame */
    u16 fileId16; /* file */
} Fx11cBurst;

extern Fx11cBurst fx_11c_op81Bursts[5];
extern Unk800F78A4 *fx_11c_env;
extern s32 fx_11c_op80Trails0[];
extern s32 fx_11c_op80Trails1[];
extern s32 fx_11c_op82Trails[];

#endif
