#ifndef FX_17D_6_H
#define FX_17D_6_H

/* Effect module fx_17d_6 (dir 13 file 0x17d): Ark, full summon (load group 6). Evidence: btlfx_start and btlfx_loadFile get the same id for summons (docs/runtime-findings.md). */
#include "btlfx_mod.h"

/* fx_17d_6 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_ARK_PLANET_LIMB   0x1 /* emitter: fan, the blue edge of a planet (Earth-like limb) fading out */
#define FX_ARK_GOLD_SPARKLES 0x2 /* emitter: gold square sparkles */
#define FX_ARK_FLAME_BURST   0x4 /* emitter: red-orange flame burst sprites (partly white: texture mix) */
#define FX_ARK_CLOUD_TRAIL   0x5 /* emitter: orbiting 3D trails with a grey cloud texture */
#define FX_ARK_ORANGE_RING   0x6 /* rings: white-orange-brown gouraud rings */
#define FX_ARK_RED_RING      0xA /* rings: red to dark red gouraud rings */
#define FX_ARK_PEACH_RING    0xB /* rings: white-peach-mauve gouraud rings */
#define FX_ARK_FAN_EMITTER_2 0xC /* emitter: two fans; the render shows small olive / orange garbled patches; runtime (docs/runtime-findings.md effect-data pass): not fetched within 4600 frames of a full Ark (look pending) */
#define FX_ARK_SPRITE_LINE   0xD /* sprite line: green textures, partly not resolved (almost empty in the render); runtime (docs/runtime-findings.md effect-data pass): not fetched within 4600 frames of a full Ark (look pending) */
#define FX_ARK_BRANCH_TRAIL  0x7 /* table: trail strip of op83's bolt branches (fxm_drawArcTrailBranch's file; look pending) */
#define FX_ARK_BOLT_TRAIL    0x8 /* table: trail strip of op83's lightning bolts (fxm_drawArcTrail's file; fxdata classifies it as camera; look pending) */
#define FX_ARK_GREY_RING     0xE /* emitter: grey-white gouraud ring */
#define FX_ARK_ORANGE_DISC 0x8B /* summon data: 'so' model, flat disc with an orange-brown speckle texture */
#define FX_ARK_GREY_SWIRL_DISC 0x8D /* summon data: 'so' model, flat disc with a grey swirl texture */

/* fx_17d_6 choreography (docs/fxdata.md, "Choreography literals"): op80's states (work->state0; the frame restarts at
 * each), lengths and keyframes, the trail buffer and scroll; op83's lightning bolt pool. */
#define FX_ARK_STATE_GLOW    0    /* ORANGE_DISC, GREY_SWIRL_DISC, ORANGE_RING; then FLAME_BURST */
#define FX_ARK_GLOW_FRAMES   0x42
#define FX_ARK_STATE_BURST   1    /* ORANGE / RED_RINGs, PLANET_LIMB, CLOUD_TRAIL, GREY_RING, a white flash */
#define FX_ARK_BURST_FRAMES  0x64
#define FX_ARK_LIMB_FRAME    0x18 /* STATE_BURST: PLANET_LIMB starts */
#define FX_ARK_CLOUD_TRAIL_FRAME 0x20 /* STATE_BURST: CLOUD_TRAIL starts */
#define FX_ARK_GREY_RING_FRAME   0x32 /* STATE_BURST: GREY_RING starts */
#define FX_ARK_FLASH_FRAME   0x54 /* STATE_BURST: the screen grid brightens from here */
#define FX_ARK_STATE_RINGS   2    /* PEACH_RING and SPRITE_LINE */
#define FX_ARK_RINGS_FRAMES  0x12
#define FX_ARK_STATE_FAN     3    /* FAN_EMITTER_2 and PEACH_RING; then op83's bolts may strike (fx_17d_6_boltState) */
#define FX_ARK_FAN_FRAMES    0xE
#define FX_ARK_STATE_DONE    4    /* (the op has ended) */
#define FX_ARK_MODEL_COUNT   3    /* btlfx_initEffModels slots */
#define FX_ARK_CLOUD_TRAIL_TRAIL_SLOTS  0x28 /* CLOUD_TRAIL's trail buffer: slots of */
#define FX_ARK_CLOUD_TRAIL_TRAIL_STRIDE 0xAC /* this many bytes (Pool6CEntry trailSlots / trailStride) */
/* STATE_GLOW / STATE_RINGS scroll (SRC_X, SRC_Y) back 6 texels a frame through this rect (not checked against the models) */
#define FX_ARK_SCROLL_VRAM_X 0x280
#define FX_ARK_SCROLL_VRAM_Y 0x100
#define FX_ARK_SCROLL_VRAM_W 0x20
#define FX_ARK_SCROLL_VRAM_H 0x80
#define FX_ARK_SCROLL_SRC_X  0x2A0
#define FX_ARK_SCROLL_SRC_Y  0x100
#define FX_ARK_BOLT_POOL     6    /* op83 pool items (Fx17d6Bolt) */
#define FX_ARK_BOLT_FRAMES   6    /* a bolt's life (it fades over it) */
#define FX_ARK_NOISE_ROWS    64   /* fx_17d_6_noise rows (three random offsets each) */

