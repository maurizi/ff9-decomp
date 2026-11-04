#ifndef FX_0FB_1_H
#define FX_0FB_1_H

/* Effect module fx_0fb_1 (dir 13 file 0x0FB): Madeen, full summon (load group 1). Evidence: btlfx_start and btlfx_loadFile get the same id for summons (docs/runtime-findings.md). */
#include "btlfx_mod.h"

/* fx_0fb_1 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s4.md; "(look pending)" = by kind, appearance not settled). */
#define FX_MADEEN_CLOUD_STRIP      0x1 /* sprite: wide grey-white cloud strip drawn scrolling across the screen (byte-identical to fx_0fb_0 entry 0x3) */
#define FX_MADEEN_BLUE_STREAKS     0x2 /* emitter: orbiting blue light streaks + blue wing-pattern particles */
#define FX_MADEEN_LOOP_RIBBON      0x3 /* model: figure-eight ribbon loop (its texture page 0x138 holds a handwritten placeholder in the file) */
#define FX_MADEEN_CRYSTAL_SPARKLES 0x5 /* emitter: blue-gold crystal sparkles + blue specks */
#define FX_MADEEN_TEAL_RINGS       0x6 /* rings: navy-teal gouraud rings */
#define FX_MADEEN_WHITE_GLOW       0x7 /* particles: white-violet glow */
#define FX_MADEEN_BLUE_RINGS       0x8 /* rings: white-blue gouraud rings */
#define FX_MADEEN_BLUE_GLOW        0xA /* particles: blue glowing dot (byte-identical to fx_0fb_0 entry 0x10) */
#define FX_MADEEN_LENS_FLARES      0xC /* sprite line: green / red / orange lens-flare rings */
#define FX_MADEEN_RAINBOW_RINGS    0xD /* rings: rings textured with a rainbow patch */
#define FX_MADEEN_BLUE_SPECKS      0xE /* emitter: small blue specks in a spiral */
#define FX_MADEEN_BLUE_RINGS_2     0xF /* rings: navy-blue gouraud rings */
#define FX_MADEEN_PALE_RINGS       0x10 /* rings: white / pale-cyan / grey gouraud rings, 8 frames */
#define FX_MADEEN_BACKDROP_3       0x11 /* table: backdrop layers (btlfx_drawBackdrop) */
#define FX_MADEEN_WHITE_RINGS      0x12 /* rings: white to grey fading gouraud rings, 8 frames */
#define FX_MADEEN_RAINBOW_ARCH     0x13 /* model: arch, rainbow-cloud texture */
#define FX_MADEEN_TRAIL            0x14 /* trail: strip texture not in the file (magenta in the render) (look pending) */
#define FX_MADEEN_VIOLET_GLOW      0x15 /* emitter: three fans, a horizontal violet-white glow */
#define FX_MADEEN_BLUE_RINGS_3     0x16 /* rings: white-blue gouraud rings, 16 frames */
#define FX_MADEEN_LENS_FLARES_2    0x17 /* sprite line: green / red / orange lens-flare rings */
#define FX_MADEEN_ROCK_BURST       0x18 /* emitter: teal-green rock debris + tan dust puffs */
#define FX_MADEEN_ROCK_BURST_2     0x19 /* emitter: teal-green rock debris + tan dust puffs */
#define FX_MADEEN_VIOLET_RINGS     0x1A /* rings: white / violet / indigo gouraud rings */
#define FX_MADEEN_Y_FRAME          0x23 /* model: three-armed frame, grey streak texture */
#define FX_MADEEN_PINK_CLOUD_BALL  0x82 /* summon data entry 2: textured model, ball with a dark pink cloud texture */
#define FX_MADEEN_LEAF_MOUND       0x87 /* summon data entry 7: textured model, low mound with a green foliage texture */
#define FX_MADEEN_RAINBOW_SPHERE   0x89 /* summon data entry 9: textured model, sphere wrapped in red-green-blue stripe (rainbow) bands */
#define FX_MADEEN_BLUE_SWIRL_DISC  0x8C /* summon data entry 12: textured model, disc with a blue swirl texture */
#define FX_MADEEN_RAINBOW_SPHERE_2 0x8F /* summon data entry 15: textured model, sphere wrapped in red-green-blue stripe (rainbow) bands */
#define FX_MADEEN_RAINBOW_BANDS    0x90 /* summon data entry 16: textured model, curved red-green-blue stripe (rainbow) bands */

typedef struct {
    s32 state0; /* state */
    SVECTOR pos4; /* unit home position */
    SVECTOR rotC; /* unit rotation */
    s16 spinX14; /* spin speed */
    s16 spinY16;
    s16 spinZ18;
    u8 pad1A[0x1C - 0x1A];
    SVECTOR rot1C; /* current rotation */
    s32 unitCount24; /* unit count */
    s32 lift28; /* lift height */
    s32 unit2C; /* unit */
} Fx0fb1Work82; /* 0x30 */

