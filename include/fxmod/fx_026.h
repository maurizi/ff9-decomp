#ifndef FX_026_H
#define FX_026_H

/* Effect module fx_026 (dir 13 file 0x26): Shiva, full summon. Evidence: btlfx_start and btlfx_loadFile get the same id for summons (docs/runtime-findings.md). */
#include "btlfx_mod.h"

/* fx_026 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_SHIVA_STREAK_PATH      0x2 /* track: path of the ice streak strips (op82 pool items) */
#define FX_SHIVA_ICE_CRESCENTS    0x4 /* emitter: blue ice crescent sprites + small shard sprites */
#define FX_SHIVA_ICE_SHARD_BURST  0x5 /* emitter: four ice-crescent sprite elements + cyan-white gouraud rings */
#define FX_SHIVA_ICE_CHUNKS       0x6 /* emitter: falling ice chunk sprites, faint sparkles */
#define FX_SHIVA_STAR_GLINTS      0x7 /* emitter: blue glows, white / green star glints, blue glow domes */
#define FX_SHIVA_PATH             0xB /* track: keyframed position (camera point 2) in op80 state 1 */
#define FX_SHIVA_PATH_2           0xE /* track: keyframed position (camera point 2) in op80 state 2 */
#define FX_SHIVA_PATH_3           0x11 /* track: keyframed position in op80 state 3 */
#define FX_SHIVA_ICE_SPRAY        0x17 /* emitter: white ice chunk sprites */
#define FX_SHIVA_ICE_CHUNKS_2     0x19 /* emitter: falling ice chunk sprites, faint sparkles */
#define FX_SHIVA_SHATTER_RINGS    0x1A /* emitter: blue gouraud rings + ice chunk sprites, sparks (pool item per piece) */
#define FX_SHIVA_BLUE_TRAIL       0x1F /* emitter: spiralling trail, blue strip texture */
#define FX_SHIVA_ICE_DEBRIS       0x20 /* emitter: small ice chunk sprites */
#define FX_SHIVA_ICE_DEBRIS_2     0x21 /* emitter: small ice chunk sprites */
#define FX_SHIVA_CYAN_RING        0x22 /* emitter: cyan-white gouraud ring */
#define FX_SHIVA_ICE_STREAK_3     0x34 /* trail: blue ice strip (third streak kind) */
#define FX_SHIVA_ICE_STREAK_2     0x35 /* trail: blue ice strip (second streak kind) */
#define FX_SHIVA_ICE_STREAK       0x36 /* trail: blue ice strip */
#define FX_SHIVA_ICICLE           0x85 /* 'so' model: long pale-blue marbled icicle, registered at the end of op80 state 1 (summon data) */
#define FX_SHIVA_SHARD_SPRAY      0x86 /* emitter: light / dark blue gouraud rings, white ice-shard sprites and dots, started at each op80 pool item's track point (summon data) */
#define FX_SHIVA_ICICLE_2         0x87 /* 'so' model: the same icicle, dark marbled ice texture, registered with 0x85 (summon data) */
#define FX_SHIVA_STREAK_PATH_2    0x9D /* track: path of the ice streak strips (op82 pool items) (summon data 0x80+, not analysed by the tool) */
#define FX_SHIVA_ICE_SHATTER_2    0x9E /* emitter: white ice-shard and ice-block sprites at the target, op82 frame 39 (summon data) */
#define FX_SHIVA_ICE_SHATTER      0x9F /* emitter: white ice-shard and ice-block sprites at the target, op82 frame 39 (summon data) */
#define FX_SHIVA_FROST_RINGS      0xA0 /* rings: white / blue gouraud rings drawn growing at summon bone 0x19 (summon data) */
#define FX_SHIVA_STAR_GLINTS_2    0xA1 /* emitter: blue glows, white / green star glints, blue glow dome at summon bone 0, op80 state 2 start (summon data) */
#define FX_SHIVA_GLOW_DOMES       0xA2 /* emitter: blue glows, faint streaks, blue glow domes at summon bone 0, op80 state 1 frame 38 (summon data) */
#define FX_SHIVA_GLOW_DOMES_2     0xA3 /* emitter: blue glows, faint streaks, blue glow domes at summon bone 0, op80 state 1 frame 42 (summon data) */
#define FX_SHIVA_GLOW_DOMES_3     0xA4 /* emitter: blue glows, faint streaks, blue glow domes at summon bone 0, op80 state 2 end (summon data) */
#define FX_SHIVA_ICE_CUBES        0xA5 /* emitter: blue ice-cube sprites at summon bone 0x19, op80 state 3 (summon data) */
#define FX_SHIVA_ICE_BURST        0xA6 /* emitter: white ice-chunk burst sprites at summon bone 0x19, op80 state 2 frame 29 (summon data) */
#define FX_SHIVA_ICE_VORTEX       0xAB /* 'so' model: blue swirling ice cone (spiral seen from above), drawn spinning and widening at the target (op82) (summon data) */

