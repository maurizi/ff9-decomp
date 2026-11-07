#ifndef FX_0B2_H
#define FX_0B2_H

/* Effect module fx_0b2 (dir 13 file 0xB2): Twister (Blue Magic; the enemy Twister / Whirlwind, Ark's among them).
 * Not Ark's summon, which is fx_17d (file 0x17D; short 0x1BF). Evidence: [data] the party ability / item tables and
 * enemy sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime
 * pass"). */
#include "btlfx_mod.h"

/* fx_0b2 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_TWISTER_VORTEX      0x1 /* particles: tan swirling vortex, drawn directly */
#define FX_TWISTER_BACKDROP    0x2 /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */
#define FX_TWISTER_WIND_STREAK 0x4 /* trail: violet-white wind streak (fxm_drawOffsetTrail) */
#define FX_TWISTER_DUST_CLOUDS 0x5 /* emitter: spiralling grey-blue dust clouds */

/* fx_0b2 choreography (docs/fxdata.md, "Choreography literals"): op80's states and their lengths, the three
 * funnels' textures (fx_0b2_drawFunnel) and the VRAM rects their scrolling textures fill, the wind streaks. */
#define FX_TWISTER_STATE_GROW  0 /* the funnels grow out of the ground */
#define FX_TWISTER_STATE_WIDEN 1 /* they widen; at the end FX_TWISTER_DUST_CLOUDS start */
#define FX_TWISTER_STATE_SPIN  2 /* full-size funnels spin and grow */
#define FX_TWISTER_STATE_FADE  3 /* they fade out */
#define FX_TWISTER_GROW_FRAMES  12   /* FX_TWISTER_STATE_GROW's length */
#define FX_TWISTER_WIDEN_FRAMES 12   /* FX_TWISTER_STATE_WIDEN's length */
#define FX_TWISTER_SPIN_FRAMES  0x20 /* FX_TWISTER_STATE_SPIN's length */
#define FX_TWISTER_FADE_FRAMES  0x10 /* FX_TWISTER_STATE_FADE's length (the op ends) */
/* 72: the funnels' orbit round the targets' centre spans the whole op */
#define FX_TWISTER_FRAMES (FX_TWISTER_GROW_FRAMES + FX_TWISTER_WIDEN_FRAMES + FX_TWISTER_SPIN_FRAMES + FX_TWISTER_FADE_FRAMES)
#define FX_TWISTER_FUNNEL_ROWS  7    /* fx_0b2_drawFunnel: rows of quads (the first and last gouraud POLY_GT4, the rest POLY_FT4) */
/* The funnels: CORE (16 segments, 15-bit page (0x280, 0x100)), INNER and OUTER (8 segments, 4-bit pages, OUTER 3/2
 * as wide). INNER and OUTER sample rects that btlfx_drawScrollMove fills every frame from the texture beside them
 * (SRC_X), scrolled: INNER (0x280, 0x100) = tpage 0x1A + v 0, OUTER (0x1C0, 0x170) = tpage 0x17 + v 0x70. */
#define FX_TWISTER_CORE_TPAGE  0x11A
#define FX_TWISTER_CORE_CLUT   0xFFFF /* (no CLUT) */
#define FX_TWISTER_CORE_U      0x10
#define FX_TWISTER_CORE_V      0x90
#define FX_TWISTER_INNER_TPAGE 0x1A   /* 4-bit page (0x280, 0x100) */
#define FX_TWISTER_INNER_CLUT  0x3D01 /* CLUT (0x10, 0xF4) */
#define FX_TWISTER_INNER_U     0
#define FX_TWISTER_INNER_V     0
#define FX_TWISTER_OUTER_TPAGE 0x17   /* 4-bit page (0x1C0, 0x100) */
#define FX_TWISTER_OUTER_CLUT  0x3D03 /* CLUT (0x30, 0xF4) */
#define FX_TWISTER_OUTER_U     0
#define FX_TWISTER_OUTER_V     0x70
#define FX_TWISTER_INNER_VRAM_X 0x280
#define FX_TWISTER_INNER_VRAM_Y 0x100
#define FX_TWISTER_INNER_VRAM_W 0x1C
#define FX_TWISTER_INNER_VRAM_H 0x70
#define FX_TWISTER_INNER_SRC_X  0x29C
#define FX_TWISTER_INNER_SRC_Y  0x100
#define FX_TWISTER_OUTER_VRAM_X 0x1C0
#define FX_TWISTER_OUTER_VRAM_Y 0x170
#define FX_TWISTER_OUTER_VRAM_W 0x1C
#define FX_TWISTER_OUTER_VRAM_H 0x70
#define FX_TWISTER_OUTER_SRC_X  0x1DC
#define FX_TWISTER_OUTER_SRC_Y  0x170
#define FX_TWISTER_STREAKS        3    /* FX_TWISTER_WIND_STREAK trails drawn each frame (fx_0b2_trails) */
#define FX_TWISTER_STREAK_POINTS  25   /* points of a streak (fxm_drawOffsetTrail) */

typedef struct {
    s32 state0; /* state 0-3 */
    s32 frame4; /* frame */
    SVECTOR center8; /* centre: env->targets->centerX */
    SVECTOR pos10; /* position */
    SVECTOR rot18;
} Fx0b2Work; /* 0x20 */

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR pts4[FX_TWISTER_STREAK_POINTS];
} Fx0b2Trail; /* 0xCC */

typedef struct {
    u8 r;
    u8 g;
    u8 b;
    u8 code;
} Fx0b2Rgbc; /* primitive colour word */

extern Unk800F78A4 *fx_0b2_env;
extern Fx0b2Trail fx_0b2_trails[FX_TWISTER_STREAKS];

static void fxm_drawOffsetTrail(Unk80100C78Entry *entry, Fx0b2Trail *trail, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1, s32 ang,
                   s32 dang, s32 frame, s32 fade, s32 lineWidth);
void fx_0b2_drawFunnel(SVECTOR *pos, SVECTOR *rot, VECTOR *scale, s32 radius0, s32 radius1, s32 bright, s32 tpage, s32 clut, s32 texU,
                   s32 texV, s32 drift, s32 driftAng, s32 driftStep, s32 twist, s32 abr, s32 segs);

#endif