typedef struct {
    s32 vx, vy, vz;
} Fx0fb1Vec3;

typedef struct {
    Fx0fb1Vec3 scale0[8]; /* unit scales */
    SVECTOR homePos60[8]; /* unit home positions */
    SVECTOR liftPosA0[8]; /* lifted positions */
    SVECTOR rotE0[8]; /* unit rotations */
    SVECTOR spin120[8]; /* spin speeds */
    SVECTOR curRot160[8]; /* current rotations */
    s32 unitCount1A0; /* unit count */
    s32 liftJitter1A4;
} Fx0fb1Work81; /* 0x1A8 */

extern Unk800F78A4 *fx_0fb_1_env;
extern s32 fx_0fb_1_unitsRestored;
extern s32 fx_0fb_1_rockRgb; /* rock model brightness */

static void fxm_drawTexCylinder(SVECTOR *pos, SVECTOR *rot, VECTOR *scale, s32 radius, s32 bright, CVECTOR *rgb, s32 tpage, s32 clut, s32 u,
                   s32 v, s32 dang, s32 abr);
s32 fx_0fb_1_op81(s32 mode, Fx0fb1Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);
s32 fx_0fb_1_op82(s32 mode, Fx0fb1Work82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

typedef struct {
    s16 kind0; /* kind */
    s16 state2; /* state */
    s16 velX4; /* velocity */
    s16 velY6;
    s16 velZ8;
    s16 speedA; /* speed */
    SVECTOR posC; /* position */
    SVECTOR rot14; /* rotation */
    s16 spinX1C; /* spin */
    s16 spinY1E;
    Pool6CEntry *emit20;
    Pool6CEntry *emit24;
} Fx0fb1Rock; /* 0x28 */
/* Fx0fb1Rock.kind0 */
#define FX_0FB_1_ROCK_BIG 0 /* model4C; bursts into three shards near the model */
#define FX_0FB_1_ROCK_SMALL 1 /* model50 */
#define FX_0FB_1_ROCK_SHARD 2 /* model50, from a burst */
#define FX_0FB_1_ROCK_PASS 3 /* as BIG, but flies past and ends off screen */
#define FX_0FB_1_SPARK 4 /* particles, no model or spin */

typedef struct {
    u8 pad0[0x4C];
    s32 model4C; /* model */
    s32 model50; /* model */
    u8 pad54[0x5C - 0x54];
    s32 modelZ5C; /* pos54.vz of Fx0fb1Op80Work */
    u8 pad60[0x6C - 0x60];
    SVECTOR rot6C; /* model rotation (rot6C of Fx0fb1Op80Work) */
    u8 pad74[0xC0 - 0x74];
    s32 modelC0; /* model */
} Fx0fb1Work80;

extern s32 fx_0fb_1_rocksHeld;


typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    u8 pad8[4];
    SVECTOR centerC; /* position */
    SVECTOR pos14;
    Fx0fb1Vec3 summonPos1C; /* summon position */
    Fx0fb1Vec3 summonScale28; /* summon scale */
    SVECTOR rot34; /* summon rotation */
    SVECTOR camTarget3C; /* camera */
    u8 pad44[8];
    s32 model4C[2]; /* models */
    Fx0fb1Vec3 pos54; /* model position */
    Fx0fb1Vec3 scale60; /* model scale */
    SVECTOR rot6C; /* model rotation */
    Fx0fb1Vec3 pos74;
    Fx0fb1Vec3 scale80;
    SVECTOR rot8C;
    Fx0fb1Vec3 scale94;
    s32 modelA0; /* models */
    s32 modelA4;
    s32 modelA8;
    s32 modelAC;
    s32 modelB0;
    s32 modelB4;
    s32 modelB8;
    s32 modelBC;
    s32 modelC0;
    SVECTOR rotC4[4];
    Pool6CEntry *emitE4;
} Fx0fb1Op80Work; /* 0xE8 */

typedef struct {
    u8 pad0[4]; /* ring header (lastSlot0, head1, count2; btlfx_drawTrail) */
    SVECTOR pts4[0x40]; /* ring points (btlfx_pushTrailPoint; pad: width * 2) */
    u8 pad204[8];
} Fx0fb1Ring; /* 0x20C */

typedef struct {
    u8 pad0[0x38];
    BtlModelBuf *effModels38;
    u8 pad3C[0x54 - 0x3C];
    struct {
        u8 pad0[0x18C];
        BtlMotion *motion18C;
    } *summon54; /* g_btlfxLoader.summon */
    u8 pad58[8];
    s32 summonModel60;
} Fx0fb1Summon;

extern SVECTOR fx_0fb_1_origin;
extern u8 fx_0fb_1_gradient[];
extern s16 fx_0fb_1_modelBlocks[2];
extern s16 fx_0fb_1_rockOffsets[];
extern Fx0fb1Ring fx_0fb_1_strips[2];

#endif
