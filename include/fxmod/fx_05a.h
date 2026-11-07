#ifndef FX_05A_H
#define FX_05A_H

/* Effect module fx_05a (dir 13 file 0x5A). */
#include "btlfx_mod.h"

/* fx_05a effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_MIGHT_AURA_TEXTURE 0x7 /* layer table: render-to-texture (btlfx_drawLayerTexture): 6 layers into VRAM (0x1C0,0x1C0) 64x64, a blurred red-brown glow sampled by the light shafts of the Might pillar; runtime (docs/runtime-findings.md effect-data pass, medium) */
#define FX_MIGHT_LIGHT_TRAIL 0x8 /* emitter: screen trail with a pale crystal strip + sparks (texture not in the file) */
#define FX_MIGHT_SWIRL       0x9 /* emitter: blue-white swirl particles + fan */
#define FX_MIGHT_STAR        0xA /* model: four-pointed star, plain white texture */

/* fx_05a choreography (docs/fxdata.md, "Choreography literals"): op80's keyframes, flash and length, the star's size and texture. */
#define FX_MIGHT_STAR_FRAME      0x19 /* op80: FX_MIGHT_SWIRL starts, the spinning star and FX_MIGHT_AURA_TEXTURE are drawn from here */
#define FX_MIGHT_FLASH_FRAMES    8    /* from STAR_FRAME: additive white flash fading from FLASH_LEVEL for this many frames */
#define FX_MIGHT_FLASH_LEVEL     0xFF
#define FX_MIGHT_FADE_FRAME      0x2D /* op80: the star darkens from here */
#define FX_MIGHT_FRAMES          0x3C /* op80: the task ends at this frame */
#define FX_MIGHT_MODEL_COUNT     1    /* btlfx_initEffModels slots: STAR */
#define FX_MIGHT_STAR_SCALE      0x155 /* FX_MIGHT_STAR's scale (0x1000 = 1); for enemies it grows with */
#define FX_MIGHT_BIG_HEIGHT      0x190 /* the target's height above this, */
#define FX_MIGHT_BIG_RADIUS      0x320 /* and its radius above this (a quarter of each excess), the swirl rising with it, */
#define FX_MIGHT_STAR_MAX_SCALE  0x400 /* up to this */
/* FX_MIGHT_STAR: 15-bit page (0x1C0, 0x100), v 0xC0 = VRAM (0x1C0, 0x1C0), where FX_MIGHT_AURA_TEXTURE renders. */
#define FX_MIGHT_STAR_TPAGE      0x117
#define FX_MIGHT_STAR_V          0xC0
#define FX_MIGHT_LIGHT_TRAIL_TRAIL_SLOTS  0xA /* FX_MIGHT_LIGHT_TRAIL's trail buffer fx_05a_trails: slots of */
#define FX_MIGHT_LIGHT_TRAIL_TRAIL_STRIDE 0xB0 /* this many bytes (Pool6CEntry trailSlots / trailStride) */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* target position */
    SVECTOR pos10; /* effect position (raised for big enemies) */
    SVECTOR rot18; /* model rotation */
    VECTOR pos20; /* model position */
    VECTOR scale30; /* model scale */
    s16 rgb40; /* model brightness */
    s16 tint42; /* screen tint */
    u16 model44; /* model */
} Fx05aWork; /* 0x48 */

extern Unk800F78A4 *fx_05a_env;
extern BtlModelBuf fx_05a_effModels[];
extern s32 fx_05a_trails[];

#endif
