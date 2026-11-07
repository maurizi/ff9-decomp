#ifndef FX_18A_H
#define FX_18A_H

/* Effect module fx_18a (dir 13 file 0x18a): enemy Transform (Kuja/Trance Kuja). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_18a effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s6.md; "(look pending)" = by kind, appearance not settled). */
#define FX_TRANSFORM_AURA_CYLINDER  0x1 /* model: open cone ring, magenta-violet cloud texture (byte-identical to fx_101 FX_TRANCE_AURA_CYLINDER) */
#define FX_TRANSFORM_VIOLET_RINGS   0x3 /* rings: white-lilac-violet gouraud rings */
#define FX_TRANSFORM_VIOLET_RINGS_2 0x4 /* rings: white-violet gouraud rings */
#define FX_TRANSFORM_VIOLET_TRAIL   0x6 /* trail: violet speckled strip, 12 frames (per pool item) */
#define FX_TRANSFORM_BLUE_RING      0x9 /* rings: navy-blue gouraud ring */
#define FX_TRANSFORM_RED_SWEEP      0xA /* emitter: fan elements, the red light streaks sweeping in from the left (played by forcing the effect id on Zidane's Attack, Zidane as the caster: a real enemy cast may place it differently); runtime (docs/runtime-findings.md effect-data pass 2, medium) */
#define FX_TRANSFORM_RED_SPECKS     0xC /* emitter: spiral of sprites + sub-sprites, small dark-red glowing specks at run time (ablation changes part of the white-out) (played by forcing the effect id on Zidane's Attack, Zidane as the caster: a real enemy cast may place it differently); runtime (docs/runtime-findings.md effect-data pass 2, low) (look pending: low) */
#define FX_TRANSFORM_PINK_STREAKS   0xE /* emitter: orbiting fans, pink-white light streaks */
#define FX_TRANSFORM_RED_STREAKS    0xF /* emitter: orbiting fans, red light streaks with pink orbs */
#define FX_TRANSFORM_VIOLET_RING    0x12 /* rings: violet gouraud ring */

/* fx_18a choreography (docs/fxdata.md, "Choreography literals"): op80's rings and aura cylinder swelling around the
 * caster, pulsing until it transforms, then the screen-grid warp, ring bursts and the aura shrinking; op82's
 * crackling trails (pool sparks along 15-point trails); textures, states, keyframes, pool sizes and lengths. */
#define FX_TRANSFORM_AURA_TPAGE 0xB7   /* Hi_RegisterTexEffModel texture: 8-bit page (0x1C0, 0x100), abr 1 */
#define FX_TRANSFORM_AURA_CLUT  0x3D80 /* CLUT (0x0, 0xF6) */
#define FX_TRANSFORM_AURA_U     0
#define FX_TRANSFORM_AURA_V     0
#define FX_TRANSFORM_AURA_VRAM_X 0x1C0 /* btlfx_drawScrollMove: the 32x64 rect of the aura's page it scrolls */
#define FX_TRANSFORM_AURA_VRAM_Y 0x120
#define FX_TRANSFORM_AURA_VRAM_W 0x20
#define FX_TRANSFORM_AURA_VRAM_H 0x40
#define FX_TRANSFORM_AURA_SRC_X  0x1E0 /* from the rect beside it */
#define FX_TRANSFORM_AURA_SRC_Y  0x120
#define FX_TRANSFORM_STATE_SWELL     0 /* work->state0: rings and aura grow in */
#define FX_TRANSFORM_STATE_PULSE     1 /* they pulse; RED_SWEEP */
#define FX_TRANSFORM_STATE_TRANSFORM 2 /* the caster is swapped, screen-grid warp, ring bursts */
#define FX_TRANSFORM_SWELL_FRAMES    0x10 /* STATE_SWELL's length */
#define FX_TRANSFORM_PULSE_FRAMES    10   /* STATE_PULSE: the pulse period */
#define FX_TRANSFORM_PULSE_MIN_FRAMES 0xB /* the state ends at the first period end from this frame */
#define FX_TRANSFORM_FLASH_FRAMES    6    /* STATE_TRANSFORM: a BLUE_RING flash */
#define FX_TRANSFORM_TINT_FRAMES     4    /* an additive violet tint fading out */
#define FX_TRANSFORM_SHRINK_FRAMES   0x16 /* the aura shrinks away */
#define FX_TRANSFORM_CUE_FRAME       0x1E /* btlfx_signalScriptCue */
#define FX_TRANSFORM_BURST_FRAME     0x8C /* VIOLET_RING bursts and the grid's bounce until the end */
#define FX_TRANSFORM_FRAMES          0xB4 /* STATE_TRANSFORM's length (its ramp; the op ends) */
#define FX_TRANSFORM_SPARKS          0xB4 /* op82: pool items (init->poolCount), sparks along the trails */
#define FX_TRANSFORM_TRAILS          12   /* op82: trail slots (fx_18a_trails), reused round robin */
#define FX_TRANSFORM_TRAIL_POINTS    15   /* points per trail, one spark each */
#define FX_TRANSFORM_SPARK_FRAMES    0xC  /* a spark's life (and its trail's fade) */
#define FX_TRANSFORM_TRAILS_FRAMES   0xE6 /* op82: a trail per frame while frame < this, then the op ends */

typedef struct {
    SVECTOR pos0; /* caster body position */
    SVECTOR rot8; /* angles */
} Fx18aWork82; /* 0x10 */

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR points4[FX_TRANSFORM_TRAIL_POINTS];
} Fx18aTrail; /* 0x7C */

typedef struct {
    s32 pos0; /* position, 20.12 */
    s32 pos4;
    s32 pos8;
    s16 velC; /* velocity */
    s16 velE;
    s16 vel10;
    s16 drag12; /* velocity factor per frame (btlfx_stepModuleParticle), 0x1000 = none */
    u16 slot14; /* trail */
    s16 index16; /* trail point index */
    SVECTOR *point18; /* trail point */
} Fx18aSpark; /* 0x1C */

typedef struct {
    s32 frame0; /* frame */
    u8 pad4[0xC];
    s16 *pool10; /* pool */
} Fx18aInst;

extern Unk800F78A4 *fx_18a_env;
extern Fx18aTrail fx_18a_trails[FX_TRANSFORM_TRAILS];
extern s32 fx_18a_nextTrail;

static s32 fx_18a_op82PoolTask(s32 mode, Fx18aSpark *spark, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);
void fx_18a_resetTrails(void);
void fx_18a_spawnTrail(Fx18aInst *inst, SVECTOR *origin, s32 reach, SVECTOR *from, SVECTOR *to);

#define TRAIL(e) ((Fx18aTrail *)((e)->point18 - 2))

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    Pool6CEntry *emit8;
    SVECTOR posC; /* caster position */
    SVECTOR pos14; /* caster body position */
    s32 frame1C; /* charge counter */
    s32 unk20; /* write-only (cleared at init) */
    s32 model24; /* model */
} Fx18aWork80; /* 0x28 */

extern s32 fx_18a_op80Done; /* set at the end of op80, nothing reads it (runtime watch, round 33) */
extern BtlModelBuf fx_18a_effModels[];

#endif
