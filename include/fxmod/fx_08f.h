#ifndef FX_08F_H
#define FX_08F_H

/* Effect module fx_08f (dir 13 file 0x8F): Meteor, alternate file. Streak field (fxm_drawStreakField). Evidence:
 * [data] the party ability / item tables and enemy sequences that play this file id (tools/effect_labels.py;
 * docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_08f effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_METEOR_FIRE_RING      0x1 /* rings: cream-red-blue-violet-orange gouraud rings */
#define FX_METEOR_STREAK_SHELL   0x4 /* model: bullet-shaped shell (UV box reads a placeholder page) (byte-identical to fx_086 entry 0x4) (look pending) */
#define FX_METEOR_BACKDROP       0x7 /* table: backdrop layers (btlfx_drawBackdrop) (byte-identical to fx_086 entry 0x7) */
#define FX_METEOR_CONE_SPRITES   0xB /* emitter: cone of sprites, render all black (byte-identical to fx_086 entry 0xB) (look pending) */
#define FX_METEOR_RING_BAND      0xC /* model: ring band (texture not resolved) (byte-identical to fx_086 entry 0xC) (look pending) */
#define FX_METEOR_CLOUDS         0xE /* sprite: grey cloud strip */
#define FX_METEOR_SKY_CYLINDER   0xF /* model: cylinder with a blue sky / cloud texture (byte-identical to fx_086 entry 0xF) */
#define FX_METEOR_DARK_CLOUDS    0x10 /* sprite: dark cloud strip */
#define FX_METEOR_ROCK           0x11 /* model: rock with a cratered texture (the meteor) (byte-identical to fx_086 entry 0x11) */
#define FX_METEOR_DEBRIS_SPRITES 0x13 /* sprites: 44 frames, render all black (byte-identical to fx_086 entry 0x13) (look pending) */
#define FX_METEOR_STREAK         0x17 /* model: long thin gouraud spike (byte-identical to fx_086 entry 0x18) */
#define FX_METEOR_BOWL           0x18 /* model: gouraud hemisphere bowl (byte-identical to fx_086 entry 0x19) */
#define FX_METEOR_RING_DISC      0x19 /* model: gouraud flat annulus (byte-identical to fx_086 entry 0x1a) */
#define FX_METEOR_FIRE_DOME      0x1A /* model: dome with a fire texture (byte-identical to fx_086 entry 0x1b) */
#define FX_METEOR_EXPLOSION      0x1B /* emitter: green and orange explosion frames */
#define FX_METEOR_SMOKE_TRAIL    0x1C /* trail: grey smoke strip */

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
