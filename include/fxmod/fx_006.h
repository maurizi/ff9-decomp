#ifndef FX_006_H
#define FX_006_H

/* Effect module fx_006 (dir 13 file 0x06). */
#include "btlfx_mod.h"

/* fx_006 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_BLIZZARD_ICE_FLARES     0x1 /* emitter: cyan ice glint / light-streak frames */
#define FX_BLIZZARD_ICE_REFLECTION 0x2 /* model: ice shell drawn with the scrolling reflection map */
#define FX_BLIZZARD_ICE_BLOCK      0x3 /* model: ice block (cracked-ice texture), morph start */
#define FX_BLIZZARD_ICE_CRACKING   0x4 /* model: the block breaking into pieces, morph middle */
#define FX_BLIZZARD_ICE_SHARDS     0x5 /* model: scattered ice shards, morph end */
#define FX_BLIZZARD_ICE_FLARES_2   0x6 /* emitter: cyan ice glints */
#define FX_BLIZZARD_FROST_RING     0xD /* emitter: lilac-blue gouraud rings */
#define FX_BLIZZARD_FREEZE_BURST   0xE /* emitter: white-pink rings + spark sprites as the ice forms */
#define FX_BLIZZARD_SHATTER_WAVE   0x10 /* emitter: blue ripple wave when the ice shatters */
#define FX_BLIZZARD_FROST_SWIRL    0x12 /* emitter: orbiting pale trail, cyan ice flecks and white dots */

/* fx_006 choreography (docs/fxdata.md, "Choreography literals"): op80's states, their lengths and keyframes,
 * and the VRAM rect of the reflection map. */
#define FX_BLIZZARD_STATE_SWIRL   0 /* FX_BLIZZARD_FROST_SWIRL starts */
#define FX_BLIZZARD_STATE_GROW    1 /* FX_BLIZZARD_ICE_BLOCK grows to full size */
#define FX_BLIZZARD_STATE_GLEAM   2 /* the block turns, FX_BLIZZARD_ICE_REFLECTION gleams over it */
#define FX_BLIZZARD_STATE_SHATTER 3 /* the block morphs to FX_BLIZZARD_ICE_CRACKING, then FX_BLIZZARD_ICE_SHARDS */
#define FX_BLIZZARD_SWIRL_FRAMES      8    /* FX_BLIZZARD_STATE_SWIRL's length */
#define FX_BLIZZARD_GROW_FRAMES       4    /* FX_BLIZZARD_STATE_GROW's length */
#define FX_BLIZZARD_GLEAM_FRAMES      0x10 /* FX_BLIZZARD_STATE_GLEAM's length; at its end the freeze burst, frost ring and flares start */
#define FX_BLIZZARD_SHATTER_FRAMES    0xC  /* FX_BLIZZARD_STATE_SHATTER's length (the op ends) */
#define FX_BLIZZARD_SHATTER_WAVE_FRAME 1   /* FX_BLIZZARD_STATE_SHATTER: FX_BLIZZARD_SHATTER_WAVE starts */
#define FX_BLIZZARD_CRACK_FRAMES      2    /* FX_BLIZZARD_STATE_SHATTER: ICE_BLOCK -> ICE_CRACKING morph frames, then */
#define FX_BLIZZARD_SHARDS_FRAMES     14   /* ICE_CRACKING -> ICE_SHARDS over this many (cut off by SHATTER_FRAMES) */
#define FX_BLIZZARD_ICE_SCALE         0xC00 /* FX_BLIZZARD_ICE_BLOCK's full scale (0x1000 = 1) */
#define FX_BLIZZARD_MODEL_COUNT       4    /* btlfx_initEffModels slots: ICE_BLOCK, ICE_CRACKING, ICE_SHARDS, ICE_REFLECTION */
#define FX_BLIZZARD_FROST_SWIRL_TRAIL_SLOTS  0xA  /* FX_BLIZZARD_FROST_SWIRL's trail buffer fx_006_trails: slots of */
#define FX_BLIZZARD_FROST_SWIRL_TRAIL_STRIDE 0x5C /* this many bytes (Pool6CEntry trailSlots / trailStride) */
#define FX_BLIZZARD_FREEZE_TINT_RGB   0, 0x20, 0x46 /* additive screen tint (blue) as FX_BLIZZARD_FREEZE_BURST starts */
/* FX_BLIZZARD_ICE_REFLECTION samples (tpage 0xB8 = 8-bit page (0x200, 0x100), u / v 0x80) the rect the file fills
 * with a "dummy" placeholder; each GLEAM frame btlfx_drawScrollMove copies the file's reflection texture from
 * (SRC_X, SRC_Y) into it, scrolled vertically. */
#define FX_BLIZZARD_REFLECTION_VRAM_X 0x240
#define FX_BLIZZARD_REFLECTION_VRAM_Y 0x180
#define FX_BLIZZARD_REFLECTION_VRAM_W 0x40
#define FX_BLIZZARD_REFLECTION_VRAM_H 0x80
#define FX_BLIZZARD_REFLECTION_SRC_X  0x240
#define FX_BLIZZARD_REFLECTION_SRC_Y  0x100

typedef struct {
    s32 state0; /* state */
    s32 model4; /* models */
    s32 model8;
    s32 modelC;
    s32 model10;
    SVECTOR pos14; /* target body position */
    SVECTOR pos1C; /* target position */
    s32 frame24; /* frame */
} Fx006Work; /* 0x28 */

extern Unk800F78A4 *fx_006_env;
extern BtlModelBuf fx_006_effModels[];
extern s32 fx_006_trails[];

#endif
