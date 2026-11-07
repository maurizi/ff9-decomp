#ifndef FX_0ED_H
#define FX_0ED_H

/* Effect module fx_0ed (dir 13 file 0x0ED): Roulette (party and enemy). Ribbon trails (fxm_drawRibbonTrail).
 * Evidence: [data] the party ability / item tables and enemy sequences that play this file id
 * (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0ed effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s3.md; "(look pending)" = by kind, appearance not settled). */
#define FX_ROULETTE_MODEL_DATA 0x2 /* model data passed to btlfx_registerSummonModel with ROULETTE_SUMMON_MODEL */
#define FX_ROULETTE_SMOKE_TEXTURE     0x4 /* layer table: render-to-texture (btlfx_drawLayerTexture): 2 layers into VRAM (0x200,0x1A0) 64x64, rising purple smoke (byte-identical to fx_05e entry 0xB, played there); runtime (docs/runtime-findings.md effect-data pass, medium) */
#define FX_ROULETTE_TARGET_RING       0x6 /* sprite: white ring outline on the chosen target */
#define FX_ROULETTE_REAPER 0x7 /* 'so' summon model (btlfx_registerSummonModel): the robed skull reaper holding a scythe (byte-identical to fx_05e entry 0x6, same render) */
#define FX_ROULETTE_REAPER_2 0x8 /* 'so' model: the robed skull reaper with the scythe in another pose (same textures, 354 polys; byte-identical to fx_05e entry 0x1C, same render) */
#define FX_ROULETTE_BLUE_TRAIL 0xA /* emitter: spiralling blue light-beam trail */
#define FX_ROULETTE_GREEN_ORB 0xB /* emitter: green glows + white-salmon-violet rings */
#define FX_ROULETTE_RED_RINGS 0xC /* emitter: pink-red spark sprites + white-red rings */
#define FX_ROULETTE_SMOKE 0x10 /* emitter: grey-white smoke puffs (byte-identical to fx_05e entry 0x10, same render) */
#define FX_ROULETTE_SOUL_ORBS 0x11 /* emitter: blue bubbles */
#define FX_ROULETTE_BLUE_TRAIL_2 0x1D /* emitter: spiralling blue light-beam trail */

/* fx_0ed choreography (docs/fxdata.md, "Choreography literals"): op80's reaper states, their lengths, keyframes and
 * screen tints, the trail buffers and ribbons; op81's roulette over the units. */
#define FX_ROULETTE_STATE_RISE   0 /* op80: the reaper (summon model) rises behind the unit, SOUL_ORBS / BLUE_TRAIL_2 */
#define FX_ROULETTE_STATE_POSE   1 /* it holds motion frame 0 */
#define FX_ROULETTE_STATE_SWING  2 /* the swing (motion played back), the scythe ribbons, the hit, SMOKE_TEXTURE */
#define FX_ROULETTE_STATE_VANISH 3 /* the reaper narrows and fades (the op ends) */
#define FX_ROULETTE_RISE_FRAMES   0x18 /* STATE_RISE's length */
#define FX_ROULETTE_POSE_FRAMES   8    /* STATE_POSE's length */
#define FX_ROULETTE_RIBBON_FRAMES 0x18 /* STATE_SWING: the ribbons show at full strength until this frame */
#define FX_ROULETTE_HIT_FRAME     0x1F /* STATE_SWING: the hit emitter (fx_0ed_facingBlocks, by unit / camera facing) */
#define FX_ROULETTE_DARKEN_FRAME  0x20 /* STATE_SWING: subtractive DARKEN tint */
#define FX_ROULETTE_TINT_FRAME    0x21 /* STATE_SWING: additive TINT, GREEN_ORB / RED_RINGS */
#define FX_ROULETTE_TRAIL_FRAME   0x26 /* STATE_SWING: FX_ROULETTE_BLUE_TRAIL */
#define FX_ROULETTE_SWING_FRAMES  0x29 /* STATE_SWING's length; STATE_VANISH plays the motion back from this frame */
#define FX_ROULETTE_VANISH_FRAMES 8    /* STATE_VANISH's length */
#define FX_ROULETTE_DARKEN_RGB    0xFF, 0xFF, 0    /* subtractive screen tint (the screen turns blue) */
#define FX_ROULETTE_TINT_RGB      0xDC, 0, 0x20    /* additive (crimson) */
#define FX_ROULETTE_RIBBON_BONE     9  /* summon model bone the two ribbons follow (fxm_getSummonBonePoint) */
#define FX_ROULETTE_RIBBON_SEGMENTS 17 /* fx_0ed_ribbon0 / ribbon1 history steps */
#define FX_ROULETTE_BLUE_TRAIL_2_TRAIL_SLOTS  0x18 /* FX_ROULETTE_BLUE_TRAIL_2's trail buffer fx_0ed_op80Trails0: slots of */
#define FX_ROULETTE_BLUE_TRAIL_2_TRAIL_STRIDE 0x3C /* this many bytes (Pool6CEntry trailSlots / trailStride) */
#define FX_ROULETTE_BLUE_TRAIL_TRAIL_SLOTS    0x18 /* FX_ROULETTE_BLUE_TRAIL's trail buffer fx_0ed_op80Trails1: slots of */
#define FX_ROULETTE_BLUE_TRAIL_TRAIL_STRIDE   0x44 /* this many bytes */
#define FX_ROULETTE_PICK_STATE_NEXT 0 /* op81: the pointer moves to the next unit (btlfx_nextUnitPos), the camera follows */
#define FX_ROULETTE_PICK_STATE_MARK 1 /* op81: TARGET_RING on it for endFrame4 frames */
#define FX_ROULETTE_PICK_STATE_DONE 2 /* op81: sets signal 0 and ends */
#define FX_ROULETTE_PICK_FIRST_FRAMES 0xA  /* op81: the first unit is marked this long, each next one a frame less */
#define FX_ROULETTE_PICK_STEPS        0x10 /* op81: after this many steps it stops on the chosen unit; the screen grid brightens over them */
#define FX_ROULETTE_GRID_BRIGHT       100  /* op81: the screen grid's full brightness */
#define FX_ROULETTE_CAM_DISTANCE      0x640 /* op81: the camera sits this far from the unit along z, */
#define FX_ROULETTE_CAM_HEIGHT        0x200 /* this high */

