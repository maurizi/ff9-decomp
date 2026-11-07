#ifndef FX_17D_5_H
#define FX_17D_5_H

/* Effect module fx_17d_5 (dir 13 file 0x17d): Ark, full summon (load group 5). Evidence: btlfx_start and btlfx_loadFile get the same id for summons (docs/runtime-findings.md). */
#include "btlfx_mod.h"

/* fx_17d_5 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_ARK_BACKDROP         0x1 /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */
#define FX_ARK_CREAM_RING       0x2 /* emitter: cream-white and white-grey-violet gouraud rings */
#define FX_ARK_LILAC_RING       0x3 /* emitter: dark noise particle + lilac-white gouraud ring */
#define FX_ARK_ORANGE_RING      0x4 /* rings: white-orange-brown gouraud rings (pool task) */
#define FX_ARK_TRAIL            0x5 /* trail: strip texture not resolved (noise and green in the render); runtime (docs/runtime-findings.md effect-data pass): its strip reads noise in the run-time page (look pending) */
#define FX_ARK_PLANET           0x8 /* particles: green-tinted planet with cloud swirls, fading */
#define FX_ARK_TRAIL_2          0x9 /* trail: strip texture not resolved (noise and green in the render); runtime (docs/runtime-findings.md effect-data pass): noise in the run-time page (yellow-white curling light trails on screen meanwhile) (look pending) */
#define FX_ARK_PLANET_FRAGMENTS 0xA /* emitter: sprites of planet-surface fragments */
#define FX_ARK_RED_FLASH_RING   0xB /* rings: red-white flat flash ring */
#define FX_ARK_LENS_FLARE       0xD /* sprite line: lens-flare set during the white blast; runtime (docs/runtime-findings.md effect-data pass) */

/* fx_17d_5 choreography (docs/fxdata.md, "Choreography literals"): op80's states (work->state0; the frame restarts at
 * each) and their lengths and keyframes, the pool items' states (Fx17d5Spark.state0) and lengths, the scroll. */
#define FX_ARK_STATE_GATHER  0    /* LENS_FLARE, CREAM_RING / LILAC_RING, homing trails spawn */
#define FX_ARK_GATHER_FRAMES 0x41
#define FX_ARK_LILAC_RING_FRAME 0x1B /* STATE_GATHER: LILAC_RING starts */
#define FX_ARK_CREAM_RING_FRAME 0x1F /* STATE_GATHER: CREAM_RING starts */
#define FX_ARK_TRAILS_FRAME  0x21 /* STATE_GATHER: 3 trails a frame from here, SPARK_POOL in all */
#define FX_ARK_STATE_HOLD    1
#define FX_ARK_HOLD_FRAMES   0x10
#define FX_ARK_STATE_HOMING  2    /* the trails home in on the target, RED_FLASH_RING */
#define FX_ARK_HOMING_FRAMES 0x20
#define FX_ARK_STATE_STRIKE  3    /* strikers (pool) hit the target, the scroll, a growing tint; the state restarts */
#define FX_ARK_STRIKE_FRAMES 0x48
#define FX_ARK_STRIKE_TINT_FRAME 0x38 /* STATE_STRIKE: the additive tint grows from here */
#define FX_ARK_SPARK_POOL    0x39 /* op80 pool items (Fx17d5Spark) */
#define FX_ARK_SPARK_TRAIL   0    /* Fx17d5Spark.state0: a homing trail */
#define FX_ARK_SPARK_STRIKE  1    /* a striker falling on the target, */
#define FX_ARK_SPARK_STRIKE_FRAMES 7
#define FX_ARK_SPARK_HIT     2    /* then a hit (PLANET, ORANGE_RING), */
#define FX_ARK_SPARK_HIT_FRAMES 24
#define FX_ARK_SPARK_FINAL_HIT 3  /* or the final hit (a darkening, bigger ring) */
#define FX_ARK_SPARK_FINAL_HIT_FRAMES 32
#define FX_ARK_MODEL_COUNT   5    /* btlfx_initEffModels slots */
/* STATE_STRIKE scrolls (SRC_X, SRC_Y) 29 texels a frame through this rect (not checked against the models) */
#define FX_ARK_STRIKE_VRAM_X 0x200
#define FX_ARK_STRIKE_VRAM_Y 0x100
#define FX_ARK_STRIKE_VRAM_W 0x40
#define FX_ARK_STRIKE_VRAM_H 0x80
#define FX_ARK_STRIKE_SRC_X  0x200
#define FX_ARK_STRIKE_SRC_Y  0x180

