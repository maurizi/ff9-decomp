#ifndef FX_1B3_H
#define FX_1B3_H

/* Effect module fx_1b3 (dir 13 file 0x1b3): enemy Death1 (Dummy/Necron). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_1b3 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s6.md; "(look pending)" = by kind, appearance not settled). */
#define FX_DEATH1_DARK_NEBULA        0x2 /* emitter: dark violet nebula particles + pale-yellow light-streak fans */
#define FX_DEATH1_LIGHTNING_TRAILS   0x4 /* emitter: orbiting 3D trails with violet lightning strips + dark violet nebula particles */
#define FX_DEATH1_TEAL_RINGS         0x5 /* emitter: white / grey / violet and teal / orange gouraud rings */
#define FX_DEATH1_GOLD_RAYS          0x6 /* emitter: two fan elements, the pale-gold light rays fanning out (played by forcing the effect id on Zidane's Attack, Zidane as the caster: a real enemy cast may place it differently); runtime (docs/runtime-findings.md effect-data pass 2) */
#define FX_DEATH1_YELLOW_STREAKS     0x7 /* emitter: pale-yellow light-streak fans + white-violet and cream-crimson gouraud rings */
#define FX_DEATH1_EXPLOSIONS         0x8 /* emitter: fire explosion quads, three elements, + speck sprites */
#define FX_DEATH1_ROCKS              0x9 /* emitter: grey rock particles + magenta patterned particles */
#define FX_DEATH1_BACKDROP           0xA /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */
#define FX_DEATH1_TAN_STREAKS        0xB /* emitter: tan light-streak fans + white-violet gouraud rings with a silver nebula texture */
#define FX_DEATH1_GREEN_SMOKE        0xC /* emitter: teal grid quads + pale-green smoke puffs + speck sprites */
#define FX_DEATH1_ROCK_DEBRIS        0xD /* emitter: grey rock quads, three elements */
#define FX_DEATH1_ROCK_DEBRIS_2      0xE /* emitter: orbiting grey rock quads */
#define FX_DEATH1_TEAL_RINGS_2       0x10 /* emitter: white / grey / violet and teal / orange gouraud rings */
#define FX_DEATH1_RIPPLE_RINGS       0x11 /* emitter: rings textured with orange and grey ripple patterns */
#define FX_DEATH1_LIGHTNING_TRAILS_2 0x12 /* emitter: orbiting 3D trails with violet lightning strips + white / navy / cream gouraud rings */
#define FX_DEATH1_ORANGE_RINGS       0x13 /* emitter: white-orange-plum gouraud rings, 40 frames */
#define FX_DEATH1_TAN_STREAKS_2      0x14 /* emitter: two tan light-streak fans */
#define FX_DEATH1_LIGHTNING          0x15 /* trail: violet lightning strip */
#define FX_DEATH1_SCREEN_TRAIL       0x39 /* emitter: orbiting screen trails (strip texture not resolved) + speck sprites; runtime (docs/runtime-findings.md effect-data pass 2): not fetched within 1000 frames of a forced cast (Zidane as the caster) (look pending) */
#define FX_DEATH1_SCREEN_TRAIL_2     0x3A /* emitter: orbiting screen trails (strip texture not resolved) + speck sprites; runtime (docs/runtime-findings.md effect-data pass 2): not fetched within 1000 frames of a forced cast (Zidane as the caster) (look pending) */
#define FX_DEATH1_SCRATCH_BUF        0x93 /* summon data entry 0x13 (0x93 - 0x80): its address is the base of the module's trail and split-piece work buffers */

