#ifndef FX_0FB_0_H
#define FX_0FB_0_H

/* Effect module fx_0fb_0 (dir 13 file 0x0FB): Madeen, full summon, and Terra Homing (load group 0). Ribbon trails
 * (fxm_drawRibbonTrail). Evidence: btlfx_start and btlfx_loadFile get the same id for summons
 * (docs/runtime-findings.md). [data] the party ability / item tables and enemy sequences that play this file id
 * (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0fb_0 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s4.md; "(look pending)" = by kind, appearance not settled). */
#define FX_MADEEN_SPECKS          0x1 /* emitter: white dot specks */
#define FX_MADEEN_ROCK_DUST       0x2 /* emitter: tan dust puff + teal-green rock debris */
#define FX_MADEEN_CLOUD_STRIP     0x3 /* sprite: wide grey-white cloud strip drawn scrolling across the screen (byte-identical to fx_0fb_1 entry 0x1) */
#define FX_MADEEN_BLUE_SPECKS     0x4 /* emitter: small blue sparkle specks */
#define FX_MADEEN_GOLD_LIGHT_BAND 0x5 /* emitter: fan, a horizontal gold light band */
#define FX_MADEEN_ROCKS           0x6 /* emitter: teal-green rock debris, four elements */
#define FX_MADEEN_BLUE_RING       0x8 /* emitter: white-blue gouraud ring */
#define FX_MADEEN_VIOLET_TRAIL    0x9 /* trail: violet speckled strip */
#define FX_MADEEN_BLUE_RINGS      0xA /* rings: white-blue gouraud rings, 16 frames */
#define FX_MADEEN_DUST_CLOUD      0xB /* emitter: tan dust puffs, 27 animated frames */
#define FX_MADEEN_ROCK_SPIRAL     0xC /* emitter: teal-green rock debris rising in a spiral */
#define FX_MADEEN_BACKDROP        0xD /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */
#define FX_MADEEN_BACKDROP_2      0xE /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */
#define FX_MADEEN_BLUE_RING_2     0xF /* emitter: white-blue gouraud ring */
#define FX_MADEEN_BLUE_GLOW       0x10 /* particles: blue glowing dot (byte-identical to fx_0fb_1 entry 0xa) */
#define FX_MADEEN_WHITE_RINGS     0x14 /* rings: white-blue gouraud rings, five rings */
#define FX_MADEEN_LIGHT_STREAKS   0x15 /* emitter: two fans, thin orange and violet light streaks */
#define FX_MADEEN_PINK_CLOUD_BALL 0x82 /* summon data entry 2: textured model, ball with a dark pink cloud texture */
#define FX_MADEEN_CLOUD_RING      0x84 /* summon data entry 4: textured model, ring band with a white cloud texture */
#define FX_MADEEN_LEAF_MOUND      0x87 /* summon data entry 7: textured model, low mound with a green foliage texture */
#define FX_MADEEN_RAINBOW_ARC     0x94 /* summon data entry 20: textured model, thin curved ribbon with a red-green-blue stripe (rainbow) texture */

/* fx_0fb_0 choreography (docs/fxdata.md, "Choreography literals"): op80's seven states (ground ripple, rocks, the
 * leaf mound, Madeen's appearance, charge, release, finale) and their lengths, the rock pool, the model textures,
 * op81's length. The keys inside the states are still numeric. Shared names with group 1 (fx_0fb_1). */
