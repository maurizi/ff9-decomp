#ifndef FX_186_H
#define FX_186_H

/* Effect module fx_186 (dir 13 file 0x186): Earth Shake; enemy Earth Shake (Adamantoise, ...). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_186 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_EARTH_SHAKE_ROCK_ARCH     0xB /* 'so' model: arch with a rock texture, registered at the start */
#define FX_EARTH_SHAKE_GREY_RING     0xF /* rings: white-to-grey gouraud rings at each target */
#define FX_EARTH_SHAKE_ROCK_DEBRIS   0x1A /* emitter: small rock debris frames */
#define FX_EARTH_SHAKE_RUBBLE_STRIP  0x1B /* 'so' model: flat strip of rubble with a grey smoke texture, registered at the start */
#define FX_EARTH_SHAKE_ROCK_DEBRIS_2 0x1D /* emitter: small rock debris frames, brighter */
#define FX_EARTH_SHAKE_LILAC_RING    0x1E /* rings: white-lilac gouraud rings */
#define FX_EARTH_SHAKE_CYAN_RING     0x1F /* rings: white-cyan gouraud rings */

/* fx_186 choreography (docs/fxdata.md, "Choreography literals"): op80's states and lengths (work->state0, the frame
 * restarts at each), the rock pool (Fx186Rock states), the background copy, the VRAM probes that pick the rock
 * models, the rubble scroll; op81's target toss. */
#define FX_EARTH_SHAKE_STATE_RISE   0    /* op80: ROCK_ARCH rings rise, the RUBBLE_STRIP pulses, LILAC / CYAN_RING from RINGS_FRAME */
#define FX_EARTH_SHAKE_RISE_FRAMES  0x10 /* then a white flash; the rock models are registered */
#define FX_EARTH_SHAKE_STATE_BURST  1    /* ROCK_DEBRIS and the rock pool burst out of the ground */
#define FX_EARTH_SHAKE_BURST_FRAMES 0x20
#define FX_EARTH_SHAKE_STATE_SETTLE 2    /* ROCK_DEBRIS_2, a darken-then-flash and the screen-grid tint */
#define FX_EARTH_SHAKE_SETTLE_FRAMES 0x28 /* (also its shake ramp, frame / 40); the op ends */
#define FX_EARTH_SHAKE_STATE_BG     3    /* first frame: the background is copied offscreen (the rocks' texture page), then RISE */
#define FX_EARTH_SHAKE_BG_VRAM_X    0x1C0 /* the copy's rect: page 0x117 (15-bit, (0x1C0, 0x100)) */
#define FX_EARTH_SHAKE_BG_VRAM_Y    0x100
#define FX_EARTH_SHAKE_BG_VRAM_W    0x100
#define FX_EARTH_SHAKE_BG_VRAM_H    0x100
#define FX_EARTH_SHAKE_ROCK_MODELS  10   /* model1C / fx_186_modelFileIds0 / 1 entries; the rock pool's size */
#define FX_EARTH_SHAKE_ROCK_SPAWNS  25   /* STATE_BURST tries this many pool spawns (only ROCK_MODELS succeed) */
#define FX_EARTH_SHAKE_ROCK_SCALE   0x2000 /* the rocks' scale (and RUBBLE_STRIP's width) */
#define FX_EARTH_SHAKE_ROCK_STATE_BOB 0  /* Fx186Rock.state0: the rock bobs at the rubble for BOB_FRAMES, */
#define FX_EARTH_SHAKE_ROCK_BOB_FRAMES 0x20
#define FX_EARTH_SHAKE_ROCK_STATE_FLY 1  /* then flies out and fades (additive below half) over FLY_FRAMES */
#define FX_EARTH_SHAKE_ROCK_FLY_FRAMES 0x28
#define FX_EARTH_SHAKE_PROBE_SIZE   4    /* frame 1 of STATE_RISE: three 4x4 VRAM probes (StoreImage into fx_186_vramSample); */
#define FX_EARTH_SHAKE_PROBE_0_X    0x200 /* a probe pixel that is 0 picks fx_186_modelFileIds1's rocks */
#define FX_EARTH_SHAKE_PROBE_0_Y    0x180
#define FX_EARTH_SHAKE_PROBE_1_X    0x27C
#define FX_EARTH_SHAKE_PROBE_1_Y    0x180
#define FX_EARTH_SHAKE_PROBE_2_X    0x240
#define FX_EARTH_SHAKE_PROBE_2_Y    0x1EC
#define FX_EARTH_SHAKE_SAMPLE_PIXELS 0x60 /* fx_186_vramSample entries checked */
#define FX_EARTH_SHAKE_ARCH_COPIES  6    /* ROCK_ARCH is drawn this many times round the axis */
#define FX_EARTH_SHAKE_RUBBLE_Y     0x5AE /* RUBBLE_STRIP's height; the rocks spawn there */
/* RUBBLE_STRIP's smoke texture scrolls 0x10 a frame from (SRC_X, SRC_Y) through this rect (not checked against the model) */
#define FX_EARTH_SHAKE_SMOKE_VRAM_X 0x2C0
#define FX_EARTH_SHAKE_SMOKE_VRAM_Y 0x180
#define FX_EARTH_SHAKE_SMOKE_VRAM_W 0x20
#define FX_EARTH_SHAKE_SMOKE_VRAM_H 0x80
#define FX_EARTH_SHAKE_SMOKE_SRC_X  0x2E0
#define FX_EARTH_SHAKE_SMOKE_SRC_Y  0x180
#define FX_EARTH_SHAKE_RINGS_FRAME  9    /* STATE_RISE: LILAC_RING and CYAN_RING grow */
#define FX_EARTH_SHAKE_FLASH_FRAMES 4    /* STATE_BURST: the white flash fades over this many frames */
#define FX_EARTH_SHAKE_ARCH_FRAMES  0x18 /* STATE_BURST: the arch rings rise and fade over this many frames */
#define FX_EARTH_SHAKE_GRID_TINT    0x89 /* STATE_SETTLE: the screen grid's grey */
#define FX_EARTH_SHAKE_UNITS        8    /* op81: Fx186Work81's per-unit arrays */
#define FX_EARTH_SHAKE_TOSS_STATE_SHAKE 0 /* op81 work->state0: the targets shake upwards for SHAKE_FRAMES, */
#define FX_EARTH_SHAKE_TOSS_SHAKE_FRAMES 0x20
#define FX_EARTH_SHAKE_TOSS_STATE_TOSS 1  /* are tossed up spinning (half a sine) for TOSS_FRAMES and put back, */
#define FX_EARTH_SHAKE_TOSS_FRAMES  0x28
#define FX_EARTH_SHAKE_TOSS_STATE_RING 2  /* then GREY_RING at each for RING_FRAMES (the op ends) */
#define FX_EARTH_SHAKE_TOSS_RING_FRAMES 7