/* fx_026 choreography (docs/fxdata.md, "Choreography literals"): op80's states, lengths, keyframes and icicle
 * pool; op82's streaks, vortex and keyframes; op81's barrier pieces, shatter, tints and VRAM scrolls. */
#define FX_SHIVA_STATE_INTRO    0 /* op80: FX_SHIVA_ICE_CHUNKS_2 / ICE_SPRAY / ICE_CRESCENTS start */
#define FX_SHIVA_STATE_ENTER    1 /* the summon's motion starts; it is drawn along FX_SHIVA_PATH */
#define FX_SHIVA_STATE_CHARGE   2 /* the summon moves along FX_SHIVA_PATH_2, FX_SHIVA_FROST_RINGS grow at its bone 0x19 */
#define FX_SHIVA_STATE_ICICLES  3 /* along FX_SHIVA_PATH_3; the icicle pool items fly */
#define FX_SHIVA_INTRO_FRAMES   40 /* FX_SHIVA_STATE_INTRO's length */
#define FX_SHIVA_ENTER_FRAMES   66 /* FX_SHIVA_STATE_ENTER's length; at its end the icicle models are registered */
#define FX_SHIVA_CHARGE_FRAMES  50 /* FX_SHIVA_STATE_CHARGE's length (the frost rings' ramp) */
#define FX_SHIVA_ICICLES_FRAMES 29 /* FX_SHIVA_STATE_ICICLES' length (the op ends) */
#define FX_SHIVA_SUMMON_SHOW_FRAME  2  /* FX_SHIVA_STATE_ENTER: the summon is drawn from this frame */
#define FX_SHIVA_GLOW_DOMES_FRAME   38 /* FX_SHIVA_STATE_ENTER: FX_SHIVA_GLOW_DOMES start */
#define FX_SHIVA_GLOW_DOMES_2_FRAME 42 /* FX_SHIVA_STATE_ENTER: FX_SHIVA_GLOW_DOMES_2 start */
#define FX_SHIVA_ICE_BURST_FRAME    29 /* FX_SHIVA_STATE_CHARGE: FX_SHIVA_ICE_BURST starts */
#define FX_SHIVA_TEX_ANIM_FRAME     32 /* FX_SHIVA_STATE_CHARGE: the summon's texture animation 1 starts */
#define FX_SHIVA_ICICLES_FRAME      4  /* FX_SHIVA_STATE_ICICLES: one icicle pool item spawns per frame from here */
#define FX_SHIVA_ICICLE_COUNT       6  /* icicle pool items (op80 poolCount), each on its own fx_026_op80TrackBlocks track */
#define FX_SHIVA_ICICLE_SHOW_FRAME  4  /* icicle pool item: drawn, and FX_SHIVA_SHARD_SPRAY started, from this frame of its track */
#define FX_SHIVA_MODEL_COUNT        0x20 /* btlfx_initEffModels slots */
/* op82: three kinds of FX_SHIVA_STREAKS ice-streak strips round the vortex, each strip a pool item */
#define FX_SHIVA_STREAKS            6    /* strips per kind (the path tables hold 6 tracks per kind) */
#define FX_SHIVA_STREAK_POOL_COUNT  0x18 /* op82 poolCount */
#define FX_SHIVA_STREAK_KIND_1      0    /* FX_SHIVA_ICE_STREAK along FX_SHIVA_STREAK_PATH */
#define FX_SHIVA_STREAK_KIND_2      1    /* FX_SHIVA_ICE_STREAK_2 along FX_SHIVA_STREAK_PATH_2 */
#define FX_SHIVA_STREAK_KIND_3      2    /* FX_SHIVA_ICE_STREAK_3 along STREAK_PATH_2's tracks 6-11, morphing to 12-17 */
#define FX_SHIVA_STREAK_POINTS      32   /* points of a kind 1 / 2 strip (Fx026Strip32) */
#define FX_SHIVA_STREAK_3_POINTS    40   /* points of a kind 3 strip (Fx026Strip40) */
#define FX_SHIVA_STREAK_WIDTH       0x46 /* half-width of the kind 1 / 2 strips */
#define FX_SHIVA_STREAK_3_WIDTH     0x58 /* half-width of the kind 3 strips */
#define FX_SHIVA_STREAK_FADE_FRAME  16   /* kind 1: fades out from this frame, */
#define FX_SHIVA_STREAK_FRAMES      32   /* ends at this one */
#define FX_SHIVA_STREAK_2_FADE_FRAME 8   /* kind 2: fades out from this frame, */
#define FX_SHIVA_STREAK_2_FRAMES    24   /* ends at this one */
#define FX_SHIVA_STREAK_3_MORPH_FRAME 10 /* kind 3: morphs towards its second track from this frame, */
#define FX_SHIVA_STREAK_3_MORPH_FRAMES 30 /* over this many (a quarter cosine) */
#define FX_SHIVA_STREAK_3_FADE_FRAME 24  /* fades out from this frame, */
#define FX_SHIVA_STREAK_3_FRAMES    40   /* ends at this one */
#define FX_SHIVA_STREAKS_FRAME      13   /* op82: the kind 1 strips and FX_SHIVA_ICE_SHARD_BURST start */
#define FX_SHIVA_VORTEX_FRAME       39   /* op82: FX_SHIVA_ICE_VORTEX is registered and drawn (spinning, widening, fading), */
#define FX_SHIVA_VORTEX_FRAMES      53   /* for this many frames; the ice shatter emitters and kind 2 / 3 strips start */
#define FX_SHIVA_VORTEX_OP_FRAMES   100  /* op82: the task ends at this frame */
/* FX_SHIVA_ICE_VORTEX samples the 8-bit page (0x200, 0x100) from v 128 (tpage 0x98, VRAM (0x200, 0x180)); while it
 * is drawn, fxm_drawScrollMoveX copies the 0x80 x 0x40 band at (SRC_X, SRC_Y) into the rect below it, scrolled
 * horizontally. */