/* fx_1b3 choreography (docs/fxdata.md, "Choreography literals"; partial: sizes, trails). */
#define FX_DEATH1_LIGHTNING_TRAILS_TRAIL_SLOTS  0x24 /* FX_DEATH1_LIGHTNING_TRAILS's trail buffer: slots of */
#define FX_DEATH1_LIGHTNING_TRAILS_TRAIL_STRIDE 0x6C /* this many bytes (Pool6CEntry trailSlots / trailStride) */
#define FX_DEATH1_SCREEN_TRAIL_2_TRAIL_SLOTS  0x36 /* FX_DEATH1_SCREEN_TRAIL_2's trail buffer: slots of */
#define FX_DEATH1_SCREEN_TRAIL_2_TRAIL_STRIDE 0xAC /* this many bytes (Pool6CEntry trailSlots / trailStride) */
#define FX_DEATH1_SCREEN_TRAIL_TRAIL_SLOTS  0x36 /* FX_DEATH1_SCREEN_TRAIL's trail buffer: slots of */
#define FX_DEATH1_SCREEN_TRAIL_TRAIL_STRIDE 0xAC /* this many bytes (Pool6CEntry trailSlots / trailStride) */
#define FX_DEATH1_LIGHTNING_TRAILS_2_TRAIL_SLOTS  0x24 /* FX_DEATH1_LIGHTNING_TRAILS_2's trail buffer: slots of */
#define FX_DEATH1_LIGHTNING_TRAILS_2_TRAIL_STRIDE 0x6C /* this many bytes (Pool6CEntry trailSlots / trailStride) */

/* fx_1b3 choreography (deep pass). op80 is one frame switch that starts emitters, splits models into pieces and loads
 * the countdowns work->timers0[FX_DEATH1_T_*]; each countdown drives one piece below the switch. The split models
 * (summon data 0x80 .. 0x92, on the caster's textures) break where fx_1b3_splitTrigger passes, by its mode. */
