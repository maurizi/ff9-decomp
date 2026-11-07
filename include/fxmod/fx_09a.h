#ifndef FX_09A_H
#define FX_09A_H

/* Effect module fx_09a (dir 13 file 0x9A): LV5 Death (party and enemy). Ribbon trails (fxm_drawRibbonTrail).
 * Evidence: [data] the party ability / item tables and enemy sequences that play this file id
 * (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_09a effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_LV5_DEATH_GREEN_ORB 0x1 /* emitter: fan + green glow sprites, orange / violet rings */
#define FX_LV5_DEATH_MODEL_DATA 0x2 /* table: data block passed with the model to btlfx_registerSummonModel */
#define FX_LV5_DEATH_SMOKE_TEXTURE 0x3 /* layer table: render-to-texture (btlfx_drawLayerTexture): 2 layers into VRAM (0x200,0x1A0) 64x64, rising purple smoke (byte-identical to fx_05e entry 0xB, played there); runtime (docs/runtime-findings.md effect-data pass, medium) */
#define FX_LV5_DEATH_BLUE_TRAIL 0x7 /* emitter: blue spiralling trail */
#define FX_LV5_DEATH_REAPER     0x8 /* 'so' model (btlfx_registerSummonModel): the robed skull reaper holding a scythe */
#define FX_LV5_DEATH_RED_RINGS 0x9 /* emitter: pink-red sprites + white-red rings */
#define FX_LV5_DEATH_SMOKE 0xE /* emitter: grey smoke blobs */

/* fx_09a choreography (docs/fxdata.md, "Choreography literals"): op80's states (one swing per target), keyframes,
 * the scythe ribbons and the screen tints. */
#define FX_LV5_DEATH_STATE_NEXT   1 /* op80: pick the next target and place the reaper beside it (or end) */
#define FX_LV5_DEATH_STATE_SWING  2 /* the reaper fades in and swings, its scythe ribbons trailing */
#define FX_LV5_DEATH_STATE_VANISH 3 /* it shrinks away, blue trail and smoke */
#define FX_LV5_DEATH_STATE_END    4 /* wait, then signal 0 and end */
#define FX_LV5_DEATH_FADE_IN_FRAMES 6  /* FX_LV5_DEATH_STATE_SWING: the reaper brightens in over this many frames */
#define FX_LV5_DEATH_RIBBON_FRAMES 22  /* the ribbons fade in over this many */
#define FX_LV5_DEATH_SOUND_FRAME  24   /* the swing sounds play */
#define FX_LV5_DEATH_DARKEN_FRAME 26   /* subtractive screen tint */
#define FX_LV5_DEATH_HIT_FRAME    27   /* the hit emitter (fx_09a_hitBlocks, by facing), GREEN_ORB and RED_RINGS start */
#define FX_LV5_DEATH_FLASH_FRAME  28   /* red screen flash */
#define FX_LV5_DEATH_RESULT_FRAME 29   /* btlfx_execCmd: the target's result */
#define FX_LV5_DEATH_SWING_FRAMES 30   /* FX_LV5_DEATH_STATE_SWING's length */
#define FX_LV5_DEATH_VANISH_FRAMES 6   /* FX_LV5_DEATH_STATE_VANISH's length (the (frame << 12) / N ramp) */
#define FX_LV5_DEATH_BLUE_TRAIL_FRAME 2 /* FX_LV5_DEATH_STATE_VANISH: BLUE_TRAIL and SMOKE start */
#define FX_LV5_DEATH_END_FRAMES   8    /* FX_LV5_DEATH_STATE_END's length */
#define FX_LV5_DEATH_REAPER_GAP   0x78  /* the reaper stands this far beyond the target's radius */
#define FX_LV5_DEATH_CAM_HEIGHT   0x140 /* the camera points are this far above the reaper / target */
#define FX_LV5_DEATH_RIBBON_SEGMENTS 0x10 /* scythe ribbon segments (fx_09a_ribbon0 / 1 hold one more point) */
#define FX_LV5_DEATH_RIBBON_RESET 0x3F8 /* fxm_drawRibbonTrail n >= 1000: refill the ribbon with the current points */
#define FX_LV5_DEATH_BLUE_TRAIL_TRAIL_SLOTS  0x18 /* FX_LV5_DEATH_BLUE_TRAIL's trail buffer fx_09a_trails: slots of */
#define FX_LV5_DEATH_BLUE_TRAIL_TRAIL_STRIDE 0x44 /* this many bytes */
#define FX_LV5_DEATH_DARKEN_RGB    0xFF, 0xFF, 0  /* subtractive tint on FX_LV5_DEATH_DARKEN_FRAME */
#define FX_LV5_DEATH_RED_FLASH_RGB 0xDC, 0, 0x20  /* additive tint on FX_LV5_DEATH_FLASH_FRAME */

typedef struct {
    SVECTOR a0;
    SVECTOR b8;
} Fx09aSeg; /* 0x10 */
static void fxm_drawRibbonTrail(SVECTOR *posA, SVECTOR *posB, u32 n, CVECTOR *colorA, CVECTOR *colorB, s32 fade, Fx09aSeg *buf, s32 abr);
extern Unk800F78A4 *fx_09a_env;
static void fxm_getSummonBonePoint(s32 model, s32 bone, SVECTOR *in, SVECTOR *out);

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame */
    s32 unit8; /* unit */
    s32 unitsDoneC; /* units done */
    s32 unitCount10; /* unit count */
    SVECTOR pos14; /* unit position */
    SVECTOR rot1C; /* unit rotation */
    SVECTOR pos24; /* summon position */
    s32 ribbonHead2C; /* ribbon head */
    s32 unk30; /* unused */
    SVECTOR rot34; /* summon rotation */
    s32 scale3C[3]; /* summon scale */
} Fx09aWork; /* 0x48 */

/* the summon slot behind env->loader */
typedef struct {
    u8 pad0[0x54];
    struct {
        u8 pad0[0x180];
        BtlMotion *motion180;
    } *summon54; /* g_btlfxLoader.summon */
    u8 pad58[8];
    s32 model60; /* summon model */
} Fx09aSummon;

extern s32 fx_09a_hitBlocks[2];
extern SVECTOR fx_09a_ribbonColA;
extern SVECTOR fx_09a_ribbonColB;
extern SVECTOR fx_09a_ribbonColC;
extern SVECTOR fx_09a_bonePtA;
extern SVECTOR fx_09a_bonePtB;
extern SVECTOR fx_09a_bonePtC;
extern Unk80100DC054 fx_09a_summonModel;
extern u8 fx_09a_trails[];
extern Fx09aSeg fx_09a_ribbon0[];
extern Fx09aSeg fx_09a_ribbon1[];

#endif
