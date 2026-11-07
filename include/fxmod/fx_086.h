#ifndef FX_086_H
#define FX_086_H

/* Effect module fx_086 (dir 13 file 0x86): Meteor (party and enemy). Streak field (fxm_drawStreakField). Evidence:
 * [data] the party ability / item tables and enemy sequences that play this file id (tools/effect_labels.py;
 * docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_086 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_METEOR_RED_ROCK       0x4 /* model: bullet-shaped shell on a red, dark-pitted rock page at run time: the huge red meteor; runtime (docs/runtime-findings.md effect-data pass, medium) */
#define FX_METEOR_GLOWING_EMBERS 0x6 /* emitter: larger orange glowing embers in the explosion; runtime (docs/runtime-findings.md effect-data pass, medium) */
#define FX_METEOR_BACKDROP       0x7 /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */
#define FX_METEOR_EXPLOSION      0x9 /* emitter: green and orange explosion frames */
#define FX_METEOR_EMBERS         0xB /* emitter: small amber sparks spreading out round the falling meteor; runtime (docs/runtime-findings.md effect-data pass, medium) */
#define FX_METEOR_DUST_RING      0xC /* model: ring band with a pink-brown rocky dust texture */
#define FX_METEOR_CLOUDS         0xE /* sprite: grey cloud strip */
#define FX_METEOR_SKY_CYLINDER   0xF /* model: cylinder with a blue sky / cloud texture */
#define FX_METEOR_DARK_CLOUDS    0x10 /* sprite: dark cloud strip */
#define FX_METEOR_ROCK           0x11 /* model: rock with a cratered texture (the meteor) */
#define FX_METEOR_ORANGE_RING    0x12 /* rings: white-orange gouraud rings */
#define FX_METEOR_EMBERS_2       0x13 /* sprites: the same amber spark sprites during the impact; runtime (docs/runtime-findings.md effect-data pass, medium) */
#define FX_METEOR_STREAK         0x18 /* model: long thin gouraud spike */
#define FX_METEOR_BOWL           0x19 /* model: gouraud hemisphere bowl */
#define FX_METEOR_RING_DISC      0x1A /* model: gouraud flat annulus */
#define FX_METEOR_FIRE_DOME      0x1B /* model: dome with a fire texture */

/* fx_086 choreography (docs/fxdata.md, "Choreography literals"; fx_08f's names where they mean the same): op80's
 * states, their lengths and keyframes, screen tints, the model textures and scrolled texture rects, the streak
 * field and the flame sheath round the falling meteor. */
