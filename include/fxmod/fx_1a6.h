#ifndef FX_1A6_H
#define FX_1A6_H

/* Effect module fx_1a6 (dir 13 file 0x1a6): enemy Water-gun (Kraken, ...). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_1a6 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s6.md; "(look pending)" = by kind, appearance not settled). */
#define FX_WATER_GUN_BUBBLES   0x1 /* emitter: white bubble-ring anim quads, 22 frames */
#define FX_WATER_GUN_SPRAY     0x3 /* emitter: one particle element, the white water splash spray over the target; runtime (docs/runtime-findings.md effect-data pass 2) */
#define FX_WATER_GUN_SPLASH    0x5 /* emitter: blue water-splash crescents + white splash */
#define FX_WATER_GUN_WATER_JET 0x6 /* emitter: screen trail with a water strip + white splash particles (sprites render black) */

/* fx_1a6 choreography (docs/fxdata.md, "Choreography literals"): per shot a spray at the caster's mouth bone and a
 * stream lerped to a random point near the target, the jet and splash there, trail buffers and lengths. */
#define FX_WATER_GUN_SPRAY_AHEAD  0x12C /* SPRAY starts this far ahead of unit 0x10's argument bone, towards the target */
#define FX_WATER_GUN_STREAM_AHEAD 0xC8  /* the stream (fx_1a6_emitFileIds[shot]) leaves from this far ahead */
#define FX_WATER_GUN_SPRAY_TRAIL_SLOTS   0xF  /* SPRAY's trail buffer fx_1a6_op80Trails0[shot]: slots of */
#define FX_WATER_GUN_SPRAY_TRAIL_STRIDE  0x18 /* this many bytes (Pool6CEntry trailSlots / trailStride) */
#define FX_WATER_GUN_STREAM_TRAIL_SLOTS  0x1D /* the stream's trail buffer fx_1a6_op80Trails1[shot]: slots of */
#define FX_WATER_GUN_STREAM_TRAIL_STRIDE 0x40 /* this many bytes */
#define FX_WATER_GUN_HIT_FRAME    6    /* WATER_JET and SPLASH at the target, its hit motion (not for the last shot) */
#define FX_WATER_GUN_WATER_JET_TRAIL_SLOTS  0x56 /* WATER_JET's trail buffer fx_1a6_op80Trails2[shot]: slots of */
#define FX_WATER_GUN_WATER_JET_TRAIL_STRIDE 0x40 /* this many bytes */
#define FX_WATER_GUN_SOUND_FRAME  8    /* three more file sounds */
#define FX_WATER_GUN_FRAMES       0x28 /* op80's length (the op ends) */
#define FX_WATER_GUN_BUBBLES_FRAMES 0x28 /* op81 (BUBBLES at the target): its length */

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4; /* target position */
} Fx1a6Work; /* 0xC */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    SVECTOR pos10; /* caster body position */
    SVECTOR pos18; /* target body position */
    SVECTOR pos20; /* bone position */
    SVECTOR pos28;
    u8 pad30[0x38 - 0x30];
    SVECTOR angle38; /* angles */
    SVECTOR angle40;
    s32 index48; /* index */
} Fx1a6Work80; /* 0x4C */
#define FX_1A6_SHOTS 3 /* instances, one per shot */
#define FX_1A6_LAST_SHOT (FX_1A6_SHOTS - 1) /* starts no hit motion */

extern u16 fx_1a6_emitFileIds[FX_1A6_SHOTS]; /* files */
extern s32 (*fx_1a6_op80ParticleMotions[FX_1A6_SHOTS])();
extern Unk800F78A4 *fx_1a6_env;
extern s32 fx_1a6_op80Trails0[FX_1A6_SHOTS][FX_WATER_GUN_SPRAY_TRAIL_SLOTS * FX_WATER_GUN_SPRAY_TRAIL_STRIDE / 4];
extern s32 fx_1a6_op80Trails1[FX_1A6_SHOTS][FX_WATER_GUN_STREAM_TRAIL_SLOTS * FX_WATER_GUN_STREAM_TRAIL_STRIDE / 4];
extern s32 fx_1a6_op80Trails2[FX_1A6_SHOTS][FX_WATER_GUN_WATER_JET_TRAIL_SLOTS * FX_WATER_GUN_WATER_JET_TRAIL_STRIDE / 4];
extern SVECTOR fx_1a6_motionFrom[FX_1A6_SHOTS]; /* from */
extern SVECTOR fx_1a6_motionTo[FX_1A6_SHOTS]; /* to */
extern SVECTOR fx_1a6_emitPos0[FX_1A6_SHOTS];

static s32 fxm_lerpParticle(Pool30Entry *pt, SVECTOR *pos, SVECTOR *from, SVECTOR *to, s32 frame, s32 frames);
static void fxm_startUnitMotion(s32 unit, s32 motion);
static s32 fxm_isMotionDone(s32 unit);
static void fxm_startHitMotionIfActive(s32 unit);

/* propagated */
s32 fx_1a6_op80ParticleMotion0(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
s32 fx_1a6_op80ParticleMotion1(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
s32 fx_1a6_op80ParticleMotion2(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);

#endif