typedef struct {
    s32 state0; /* state */
    s32 endFrame4; /* end frame of the current target */
    s32 frame8; /* frame counter */
    BtlUnit *unitC; /* target */
    SVECTOR pos10; /* target position */
} Fx0edWork81; /* 0x18 */


extern Unk800F78A4 *fx_0ed_env;

s32 fx_0ed_op81(s32 mode, Fx0edWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

typedef struct {
    SVECTOR a0;
    SVECTOR b8;
} Fx0edSeg; /* 0x10 */
static void fxm_drawRibbonTrail(SVECTOR *posA, SVECTOR *posB, u32 n, CVECTOR *colorA, CVECTOR *colorB, s32 fade, Fx0edSeg *buf, s32 abr);
static void fxm_getSummonBonePoint(s32 model, s32 bone, SVECTOR *in, SVECTOR *out);

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    SVECTOR pos8; /* unit position */
    SVECTOR rot10; /* unit rotation */
    SVECTOR pos18; /* model position */
    SVECTOR unk20; /* unused (runtime watch, round 33) */
    s32 model28; /* model */
    SVECTOR rot2C; /* model rotation */
    s32 scale34[3]; /* model scale */
} Fx0edWork80; /* 0x40 */

typedef struct {
    u8 pad0[0x54];
    struct {
        u8 pad0[0x180];
        BtlMotion *motion180;
    } *summon54; /* g_btlfxLoader.summon */
    u8 pad58[0x60 - 0x58];
    s32 summonModel60; /* summon model */
} Fx0edSummon;

extern s32 fx_0ed_facingBlocks[2];
extern CVECTOR fx_0ed_ribbonRgb0;
extern CVECTOR fx_0ed_ribbonRgb1;
extern CVECTOR fx_0ed_ribbonRgb2;
extern SVECTOR fx_0ed_ribbonPt0;
extern SVECTOR fx_0ed_ribbonPt1;
extern SVECTOR fx_0ed_ribbonPt2;
extern u8 fx_0ed_op80Trails0[];
extern Unk80100DC054 fx_0ed_summonModel;
extern u8 fx_0ed_op80Trails1[];
extern Fx0edSeg fx_0ed_ribbon0[FX_ROULETTE_RIBBON_SEGMENTS];
extern Fx0edSeg fx_0ed_ribbon1[FX_ROULETTE_RIBBON_SEGMENTS];
extern BtlModelBuf fx_0ed_effModels[];

#endif