typedef struct {
    s32 state0; /* state */
    s16 pad4;
    s16 bobVelY6; /* bob height (state 0), then y velocity (state 1) */
    s16 vel8; /* velocity */
    s16 velA;
    SVECTOR posC; /* position */
    SVECTOR rot14; /* rotation */
    s16 spin1C; /* spin */
    s16 spin1E;
    s32 model20; /* model */
    u8 unused24[4]; /* pads to the pool's element size */
} Fx186Rock; /* 0x28 */

typedef struct {
    s32 state0; /* state */
    SVECTOR pos4[FX_EARTH_SHAKE_UNITS]; /* home positions */
    SVECTOR rot44[FX_EARTH_SHAKE_UNITS]; /* rotations */
    SVECTOR spin84[FX_EARTH_SHAKE_UNITS]; /* spin */
    SVECTOR rotC4[FX_EARTH_SHAKE_UNITS]; /* current rotations */
    s32 count104; /* unit count */
    s32 height108; /* bounce height */
} Fx186Work81; /* 0x10C */

extern Unk800F78A4 *fx_186_env;

typedef struct {
    s32 unk0; /* unused (the typedef has no users) */
    s32 unk4; /* unused (the typedef has no users) */
} Fx186Word2; /* an SVECTOR copied as two words */

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    SVECTOR pos8; /* position */
    SVECTOR pos10;
    Pool6CEntry *emit18;
    s32 model1C[FX_EARTH_SHAKE_ROCK_MODELS]; /* rock models */
    s32 model44; /* model */
    s32 model48; /* model */
} Fx186Work80; /* 0x4C */

extern s16 fx_186_vramSample[]; /* StoreImage buffer */
extern u16 fx_186_modelFileIds0[FX_EARTH_SHAKE_ROCK_MODELS]; /* rock model files */
extern u16 fx_186_modelFileIds1[FX_EARTH_SHAKE_ROCK_MODELS]; /* rock model files (alternate) */
extern u32 fx_186_bgOt[];
extern BtlModelBuf fx_186_effModels[];
static s32 fx_186_op80PoolTask(s32 mode, Fx186Rock *rock, Unk800F78A4 *env, s32 entry, s32 frame);

#endif
