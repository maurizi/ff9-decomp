#ifndef FX_12C_H
#define FX_12C_H

/* Effect module fx_12c (dir 13 file 0x12C): enemy Earthquake, Power of the Land (Catoblepas, ...). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_12c effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_EARTHQUAKE_ROCK_CHUNK_8 0x1 /* 'so' model: bouncing rock chunk with rock-textured sides, its top samples the offscreen background copy (page 0x117), fx_12c_bounceModels[8] */
#define FX_EARTHQUAKE_ROCK_CHUNK_1 0x2 /* 'so' model: bouncing rock chunk with rock-textured sides, its top samples the offscreen background copy (page 0x117), fx_12c_bounceModels[1] */
#define FX_EARTHQUAKE_ROCK_CHUNK_2 0x3 /* 'so' model: bouncing rock chunk with rock-textured sides, its top samples the offscreen background copy (page 0x117), fx_12c_bounceModels[2] */
#define FX_EARTHQUAKE_ROCK_CHUNK_3 0x4 /* 'so' model: bouncing rock chunk with rock-textured sides, its top samples the offscreen background copy (page 0x117), fx_12c_bounceModels[3] */
#define FX_EARTHQUAKE_ROCK_CHUNK_4 0x5 /* 'so' model: bouncing rock chunk with rock-textured sides, its top samples the offscreen background copy (page 0x117), fx_12c_bounceModels[4] */
#define FX_EARTHQUAKE_ROCK_CHUNK_5 0x6 /* 'so' model: bouncing rock chunk with rock-textured sides, its top samples the offscreen background copy (page 0x117), fx_12c_bounceModels[5] */
#define FX_EARTHQUAKE_ROCK_CHUNK_6 0x7 /* 'so' model: bouncing rock chunk with rock-textured sides, its top samples the offscreen background copy (page 0x117), fx_12c_bounceModels[6] */
#define FX_EARTHQUAKE_ROCK_CHUNK_7 0x8 /* 'so' model: bouncing rock chunk with rock-textured sides, its top samples the offscreen background copy (page 0x117), fx_12c_bounceModels[7] */
#define FX_EARTHQUAKE_ROCK_CHUNK_0 0x9 /* 'so' model: bouncing rock chunk with rock-textured sides, its top samples the offscreen background copy (page 0x117), fx_12c_bounceModels[0] */
#define FX_EARTHQUAKE_ROCK_CHUNK_9 0xE /* 'so' model: bouncing rock chunk with rock-textured sides, its top samples the offscreen background copy (page 0x117), fx_12c_bounceModels[9] */
#define FX_EARTHQUAKE_RUBBLE 0x10 /* emitter: spiral of small rock debris / rubble frames (byte-identical to fx_03a entry 0x10) */
#define FX_EARTHQUAKE_DUST_RING     0x11 /* emitter: ring with a sand-dust texture on the ground */
#define FX_EARTHQUAKE_ROCK_DEBRIS   0x12 /* emitter: small rock debris frames thrown up */

typedef struct {
    s32 timer0[8]; /* countdowns */
    SVECTOR pos20; /* effect position */
    SVECTOR pos28; /* effect position */
} Fx12cWork; /* 0x30 */

typedef struct {
    s32 model0; /* model */
    VECTOR scale4; /* scale (vy: bounce height) */
    VECTOR pos14; /* position */
    SVECTOR rot24; /* rotation */
    s32 speed2C; /* bounce speed */
    s32 frame30; /* bounce time */
} Fx12cRock; /* 0x34 */

extern Unk800F78A4 *fx_12c_env;
extern Fx12cRock fx_12c_bounceModels[10];
extern s32 fx_12c_intensity; /* bounce strength */
extern BtlModelBuf fx_12c_effModels[];
extern u32 fx_12c_bgOt[];

s32 fx_12c_op80(s32 mode, Fx12cWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

#endif
