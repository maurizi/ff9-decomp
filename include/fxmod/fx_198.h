#ifndef FX_198_H
#define FX_198_H

/* Effect module fx_198 (dir 13 file 0x198): enemy Tsunami (Black Waltz 1/Sealion). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_198 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s6.md; "(look pending)" = by kind, appearance not settled). */
#define FX_TSUNAMI_WATER_SPLASH    0x6 /* anim quads: blue-white water bubbles and foam, 30 frames */
#define FX_TSUNAMI_BOWL            0x15 /* model: hemisphere bowl, plain white texture */
#define FX_TSUNAMI_BOWL_KEYS       0x16 /* table: per-frame rotation keys of the bowl model */
#define FX_TSUNAMI_BACKDROP        0x17 /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */
#define FX_TSUNAMI_SPLASH_ROW_KEYS 0x18 /* table: per-frame position keys of the row of water splashes */

/* fx_198 choreography (docs/fxdata.md, "Choreography literals"): the bowl model keyed by BOWL_KEYS, the two rings of
 * water splashes on their quad paths and the row of splashes keyed by SPLASH_ROW_KEYS, the states and lengths. */
#define FX_TSUNAMI_MODELS     5      /* btlfx_initEffModels slots */
#define FX_TSUNAMI_BOWL_TPAGE 0x137  /* Hi_RegisterTexEffModel texture: 15-bit page (0x1C0, 0x100), abr 1 */
#define FX_TSUNAMI_BOWL_CLUT  0xFFFF /* no CLUT (15-bit page) */
#define FX_TSUNAMI_BOWL_U     0
#define FX_TSUNAMI_BOWL_V     0x80
#define FX_TSUNAMI_STATE_WAVE 0      /* work->state0: the bowl and the splashes */
#define FX_TSUNAMI_STATE_END  1      /* clear the models, the op ends */
#define FX_TSUNAMI_BOWL_FRAMES     0x1E /* the bowl and the splash rings draw while frame < this */
#define FX_TSUNAMI_BOWL_FADE_FRAME 0x18 /* the bowl fades from this frame */
#define FX_TSUNAMI_QUAD_PATHS      0x11 /* splashes per ring (fx_198_quadPathFileIds: their path tables) */
#define FX_TSUNAMI_QUAD_PATH_END   0x15 /* the outer ring follows its paths up to this key, then stays */
#define FX_TSUNAMI_RING_FADE_FRAME 0x14 /* the outer ring fades from this frame; the inner ring fades out by it */
#define FX_TSUNAMI_ROW_FRAME       0x14 /* the row of splashes draws from this frame, fading out by the end of the state */
#define FX_TSUNAMI_ROW_SPLASHES    0xA  /* splashes in the row */
#define FX_TSUNAMI_WAVE_FRAMES     0x28 /* STATE_WAVE's length */

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    s32 scale8; /* scale */
    SVECTOR posC; /* caster position */
    s16 unk14; /* write-only (cleared at init) */
    s16 unk16; /* write-only (cleared at init) */
    s16 unk18; /* write-only (cleared at init) */
    s32 model1C; /* model */
    s32 unk20; /* unused */
} Fx198Work; /* 0x24 */

typedef struct {
    s16 x0; /* x (block 0x16: rotation y - 0x800) */
    s16 y2; /* y (block 0x16: rotation x - 0x400) */
    s16 z4; /* z (block 0x16: rotation z) */
} Fx198Key;

extern s16 fx_198_quadPathFileIds[FX_TSUNAMI_QUAD_PATHS]; /* file ids */
extern Unk800F78A4 *fx_198_env;
extern BtlModelBuf fx_198_effModels[];

#endif
