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
#define FX_MADEEN_BACKDROP_3       0x11 /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */
#define FX_MADEEN_WHITE_RINGS      0x12 /* rings: white to grey fading gouraud rings, 8 frames */
#define FX_MADEEN_RAINBOW_ARCH     0x13 /* model: arch, rainbow-cloud texture */
#define FX_MADEEN_TRAIL            0x14 /* trail: strip texture not in the file (magenta in the render); runtime (docs/runtime-findings.md effect-data pass): fetched as the blue-white blast starts; its strip reads only sparse dots in the run-time page (look pending) */
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

/* fx_0fb_1 choreography (docs/fxdata.md, "Choreography literals"): the mechanical names (op sizes, the rock pool,
 * model slots and textures, GPU words; op80's seven states and their keys: deep pass below). Shared names with
 * group 0 (fx_0fb_0), own values. */
#define FX_MADEEN_ROCK_COUNT        0x30  /* op80's pool: rocks (Fx0fb1Rock) */
#define FX_MADEEN_MODEL_SLOTS       0x1F  /* btlfx_initEffModels slots */
#define FX_MADEEN_PINK_CLOUD_BALL_TPAGE 0x96   /* Hi_RegisterTexEffModel texture: 8-bit page (0x180, 0x100), abr 0 */
#define FX_MADEEN_PINK_CLOUD_BALL_CLUT  0x3D40 /* CLUT at (0x0, 0xF5) */
#define FX_MADEEN_LEAF_MOUND_TPAGE  0x9A   /* 8-bit page (0x280, 0x100), abr 0 */
#define FX_MADEEN_LEAF_MOUND_CLUT   0x3E00 /* CLUT at (0x0, 0xF8) */
#define FX_MADEEN_RAINBOW_TPAGE     0xBD   /* RAINBOW_BANDS, RAINBOW_SPHERE(_2), RAINBOW_ARCH: 8-bit page (0x340, 0x100), abr 1 */
#define FX_MADEEN_RAINBOW_CLUT      0x3E40 /* CLUT at (0x0, 0xF9) */
#define FX_MADEEN_RAINBOW_ARCH_CLUT 0x3EC0 /* RAINBOW_ARCH's CLUT at (0x0, 0xFB) */
#define FX_MADEEN_Y_FRAME_TPAGE     0x3D   /* 4-bit page (0x340, 0x100), abr 1 */
#define FX_MADEEN_Y_FRAME_CLUT      0x3E81 /* CLUT at (0x10, 0xFA) */
#define FX_MADEEN_LOOP_RIBBON_TPAGE 0x138  /* 15-bit page (0x200, 0x100), abr 1 */
#define FX_MADEEN_LOOP_RIBBON_CLUT  0xFFFF /* no CLUT */
#define FX_MADEEN_BLUE_SWIRL_DISC_TPAGE 0x3A   /* 4-bit page (0x280, 0x100), abr 1 */
#define FX_MADEEN_BLUE_SWIRL_DISC_CLUT  0x3D0A /* CLUT at (0xA0, 0xF4) */
#define FX_MADEEN_MODEL_U           0      /* the models' texture origin (u, v) in their pages */
#define FX_MADEEN_MODEL_V           0
#define FX_MADEEN_MODEL_V2          0x80   /* v of the second half (RAINBOW_ARCH, Y_FRAME, LOOP_RIBBON, BLUE_SWIRL_DISC) */
#define FX_MADEEN_RETURN_FRAMES     0x10  /* op82: the units turn back home after this many frames */

/* fx_0fb_1 deep pass: op80's states (Madeen's second half: the rainbow bands, the sphere, the arches, the beam),
 * their keys, the rock tasks and the cylinder helper. */
