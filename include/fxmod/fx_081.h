#ifndef FX_081_H
#define FX_081_H

/* Effect module fx_081 (dir 13 file 0x81): LV4 Holy (party and enemy). Rings, two models and a sphere of
 * particles (fx_081_ringCounts). Evidence: [data] the party ability / item tables and enemy sequences that play
 * this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_081 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_LV4_HOLY_BLUE_RING       0x4 /* rings: white-cyan-blue gouraud rings */
#define FX_LV4_HOLY_LENS_FLARE 0x5 /* emitter: white star glints */
#define FX_LV4_HOLY_CYAN_RING       0x6 /* rings: white-cyan-blue gouraud rings, 8 segments */
#define FX_LV4_HOLY_LILAC_RING      0x9 /* rings: white-lilac gouraud rings */
#define FX_LV4_HOLY_RAINBOW_ARCH    0xA /* model: half-ring arch, rainbow cloud texture */
#define FX_LV4_HOLY_SPRITE_LINE     0xB /* sprite line; its frames read a page of unrelated sprites, filled at run time (look pending) */
#define FX_LV4_HOLY_BLUE_SPARKLE_DOTS 0xC /* particles: blue sparkle frames */
#define FX_LV4_HOLY_BLUE_STREAKS    0xF /* emitter: blue light streaks + sparkles */
#define FX_LV4_HOLY_BACKDROP        0x10 /* table: backdrop layers (btlfx_drawBackdrop) */
#define FX_LV4_HOLY_BLUE_SPARKLES 0x11 /* emitter: callback element + blue sparkle sprites (byte-identical to fx_057 entry 0x4) */
#define FX_LV4_HOLY_TRAIL           0x12 /* trail: the strip reads unrelated multicoloured VRAM, filled at run time (look pending) */
#define FX_LV4_HOLY_VIOLET_RAYS     0x13 /* emitter: two fans drawn as narrow violet-white dithered rays */
#define FX_LV4_HOLY_VIOLET_RAYS_2   0x14 /* emitter: two fans drawn as narrow violet-white dithered rays */
#define FX_LV4_HOLY_BLUE_SPARKLES_2 0x15 /* emitter: blue speck sprites */
#define FX_LV4_HOLY_VIOLET_RING     0x16 /* rings: white-grey-violet-blue gouraud rings */
#define FX_LV4_HOLY_GLOW_BAND 0x17 /* model: gouraud flat annulus */
#define FX_LV4_HOLY_DARK_BLUE_RING  0x19 /* emitter: dark blue gouraud rings */
#define FX_LV4_HOLY_BLUE_RAYS       0x1A /* emitter: two fans drawn as thin vertical blue light rays */
#define FX_LV4_HOLY_BLUE_SHIMMER 0x1C /* emitter: blue dot sprites + shimmering particles (byte-identical to fx_057 entry 0x9, same render) */
#define FX_LV4_HOLY_PRISM_BURST     0x1D /* emitter: prism-coloured particles + violet / cyan rings */
#define FX_LV4_HOLY_BLUE_RING_2     0x1F /* rings: blue gouraud rings */
#define FX_LV4_HOLY_BACKDROP_2      0x20 /* table: backdrop layers (btlfx_drawBackdrop) */
#define FX_LV4_HOLY_LIGHT_SPHERE 0x21 /* model: sphere, white texture */
#define FX_LV4_HOLY_BACKDROP_3      0x22 /* table: backdrop layers (btlfx_drawBackdrop) */
#define FX_LV4_HOLY_TEXTURED_RING 0x23 /* rings: textured rings; the UV box reads unrelated multicoloured VRAM, filled at run time (look pending) */
#define FX_LV4_HOLY_WHITE_VIOLET_RINGS 0x24 /* rings: white-blue-violet gouraud rings, 4 segments */
#define FX_LV4_HOLY_SPARK 0x25 /* particles: one small frame; reads grey stripes that look like unrelated VRAM (look pending) */
#define FX_LV4_HOLY_FLASH_RINGS     0x27 /* emitter: white-yellow and white-grey gouraud rings */

extern Unk800F78A4 *fx_081_env;
extern SVECTOR fx_081_camEndPos;
extern SVECTOR fx_081_leadPos;
extern Pool6CEntry *fx_081_emitter0;
extern Unk80100C78Entry *fx_081_task;
extern s32 fx_081_glow; /* glow level */
extern Pool6CEntry *fx_081_emitter1;
extern SVECTOR fx_081_bodyPos; /* caster body position */
extern BtlModelBuf fx_081_effModels[];
extern u8 fx_081_trails[];

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx081Vec3;

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame */
    s32 model8; /* model */
    s32 modelC; /* model */
    SVECTOR pos10;
    Fx081Vec3 modelPos18;
    SVECTOR pos24;
    Pool6CEntry *emit2C;
    Pool6CEntry *emit30;
    Pool6CEntry *emit34;
} Fx081Work80; /* 0x38 */

static void fx_081_op80ElemDraw(Pool30Entry *pt, Pool6CEntry *owner, SVECTOR *pos, s32 rot, s32 *scale, s32 alpha, s32 blend, s32 frame,
                   s32 n);

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame */
    s32 otZ8; /* priority base */
    SVECTOR posC; /* target position */
    SVECTOR pos14;
    Fx081Vec3 unk1C; /* write-only (copy of posC) */
    s32 model28; /* model */
} Fx081Work81; /* 0x2C */

extern s16 fx_081_ringCounts[10]; /* ring point counts */
s32 fx_081_op81(s32 mode, Fx081Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

#endif