#define FX_DEATH1_MODEL_COUNT   0x14 /* btlfx_initEffModels slots */
#define FX_DEATH1_TIMERS        32   /* work->timers0 and work->emit80 slots */
#define FX_DEATH1_ARCS          16   /* fx_1b3_trails: lightning arcs (Fx1b3Trail) carved from SCRATCH_BUF */
#define FX_DEATH1_ARC_POINTS    9    /* points of an arc */
#define FX_DEATH1_ARC_WIDTH     0x80
#define FX_DEATH1_ARC_FADE_FRAMES 4  /* an arc (life 4 + rand) is drawn, fading, over its last frames */
#define FX_DEATH1_ARC_TAIL_FRAMES 0xD /* no new arcs in the last frames of T_ARCS */
#define FX_DEATH1_LIMB_ARCS     5    /* ARCS_LIMBS / ARCS_LIMBS_2: arcs, each along 4 bone links (2 points a link) */
#define FX_DEATH1_LIMB_LINKS    4
#define FX_DEATH1_RING_ARCS     6    /* ARCS_RINGS: arcs, rings round bone 3 or 9 */
#define FX_DEATH1_ARCS_SCATTER  0    /* T_ARC_KIND: straight arcs from random bones (fx_1b3_trailBones0), */
#define FX_DEATH1_ARCS_LIMBS    1    /* along fx_1b3_trailBones1, */
#define FX_DEATH1_ARCS_LIMBS_2  2    /* along fx_1b3_trailBones2, */
#define FX_DEATH1_ARCS_RINGS    3    /* rings round two bones */
#define FX_DEATH1_SPLIT_GROUPS  8    /* Fx1b3Split: primitive groups (even: quads, odd: triangles) */
#define FX_DEATH1_BREAK_SWEEP_X 0    /* fx_1b3_splitTrigger.mode0: pieces within radius (x, z) fly +x, */
#define FX_DEATH1_BREAK_SWEEP_Z 1    /* fly -z, */
#define FX_DEATH1_BREAK_LEVEL   2    /* pieces within radius in y fly away from the point, */
#define FX_DEATH1_BREAK_COLUMN  3    /* pieces within radius (x, z) fly out and up, */
#define FX_DEATH1_BREAK_SLICE   4    /* pieces within radius in x fly out and up, */
#define FX_DEATH1_BREAK_OUTSIDE 5    /* pieces outside radius (x, y) fly outwards */
#define FX_DEATH1_PIECE_FRAMES      0x40 /* a broken piece's flight (BREAK_SWEEP_X / Z, OUTSIDE), */
#define FX_DEATH1_PIECE_LONG_FRAMES 0x80 /* (BREAK_LEVEL, COLUMN) */
#define FX_DEATH1_T_FLASH       0    /* countdown: the opening additive flash fades */
#define FX_DEATH1_T_GLOW        1    /* countdown: a constant additive grey */
#define FX_DEATH1_T_ARCS        2    /* countdown: lightning arcs, of the kind in T_ARC_KIND */
#define FX_DEATH1_T_ARC_KIND    3
#define FX_DEATH1_T_SWEEP       4    /* countdown: the trigger sweeps round the floor, breaking three splits0 models */
#define FX_DEATH1_T_SWEEP_FIRST 5    /* the first of those three */
#define FX_DEATH1_T_LEVEL_A     6    /* countdown: a level descends through splits1[0] */
#define FX_DEATH1_T_LEVEL_B     7    /* countdown: a level rises through splits1[1], a column crosses splits2 */
#define FX_DEATH1_T_SLICE       8    /* countdown: splits3 drawn, */
#define FX_DEATH1_T_SLICE_RUN   9    /* while this runs a slice crosses them */
#define FX_DEATH1_T_BURST       10   /* countdown: splits4 break from the outside in */
#define FX_DEATH1_T_ZOOM        11   /* countdown: screen-grid zoom */
#define FX_DEATH1_T_WHITE       12   /* countdown: the closing white fades */
#define FX_DEATH1_T_CRACKS      13   /* countdown: the floor texture steps through four crack frames */
#define FX_DEATH1_FLASH_FRAMES  0x10
#define FX_DEATH1_RINGS_FRAME   8    /* TEAL_RINGS and GOLD_RAYS start */
#define FX_DEATH1_NEBULA_FRAME  0x10 /* DARK_NEBULA starts, T_GLOW */
#define FX_DEATH1_GLOW_FRAMES   0x90 /* (cut short at ARCS_FRAME) */
#define FX_DEATH1_ARCS_FRAME    0x30 /* LIGHTNING_TRAILS start, scattered arcs */
#define FX_DEATH1_ARCS_FRAMES   0x40
#define FX_DEATH1_SWEEP_1_FRAME 0x60 /* the four emitters stop; first sweep (splits0[0..2]), GREEN_SMOKE and ROCK_DEBRIS follow the trigger */
#define FX_DEATH1_SWEEP_2_FRAME 0x9C /* second sweep (splits0[3..5]) */
#define FX_DEATH1_SWEEP_3_FRAME 0xD8 /* third sweep (splits0[6..8]) */
#define FX_DEATH1_SWEEPS        3
#define FX_DEATH1_SWEEP_MODELS  3    /* splits0 models broken by one sweep */
#define FX_DEATH1_SWEEP_FRAMES  0x3C
#define FX_DEATH1_SWEEP_3_FRAMES 0x4B
#define FX_DEATH1_SWEEP_RADIUS  0x1A08 /* the trigger's circle */
#define FX_DEATH1_SWEEP_DRAW_FRAMES   0x40 /* splits0[0..5] drawn this long once their sweep starts, */
#define FX_DEATH1_SWEEP_3_DRAW_FRAMES 0x64 /* splits0[6..8] */
#define FX_DEATH1_CRACKS_FRAMES 0x20 /* T_CRACKS, loaded at each sweep; a frame copied at these frames left: */
#define FX_DEATH1_CRACK_1_LEFT  0x1F
#define FX_DEATH1_CRACK_2_LEFT  0x17
#define FX_DEATH1_CRACK_3_LEFT  0x13
#define FX_DEATH1_CRACK_4_LEFT  0xF
#define FX_DEATH1_FLOOR_VRAM_X  0x380 /* btlfx_drawScrollMove rect: 0x50 x 0x50 8-bit texels of page 0x8E (the split models' texture list) */
#define FX_DEATH1_FLOOR_VRAM_Y  0
#define FX_DEATH1_FLOOR_VRAM_W  0x28
#define FX_DEATH1_FLOOR_VRAM_H  0x50
#define FX_DEATH1_ARCS_LIMBS_FRAME   0xFE  /* limb arcs (trailBones1) */
#define FX_DEATH1_LIMB_ARCS_FRAMES   0x48
#define FX_DEATH1_STREAKS_1_FRAME    0x106 /* YELLOW_STREAKS at bone 0x3A */
#define FX_DEATH1_BLAST_1_FRAME      0x11E /* the third sweep's emitters stop; EXPLOSIONS and ROCKS at bone 0x3A */
#define FX_DEATH1_LEVEL_A_FRAME      (FX_DEATH1_BLAST_1_FRAME + 1) /* splits1[0] split, T_LEVEL_A, T_ZOOM */
#define FX_DEATH1_LEVEL_FRAMES       0x80 /* T_LEVEL_A / T_LEVEL_B and the splits1 / splits2 draw countdowns */
#define FX_DEATH1_LEVEL_STEP         0x80 /* the level moves this far a frame, */
#define FX_DEATH1_LEVEL_TOP_Y        0x990 /* from here */
#define FX_DEATH1_LEVEL_SIDE_X       0x3CF /* x of the level point: + for splits1[0], - for splits1[1] */
#define FX_DEATH1_SWIRL_1_FRAME      0x136 /* TAN_STREAKS, ROCK_DEBRIS_2, SCREEN_TRAIL_2 at bone 0x3A (BACKDROP drawn while a screen trail runs) */
#define FX_DEATH1_ARCS_LIMBS_2_FRAME 0x14F /* limb arcs (trailBones2) */
#define FX_DEATH1_STREAKS_2_FRAME    0x157 /* YELLOW_STREAKS at bone 0x18 */
#define FX_DEATH1_BLAST_2_FRAME      0x16F /* T_LEVEL_A ends; EXPLOSIONS and ROCKS at bone 0x18 */
#define FX_DEATH1_LEVEL_B_FRAME      (FX_DEATH1_BLAST_2_FRAME + 1) /* splits1[1] and splits2 split, T_LEVEL_B, T_ZOOM */
#define FX_DEATH1_SWIRL_2_FRAME      0x187 /* the first swirl stops; TAN_STREAKS, ROCK_DEBRIS_2, SCREEN_TRAIL at bone 0x18 */
#define FX_DEATH1_ARCS_RINGS_FRAME   0x1A8 /* ring arcs */
#define FX_DEATH1_RING_ARCS_FRAMES   0x30
#define FX_DEATH1_SLICE_FRAME        0x1C0 /* T_LEVEL_B ends; splits3 split, T_SLICE */
#define FX_DEATH1_SLICE_DRAW_FRAMES  0x3C
#define FX_DEATH1_BLAST_3_FRAME      (FX_DEATH1_SLICE_FRAME + 1) /* EXPLOSIONS and ROCKS at bone 3 */
#define FX_DEATH1_SLICE_RUN_FRAME    (FX_DEATH1_SLICE_FRAME + 2) /* T_SLICE_RUN, T_ZOOM */
#define FX_DEATH1_SLICE_FRAMES       0x22 /* T_SLICE_RUN, and a sliced piece's flight */
#define FX_DEATH1_BURST_SPLIT_FRAME  0x1E2 /* splits4 split */
#define FX_DEATH1_BURST_MODELS       3
#define FX_DEATH1_FINALE_FRAME       (FX_DEATH1_BURST_SPLIT_FRAME + 1) /* LIGHTNING_TRAILS_2, TEAL_RINGS_2, RIPPLE_RINGS, TAN_STREAKS_2 */
#define FX_DEATH1_BURST_FRAME        (FX_DEATH1_BURST_SPLIT_FRAME + 2) /* T_SLICE ends, T_BURST */
#define FX_DEATH1_HIDE_FRAME         (FX_DEATH1_BURST_SPLIT_FRAME + 3) /* the caster (unit 0x10) faded out */
#define FX_DEATH1_BURST_FRAMES       0xE0
#define FX_DEATH1_BURST_CLOSED_LEFT  0x80 /* T_BURST: the unbroken radius shrinks to 0 by these frames left */
#define FX_DEATH1_CORE_HEIGHT        0x514 /* the burst centre's height (emitPos0, camera point 1) */
#define FX_DEATH1_CORE_Z             0x800 /* and its z (from the caster) */
#define FX_DEATH1_ORANGE_RINGS_FRAME 0x264
#define FX_DEATH1_WHITE_FRAME        0x27C /* T_WHITE */
#define FX_DEATH1_WHITE_FADE_FRAMES  0x20
#define FX_DEATH1_FRAMES             0x29C /* script cue, op80 ends */
#define FX_DEATH1_ZOOM_FRAMES        0x10