/* fx_17d_6 choreography, deep pass (docs/fxchoreo_deep_c.md): keys inside op80's states, op83's bolt cadence */
#define FX_ARK_GLOW_TINT_FRAMES   0x10 /* STATE_GLOW: an orange flash fades over these frames, */
#define FX_ARK_GLOW_FLASH_FRAMES  4    /* a white one over these */
#define FX_ARK_GLOW_MOTION_FRAME  0x1D /* STATE_GLOW: the summon motion is held at frame 0 until here */
#define FX_ARK_DISC_LAYERS        4    /* GLOW / RINGS: ORANGE_DISC drawn as nested, counter-spinning layers, */
#define FX_ARK_SWIRL_DISC_LAYER   2    /* this one GREY_SWIRL_DISC */
#define FX_ARK_SCROLL_SPEED       6    /* SCROLL texels a frame (backwards from 0x3E8) */
#define FX_ARK_GRID_SCALE         0x1080 /* GLOW / RINGS: the screen grid's scale */
#define FX_ARK_BURST_CAMERA_FRAMES 0x20 /* STATE_BURST: camera point 2 slides from bone 0 to bone 2 (a sine quarter) */
#define FX_ARK_MUZZLE_BONE        0x21 /* STATE_FAN: camera point 2 on this summon bone (fx_17d_4's beam muzzle) */
#define FX_ARK_BOLTS_ON           0    /* fx_17d_6_boltState: op83 spawns bolts */
#define FX_ARK_BOLTS_END          1    /* op83 clears its pool and ends (op80 has ended) */
#define FX_ARK_BOLTS_PAUSED       2    /* STATE_RINGS: no bolts */
#define FX_ARK_BOLT_INTERVAL      4    /* op83: a bolt every 4 frames until RUSH_FRAME, then every frame */
#define FX_ARK_BOLT_RUSH_FRAME    0x43
#define FX_ARK_BOLT_JITTER_FRAMES 3    /* a bolt's noise row moves for its first frames, then it holds still */
#define FX_ARK_BOLT_WIDTH         0xB  /* Fx17d6Bolt.width15 */
#define FX_ARK_BOLT_POINTS        17   /* points of a bolt's trail (Fx17d6Trail17); the branch leaves at a random point < 15, */
#define FX_ARK_BRANCH_POINTS      5    /* with this many (Fx17d6Trail5) */

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR points4[1];
} Fx17d6Trail;

extern Unk800F78A4 *fx_17d_6_env;

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR points4[FX_ARK_BOLT_POINTS];
} Fx17d6Trail17; /* 0x8C */

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR points4[FX_ARK_BRANCH_POINTS];
} Fx17d6Trail5; /* 0x2C */

typedef struct {
    SVECTOR to0; /* to */
    SVECTOR from8; /* from */
    u8 slot10; /* trail index */
    u8 kind11;
    u8 index12;
    u8 unk13; /* write-only (rand % 15) */
    u8 angle14; /* angle */
    u8 width15;
    u8 pad16[2];
    s32 *noise18; /* noise */
} Fx17d6Bolt;

extern Fx17d6Trail17 fx_17d_6_boltTrails[FX_ARK_BOLT_POOL];
extern Fx17d6Trail5 fx_17d_6_branchTrails[FX_ARK_BOLT_POOL];

typedef struct {
    s32 frame0; /* frame */
    u8 pad4[0xC];
    s16 *pool10; /* pool */
} Fx17d6Inst;

typedef struct {
    s32 *noise0; /* noise */
    s32 count4; /* spawn count */
    s32 kind8; /* always 0: the file 0x17D script runs group 6's op 0x83 only as `83 00 00` */
} Fx17d6Work83;

extern SVECTOR fx_17d_6_boltTo; /* to */
extern SVECTOR fx_17d_6_boltFrom; /* from */
extern s32 fx_17d_6_boltState; /* state */
extern s32 fx_17d_6_noise[FX_ARK_NOISE_ROWS][3]; /* noise */

static s32 fx_17d_6_op83PoolTask(s32 mode, Fx17d6Bolt *bolt, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);

static void fxm_drawArcTrail(Unk80100C78Entry *entry, s32 file, s32 mode, Fx17d6Trail *trail, s32 size, s32 *noise, SVECTOR *from,
                   SVECTOR *to, s32 amp0, s32 amp1, s32 ang, s32 dang, s32 frame, s32 fade, s32 lineWidth, s32 dip);

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx17d6Vec3;

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    SVECTOR pos8; /* bolt target */
    SVECTOR pos10; /* particle position */
    SVECTOR pos18; /* camera position */
    SVECTOR pos20; /* camera target */
    SVECTOR pos28; /* bone positions */
    SVECTOR pos30;
    Fx17d6Vec3 pos38; /* model position */
    SVECTOR rot44; /* model rotation */
    Fx17d6Vec3 scale4C; /* model scale */
    Fx17d6Vec3 pos58; /* summon position */
    Fx17d6Vec3 scale64; /* summon scale */
    SVECTOR rot70; /* summon rotation */
    Pool6CEntry *emit78;
    s32 model7C; /* models */
    s32 model80;
} Fx17d6Work80; /* 0x84 */

/* the summon slot behind env->unk4 */
typedef struct {
    u8 pad0[0x54];
    struct {
        u8 pad0[0x1A8];
        BtlMotion *motion1A8;
        BtlMotion *motion1AC;
    } *summon54; /* g_btlfxLoader.summon */
    u8 pad58[8];
    s32 model60; /* summon model */
} Fx17d6Summon;

extern SVECTOR fx_17d_6_basePos;
extern u8 fx_17d_6_bones[];
extern BtlModelBuf fx_17d_6_effModels[];
extern s32 fx_17d_6_op80Trails[];
extern SVECTOR fx_17d_6_boltTo;
extern SVECTOR fx_17d_6_boltFrom;

#define SUMMON(env) ((Fx17d6Summon *)(env)->loader)

s32 fx_17d_6_op80(s32 mode, Fx17d6Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

#endif