#define FX_SHIVA_VORTEX_VRAM_X      0x200
#define FX_SHIVA_VORTEX_VRAM_Y      0x1C0
#define FX_SHIVA_VORTEX_VRAM_W      0x80
#define FX_SHIVA_VORTEX_VRAM_H      0x40
#define FX_SHIVA_VORTEX_SRC_X       0x200
#define FX_SHIVA_VORTEX_SRC_Y       0x180
/* op81: the ice barrier, FX_SHIVA_PIECE_COUNT pieces (three morph stages each) and the core, which shatter */
#define FX_SHIVA_BARRIER_STATE_BUILD   0  /* op81: the pieces fly in (pool items) */
#define FX_SHIVA_BARRIER_STATE_SHATTER 1  /* the shatter line sweeps down, the pieces and the core break */
#define FX_SHIVA_BUILD_FRAMES          16 /* FX_SHIVA_BARRIER_STATE_BUILD's length */
#define FX_SHIVA_SHATTER_OP_FRAMES     126 /* FX_SHIVA_BARRIER_STATE_SHATTER's length (the op ends) */
#define FX_SHIVA_BARRIER_POOL_COUNT    0x4A /* op81 poolCount */
#define FX_SHIVA_PIECE_COUNT           6  /* barrier pieces (pool items 0-5, fx_026_pieceModelBlocks rows) */
#define FX_SHIVA_PIECE_STAGES          3  /* morph-stage models per piece (fx_026_pieceStages picks them) */
#define FX_SHIVA_PIECE_CORE            6  /* pool item piece index of the core (model6C[6]) */
#define FX_SHIVA_PIECE_STATE_FLY       0  /* piece pool item: flies along its fx_026_pieceTrackBlocks track, */
#define FX_SHIVA_PIECE_FLY_FRAMES      3  /* for this many frames; then a blue tint and FX_SHIVA_SHATTER_RINGS */
#define FX_SHIVA_PIECE_STATE_STAND     1  /* grows through its stages (FX_SHIVA_PIECE_FORM_FRAMES) until the shatter */
#define FX_SHIVA_PIECE_STATE_SHATTER   2  /* breaks into shards and fades out */
#define FX_SHIVA_PIECE_FORM_FRAMES     16 /* FX_SHIVA_PIECE_STATE_STAND: fx_026_pieceStages entries; then the piece's model6C is drawn too */
#define FX_SHIVA_PIECE_FADE_FRAME      14 /* FX_SHIVA_PIECE_STATE_SHATTER: fades out from this frame, */
#define FX_SHIVA_PIECE_FADE_FRAMES     6  /* over this many; the item ends */
#define FX_SHIVA_PIECE_SHARD_LIFE      0x14 /* frames a piece's shard flies (fxm_shatterInit life) */
#define FX_SHIVA_CORE_STATE_STAND      0  /* core pool item: fades in (8 frames), waits for the shatter */
#define FX_SHIVA_CORE_STATE_SHATTER    1  /* breaks into shards */
#define FX_SHIVA_CORE_FADE_FRAME       55 /* FX_SHIVA_CORE_STATE_SHATTER: fades out from this frame (over 32, << 12 >> 5), */
#define FX_SHIVA_CORE_FRAMES           87 /* ends at this one */
#define FX_SHIVA_CORE_SHARD_LIFE       0x57 /* frames a core shard flies */
#define FX_SHIVA_PIECE_TINT_RGB        0, 0x20, 0x46 /* additive screen tint (blue) as a piece lands */
#define FX_SHIVA_SHATTER_LINE_Y        0x400 /* op81: the shatter line's start height; it sweeps up 0x80 per frame, triangles below it break off */
#define FX_SHIVA_CYAN_RING_FRAME       18 /* FX_SHIVA_BARRIER_STATE_SHATTER: FX_SHIVA_CYAN_RING starts */
#define FX_SHIVA_DARKEN_FRAME          19 /* the screen is darkened (FX_SHIVA_DARKEN_RGB) */
#define FX_SHIVA_SHATTER_FRAME         20 /* the pieces 0-4 and the core shatter, the debris and blue trail start, blue flash */
#define FX_SHIVA_SHATTER_5_FRAME       24 /* piece 5 shatters */
#define FX_SHIVA_STAR_GLINTS_FRAME     32 /* FX_SHIVA_STAR_GLINTS start */
#define FX_SHIVA_ICE_CHUNKS_FRAME      44 /* FX_SHIVA_ICE_CHUNKS start */
#define FX_SHIVA_FLASH_FADE_FRAMES     16 /* the blue flash fades over this many frames ((frame - SHATTER_FRAME) << 7 >> 4) */
#define FX_SHIVA_DARKEN_RGB            0x80, 0x80, 0x80 /* subtractive screen tint */
#define FX_SHIVA_FLASH_RGB             0, 0x80, 0xFF    /* additive screen tint (blue) */
#define FX_SHIVA_FLASH_LEVEL           0x80 /* the fading flash's blue (green = blue / 4) */
#define FX_SHIVA_BLUE_TRAIL_TRAIL_SLOTS  0xC  /* FX_SHIVA_BLUE_TRAIL's trail buffer fx_026_trails: slots of */
#define FX_SHIVA_BLUE_TRAIL_TRAIL_STRIDE 0x4C /* this many bytes (Pool6CEntry trailSlots / trailStride) */
/* The icicles' 8-bit page (tpage 0x9A, (0x280, 0x100)): FX_SHIVA_ICICLE_2 samples u 0 v 0, FX_SHIVA_ICICLE u 128 v 0.
 * Every op81 frame btlfx_drawScrollMove copies (ICICLE_2_SRC_X, _Y) into ICICLE_2's rect, scrolled; in the shatter
 * state it also copies FX_SHIVA_ICICLE's texture into the v 128 rect below it (BARRIER_VRAM; the barrier piece
 * models are not analysed by fxdata, look pending). */
