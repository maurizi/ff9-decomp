#ifndef FX_038_H
#define FX_038_H

/* Effect module fx_038 (dir 13 file 0x38). */
#include "btlfx_mod.h"

/* fx_038 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_COMET_BLUE_GLOW    0x1 /* particles: blue glow with a white core, drawn on each falling rock */
#define FX_COMET_ROCK         0x2 /* model: rock (grey stone texture) */
#define FX_COMET_ROCK_DEBRIS  0x3 /* emitter: orange / white striped debris sprites */
#define FX_COMET_TAIL         0x4 /* trail: violet-white strip behind each rock */
#define FX_COMET_FIRE_GLOW    0x5 /* particles: orange-red fiery glow */
#define FX_COMET_IMPACT_SMOKE 0x9 /* emitter: white-pink smoke burst at the impact */
#define FX_COMET_GLOW_DISH    0xA /* gouraud model: wide shallow dish */
#define FX_COMET_SHOCK_RING   0xB /* model: flat ring, white texture */
#define FX_COMET_HAZE_TEXTURE 0xC /* layer table: render-to-texture (btlfx_drawLayerTexture): 3 layers into VRAM (0x200,0x180) 64x64, a dim violet haze band; skipping it changes nothing on screen; runtime (docs/runtime-findings.md effect-data pass, low) (look pending: low) */
#define FX_COMET_BLUE_RING    0xD /* rings: dark blue / blue gouraud rings at the impact */

/* fx_038 choreography (docs/fxdata.md, "Choreography literals"): the rock pool, its fall / impact states and their flashes, the models'
 * textures, op80's length. */
#define FX_COMET_ROCK_COUNT        3    /* falling rocks (op80 poolCount, items spawned) */
#define FX_COMET_SPAWN_FRAMES      0x18 /* fx_038_spawnFrames entries: nonzero on the frames a rock spawns */
#define FX_COMET_FRAMES            0x3E /* op80: the task ends at this frame (once all rocks have spawned) */
#define FX_COMET_MODEL_COUNT       3    /* btlfx_initEffModels slots: ROCK, SHOCK_RING, GLOW_DISH */
#define FX_COMET_STATE_FALL        0    /* rock pool item: falls to its landing point with FX_COMET_TAIL and FX_COMET_BLUE_GLOW, */
#define FX_COMET_FALL_FRAMES       15   /* for this many frames */
#define FX_COMET_STATE_IMPACT      1    /* lands: darken, FX_COMET_IMPACT_SMOKE, flash, glow dish, shock ring, blue rings, fire glow */
#define FX_COMET_IMPACT_FRAMES     18   /* FX_COMET_STATE_IMPACT's length (the item ends) */
#define FX_COMET_IMPACT_DARKEN_RGB 0x18, 0x18, 0x18 /* subtractive screen tint as a rock lands */
#define FX_COMET_FLASH_LEVEL       0x46 /* then an additive white flash from this level, */
#define FX_COMET_FLASH_FRAMES      4    /* fading over this many frames */
#define FX_COMET_GLOW_DISH_FRAMES  8    /* FX_COMET_GLOW_DISH is drawn for this many impact frames (t = frame << 9) */
#define FX_COMET_FIRE_GLOW_FRAMES  12   /* FX_COMET_FIRE_GLOW is drawn, fading, for this many */
/* FX_COMET_ROCK: 8-bit page (0x1C0, 0x100), clut (0, 0xF2). */
#define FX_COMET_ROCK_TPAGE        0x97
#define FX_COMET_ROCK_CLUT         0x3C80
/* FX_COMET_SHOCK_RING: 15-bit page (0x1C0, 0x100), abr 1, no clut. */
#define FX_COMET_SHOCK_RING_TPAGE  0x137
#define FX_COMET_SHOCK_RING_CLUT   0xFFFF
#define FX_COMET_SHOCK_RING_U      (-0x40)
#define FX_COMET_SHOCK_RING_V      0x80

/* falling rock, a pool item */
typedef struct {
    s32 state0; /* state */
    SVECTOR pos4; /* position */
    SVECTOR rotC; /* rotation */
    Pool6CEntry *emit14;
    SVECTOR pos18; /* landing position */
    s32 strip20; /* index */
} Fx038Item; /* 0x24 */

/* trail strip for btlfx_drawTrail */
typedef struct {
    u8 pad0[0x8C];
} Fx038Strip;

extern Fx038Strip fx_038_strips[];

typedef struct {
    s32 hold0; /* nonzero: the op idles and never ends by itself (runtime); always 0 in the game (see Fx046Work.hold0) */
    SVECTOR pos4; /* target position */
    s32 countC; /* items spawned */
    s32 model10; /* models */
    s32 model14;
    s32 model18;
} Fx038Work; /* 0x1C */

extern u8 fx_038_spawnFrames[FX_COMET_SPAWN_FRAMES]; /* spawn frames */
extern Unk800F78A4 *fx_038_env;
extern BtlModelBuf fx_038_effModels[];

static s32 fx_038_op80PoolTask(s32 mode, Fx038Item *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, Unk800DA1B8 *pool,
                  Fx038Work *work);

#endif
