#ifndef FX_0ED_H
#define FX_0ED_H

/* Effect module fx_0ed (dir 13 file 0x0ED): Roulette (party and enemy). Ribbon trails (fxm_drawRibbonTrail).
 * Evidence: [data] the party ability / item tables and enemy sequences that play this file id
 * (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0ed effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s3.md; "(look pending)" = by kind, appearance not settled). */
#define FX_ROULETTE_MODEL_DATA 0x2 /* model data passed to btlfx_registerSummonModel with ROULETTE_SUMMON_MODEL */
#define FX_ROULETTE_BACKDROP          0x4 /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */
#define FX_ROULETTE_TARGET_RING       0x6 /* sprite: white ring outline on the chosen target */
#define FX_ROULETTE_REAPER 0x7 /* 'so' summon model (btlfx_registerSummonModel): the robed skull reaper holding a scythe (byte-identical to fx_05e entry 0x6, same render) */
#define FX_ROULETTE_REAPER_2 0x8 /* 'so' model: the robed skull reaper with the scythe in another pose (same textures, 354 polys; byte-identical to fx_05e entry 0x1C, same render) */
#define FX_ROULETTE_BLUE_TRAIL 0xA /* emitter: spiralling blue light-beam trail */
#define FX_ROULETTE_GREEN_ORB 0xB /* emitter: green glows + white-salmon-violet rings */
#define FX_ROULETTE_RED_RINGS 0xC /* emitter: pink-red spark sprites + white-red rings */
#define FX_ROULETTE_SMOKE 0x10 /* emitter: grey-white smoke puffs (byte-identical to fx_05e entry 0x10, same render) */
#define FX_ROULETTE_SOUL_ORBS 0x11 /* emitter: blue bubbles */
#define FX_ROULETTE_BLUE_TRAIL_2 0x1D /* emitter: spiralling blue light-beam trail */

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
extern Fx0edSeg fx_0ed_ribbon0[17];
extern Fx0edSeg fx_0ed_ribbon1[17];
extern BtlModelBuf fx_0ed_effModels[];

#endif
