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

/* fx_12c choreography (docs/fxdata.md, "Choreography literals"): op80's keyframes, its countdown slots
 * (work->timer0[FX_EARTHQUAKE_T_*]) and the lengths written into them, the rocks and the background copy. */
#define FX_EARTHQUAKE_MODEL_COUNT     10   /* btlfx_initEffModels slots; fx_12c_bounceModels entries (ROCK_CHUNK_0..9) */
#define FX_EARTHQUAKE_BOUNCING_ROCKS  9    /* the rocks that bounce (ROCK_CHUNK_9 is registered but never drawn) */
#define FX_EARTHQUAKE_ROCK_SCALE      0xF00 /* the rocks' horizontal scale (vy is the bounce height) */
#define FX_EARTHQUAKE_GROUND_Z        (-0x550) /* the rocks, RUBBLE and the background copy sit this far along z */
#define FX_EARTHQUAKE_TIMERS          8    /* work->timer0[] countdowns (cleared at init): */
#define FX_EARTHQUAKE_T_ROCKS         0    /* the rocks bounce while it runs */
#define FX_EARTHQUAKE_T_RISE          1    /* the bounce strength fx_12c_intensity grows by 0x3C a frame */
#define FX_EARTHQUAKE_T_FALL          2    /* it shrinks by 0x1E a frame */
#define FX_EARTHQUAKE_T_SHAKE         3    /* the screen grid stretches and the screen darkens by the strength */
#define FX_EARTHQUAKE_BG_FRAME        0    /* the background is copied offscreen into the rect the rocks' tops sample: */
#define FX_EARTHQUAKE_BG_VRAM_X       0x1C0 /* page 0x117 (15-bit, (0x1C0, 0x100)) */
#define FX_EARTHQUAKE_BG_VRAM_Y       0x100
#define FX_EARTHQUAKE_BG_VRAM_W       0x100
#define FX_EARTHQUAKE_BG_VRAM_H       0x100
#define FX_EARTHQUAKE_DUST_FRAME      1    /* DUST_RING and ROCK_DEBRIS start at the caster's bone on the ground */
#define FX_EARTHQUAKE_RUBBLE_FRAME    9    /* RUBBLE starts; T_SHAKE and T_RISE start */
#define FX_EARTHQUAKE_SHAKE_FRAMES    0x40
#define FX_EARTHQUAKE_RISE_FRAMES     0x10
#define FX_EARTHQUAKE_ROCKS_FRAME     17   /* T_ROCKS starts */
#define FX_EARTHQUAKE_ROCKS_FRAMES    0x38
#define FX_EARTHQUAKE_SETTLE_FRAMES   8    /* a rock that lands with fewer T_ROCKS frames left stays down */
#define FX_EARTHQUAKE_FALL_FRAME      41   /* T_FALL starts */
#define FX_EARTHQUAKE_FALL_FRAMES     0x20
#define FX_EARTHQUAKE_FRAMES          0x80 /* op80's length */

typedef struct {
    s32 timer0[FX_EARTHQUAKE_TIMERS]; /* countdowns (FX_EARTHQUAKE_T_*) */
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
extern Fx12cRock fx_12c_bounceModels[FX_EARTHQUAKE_MODEL_COUNT];
extern s32 fx_12c_intensity; /* bounce strength */
extern BtlModelBuf fx_12c_effModels[];
extern u32 fx_12c_bgOt[];

s32 fx_12c_op80(s32 mode, Fx12cWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

#endif