#define FX_MADEEN_STATE_RIPPLE      0     /* op80 state: the ground ripple grid and GOLD_LIGHT_BAND; the models register at its end */
#define FX_MADEEN_STATE_ROCKS       1     /* ripple and flash, ROCKS fly up (pool tasks) */
#define FX_MADEEN_STATE_MOUND       2     /* LEAF_MOUND rises */
#define FX_MADEEN_STATE_APPEAR      3     /* Madeen appears on the mound, ROCK_DUST, BLUE_SPECKS; the camera pulls back */
#define FX_MADEEN_STATE_CHARGE      4     /* VIOLET_TRAILs and BLUE_GLOW */
#define FX_MADEEN_STATE_RELEASE     5     /* WHITE_RINGS, LIGHT_STREAKS, BLUE_RING_2 */
#define FX_MADEEN_STATE_FINALE      6     /* BLUE_RINGS, the rainbow arc, the cloud models (op80 ends) */
#define FX_MADEEN_RIPPLE_FRAMES     30    /* RIPPLE lasts this many frames (the (frame << 12) / N sheet phase) */
#define FX_MADEEN_RIPPLE_FRAME      0xE   /* the ripple grid draws from this frame of RIPPLE */
#define FX_MADEEN_ROCKS_FRAMES      55    /* ROCKS */
#define FX_MADEEN_MOUND_FRAMES      40    /* MOUND */
#define FX_MADEEN_APPEAR_FRAMES     25    /* APPEAR */
#define FX_MADEEN_PULLBACK_FRAME    8     /* APPEAR: the camera pulls back from this frame to the end */
#define FX_MADEEN_CHARGE_FRAMES     44    /* CHARGE */
#define FX_MADEEN_GLOW_FRAME        0x26  /* CHARGE: the glow swells from this frame to the end */
#define FX_MADEEN_RELEASE_FRAMES    21    /* RELEASE */
#define FX_MADEEN_FINALE_FADE_FRAME 0x1C  /* FINALE: fades out from this frame */
#define FX_MADEEN_FINALE_FRAMES     0x20  /* FINALE's length (op80 ends) */
#define FX_MADEEN_ROCK_COUNT        0x32  /* op80's pool: rocks (Fx0fb0Rock) */
#define FX_MADEEN_MODEL_SLOTS       0x1F  /* btlfx_initEffModels slots */
#define FX_MADEEN_PINK_CLOUD_BALL_TPAGE 0x96   /* Hi_RegisterTexEffModel texture: 8-bit page (0x180, 0x100), abr 0 */
#define FX_MADEEN_PINK_CLOUD_BALL_CLUT  0x3D40 /* CLUT at (0x0, 0xF5) */
#define FX_MADEEN_LEAF_MOUND_TPAGE  0x9A   /* 8-bit page (0x280, 0x100), abr 0 */
#define FX_MADEEN_LEAF_MOUND_CLUT   0x3E00 /* CLUT at (0x0, 0xF8) */
#define FX_MADEEN_CLOUD_RING_TPAGE  0x7C   /* 4-bit page (0x300, 0x100), abr 3 */
#define FX_MADEEN_CLOUD_RING_CLUT   0x3D02 /* CLUT at (0x20, 0xF4) */
#define FX_MADEEN_RAINBOW_TPAGE     0xBD   /* RAINBOW_ARC: 8-bit page (0x340, 0x100), abr 1 */
#define FX_MADEEN_RAINBOW_CLUT      0x3E40 /* CLUT at (0x0, 0xF9) */
#define FX_MADEEN_MODEL_U           0      /* the models' texture origin (u, v) in their pages */
#define FX_MADEEN_MODEL_V           0
#define FX_MADEEN_BLESS_FRAMES      0x2E  /* op81's length */

typedef struct {
    s32 state0; /* state */
    SVECTOR homePos4[8]; /* unit home positions */
    SVECTOR rot44[8]; /* unit rotations */
    SVECTOR spin84[8]; /* spin speeds */
    SVECTOR curRotC4[8]; /* current rotations */
    s32 unitCount104; /* unit count */
    s32 lift108; /* lift height */
} Fx0fb0Work81; /* 0x10C */

typedef struct {
    s32 state0; /* state */
    s16 kind4; /* kind */
    s16 bob6; /* bob height, then phase */
    s32 frame8; /* frame */
    SVECTOR posC; /* position */
    SVECTOR rot14; /* rotation */
    u16 spinX1C; /* spin speed */
    u16 spinY1E;
    s32 model20; /* model */
    u8 unused24[4]; /* op80's pool asks for 0x28 per rock */
} Fx0fb0Rock; /* 0x28 */