#define FX_SHIVA_ICICLE_2_VRAM_X       0x280
#define FX_SHIVA_ICICLE_2_VRAM_Y       0x100
#define FX_SHIVA_ICICLE_2_VRAM_W       0x40
#define FX_SHIVA_ICICLE_2_VRAM_H       0x80
#define FX_SHIVA_ICICLE_2_SRC_X        0x280
#define FX_SHIVA_ICICLE_2_SRC_Y        0x180
#define FX_SHIVA_BARRIER_VRAM_X        0x2C0
#define FX_SHIVA_BARRIER_VRAM_Y        0x180
#define FX_SHIVA_BARRIER_VRAM_W        0x40
#define FX_SHIVA_BARRIER_VRAM_H        0x80
#define FX_SHIVA_BARRIER_SRC_X         0x2C0
#define FX_SHIVA_BARRIER_SRC_Y         0x100

/* keyframe track read from the effect file: count, then the keys */
typedef struct {
    s16 unk0; /* not read by the module */
    s16 count2; /* count */
    s16 keys4[1]; /* keys: 1, 3, 6 or 9 halfwords each */
} Fx026Track;

/* shattered model: triangles (three vertices each) that fall off as particles */
typedef struct {
    SVECTOR vert0; /* pad: already split off */
    SVECTOR vert8;
    SVECTOR vert10;
} Fx026Tri; /* 0x18 */