typedef struct {
    s16 velX0; /* velocity */
    s16 velY2;
    s16 velZ4;
    s16 timer6; /* frames left */
    s16 rot8; /* spin */
    s16 unkA; /* write-only (random +-0x80) */
    s16 unkC; /* write-only (random +-0x80) */
    s16 scaleStepE; /* scale step */
    s16 posX10; /* center */
    s16 posY12;
    s16 posZ14;
    s16 scale16; /* scale */
} Fx1b3Piece; /* 0x18 */

typedef struct {
    s32 model0; /* model */
    s32 timer4; /* frames left */
    s32 count8[FX_DEATH1_SPLIT_GROUPS]; /* primitive counts */
    SVECTOR *verts28[FX_DEATH1_SPLIT_GROUPS]; /* vertices */
    Fx1b3Piece *pieces48[FX_DEATH1_SPLIT_GROUPS]; /* pieces */
} Fx1b3Split;

extern u8 *fx_1b3_splitBufPtr;

typedef struct {
    s32 mode0; /* mode */
    s16 posX4; /* target */
    s16 posY6;
    s16 posZ8;
    s16 padA;
    s32 radiusC; /* radius */
    s32 angle10; /* sweep angle */
    s32 spin14; /* sweep step */
} Fx1b3Blast;