extern s32 fx_0fb_0_rocksReleased;
extern s32 fx_0fb_0_unitsRestored;
extern Unk800F78A4 *fx_0fb_0_env;

static s32 fx_0fb_0_op80PoolTask(s32 mode, Fx0fb0Rock *r, Unk800F78A4 *env);
s32 fx_0fb_0_op81(s32 mode, Fx0fb0Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

typedef struct {
    SVECTOR a0;
    SVECTOR b8;
} Fx0fb0Seg; /* 0x10 */
static void fxm_drawRibbonTrail(SVECTOR *posA, SVECTOR *posB, u32 n, CVECTOR *colorA, CVECTOR *colorB, s32 fade, Fx0fb0Seg *buf, s32 abr);

typedef struct {
    s32 vx, vy, vz;
} Fx0fbVec3;

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    s32 rockCount8; /* rocks spawned */
    SVECTOR basePosC; /* base position */
    SVECTOR pos14;
    SVECTOR pos1C;
    SVECTOR pos24;
    Fx0fbVec3 summonPos2C; /* model position */
    Fx0fbVec3 summonScale38; /* model scale */
    SVECTOR rot44; /* model rotation */
    SVECTOR pos4C;
    SVECTOR pos54;
    SVECTOR pos5C; /* summon position */
    SVECTOR camPos64;
    s32 model6C[27]; /* rock models */
    Fx0fbVec3 posD8;
    Fx0fbVec3 scaleE4;
    SVECTOR rotF0;
    Fx0fbVec3 posF8;
    Fx0fbVec3 scale104;
    SVECTOR rot110;
    s32 model118; /* models */
    s32 model11C;
    s32 model120;
    s32 model124;
} Fx0fb0Work80; /* 0x128 */

typedef struct {
    u8 data[0x84];
} Fx0fb0Trail16; /* 4 + 16 points */

typedef struct {
    Fx0fb0Seg seg[12];
} Fx0fb0SegBuf; /* 0xC0 */

typedef struct {
    s16 x;
    s16 z;
} Fx0fb0Ofs;

typedef struct {
    u8 pad0[0x38];
    BtlModelBuf *effModels38;
    u8 pad3C[0x18];
    struct {
        u8 pad0[0x180];
        BtlMotion *motion180[8]; /* summon motions */
    } *summon54; /* g_btlfxLoader.summon */
    u8 pad58[8];
    s32 summonModel60; /* summon model */
} Fx0fb0Summon;

#define FX0FB_W(v, i) (((s32 *)&(v))[i])
#define SUMMON0FB(env) ((Fx0fb0Summon *)(env)->loader)

void fx_0fb_0_drawWaveSheet(SVECTOR *pos, SVECTOR *rot, VECTOR *scale, s32 col, s32 tpage, s32 clut, s32 u, s32 v, s32 amp,
                   s32 ph, s32 step, s32 abr);
void fx_0fb_0_drawWaveCurtain(SVECTOR *pos, s32 dy, s32 c, s32 tpage, s32 clut, s32 u, s32 v, s32 amp, s32 ph, s32 step,
                   s32 abr, s32 ph2);
extern SVECTOR fx_0fb_0_origin;
extern s16 fx_0fb_0_ribbonBones[4][2];
extern s16 fx_0fb_0_trailBones[10];
extern u16 fx_0fb_0_rockBlocks[27];
extern s16 fx_0fb_0_rockOrder[25];
extern s16 fx_0fb_0_rockOffsets[];
extern CVECTOR fx_0fb_0_rippleRgb[];
extern CVECTOR fx_0fb_0_ribbonRgb[];
extern SVECTOR fx_0fb_0_bonePts[10];
extern SVECTOR fx_0fb_0_bonePts1[];
extern Fx0fb0SegBuf fx_0fb_0_ribbons[4];
extern Fx0fb0Trail16 fx_0fb_0_strips[10];

#endif