typedef struct {
    s16 life0;
    s16 triCount2; /* triangle count */
    s32 lastCount4; /* triangles of the last model */
    Fx026Tri *tris8;
    Fx026Tri *nextTriC; /* next free triangle */
    Fx026Tri *lastTris10; /* first triangle of the last model */
} Fx026Shatter; /* 0x14 */

typedef struct {
    s32 x0; /* position (fixed 20.12) */
    s32 y4;
    s32 z8;
    s16 velC; /* velocity */
    s16 velE;
    s16 vel10;
    s16 drag12; /* velocity factor per frame (btlfx_stepModuleParticle), 0x1000 = none */
    s16 spin14; /* spin */
    s16 spin16;
    s16 frame18; /* frame, -1 = free */
    s16 tri1A; /* triangle */
} Fx026Shard; /* 0x1C */

typedef struct {
    s32 path0; /* camera path */
    SVECTOR target4; /* camera target */
    SVECTOR posC; /* camera position */
} Fx026Work87; /* 0x14 */

/* camera paths: position track, target track, length */
typedef struct {
    u16 posTrack0;
    u16 targetTrack2;
    u16 length4;
    u16 unk6; /* not read by the module */
} Fx026CamPath;

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx026Vec3;

/* pool item of fx_026_op80PoolTask: a model following a keyframe track */
typedef struct {
    s16 done0; /* done */
    u16 spin2; /* spin */
    s16 track4; /* track: fx_026_op80TrackBlocks index */
    s16 pad6;
    Pool6CEntry *emit8;
    SVECTOR posC;
} Fx026Item;

typedef struct {
    s32 state0;
    s32 frame4; /* frame */
    u8 pad8[8];
    SVECTOR pos10;
    u8 pad18[0x10];
    SVECTOR pos28; /* summon bone positions */
    SVECTOR pos30;
    SVECTOR pos38;
    Fx026Vec3 pos40; /* summon position */
    Fx026Vec3 scale4C; /* summon scale */
    SVECTOR rot58; /* summon rotation */
    u8 pad60[4];
    s32 unk64; /* unused (runtime watch, round 33) */
    s32 model68; /* models */
    s32 model6C;
} Fx026Work80; /* 0x70 */

/* the summon slot behind env->loader */
typedef struct {
    u8 pad0[0x54];
    struct {
        u8 pad0[0x180];
        BtlMotion *motion180;
    } *summon54; /* g_btlfxLoader.summon */
    u8 pad58[8];
    s32 model60; /* summon model */
} Fx026Summon;

/* op82: the barrier model */
typedef struct {
    s32 unk0; /* cleared at init and tested for 0, never set (always 0) */
    s32 frame4; /* frame */
    SVECTOR pos8; /* position */
    s32 model10; /* model */
} Fx026Work82; /* 0x14 */

/* pool item of fx_026_op82PoolTask */
typedef struct {
    s16 unk0; /* write-only (cleared at spawn) */
    s16 strip2; /* index */
    s16 kind4; /* type */
    s16 pad6;
    s32 unk8; /* write-only (cleared at spawn) */
} Fx026Item82;

