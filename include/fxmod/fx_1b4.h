#ifndef FX_1B4_H
#define FX_1B4_H

/* Effect module fx_1b4 (dir 13 file 0x1b4): enemy Flare Star (Kuja/Trance Kuja, ...). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_1b4 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s6.md; "(look pending)" = by kind, appearance not settled). */
#define FX_FLARE_STAR_PINK_SPARKS    0xA /* emitter: pink / orange glyph-like spark sprites + pink and lilac gouraud rings */
#define FX_FLARE_STAR_RED_RAYS       0xB /* emitter: two fan elements, the red light rays radiating from the target over the violet field; runtime (docs/runtime-findings.md effect-data pass 2) */
#define FX_FLARE_STAR_FLAME_TRAIL    0xC /* emitter: white fan bars + screen trail with a red flame strip */
#define FX_FLARE_STAR_YELLOW_RINGS   0xD /* emitter: pale-yellow / tan gouraud rings */
#define FX_FLARE_STAR_YELLOW_RINGS_2 0xE /* emitter: pale-yellow / pink gouraud rings */
#define FX_FLARE_STAR_PINK_RINGS     0xF /* emitter: thin pink streak fan + white-pink gouraud rings */
#define FX_FLARE_STAR_PINK_SPARKS_2  0x10 /* emitter: pink glyph-like spark sprites, 33 frames */
#define FX_FLARE_STAR_RED_RINGS      0x11 /* emitter: golden sparkle ring + lilac and crimson gouraud rings */
#define FX_FLARE_STAR_SMOKE_TRAIL    0x12 /* emitter: spiralling 3D trail, grey smoke strip */
#define FX_FLARE_STAR_RED_ORB        0x13 /* emitter: red glowing orb particles + violet-pink rings + red streak trail */
#define FX_FLARE_STAR_BACKDROP       0x1C /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */
#define FX_FLARE_STAR_PINK_RINGS_2   0x29 /* emitter: white-pink-magenta and yellow-olive gouraud rings */
#define FX_FLARE_STAR_ORANGE_GLOW    0x2A /* emitter: fading orange glow + red flare streaks */
#define FX_FLARE_STAR_RED_SHARDS     0x2B /* emitter: red shard sprites, 31 frames */

/* fx_1b4 choreography (docs/fxdata.md, "Choreography literals"): op80's eight path-following streaks gathering over
 * the targets, op81's flare (sparks, rays, flame trail, white flash, two dimmed screen grids), op82's burst, op83's
 * shards; trail buffers, keyframes and lengths. */
#define FX_FLARE_STAR_HEIGHT       0x258 /* every op's emitters sit this high over the targets' centre */
#define FX_FLARE_STAR_PATHS        8     /* op80: position paths (fx_1b4_pathFileIds), one emitter pair each */
#define FX_FLARE_STAR_GATHER_TRAIL_SLOTS  5    /* op80: frame 0 emitters' trail buffers fx_1b4_op80Trails0[i]: slots of */
#define FX_FLARE_STAR_GATHER_TRAIL_STRIDE 0x30 /* this many bytes (Pool6CEntry trailSlots / trailStride) */
#define FX_FLARE_STAR_STREAKS_FRAME 0x17 /* op80: the path-following streaks start */
#define FX_FLARE_STAR_STREAK_TRAIL_SLOTS  8    /* their trail buffers fx_1b4_op80Trails1[i]: slots of */
#define FX_FLARE_STAR_STREAK_TRAIL_STRIDE 0x70 /* this many bytes */
#define FX_FLARE_STAR_RINGS_FRAME  0x25  /* op80: PINK_RINGS_2 */
#define FX_FLARE_STAR_GATHER_FRAMES 0x3C /* op80's length (the op ends) */
#define FX_FLARE_STAR_FLAME_FRAME  0x1C  /* op81: FLAME_TRAIL and YELLOW_RINGS (PINK_SPARKS and RED_RAYS at frame 0) */
#define FX_FLARE_STAR_FLAME_TRAIL_TRAIL_SLOTS  0xC0 /* FLAME_TRAIL's trail buffer fx_1b4_op81Trails: slots of */
#define FX_FLARE_STAR_FLAME_TRAIL_TRAIL_STRIDE 0x30 /* this many bytes */
#define FX_FLARE_STAR_WHITE_FRAME  0x1E  /* op81: one-frame additive white */
#define FX_FLARE_STAR_WHITE_RGB    0xFF, 0xFF, 0xFF
#define FX_FLARE_STAR_DIM_FRAME    0x1F  /* op81: a dimming screen grid (mask-bit draw mode) from this frame */
#define FX_FLARE_STAR_DIM_FRAMES   0x3C  /* for this many frames */
#define FX_FLARE_STAR_DIM_2_FRAME  0x5B  /* op81: a growing one from this frame */
#define FX_FLARE_STAR_DIM_2_FRAMES 0xF   /* for this many frames */
#define FX_FLARE_STAR_FLARE_FRAMES 0x8C  /* op81's length (the op ends) */
#define FX_FLARE_STAR_BURST_EMITTERS 7   /* op82: emitters */
#define FX_FLARE_STAR_BURST_FRAME  20    /* op82: RED_RINGS, SMOKE_TRAIL, RED_ORB, ORANGE_GLOW */
#define FX_FLARE_STAR_SMOKE_TRAIL_TRAIL_SLOTS  0x21 /* SMOKE_TRAIL's trail buffer fx_1b4_op82Trails0: slots of */
#define FX_FLARE_STAR_SMOKE_TRAIL_TRAIL_STRIDE 0x90 /* this many bytes */
#define FX_FLARE_STAR_RED_ORB_TRAIL_SLOTS  0x58 /* RED_ORB's trail buffer fx_1b4_op82Trails1: slots of */
#define FX_FLARE_STAR_RED_ORB_TRAIL_STRIDE 0x30 /* this many bytes */
#define FX_FLARE_STAR_BURST_FRAMES 0x50  /* op82's length (the op ends) */
#define FX_FLARE_STAR_SHARDS_FRAMES 0x3C /* op83 (RED_SHARDS): its length */