extern Fx1b3Blast fx_1b3_splitTrigger;

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR points4[FX_DEATH1_ARC_POINTS]; /* drawn points */
    s32 timer4C; /* frames left */
    SVECTOR pos50[FX_DEATH1_ARC_POINTS]; /* points */
    SVECTOR vel98[FX_DEATH1_ARC_POINTS]; /* point velocities */
} Fx1b3Trail; /* 0xE0 */

typedef struct {
    s32 timers0[FX_DEATH1_TIMERS]; /* countdowns, by FX_DEATH1_T_* index */
    Pool6CEntry *emit80[FX_DEATH1_TIMERS];
    SVECTOR pos100; /* body position */
    SVECTOR pos108; /* unit position */
    SVECTOR pos110;
    SVECTOR pos118;
    SVECTOR pos120;
    SVECTOR rot128;
    SVECTOR rot130;
    SVECTOR rot138;
    SVECTOR rot140;
    SVECTOR rot148;
    SVECTOR rot150;
    s32 model158; /* model */
    SVECTOR rot15C;
    SVECTOR rot164;
    VECTOR pos16C;
    VECTOR scale17C;
    SVECTOR rot18C;
    SVECTOR rot194;
    VECTOR pos19C;
    VECTOR pos1AC;
    VECTOR scale1BC;
    SVECTOR rot1CC;
    VECTOR pos1D4;
    VECTOR scale1E4;
    SVECTOR pos1F4;
    SVECTOR rot1FC;
    SVECTOR rot204;
    VECTOR pos20C;
    VECTOR pos21C;
    VECTOR scale22C;
    SVECTOR rot23C;
    VECTOR pos244;
    VECTOR scale254;
} Fx1b3Work80; /* 0x264 */

extern s32 fx_1b3_trailBones0[];
extern s32 fx_1b3_trailBones1[];
extern s32 fx_1b3_trailBones2[];
extern BtlTexInfo fx_1b3_casterTexInfo;
extern BtlTexInfo fx_1b3_casterTexPos;
extern u8 *fx_1b3_splitBuf;
extern Fx1b3Trail *fx_1b3_trails;
extern s32 fx_1b3_op80Trails0;
extern s32 fx_1b3_op80Trails1;
extern Fx1b3Split fx_1b3_splits0[FX_DEATH1_SWEEPS * FX_DEATH1_SWEEP_MODELS];
extern Fx1b3Split fx_1b3_splits1[2];
extern Fx1b3Split fx_1b3_splits2;
extern Fx1b3Split fx_1b3_splits3[2];
extern Fx1b3Split fx_1b3_splits4[FX_DEATH1_BURST_MODELS];
extern SVECTOR fx_1b3_emitPos0;
extern BtlModelBuf fx_1b3_effModels[];

void fx_1b3_splitModel(Fx1b3Split *sp);
void fx_1b3_updateSplitPieces(Fx1b3Split *sp);

/* copied */
extern Unk800F78A4 *fx_1b3_env;
/* end copied */

#endif
