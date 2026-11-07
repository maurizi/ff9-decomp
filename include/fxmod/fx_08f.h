#ifndef FX_08F_H
#define FX_08F_H

/* Effect module fx_08f (dir 13 file 0x8F): Meteor, alternate file. Streak field (fxm_drawStreakField). Evidence:
 * [data] the party ability / item tables and enemy sequences that play this file id (tools/effect_labels.py;
 * docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_08f effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_METEOR_FIRE_RING      0x1 /* rings: cream-red-blue-violet-orange gouraud rings */
#define FX_METEOR_RED_ROCK       0x4 /* model: the red pitted meteor rock (carried from fx_086 entry 0x4; not played) (byte-identical to fx_086 entry 0x4); runtime (docs/runtime-findings.md effect-data pass, medium) */
#define FX_METEOR_BACKDROP       0x7 /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) (byte-identical to fx_086 entry 0x7) */
#define FX_METEOR_EMBERS         0xB /* emitter: small amber sparks (carried from fx_086 entry 0xB; Meteor (alt) not played) (byte-identical to fx_086 entry 0xB); runtime (docs/runtime-findings.md effect-data pass, medium) */
#define FX_METEOR_DUST_RING      0xC /* model: ring band with a pink-brown rocky dust texture (byte-identical to fx_086 entry 0xC) */
#define FX_METEOR_CLOUDS         0xE /* sprite: grey cloud strip */
#define FX_METEOR_SKY_CYLINDER   0xF /* model: cylinder with a blue sky / cloud texture (byte-identical to fx_086 entry 0xF) */
#define FX_METEOR_DARK_CLOUDS    0x10 /* sprite: dark cloud strip */
#define FX_METEOR_ROCK           0x11 /* model: rock with a cratered texture (the meteor) (byte-identical to fx_086 entry 0x11) */
#define FX_METEOR_EMBERS_2       0x13 /* sprites: amber spark sprites of the impact (carried from fx_086 entry 0x13; not played) (byte-identical to fx_086 entry 0x13); runtime (docs/runtime-findings.md effect-data pass, medium) */
#define FX_METEOR_STREAK         0x17 /* model: long thin gouraud spike (byte-identical to fx_086 entry 0x18) */
#define FX_METEOR_BOWL           0x18 /* model: gouraud hemisphere bowl (byte-identical to fx_086 entry 0x19) */
#define FX_METEOR_RING_DISC      0x19 /* model: gouraud flat annulus (byte-identical to fx_086 entry 0x1a) */
#define FX_METEOR_FIRE_DOME      0x1A /* model: dome with a fire texture (byte-identical to fx_086 entry 0x1b) */
#define FX_METEOR_EXPLOSION      0x1B /* emitter: green and orange explosion frames */
#define FX_METEOR_SMOKE_TRAIL    0x1C /* trail: grey smoke strip */

/* fx_08f choreography (docs/fxdata.md, "Choreography literals"): op80's states and lengths, the model textures and
 * the sky scroll rect. */
#define FX_METEOR_STATE_SKY  1    /* op80: the sky cylinder and clouds, the meteor rock falling in from afar */
#define FX_METEOR_STATE_FALL 2    /* the meteor approaches and strikes: dust ring, explosion, embers */
#define FX_METEOR_STATE_FADE 3    /* the smoke trail drifts off, then the op ends */
#define FX_METEOR_SKY_FRAMES  0x31 /* FX_METEOR_STATE_SKY's length (and its (frame << N) / 49 ramps) */
#define FX_METEOR_FALL_FRAMES 0x18 /* FX_METEOR_STATE_FALL's length */
#define FX_METEOR_FADE_FRAMES 6    /* FX_METEOR_STATE_FADE's length (the (frame << 12) / N ramp) */
#define FX_METEOR_MODEL_COUNT 8    /* btlfx_initEffModels slots: ROCK, RED_ROCK, FIRE_DOME, DUST_RING, STREAK, BOWL, RING_DISC, SKY_CYLINDER */
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
extern Unk800F78A4 *fx_08f_env;
static void fxm_drawStreakField(u32 *ot, s32 n, s32 projH, s32 dz, void *rot, SVECTOR *pts, CVECTOR *rgb, s32 fade, s32 abr);
typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame */
    s16 unk8[6]; /* write-only: three screen points (0xA0,0x78), (0,0x78), (0xC8,0x78) */
    SVECTOR pos14; /* summon position */
    SVECTOR camPos1C;
    s32 model24; /* models */
    s32 model28;
    s32 model2C;
    s32 model30;
    s32 model34;
    s32 model38;
    s32 model3C;
    s32 model40;
    Pool6CEntry *emit44;
} Fx08fWork80; /* 0x48 */

extern BtlModelBuf fx_08f_effModels[];
extern SVECTOR fx_08f_streakPts[];
extern u8 fx_08f_trail[];
static void fxm_randDiscPoints(s32 n, s32 radius, SVECTOR *pts);
s32 fx_08f_op80(s32 mode, Fx08fWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

#endif