typedef struct {
    s16 x, y, z;
} Fx1b4Point;

typedef struct {
    s16 unk0; /* not read by the module */
    u16 count2; /* count */
    Fx1b4Point *keys4;
} Fx1b4Path;

typedef struct {
    Pool6CEntry *emit0[FX_FLARE_STAR_PATHS];
    Pool6CEntry *emit20[FX_FLARE_STAR_PATHS];
    Pool6CEntry *emit40;
    SVECTOR pos44; /* target position */
    SVECTOR pos4C[FX_FLARE_STAR_PATHS];
    SVECTOR pos8C;
} Fx1b4Work80; /* 0x94 */

typedef struct {
    Pool6CEntry *emit0[4];
    SVECTOR pos10; /* target position */
} Fx1b4Work81; /* 0x18 */

typedef struct {
    Pool6CEntry *emit0[FX_FLARE_STAR_BURST_EMITTERS];
    SVECTOR pos1C; /* target position */
} Fx1b4Work82; /* 0x24 */

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4; /* target position */
} Fx1b4Work83; /* 0xC */

extern u16 fx_1b4_emitFileIds0[FX_FLARE_STAR_PATHS];
extern u16 fx_1b4_emitFileIds1[FX_FLARE_STAR_PATHS];
extern u16 fx_1b4_pathFileIds[FX_FLARE_STAR_PATHS];
extern s32 (*fx_1b4_op80ParticleMotions[FX_FLARE_STAR_PATHS])();
extern Unk800F78A4 *fx_1b4_env;
extern Fx1b4Path fx_1b4_posPath[FX_FLARE_STAR_PATHS];
extern Fx1b4Path fx_1b4_op80ParticlePaths[FX_FLARE_STAR_PATHS];
extern s32 fx_1b4_op80Trails0[FX_FLARE_STAR_PATHS][FX_FLARE_STAR_GATHER_TRAIL_SLOTS * FX_FLARE_STAR_GATHER_TRAIL_STRIDE / 4];
extern s32 fx_1b4_op80Trails1[FX_FLARE_STAR_PATHS][FX_FLARE_STAR_STREAK_TRAIL_SLOTS * FX_FLARE_STAR_STREAK_TRAIL_STRIDE / 4];
extern s32 fx_1b4_op81Trails[];
extern s32 fx_1b4_op82Trails0[];
extern s32 fx_1b4_op82Trails1[];
extern SVECTOR fx_1b4_pathOrigin; /* origin */
extern SVECTOR fx_1b4_emitPos;

static s32 fxm_followPathParticle(Pool30Entry *pt, SVECTOR *pos, Fx1b4Path *path, s32 frame, s32 frames);
static void fxm_posPathInit(Fx1b4Path *path, u16 *data);
static s32 fxm_posPathGet(Fx1b4Path *path, s32 t, SVECTOR *out);

/* propagated */
s32 fx_1b4_op80ParticleMotion0(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
s32 fx_1b4_op80ParticleMotion1(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
s32 fx_1b4_op80ParticleMotion2(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
s32 fx_1b4_op80ParticleMotion3(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
s32 fx_1b4_op80ParticleMotion4(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
s32 fx_1b4_op80ParticleMotion5(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
s32 fx_1b4_op80ParticleMotion6(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
s32 fx_1b4_op80ParticleMotion7(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);

#endif
