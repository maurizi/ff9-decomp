#ifndef FX_0CB_H
#define FX_0CB_H

/* Effect module fx_0cb (dir 13 file 0xCB): enemy Light Flare (Thorn/Zorn). Spinning sparkle models (pool items) and
 * rings at the caster, a radial screen grid (fxm_drawRadialScreenGrid). Evidence: [data] the party ability / item
 * tables and enemy sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31
 * runtime pass"). */
#include "btlfx_mod.h"

/* fx_0cb effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_LIGHT_FLARE_GLOW_RINGS    0x5 /* rings: white / cyan / orange / red gouraud rings */
#define FX_LIGHT_FLARE_RED_HALO      0x8 /* 'so' model: red-orange gouraud halo ring, drawn fading with the fireball */
#define FX_LIGHT_FLARE_BACKDROP      0xC /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) (with the screen tint) */
#define FX_LIGHT_FLARE_BACKDROP_2    0xF /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) (with the rings) */
#define FX_LIGHT_FLARE_GLOW_RING     0x11 /* rings: textured ring drawn twice, tilted: the yellow-green glow ring round the target; runtime (docs/runtime-findings.md effect-data pass 2) */
#define FX_LIGHT_FLARE_FIREBALL      0x12 /* 'so' model: sphere with a fire texture, drawn fading */

/* fx_0cb choreography (docs/fxdata.md, "Choreography literals"): op80's length, tints and fireball spawning, the
 * fireball pool items' look, and the VRAM rect of the fireball's scrolling texture. */
#define FX_LIGHT_FLARE_FRAMES        50   /* op80: the rings fade over this many frames, then the task ends */
#define FX_LIGHT_FLARE_DARKEN_RGB    0, 0x40, 0x40 /* op80 frame 0: subtractive screen tint (darkens green and blue) */
#define FX_LIGHT_FLARE_FLASH_FRAMES  7    /* op80: then an additive yellow-white screen flash fades until this frame */
#define FX_LIGHT_FLARE_SPAWN_FRAMES  0x20 /* op80: a fireball pool item spawns every SPAWN_GAP frames until this frame */
#define FX_LIGHT_FLARE_SPAWN_GAP     3
#define FX_LIGHT_FLARE_SLOTS         6    /* pool items */
#define FX_LIGHT_FLARE_MODEL_COUNT   3    /* btlfx_initEffModels slots (FIREBALL and RED_HALO registered) */
#define FX_LIGHT_FLARE_BURST_FRAMES     8 /* pool item: FIREBALL + RED_HALO pulse at a random spot for this many frames, */
#define FX_LIGHT_FLARE_BURST_IN_FRAMES  3 /* brightening over the first ones, then fading */
/* FX_LIGHT_FLARE_FIREBALL samples (tpage 0x99 = 8-bit page (0x240, 0x100), u / v 0) the rect at VRAM_X; each frame
 * btlfx_drawScrollMove copies the file's fire texture from (SRC_X, SRC_Y) beside it into it, scrolled. */
#define FX_LIGHT_FLARE_FIREBALL_VRAM_X 0x240
#define FX_LIGHT_FLARE_FIREBALL_VRAM_Y 0x100
#define FX_LIGHT_FLARE_FIREBALL_VRAM_W 0x38
#define FX_LIGHT_FLARE_FIREBALL_VRAM_H 0x70
#define FX_LIGHT_FLARE_FIREBALL_SRC_X  0x278
#define FX_LIGHT_FLARE_FIREBALL_SRC_Y  0x100

typedef struct {
    s16 unk0; /* write-only (cleared at spawn) */
    s16 scale2; /* scale */
    SVECTOR pos4; /* position */
    SVECTOR rotC; /* rotation */
    s16 spin14; /* spin angle */
    s16 spinSpeed16; /* spin speed */
} Fx0cbPart; /* 0x18 */

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx0cbVec3;

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame */
    s32 otZ8; /* priority */
    SVECTOR posC; /* caster body position */
    Fx0cbVec3 pos14;
    s32 model20; /* model */
    s32 model24; /* model */
    s32 unk28; /* unused */
} Fx0cbWork; /* 0x2C */

extern BtlModelBuf fx_0cb_effModels[];

extern Unk800F78A4 *fx_0cb_env;

static void fxm_anglesFromEye(s16 *target, s16 *outAng);
static s32 fx_0cb_op80PoolTask(s32 mode, Fx0cbPart *p, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, s32 a5, Fx0cbWork *work);

/* propagated */
static void fxm_drawRadialScreenGrid(u32 ot, s32 x, s32 y, s32 segs, s32 rings, s32 dr, s32 from, s32 to, RECT *src,
                   void (*fn)(s32, s32, s32, s32, DVECTOR *));

#endif