/* line strip passed to btlfx_drawTrail */
typedef struct {
    s16 vx;
    s16 vy;
    s16 vz;
    u16 halfWidth6; /* half-width of the ribbon at this point (scales the edge normal; 0 = gap) */
} Fx026Pt;

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    Fx026Pt pts4[FX_SHIVA_STREAK_POINTS]; /* ring points (btlfx_pushTrailPoint) */
} Fx026Strip32; /* 0x104 */

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    Fx026Pt pts4[FX_SHIVA_STREAK_3_POINTS]; /* ring points (btlfx_pushTrailPoint) */
} Fx026Strip40; /* 0x144 */

extern Fx026Strip32 fx_026_strips0[];
extern Fx026Strip32 fx_026_strips1[];
extern Fx026Strip40 fx_026_strips2[];
/* op81: the shattering barrier */
typedef struct {
    s32 state0;
    s32 frame4; /* frame */
    SVECTOR pos8; /* position */
    SVECTOR pos10;
    s32 pieceModels18[FX_SHIVA_PIECE_COUNT][FX_SHIVA_PIECE_STAGES]; /* models of the six pieces */
    s32 pad60[3];
    s32 model6C[FX_SHIVA_PIECE_COUNT + 1];
} Fx026Work81; /* 0x88 */

/* pool item of fx_026_op81PoolTask */
typedef struct {
    s16 state0; /* state */
    s16 piece2; /* piece */
    s32 pad4[2];
    SVECTOR posC;
} Fx026Item81;

typedef struct {
    s32 setOfs0; /* offsets from env->workBuf */
    s32 trisOfs4;
} Fx026ShatterSrc;

extern s32 fx_026_pieceModelBlocks[FX_SHIVA_PIECE_COUNT][FX_SHIVA_PIECE_STAGES];
extern s32 fx_026_pieceTrackBlocks[];
extern SVECTOR fx_026_pieceOffsets[];
extern s32 fx_026_pieceStages[];
extern Fx026ShatterSrc fx_026_shatterBufs[];
extern s32 fx_026_op80Model;
extern s32 fx_026_op80Model2;
extern Fx026Shatter fx_026_shatterBuf6[];
extern Fx026Tri fx_026_shatterTris6[];
extern s32 fx_026_op81ModelBlocks[FX_SHIVA_PIECE_COUNT + 1];
extern SVECTOR fx_026_op81Pos;
extern s32 fx_026_pieceOrder[];
extern s32 fx_026_shatterNow;
extern s32 fx_026_shatterNow6;
extern s32 fx_026_shatterNow5;
extern u8 fx_026_trails[];
extern s16 fx_026_op80TrackBlocks[];
extern BtlModelBuf fx_026_effModels[];
extern SVECTOR fx_026_stripOrigin;
extern Fx026CamPath fx_026_camPaths[];
extern Unk800F78A4 *fx_026_env;
extern Fx026Shatter *fx_026_shatterSets[8];
extern Fx026Shard *fx_026_shatterShards[8];
extern s32 fx_026_shatterLine; /* shatter line */

static void fxm_keyGetVec(Fx026Track *track, s32 t, SVECTOR *out);
static void fxm_keyGetVec2(Fx026Track *track, s32 t, SVECTOR *out0, SVECTOR *out1);
static void fxm_keyGetVec3(Fx026Track *track, s32 t, SVECTOR *out0, SVECTOR *out1, SVECTOR *out2);
static s32 fxm_keyGetValue(Fx026Track *track, s32 t);
static void fxm_drawScrollMoveX(s32 x, s32 dx, s32 dy, s32 w, s32 h, s32 sx, s32 sy);
static void fxm_shatterInit(s32 slot, s32 life, Fx026Shatter *buf, Fx026Tri *tris);
static s32 fxm_shatterAddModel(s32 slot, s32 model);
static Fx026Shard *fxm_shatterFreeShard(s32 slot);
static void fxm_shatterUpdate(s32 slot);
static s32 fxm_shatterRelease(s32 slot);
static s32 fx_026_op81PoolTask(s32 mode, Fx026Item81 *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame,
                  Unk800DA1B8 *pool, Fx026Work81 *work);
static s32 fx_026_op82PoolTask(s32 mode, Fx026Item82 *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame,
                  Unk800DA1B8 *pool, Fx026Work82 *work);

#endif