#define FX_METEOR_STATE_SKY    1 /* op80 (starts here): the sky cylinder and clouds, the meteor rock falling in from afar, streaks */
#define FX_METEOR_STATE_FALL   2 /* the meteor dives at the target inside its flame sheath, the camera follows */
#define FX_METEOR_STATE_IMPACT 3 /* the strike: streak flash, dust rings, fire dome, explosion and embers (the op ends) */
#define FX_METEOR_SKY_FRAMES   0x31 /* FX_METEOR_STATE_SKY's length (and its (frame << N) / 49 ramps); then the camera moves to the targets */
#define FX_METEOR_EMBERS_FRAME 2    /* STATE_SKY: FX_METEOR_EMBERS start round the meteor (stopped at the state's end) */
#define FX_METEOR_GLOW_FRAME   8    /* STATE_SKY: the rock starts to shake, FX_METEOR_RED_ROCK glows in over the rest of the state, BACKDROP */
#define FX_METEOR_STREAKS_FAST_FRAME 0x18 /* STATE_SKY: the streaks speed up */
#define FX_METEOR_RED_TINT_FRAME 25 /* STATE_SKY: an additive red tint and the screen grid ramp in until the state's end */
#define FX_METEOR_FALL_FRAMES  12   /* FX_METEOR_STATE_FALL's length; at its end STRIKE_FLASH and the ember sprite positions */
#define FX_METEOR_CAMERA_DIVE_FRAMES 10 /* STATE_FALL: the camera moves from the meteor to above the target */
#define FX_METEOR_FALL_GLOW_FRAME 7 /* STATE_FALL: an orange screen grid pulses until the state's end */
#define FX_METEOR_IMPACT_FRAMES 50  /* FX_METEOR_STATE_IMPACT's length (the op ends) */
#define FX_METEOR_SHAKE_FRAMES 20   /* STATE_IMPACT: the camera shakes, dying down */
#define FX_METEOR_STREAK_FRAMES 6   /* STATE_IMPACT: FX_METEOR_STREAK flashes and FX_METEOR_ORANGE_RING bursts */
#define FX_METEOR_IMPACT_GRID_FRAMES 8 /* STATE_IMPACT: grey screen grid */
#define FX_METEOR_EMBERS_2_FRAMES 12 /* STATE_IMPACT: FX_METEOR_EMBERS_2 sprites fade out */
#define FX_METEOR_EXPLOSION_FRAME 12 /* STATE_IMPACT: FX_METEOR_EXPLOSION starts */
#define FX_METEOR_GLOWING_EMBERS_FRAME 20 /* STATE_IMPACT: FX_METEOR_GLOWING_EMBERS start */
#define FX_METEOR_BLAST_FRAME  4    /* STATE_IMPACT: the blast (BLAST_DARKEN, dust rings, fire dome) starts; the times below count from here */
#define FX_METEOR_BLAST_FLASH_FRAMES 24 /* blast: additive yellow-white flash fading */
#define FX_METEOR_DUST_FRAMES  32   /* blast: FX_METEOR_RING_DISC, the two FX_METEOR_DUST_RINGs and FX_METEOR_BOWL rise and fade */
#define FX_METEOR_BLAST_HOLD_FRAMES 3 /* blast: the disc and the dome stay at full brightness */
#define FX_METEOR_FIRE_DOME_FRAMES 41 /* blast: FX_METEOR_FIRE_DOME (scrolled fire texture) and an ORANGE_RING pulse and fade */
#define FX_METEOR_STRIKE_FLASH_RGB 0xC8, 0xC8, 0x64 /* STATE_FALL's end: additive (yellow-white) */
#define FX_METEOR_IMPACT_TINT_RGB  0x64, 0x20, 0x20 /* STATE_IMPACT frame 0: additive (red) */
#define FX_METEOR_BLAST_DARKEN_RGB 0xFF, 0xFF, 0xFF /* the blast's first frame: subtractive (the screen goes black) */
#define FX_METEOR_STREAKS      0x180 /* STATE_SKY: streak lines (fx_086_streakPts) */
#define FX_METEOR_STREAK_RADIUS 0x400 /* their disc's radius round the view axis (fxm_randDiscPoints) */
#define FX_METEOR_STREAK_NONE  0x7F7F7F7F /* streak's last screen point: none yet (no line drawn) */
#define FX_METEOR_EMBER_SPRITES 64   /* fx_086_spritePos: ember sprite positions strung out from the target */
#define FX_METEOR_EMBERS_2_COUNT 20  /* STATE_IMPACT: every third of them is drawn */
#define FX_METEOR_MODEL_COUNT  8     /* btlfx_initEffModels slots: ROCK, RED_ROCK, FIRE_DOME, DUST_RING, STREAK, BOWL, RING_DISC, SKY_CYLINDER */
#define FX_METEOR_ROCK_TPAGE      0x9A   /* 8-bit page (0x280, 0x100) */
#define FX_METEOR_ROCK_CLUT       0x3E00 /* CLUT (0, 0xF8) */
#define FX_METEOR_RED_ROCK_TPAGE  0x139  /* 15-bit page (0x240, 0x100), abr 1 */
#define FX_METEOR_RED_ROCK_CLUT   0xFFFF /* none (15-bit page) */
#define FX_METEOR_RED_ROCK_V      0x80
#define FX_METEOR_FIRE_DOME_TPAGE 0xB9   /* 8-bit page (0x240, 0x100), abr 1 */
#define FX_METEOR_FIRE_DOME_CLUT  0x3D40 /* CLUT (0, 0xF5) */
#define FX_METEOR_DUST_RING_TPAGE 0x3C   /* 4-bit page (0x300, 0x100), abr 1 */
#define FX_METEOR_DUST_RING_CLUT  0x3D04 /* CLUT (0x40, 0xF4) */
#define FX_METEOR_DUST_RING_V     0x80
/* FX_METEOR_SKY_CYLINDER samples (SKY_TPAGE, u / v 0) the rect each frame's btlfx_drawScrollMove fills from
 * (SRC_X, SRC_Y), scrolled by 8 lines a frame */
