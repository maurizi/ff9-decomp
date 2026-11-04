#ifndef FX_086_H
#define FX_086_H

/* Effect module fx_086 (dir 13 file 0x86): Meteor (party and enemy). Streak field (fxm_drawStreakField). Evidence:
 * [data] the party ability / item tables and enemy sequences that play this file id (tools/effect_labels.py;
 * docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_086 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_METEOR_STREAK_SHELL   0x4 /* model: bullet-shaped shell; the UV box reads a placeholder page (hand-written Japanese text), filled at run time (look pending) */
#define FX_METEOR_SPRITES        0x6 /* emitter: sprites, still render all black (look pending) */
#define FX_METEOR_BACKDROP       0x7 /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */
#define FX_METEOR_EXPLOSION      0x9 /* emitter: green and orange explosion frames */
#define FX_METEOR_CONE_SPRITES   0xB /* emitter: cone of sprites, still render all black (look pending) */
#define FX_METEOR_DUST_RING      0xC /* model: ring band with a pink-brown rocky dust texture */
#define FX_METEOR_CLOUDS         0xE /* sprite: grey cloud strip */
#define FX_METEOR_SKY_CYLINDER   0xF /* model: cylinder with a blue sky / cloud texture */
#define FX_METEOR_DARK_CLOUDS    0x10 /* sprite: dark cloud strip */
#define FX_METEOR_ROCK           0x11 /* model: rock with a cratered texture (the meteor) */
#define FX_METEOR_ORANGE_RING    0x12 /* rings: white-orange gouraud rings */
#define FX_METEOR_DEBRIS_SPRITES 0x13 /* sprites: 44 frames, still render all black (look pending) */
#define FX_METEOR_STREAK         0x18 /* model: long thin gouraud spike */
#define FX_METEOR_BOWL           0x19 /* model: gouraud hemisphere bowl */
#define FX_METEOR_RING_DISC      0x1A /* model: gouraud flat annulus */
#define FX_METEOR_FIRE_DOME      0x1B /* model: dome with a fire texture */

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
extern SVECTOR fx_086_spritePos[64];
static void fxm_drawTexCylinder(SVECTOR *pos, SVECTOR *rot, VECTOR *scale, s32 r0, s32 r1, s32 w0, s32 w1, s32 bright, s32 tpage,
                   s32 clut, s32 u, s32 v, s32 ang, s32 dy, s32 abr, s32 dang);

#endif