typedef struct {
    s32 vx, vy, vz;
} Fx17d5Vec3;

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame */
    s32 count8; /* sparks spawned */
    SVECTOR posC; /* spark origin */
    SVECTOR target14; /* spark target */
    SVECTOR angle1C; /* spark angles */
    SVECTOR pos24;
    u8 pad2C[0x3C - 0x2C];
    SVECTOR pos3C; /* camera */
    Fx17d5Vec3 pos44; /* glow position */
    SVECTOR rot50; /* glow rotation */
    Fx17d5Vec3 scale58; /* glow scale */
    Fx17d5Vec3 pos64; /* summon position */
    Fx17d5Vec3 scale70; /* summon scale */
    SVECTOR rot7C; /* summon rotation */
    s32 model84; /* models */
    s32 model88;
    s32 model8C;
    s32 model90;
    s32 model94;
} Fx17d5Work80; /* 0x98 */

typedef struct {
    s16 state0; /* state */
    s16 speed2; /* speed */
    s16 turn4; /* turn rate */
    s16 index6; /* index */
    SVECTOR pos8; /* position */
    s16 angle10; /* angles (with angle12); also the kind (0-2) at spawn */
    s16 angle12;
    s16 flag14; /* homing done */
    s16 unk16; /* unused */
    u8 *trail18; /* trail */
    s16 pos1C; /* target */
    s16 targetY1E; /* target y (pos1C: target x) */
    s16 targetZ20; /* target z */
    s16 width22; /* trail width */
} Fx17d5Spark; /* 0x24 */
/* Fx17d5Spark.angle10 at spawn (state 1 on): how the spark hits */
#define FX_17D_5_HIT_BURST 0 /* emitter burst */
#define FX_17D_5_HIT_RING 1 /* rings and a random screen tint */
#define FX_17D_5_HIT_FINAL 2 /* the last one: wide trail, grey tint, state 3 */
#define FX_ARK_RIBBED_SPHERE 0x85 /* summon data: 'so' model, sphere with a dark ribbed texture */
#define FX_ARK_SCALE_BOWL 0x87 /* summon data: 'so' model, bowl with a red-orange scale texture */
#define FX_ARK_SCALE_DOME 0x88 /* summon data: 'so' model, hemisphere with a red-orange scale texture */
#define FX_ARK_YELLOW_RING 0x91 /* summon data: 'so' model, yellow-white gouraud ring */
#define FX_ARK_TAN_GLOW_DISC 0x9B /* summon data: 'so' model, shallow tan-white gouraud glow disc */

/* the summon slot behind env->unk4 */
typedef struct {
    u8 pad0[0x54];
    struct {
        u8 pad0[0x1A4];
        BtlMotion *motion1A4;
    } *summon54; /* g_btlfxLoader.summon */
    u8 pad58[8];
    s32 model60; /* summon model */
} Fx17d5Summon;

extern SVECTOR fx_17d_5_targetPos; /* aim point */
extern Unk800F78A4 *fx_17d_5_env;
extern BtlModelBuf fx_17d_5_effModels[];
extern u8 fx_17d_5_sparkTrails[][0xCC]; /* spark trails */

static s32 fx_17d_5_op80PoolTask(s32 mode, Fx17d5Spark *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, Unk800DA1B8 *pool,
                  Fx17d5Work80 *parent);

#endif