#define FX_METEOR_SKY_TPAGE  0x97   /* 8-bit page (0x1C0, 0x100) */
#define FX_METEOR_SKY_CLUT   0x3D80 /* CLUT (0, 0xF6) */
#define FX_METEOR_SKY_VRAM_X 0x1C0
#define FX_METEOR_SKY_VRAM_Y 0x100
#define FX_METEOR_SKY_VRAM_W 0x40
#define FX_METEOR_SKY_VRAM_H 0x80
#define FX_METEOR_SKY_SRC_X  0x200
#define FX_METEOR_SKY_SRC_Y  0x100
/* FX_METEOR_FIRE_DOME samples (FIRE_DOME_TPAGE 8-bit page (0x240, 0x100), u / v 0) the rect the blast's
 * btlfx_drawScrollMove fills from (SRC_X, SRC_Y), scrolled by 8 lines a frame */
#define FX_METEOR_FIRE_VRAM_X 0x240
#define FX_METEOR_FIRE_VRAM_Y 0x100
#define FX_METEOR_FIRE_VRAM_W 0x40
#define FX_METEOR_FIRE_VRAM_H 0x80
#define FX_METEOR_FIRE_SRC_X  0x300
#define FX_METEOR_FIRE_SRC_Y  0x100
/* STATE_FALL: six flame sheaths (fxm_drawTexCylinder: a bulging tube of SHEATH_SEGMENTS quads, the end ones
 * gouraud-faded) spin round the meteor, three each way, textured from the dust ring's 4-bit page */
#define FX_METEOR_SHEATH_SEGMENTS 16    /* quads along a sheath: 2 gouraud ends + 14 flat */
#define FX_METEOR_SHEATH_TPAGE    0x1C   /* 4-bit page (0x300, 0x100) */
#define FX_METEOR_SHEATH_CLUT     0x3D05 /* CLUT (0x50, 0xF4) */
#define FX_METEOR_SHEATH_U        0x80
#define FX_METEOR_SHEATH_V        0x80
#define FX_METEOR_SHEATH_RADIUS   0x4B0  /* radius at the ends, */
#define FX_METEOR_SHEATH_RADIUS_MID 0x2BC /* narrowing to this at the middle */
#define FX_METEOR_SHEATH_WIDTH    0x300  /* band width at the ends (the middle's varies per sheath) */
#define FX_METEOR_SHEATH_LENGTH   0x2000 /* length along the meteor's axis */

typedef struct {
    u8 r;
    u8 g;
    u8 b;
    u8 code;
} Fx086Rgbc; /* primitive colour word */

extern Unk800F78A4 *fx_086_env;

static void fxm_randDiscPoints(s32 n, s32 radius, SVECTOR *pts);
#define gte_stszotz(r0) __asm__ volatile( \
    "mfc2\t$12, $19;" \
    "nop;" \
    "sra\t$12, $12, 2;" \
    "sw\t$12, 0(%0)" \
    : : "r"(r0) : "$12", "memory")
#define gte_ReadGeomScreen(r0) __asm__ volatile( \
    "cfc2\t$12, $26;" \
    "nop;" \
    "sw\t$12, 0(%0)" \
    : : "r"(r0) : "$12", "memory")
static void fxm_drawStreakField(u32 *ot, s32 n, s32 projH, s32 dz, void *rot, SVECTOR *pts, CVECTOR *rgb, s32 fade, s32 abr);

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* counter */
    s16 unk8[6]; /* write-only: three screen points (0xA0,0x78), (0,0x78), (0xC8,0x78) */
    SVECTOR pos14; /* position */
    SVECTOR pos1C; /* target position */
    s32 model24; /* models */
    s32 model28;
    s32 model2C;
    s32 model30;
    s32 model34;
    s32 model38;
    s32 model3C;
    s32 model40;
    Pool6CEntry *emit44;
} Fx086Work80; /* 0x48 */

extern BtlModelBuf fx_086_effModels[];
extern SVECTOR fx_086_streakPts[];
extern SVECTOR fx_086_spritePos[FX_METEOR_EMBER_SPRITES];
static void fxm_drawTexCylinder(SVECTOR *pos, SVECTOR *rot, VECTOR *scale, s32 r0, s32 r1, s32 w0, s32 w1, s32 bright, s32 tpage,
                   s32 clut, s32 u, s32 v, s32 ang, s32 dy, s32 abr, s32 dang);

#endif
