#ifndef FX_05E_H
#define FX_05E_H

/* Effect module fx_05e (dir 13 file 0x5E): Death (party and enemy). Ribbon trails (fxm_drawRibbonTrail). Evidence:
 * [data] the party ability / item tables and enemy sequences that play this file id (tools/effect_labels.py;
 * docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_05e effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_DEATH_MODEL_DATA 0x2 /* table: texture block passed with the summon model (btlfx_registerSummonModel) */
#define FX_DEATH_GREEN_ORB       0x4 /* emitter: fan + pale green orb sprites, white-salmon-violet gouraud rings */
#define FX_DEATH_RED_RINGS       0x5 /* emitter: pink spark sprites + white-red gouraud rings */
#define FX_DEATH_REAPER          0x6 /* summon model (btlfx_registerSummonModel): the robed skull reaper holding a scythe */
#define FX_DEATH_SOUL_ORBS       0x9 /* emitter: blue glowing orb quads */
#define FX_DEATH_SMOKE_TEXTURE   0xB /* layer table: render-to-texture (btlfx_drawLayerTexture): 2 layers into VRAM (0x200,0x1A0) 64x64, rising purple smoke; runtime (docs/runtime-findings.md effect-data pass, medium) */
#define FX_DEATH_BLUE_TRAIL      0xC /* emitter: spiralling trail with a blue flame strip */
#define FX_DEATH_SMOKE           0x10 /* emitter: grey smoke puffs */
#define FX_DEATH_BLUE_TRAIL_2    0x1B /* emitter: spiralling trail with a blue flame strip */
#define FX_DEATH_REAPER_2        0x1C /* 'so' model: the robed skull reaper with the scythe (same textures, other mesh), registered in op80 */

/* fx_05e choreography (docs/fxdata.md, "Choreography literals"): op80's states (the reaper rises, poses, swings, fades), keyframes,
 * tints, the scythe ribbons, trail buffers; the ribbon helper's GPU words. */
#define FX_DEATH_STATE_RISE   0    /* op80: FX_DEATH_REAPER_2 rises out of the ground (slice model), soul orbs and blue trail start */
#define FX_DEATH_RISE_FRAMES  0x18
#define FX_DEATH_STATE_POSE   1    /* the summon model on its motion's frame 0 */
#define FX_DEATH_POSE_FRAMES  8
#define FX_DEATH_STATE_SWING  2    /* the swing: the scythe ribbons, the slash, darken, red flash, smoke */
#define FX_DEATH_SWING_FRAMES 0x29
#define FX_DEATH_RIBBON_FRAMES 24  /* FX_DEATH_STATE_SWING: the scythe ribbons fade in over this many frames */
#define FX_DEATH_SLASH_FRAME  0x1F /* FX_DEATH_STATE_SWING: the slash emitter (fx_05e_facingBlocks, by facing) starts at the target */
#define FX_DEATH_DARKEN_FRAME 0x20 /* the screen is darkened (FX_DEATH_DARKEN_RGB) */
#define FX_DEATH_HIT_FRAME    0x21 /* red flash, FX_DEATH_GREEN_ORB and RED_RINGS start at the target */
#define FX_DEATH_TRAIL_FRAME  0x26 /* FX_DEATH_BLUE_TRAIL and FX_DEATH_SMOKE start */
#define FX_DEATH_STATE_FADE   3    /* the reaper narrows and fades, its motion played back from SWING_FRAMES; the op ends */
#define FX_DEATH_FADE_FRAMES  8
#define FX_DEATH_DARKEN_RGB   0xFF, 0xFF, 0 /* subtractive screen tint */
#define FX_DEATH_HIT_TINT_RGB 0xDC, 0, 0x20 /* additive screen tint (red) */
#define FX_DEATH_MODEL_COUNT  1    /* btlfx_initEffModels slots: REAPER_2 */
#define FX_DEATH_RIBBON_SEGMENTS 16  /* fxm_drawRibbonTrail: segments of the two scythe ribbons (fx_05e_ribbonA / B) */
#define FX_DEATH_RIBBON_CLEAR    1000 /* fxm_drawRibbonTrail n >= this: clear the first n - 999 segments' live flags instead */
#define FX_DEATH_BLUE_TRAIL_2_TRAIL_SLOTS  0x18 /* FX_DEATH_BLUE_TRAIL_2's trail buffer fx_05e_trails: slots of */
#define FX_DEATH_BLUE_TRAIL_2_TRAIL_STRIDE 0x3C /* this many bytes (Pool6CEntry trailSlots / trailStride) */
#define FX_DEATH_BLUE_TRAIL_TRAIL_SLOTS    0x18 /* FX_DEATH_BLUE_TRAIL's trail buffer fx_05e_trails2: slots of */
#define FX_DEATH_BLUE_TRAIL_TRAIL_STRIDE   0x44 /* this many bytes */

/* ribbon history: two edge points per step, edge0.pad = live */
typedef struct {
    SVECTOR edge0;
    SVECTOR edge8;
} Fx05eSeg; /* 0x10 */

#define gte_stszotz(r0) __asm__ volatile( \
    "mfc2\t$12, $19;" \
    "nop;" \
    "sra\t$12, $12, 2;" \
    "sw\t$12, 0(%0)" \
    : : "r"(r0) : "$12", "memory")

typedef struct {
    s32 state0;
    s32 frame4; /* frame */
    SVECTOR pos8; /* summoner position */
    SVECTOR rot10; /* summoner rotation */
    SVECTOR pos18; /* summon position */
    SVECTOR pos20; /* ribbon head */
    s32 model28; /* slice model */
    SVECTOR rot2C; /* summon rotation */
    s32 scale34[3]; /* summon scale */
} Fx05eWork; /* 0x40 */

/* the summon slot behind env->loader */
typedef struct {
    u8 pad0[0x54];
    struct {
        u8 pad0[0x180];
        BtlMotion *motion180;
    } *summon54; /* g_btlfxLoader.summon */
    u8 pad58[8];
    s32 model60; /* summon model */
} Fx05eSummon;

extern s32 fx_05e_facingBlocks[2];
extern SVECTOR fx_05e_ribbonRgb0;
extern SVECTOR fx_05e_ribbonRgb1;
extern SVECTOR fx_05e_ribbonRgb2;
extern SVECTOR fx_05e_bonePt0;
extern SVECTOR fx_05e_bonePt1;
extern SVECTOR fx_05e_bonePt2;
extern u8 fx_05e_trails[];
extern Unk80100DC054 fx_05e_summon;
extern BtlModelBuf fx_05e_effModels[];
extern u8 fx_05e_trails2[];
extern Fx05eSeg fx_05e_ribbonA[];
extern Fx05eSeg fx_05e_ribbonB[];
extern Unk800F78A4 *fx_05e_env;

static void fxm_drawRibbonTrail(SVECTOR *posA, SVECTOR *posB, u32 n, CVECTOR *colorA, CVECTOR *colorB, s32 fade, Fx05eSeg *buf, s32 abr);
static void fxm_getSummonBonePoint(s32 model, s32 bone, SVECTOR *in, SVECTOR *out);

#endif