#define FX_MADEEN_STATE_BANDS       0     /* op80 state: RAINBOW_BANDS fly at the camera, rocks stream past (one a frame) */
#define FX_MADEEN_STATE_MORPH       1     /* the bands morph into RAINBOW_SPHERE_2, the screen brightens; the held rocks and enemies dim */
#define FX_MADEEN_STATE_APPEAR      2     /* Madeen on LEAF_MOUND, RAINBOW_SPHERE shrinking, BLUE_RINGS */
#define FX_MADEEN_STATE_ARCHES      3     /* Madeen fades, the sphere shrinks away, RAINBOW_ARCHes open, VIOLET_RINGS, LENS_FLARES */
#define FX_MADEEN_STATE_BURST       4     /* the camera drops to the target, rings burst; white, then darkened */
#define FX_MADEEN_STATE_BEAM        5     /* the rainbow beam cylinder, Y_FRAME, LOOP_RIBBON, the glow ring and trails, sparks */
#define FX_MADEEN_STATE_END         6     /* BLUE_RINGS and the white tint fade (op80 ends) */
#define FX_MADEEN_BANDS_FRAMES      0x1C
#define FX_MADEEN_MORPH_FRAMES      22
#define FX_MADEEN_MORPH_FRAME       5     /* MORPH: the morph to RAINBOW_SPHERE_2 starts (a quarter sine to the end) */
#define FX_MADEEN_MORPH_FLASH_FRAME 0x13  /* MORPH: the white flash grows from here (4-frame ramp) */
#define FX_MADEEN_APPEAR_FRAMES     24
#define FX_MADEEN_APPEAR_SHOW_FRAME 4     /* APPEAR: Madeen and the mound drawn from here; the flash fades before it (4-frame ramp) */
#define FX_MADEEN_ARCHES_FRAMES     37
#define FX_MADEEN_SHRINK_FRAME      0xF   /* ARCHES: RAINBOW_SPHERE shrinks from here, */
#define FX_MADEEN_SHRINK_FRAMES     8     /* over this many frames; BLUE_RINGS_2 fades over RINGS_2_FRAMES from the same frame */
#define FX_MADEEN_RINGS_2_FRAMES    6
#define FX_MADEEN_VIOLET_RINGS_FRAME 0x11 /* ARCHES: VIOLET_RINGS swell from here to the end */
#define FX_MADEEN_ARCHES_FRAME      0x15  /* ARCHES: the RAINBOW_ARCHes grow and LENS_FLARES run from here to the end */
#define FX_MADEEN_ARCHES            4     /* RAINBOW_ARCH copies (rotC4) */
#define FX_MADEEN_VIOLET_GLOW_FRAME 0x18  /* ARCHES: VIOLET_GLOW starts */
#define FX_MADEEN_WHITE_OUT_FRAME   0x22  /* ARCHES: the screen whitens from here to the end */
#define FX_MADEEN_BURST_FRAMES      10
#define FX_MADEEN_SPHERE_FADE_FRAMES 0x10 /* BURST: RAINBOW_SPHERE's last flash fades over this many frames (cut short by the state's end) */
#define FX_MADEEN_BURST_FADE_FRAMES 6     /* BURST: the white fades, */
#define FX_MADEEN_DARKEN_FRAME      7     /* then the screen darkens from here to the end */
#define FX_MADEEN_BEAM_FRAMES       0x40
#define FX_MADEEN_BEAM_FLASH_FRAMES 6     /* BEAM: violet flash at frame 0, darken at 1, the blue tint fades to this frame */
#define FX_MADEEN_SPIN_FRAMES       0x20  /* BEAM: RAINBOW_RINGS and Y_FRAME drawn (fading) until this frame */
#define FX_MADEEN_ORBIT_GROW_FRAMES 0x18  /* BEAM: the glow ring and the trails widen, LENS_FLARES_2 fade, until this frame */
#define FX_MADEEN_GLOW_RING_COUNT   0x80  /* BEAM: WHITE_GLOW particles around the beam */
#define FX_MADEEN_RING_TRAILS       2     /* BEAM: TRAIL strips orbiting the beam (fx_0fb_1_strips) */
#define FX_MADEEN_RING_TRAIL_POINTS 0x40  /* points per strip (Fx0fb1Ring) */
#define FX_MADEEN_SPARKS_FRAME      8     /* BEAM: sparks (FX_0FB_1_SPARK tasks) spawn from here, */
#define FX_MADEEN_SPARKS_PER_FRAME  3
#define FX_MADEEN_END_GLOW_FRAME    0x38  /* BEAM: VIOLET_GLOW starts; the screen grid zooms after it */
#define FX_MADEEN_END_FRAMES        0x10
#define FX_MADEEN_HELD_ROCKS        0x19  /* MORPH: rocks placed around the sphere (fx_0fb_1_rockOffsets), every other one big */
#define FX_MADEEN_BIG_ROCK_SCALE    0x199 /* rock task: BIG / PASS model scale */
#define FX_MADEEN_SMALL_ROCK_SCALE  0xCC  /* SMALL / SHARD model scale */
#define FX_MADEEN_ROCK_STATE_FLY    0     /* rock task: flies (unless held) until it passes the sphere */
#define FX_MADEEN_ROCK_STATE_BURST  1     /* bursts: ROCK_BURST(_2), BLUE_SPECKS, rings and a BLUE_SWIRL_DISC flash */
#define FX_MADEEN_SHARDS            3     /* a BIG rock's burst: shards, spread evenly around */
#define FX_MADEEN_BIG_RINGS_FRAMES  10    /* burst: BLUE_RINGS_3 of a BIG rock */
#define FX_MADEEN_SMALL_RINGS_FRAMES 6    /* of a SMALL rock */
#define FX_MADEEN_PALE_RINGS_FRAMES 3     /* burst: PALE_RINGS */
#define FX_MADEEN_SWIRL_FLASH_FRAMES 8    /* burst: the BLUE_SWIRL_DISC flash */
#define FX_MADEEN_SHARD_FRAMES      0xD   /* shard task's length */
#define FX_MADEEN_SPARK_FRAMES      0x11  /* spark task's length */
#define FX_MADEEN_BEAM_TPAGE        0x1B   /* fxm_drawTexCylinder texture of the beam: 4-bit page (0x2C0, 0x100), abr 0 */
#define FX_MADEEN_BEAM_CLUT         0x3E80 /* CLUT at (0x0, 0xFA) */
#define FX_MADEEN_BEAM_U            0
#define FX_MADEEN_BEAM_V            0
#define FX_MADEEN_BEAM_RADIUS       0x1C00
#define FX_MADEEN_BEAM_VRAM_X       0x2C0 /* btlfx_drawScrollMove rect: the beam page's 0x80 x 0x80 4-bit texels */
#define FX_MADEEN_BEAM_VRAM_Y       0x100
#define FX_MADEEN_BEAM_VRAM_W       0x20
#define FX_MADEEN_BEAM_VRAM_H       0x80
#define FX_MADEEN_BEAM_SRC_X        0x360 /* from this source */
#define FX_MADEEN_BEAM_SRC_Y        0x100
#define FX_MADEEN_CYL_ROWS          20    /* fxm_drawTexCylinder: quad rings along the axis */
#define FX_MADEEN_CYL_COLS          16    /* quads per ring */
#define FX_MADEEN_DROP_STATE_FALL   0     /* op82: the unit bobs and spins, then is put back home */
#define FX_MADEEN_DROP_STATE_LAND   1     /* WHITE_RINGS at its feet */
#define FX_MADEEN_LAND_RING_FRAMES  7

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
    SVECTOR rotC4[FX_MADEEN_ARCHES];
    Pool6CEntry *emitE4;
} Fx0fb1Op80Work; /* 0xE8 */

typedef struct {
    u8 pad0[4]; /* ring header (lastSlot0, head1, count2; btlfx_drawTrail) */
    SVECTOR pts4[FX_MADEEN_RING_TRAIL_POINTS]; /* ring points (btlfx_pushTrailPoint; pad: width * 2) */
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
extern Fx0fb1Ring fx_0fb_1_strips[FX_MADEEN_RING_TRAILS];

#endif
