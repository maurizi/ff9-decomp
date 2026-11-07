#ifndef FX_0E2_H
#define FX_0E2_H

/* Effect module fx_0e2 (dir 13 file 0x0E2): Fenrir (2nd), drawn with the summon model. Strips along key tracks
 * (btlfx_drawTrail), projected ribbons (fxm_projectEdges, fxm_drawEdgeQuads), tower and piece models and camera key
 * tracks (fxm_camChanUpdate). Evidence: [data] the party ability / item tables and enemy sequences that play this
 * file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0e2 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s3.md; "(look pending)" = by kind, appearance not settled). */
#define FX_FENRIR_RIBBON_PATHS_C     0x1 /* table: position paths of ribbons 2/3 (fxm_offsetTableEntry, fxm_posPathInit) */
#define FX_FENRIR_GREEN_RINGS        0x4 /* emitter: pale green gouraud rings, 64 frames */
#define FX_FENRIR_RIBBON_PATHS_A     0x9 /* table: position paths of ribbon 0 */
#define FX_FENRIR_CAM_TRACK_1        0xA /* table: camera key track, channel 1 (fxm_camChanAdd) */
#define FX_FENRIR_CAM_TRACK_2        0xB /* table: camera key track, channel 2 (fxm_camChanAdd) */
#define FX_FENRIR_RIBBON_PATHS_B     0xC /* table: position paths of ribbon 1 */
#define FX_FENRIR_SAND_RINGS         0xD /* emitter: brown-salmon gouraud rings + spark sprites (texture not in the file) */
#define FX_FENRIR_GREEN_WIND_TRAIL   0xE /* emitter: spiralling green wind-streak trail + grey smoke */
#define FX_FENRIR_RIBBON_PATHS_D     0xF /* table: position paths of ribbons 2/3 (other branch) */
#define FX_FENRIR_GREEN_RINGS_2      0x10 /* emitter: white-green / pale green gouraud rings */
#define FX_FENRIR_WIND_RING          0x11 /* 'so' model: flat ring with a green wind texture */
#define FX_FENRIR_CYAN_GLINTS        0x12 /* emitter: spiral of white dot sparks; at run time (fx_1fd's byte-identical entry) tiny cyan-white glints round the green whirlwind; runtime (docs/runtime-findings.md effect-data pass 2, medium) */
#define FX_FENRIR_GREEN_WIND_STRIP   0x17 /* trail: green wind-streak strip texture */
#define FX_FENRIR_WIND_RING_2        0x1D /* 'so' model: flat ring with a green wind texture (same shape as WIND_RING) */
#define FX_FENRIR_WIND_VORTEX        0x1F /* 'so' model: swirl of green wind blades (vortex), drawn with WIND_SHELL */
#define FX_FENRIR_WIND_SHELL         0x21 /* 'so' model: cone-shaped shell with a green wind texture */
#define FX_FENRIR_ROCK_SPIKE         0x80 /* summon data: 'so' model, rock spike with a rock texture, drawn sliced as falling blocks (Hi_DrawSliceEffModel) */
#define FX_FENRIR_ROCK_SHARD         0x81 /* summon data: 'so' model, small rock shard, drawn with ROCK_SPIKES / ROCK_SHARD_2 */
#define FX_FENRIR_WIND_STREAK        0x82 /* summon data: model, long teardrop streak (Hi_RegisterTexListModel; shape only) */
#define FX_FENRIR_WIND_SERPENT       0x83 /* summon data: model, serpent-like streak with a head (shape only) */
#define FX_FENRIR_WIND_BEAST         0x84 /* summon data: model, spiky beast-shaped mass (shape only) */
#define FX_FENRIR_WIND_SPINDLE       0x85 /* summon data: model, spindle with a round end (shape only) */
#define FX_FENRIR_ROCK_SPIKES        0x87 /* summon data: 'so' model, row of rock spikes */
#define FX_FENRIR_CAM_TRACK_9        0x92 /* summon data: camera key tracks, channel 9 */
#define FX_FENRIR_ROCK_SHARD_2       0x93 /* summon data: 'so' model, small rock shard */
#define FX_FENRIR_SPIKE_BURST        0x94 /* summon data: 'so' model, morph target, grey gouraud burst of spikes */
#define FX_FENRIR_WIND_BALL          0x95 /* summon data: 'so' model, morph source, ball with a green wind texture */
#define FX_FENRIR_CAM_TRACK_4        0x96 /* summon data: camera key tracks, channel 4 */
#define FX_FENRIR_CAM_TRACK_START    0x9B /* summon data: camera key tracks of channels 3, 4, 9, 10 at the start */
#define FX_FENRIR_PIECE_TRACKS       0x9C /* summon data: rotation/position paths of the pieces (fxm_rotPosPathInit) */
#define FX_FENRIR_STRIP_PATHS_2      0x9D /* summon data: position paths of strip set 1 */
#define FX_FENRIR_STRIP_PATHS        0x9E /* summon data: position paths of strip set 0 (head position) */
#define FX_FENRIR_CAM_TRACK_3        0x9F /* summon data: camera key tracks, channel 3 */
#define FX_FENRIR_SMOKE              0xA0 /* summon data: emitter, grey smoke puffs */
#define FX_FENRIR_GREEN_RING_BURST   0xA2 /* summon data: emitter, cream-white / green gouraud rings */
#define FX_FENRIR_GREEN_RING_BURST_2 0xA3 /* summon data: emitter, olive-cream / green gouraud rings */
#define FX_FENRIR_WIND_TRAIL         0xA4 /* summon data: emitter, green screen-space trail and white dot sparks */
#define FX_FENRIR_RED_MOON           0xA5 /* summon data: emitter, one big red moon sprite */
#define FX_FENRIR_RED_MOON_GLOW      0xA6 /* summon data: emitter, red moon particle */

/* fx_0e2 choreography (docs/fxdata.md, "Choreography literals"; partial: the work sizes, trail buffers, counts and
 * primitive words; the ops' frame keys are still numeric). */
#define FX_FENRIR_MODEL_COUNT 0x20 /* btlfx_initEffModels slots */
#define FX_FENRIR_EDGES       60   /* fx_0e2_edges: the screen-edge strip's points */
#define FX_FENRIR_HEADS       5    /* op80's five wind heads: fx_0e2_emitBlocks0 / 1, emit8 / emit28 slots */
#define FX_FENRIR_WIND_TRAIL_TRAIL_SLOTS   0xC  /* FX_FENRIR_WIND_TRAIL's trail buffer fx_0e2_op80Trails1: slots of */
#define FX_FENRIR_WIND_TRAIL_TRAIL_STRIDE  0x50 /* this many bytes (Pool6CEntry trailSlots / trailStride) */
#define FX_FENRIR_HEAD_TRAIL_SLOTS         6    /* the heads' trail buffers fx_0e2_op80Trails0[slot] (Fx0e2Tex1E0): slots of */
#define FX_FENRIR_HEAD_TRAIL_STRIDE        0x50 /* this many bytes */
#define FX_FENRIR_GREEN_WIND_TRAIL_TRAIL_SLOTS  0xC  /* op84: FX_FENRIR_GREEN_WIND_TRAIL's trail buffer per unit (fx_0e2_op84Trails): slots of */
#define FX_FENRIR_GREEN_WIND_TRAIL_TRAIL_STRIDE 0x30 /* this many bytes */
/* deep pass: the ops' timelines (fx_0d2's names where fx_0e2's op does the same) */
/* op80 (the summon's timeline, camera channels 1..4, 9, 10) */
#define FX_FENRIR_CAM_SHOT_2_FRAME  0x2D  /* op80: the camera tracks' second keys start (the first at frame 0), */
#define FX_FENRIR_CAM_SHOT_3_FRAME  0x41  /* the third, */
#define FX_FENRIR_CAM_SHOT_4_FRAME  0x96  /* the fifth (tracks' entry 4), */
#define FX_FENRIR_CAM_SHOT_5_FRAME  0xEF  /* the sixth, */
#define FX_FENRIR_CAM_SHOT_6_FRAME  0x10E /* the seventh (channels 1, 2); the summon / ribbons stop following the camera points here */
#define FX_FENRIR_SUMMON_FRAME      0x2E  /* op80: summon motion 0, RED_MOON_GLOW starts; the summon model is drawn from here */
#define FX_FENRIR_SUMMON_FRAMES     0x83  /* for this many frames */
#define FX_FENRIR_SUMMON_DIM_FRAMES 0x14  /* darkened for the first this many, */
#define FX_FENRIR_SUMMON_ADD_FRAME  0x69  /* additive from this many frames in */
#define FX_FENRIR_SPIKES_FRAMES     0x14  /* ROCK_SHARD and ROCK_SPIKES drawn from SUMMON_FRAME this long, then ROCK_SHARD_2 */
#define FX_FENRIR_ROCKS_FRAMES      0x69  /* until this many frames in (not while the drops fall) */
#define FX_FENRIR_RED_MOON_FRAME    0x41  /* op80: RED_MOON starts (RED_MOON_GLOW stops the frame after) */
#define FX_FENRIR_MOTION_1_FRAME    0x97  /* op80: summon motion 1; the rock models are freed, the drops fall until DROPS_END_FRAME */
#define FX_FENRIR_RED_MOON_STOP_FRAME 0x9B /* op80: RED_MOON stops */
#define FX_FENRIR_MORPH_FRAME       0xA1  /* op80: the summon fades out, the wind models (WIND_BEAST -> WIND_SPINDLE, WIND_SERPENT
                                           * -> WIND_STREAK morphs) fade in and are drawn until MORPH_END_FRAME (then freed) */
#define FX_FENRIR_MORPH_END_FRAME   0xBD
#define FX_FENRIR_SUMMON_FADE_FRAMES 0x10 /* fadeTimer140: the summon darkens to nothing */
#define FX_FENRIR_WIND_FADE_IN_FRAMES 0x10 /* fadeInTimer144: the wind models brighten */
#define FX_FENRIR_BEAST_MORPH_FRAME 8     /* MORPH_FRAME + this: WIND_BEAST starts morphing into WIND_SPINDLE, */
#define FX_FENRIR_SERPENT_MORPH_FRAME 0xC /* + this: WIND_SERPENT into WIND_STREAK, */
#define FX_FENRIR_WIND_MORPH_FRAMES 12    /* each over this many frames */
#define FX_FENRIR_WIND_TRAIL_FRAME  0xA3  /* op80: FX_FENRIR_WIND_TRAIL starts (follows the ribbon origin) */
#define FX_FENRIR_STRIPS_FRAME      0xB1  /* op80: the wind models fade out; the five GREEN_WIND_STRIPs blend along the strip tracks */
#define FX_FENRIR_WIND_FADE_OUT_FRAMES 0xC /* fadeOutTimer148: the wind models darken */
#define FX_FENRIR_RING_BURST_FRAME  0xB3  /* op80: GREEN_RING_BURST(_2) start at the first head */
#define FX_FENRIR_HEAD_EMIT_FRAME   0xB5  /* op80: the heads' emitters (fx_0e2_emitBlocks1) start, stopped at STRIPS_PATH_FRAME */
#define FX_FENRIR_STRIPS_HOLD_FRAME 0xCD  /* op80: the heads follow STRIP_PATHS */
#define FX_FENRIR_STRIPS_PATH_FRAME 0xF0  /* op80: the strips grow along STRIP_PATHS_2 with WIND_SHELL / WIND_VORTEX heads; drops freed */
#define FX_FENRIR_HEAD_TRAILS_FRAME 0xF8  /* op80: the heads' trail emitters (fx_0e2_emitBlocks0) start */
#define FX_FENRIR_STRIPS_END_FRAME  0x10F
#define FX_FENRIR_FRAMES            0x14A /* op80's length (the op ends); also the wind texture scroll's start offset (x speed) */
#define FX_FENRIR_STRIP_KEYS_FRAME  8     /* STRIPS_FRAME + this: the strips blend between successive track keys, */
#define FX_FENRIR_STRIP_KEYS        4     /* fx_0e2_stripTrackBlocks: track keys (the last one held) */
#define FX_FENRIR_STRIP_FADE_IN_FRAMES 0x10 /* the strips fade in */
#define FX_FENRIR_BALL_MORPH_FRAME  0xC   /* STRIPS_FRAME + this: the heads' WIND_BALL morphs into SPIKE_BURST, */
#define FX_FENRIR_BALL_MORPH_FRAMES 8     /* over this many frames */
#define FX_FENRIR_STRIP_PATH_POINTS 0x11  /* points of a strip along STRIP_PATHS / _2 (fx_0e2_stripPath0 / 1 countA) */
#define FX_FENRIR_DROPS             3     /* fx_0e2_drops: falling pieces (fx_0e2_dropBlocks models, model150[11..13]), */
#define FX_FENRIR_DROP_COPIES       2     /* two copies each, */
#define FX_FENRIR_DROP_DEPTH        0x4E20 /* moving through this depth and wrapping */
#define FX_FENRIR_SUMMON_MODELS     11    /* model150: WIND_BEAST .. WIND_STREAK, WIND_BALL, SPIKE_BURST, WIND_SHELL, WIND_VORTEX,
                                           * ROCK_SHARD, ROCK_SPIKES, ROCK_SHARD_2, then the drops */
/* op81 (the rock tower) */
#define FX_FENRIR_TOWER_STAGES      3     /* fx_0e2_towerBlocks: the tower's models, model124[0..2], */
#define FX_FENRIR_TOWER_PIECES      5     /* fx_0e2_pieceBlocks / fx_0e2_pieceTracks: pieces, model124[3..7], */
#define FX_FENRIR_TOWER_BLOCKS      4     /* ROCK_SPIKE blocks at the camera points, model124[8..11] (block 0 unused) */
#define FX_FENRIR_TOWER_X           (-0x18D) /* the tower (and the pieces' tracks) sit at this x, */
#define FX_FENRIR_TOWER_Z           (-0x25E) /* z */
#define FX_FENRIR_SMOKE_FRAME       3     /* op81: FX_FENRIR_SMOKE starts at the pieces, */
#define FX_FENRIR_SMOKE_2_FRAME     16    /* again, */
#define FX_FENRIR_SMOKE_3_FRAME     30    /* and at three points round the tower */
#define FX_FENRIR_SMOKE_4_FRAME     35
#define FX_FENRIR_SMOKE_5_FRAME     38
#define FX_FENRIR_SMOKE_STOP_FRAME  46    /* op81: the smokes stop */
#define FX_FENRIR_SMOKES            5     /* op81: smoke emitters (Fx0e2Work81.emit0) */
#define FX_FENRIR_TOWER_FRAMES      0x2D  /* op81: the tower, pieces and blocks are drawn (sliced) from frame 1 this long */
#define FX_FENRIR_TOWER_STAGE_2_FRAME 5   /* frame - 1 from which the tower's second model shows, */
#define FX_FENRIR_TOWER_STAGE_3_FRAME 0x12 /* its third */
#define FX_FENRIR_TOWER_OP_FRAMES   0x30  /* op81's length */
#define FX_FENRIR_BG_VRAM_X         0x300 /* op81 frame 0: the background round the tower is rendered into this rect */
#define FX_FENRIR_BG_VRAM_Y         0x180
#define FX_FENRIR_BG_VRAM_W         0x80
#define FX_FENRIR_BG_VRAM_H         0x80
/* op82 (the whirlwind round the targets) */
#define FX_FENRIR_WHIRL_Z           0x4B0 /* op82 / op83: the whirlwind's centre (0, 0, this) */
#define FX_FENRIR_GLINTS_FRAME      1     /* op82: CYAN_GLINTS start (GREEN_RINGS_2 on frame 0) */
#define FX_FENRIR_GREEN_RINGS_FRAME 6     /* op82: GREEN_RINGS start above the centre */
#define FX_FENRIR_WIND_RING_FRAME   8     /* op82: WIND_RING / WIND_RING_2 drawn from here to WHIRL_FRAMES */
#define FX_FENRIR_WHIRL_FADE_FRAME  0x1C  /* op82: the ribbons and rings fade out */
#define FX_FENRIR_WHIRL_FADE_FRAMES 0x20  /* over this many frames */
#define FX_FENRIR_WHIRL_FRAMES      60    /* op82: ribbons 0 / 1 and the rings are drawn until this frame */
#define FX_FENRIR_SPIRAL_FRAMES     16    /* op82: ribbon 3 (five spiral copies) until this frame, ribbon 2 after it */
#define FX_FENRIR_RIBBON_0_STRANDS  6     /* ribbon 0: strands (RIBBON_PATHS_A entries per key) */
#define FX_FENRIR_RIBBON_1_STRANDS  3     /* ribbon 1: strands (RIBBON_PATHS_B) */
#define FX_FENRIR_RIBBON_KEY_FRAMES 10   /* ribbons 0 / 1 blend to the next key's path set over this many frames (0x199 a frame), */
#define FX_FENRIR_RIBBON_0_CYCLE    60    /* ribbon 0's six keys loop over this many frames, */
#define FX_FENRIR_RIBBON_1_CYCLE    30    /* ribbon 1's three keys */
#define FX_FENRIR_RIBBON_2_KEY_FRAMES 8   /* ribbon 2's: over this many frames, */
#define FX_FENRIR_RIBBON_2_CYCLE    24    /* three keys */
#define FX_FENRIR_SPIRAL_LENGTH     0x20  /* ribbons 2 / 3: edge points (ribbon 3 grows to it 2 a frame) */
#define FX_FENRIR_CAM_KEY_SLOTS     64    /* fx_0e2_camTracks: camera key tracks (fxm_camChanAdd / fxm_camChanUpdate) */
#define FX_FENRIR_CAM_KEY_TARGETS   3     /* camera slots a key track writes */
#define FX_FENRIR_RIBBON_COPIES     5     /* ribbons 2 / 3: copies round the centre (0x333 apart) */
#define FX_FENRIR_WHIRL_OP_FRAMES   0x78  /* op82's length; also the texture scrolls' start offset (x speed) */
#define FX_FENRIR_RIBBON_TPAGE      0x97   /* fxm_drawEdgeQuads texture of ribbons 0, 2, 3: 8-bit page (0x1C0, 0x100), */
#define FX_FENRIR_RIBBON_CLUT       0x3D80 /* CLUT (0, 0xF6), */
#define FX_FENRIR_RIBBON_V0         0x80   /* v this .. */
#define FX_FENRIR_RIBBON_V1         0xC0
#define FX_FENRIR_RIBBON_1_TPAGE    0x9C   /* ribbon 1's: 8-bit page (0x300, 0x100), */
#define FX_FENRIR_RIBBON_1_CLUT     0x3D40 /* CLUT (0, 0xF5), */
#define FX_FENRIR_RIBBON_1_V1       0xBF   /* v RIBBON_V0 .. this */
/* fxm_drawScrollMoveX scrolls the file's textures into the ribbons' rects (v 0x80 of their pages): op82 both, op80
 * the first (WIND_SHELL / WIND_VORTEX heads) */
#define FX_FENRIR_RIBBON_VRAM_X     0x1C0
#define FX_FENRIR_RIBBON_VRAM_Y     0x180
#define FX_FENRIR_RIBBON_VRAM_W     0x80
#define FX_FENRIR_RIBBON_VRAM_H     0x40
#define FX_FENRIR_RIBBON_SRC_X      0x1C0
#define FX_FENRIR_RIBBON_SRC_Y      0x1C0
#define FX_FENRIR_RIBBON_1_VRAM_X   0x300
#define FX_FENRIR_RIBBON_1_SRC_Y    0x140 /* (y, w, h as RIBBON_VRAM_*, src x = VRAM_X) */
/* op83 .. op85 (the targets) */
#define FX_FENRIR_LIFT_FRAMES       0x40  /* op83: the targets circle the whirlwind (0x100 a frame) and rise until this frame, */
#define FX_FENRIR_LIFT_SPEED        120   /* this much a frame */
#define FX_FENRIR_LIFT_OP_FRAMES    0x46  /* op83's length */
#define FX_FENRIR_DROP_HEIGHT       3000  /* op84: the units start this high above their home positions */
#define FX_FENRIR_DROP_FRAMES       4     /* op84: and drop home in this many frames (GREEN_WIND_TRAIL and SAND_RINGS at each) */
#define FX_FENRIR_LAND_FRAMES       0x28  /* op84: the task ends at this frame */
#define FX_FENRIR_WHITE_FADE_FRAMES 0x10  /* op85: additive screen fade from white to nothing (the op ends) */

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR pts4[1]; /* ring points (btlfx_pushTrailPoint; pad: width * 2); (size - 4) / 8 of them */
} Fx0e2Trail;

typedef struct {
    DVECTOR xy0; /* screen xy of one edge end */
    DVECTOR xy4; /* screen xy of the other */
    s32 otz8; /* ot z */
} Fx0e2Edge;

typedef struct {
    SVECTOR pos0; /* unit position */
    SVECTOR rot8; /* unit rotation */
    s16 dist10; /* distance from the centre */
    s16 angle12; /* angle around it */
} Fx0e2Orbit;

extern Unk800F78A4 *fx_0e2_env;
extern Fx0e2Orbit fx_0e2_orbits[];
extern u8 fx_0e2_op84Trails[][FX_FENRIR_GREEN_WIND_TRAIL_TRAIL_SLOTS * FX_FENRIR_GREEN_WIND_TRAIL_TRAIL_STRIDE]; /* per unit */

typedef struct {
    Pool6CEntry *emit0[8];
    Pool6CEntry *emit20[8];
    SVECTOR unitPos40[8]; /* unit positions */
    SVECTOR emitPos80[8]; /* effect positions */
    s16 landedC0[8]; /* unit landed */
} Fx0e2Work84; /* 0xD0 */
extern Fx0e2Edge fx_0e2_edges[FX_FENRIR_EDGES];
extern SVECTOR fx_0e2_stripOrigin; /* trail origin */
extern SVECTOR fx_0e2_headPos; /* trail head position */
extern SVECTOR fx_0e2_headRot; /* trail head rotation */
extern SVECTOR fx_0e2_aimPos; /* trail aim target */

void fx_0e2_stripFromTrackBlend(void *trackA, void *trackB, s32 t, Fx0e2Trail *trail, s32 size, s32 pad);
void fx_0e2_stripFromTrack(void *track, Fx0e2Trail *trail, s32 size, s32 base, s32 pad);
void fx_0e2_stripFromPosPath(void *track, Fx0e2Trail *trail, s32 size, s32 base, s32 count, s32 pad);
void fx_0e2_stripFromTrackBlendRot(void *trackA, void *trackB, Fx0e2Trail *trail, s32 size, s32 base, s32 count, s32 pad, s32 t,
                   s32 rotY);
void fxm_projectEdgesBlend(void *trackA, void *trackB, s32 unused, s32 unusedLength, SVECTOR *scale, s32 width, s32 t, s32 rotY, s32 twist);
void fxm_projectEdges(void *track, s32 unused, s32 unusedLength, SVECTOR *scale, s32 width, s32 rotY);
void fxm_drawEdgeQuads(s32 n, s32 bright, s32 tpage, s32 clut, u16 v0, u16 v1);
typedef struct {
    Pool6CEntry *emit0[FX_FENRIR_SMOKES];
    SVECTOR rot14; /* tower rotation */
    VECTOR pos1C; /* tower position */
    VECTOR scale2C; /* tower scale */
    SVECTOR rot3C; /* piece rotation */
    VECTOR pos44; /* piece position */
    VECTOR scale54; /* piece scale */
    SVECTOR blockRot64[FX_FENRIR_TOWER_BLOCKS]; /* block rotations */
    VECTOR blockPos84[FX_FENRIR_TOWER_BLOCKS]; /* block positions */
    VECTOR blockScaleC4[FX_FENRIR_TOWER_BLOCKS]; /* block scales */
    SVECTOR emitPos104[4]; /* effect positions */
    u16 model124[FX_FENRIR_TOWER_STAGES + FX_FENRIR_TOWER_PIECES + FX_FENRIR_TOWER_BLOCKS]; /* models: 3 tower stages, 5 pieces, 4 blocks */
} Fx0e2Work81; /* 0x13C */

extern u16 fx_0e2_towerBlocks[FX_FENRIR_TOWER_STAGES];
extern u16 fx_0e2_pieceBlocks[FX_FENRIR_TOWER_PIECES];
extern u32 fx_0e2_bgBuf[];
extern SVECTOR fx_0e2_blockPos[FX_FENRIR_TOWER_BLOCKS];

s32 fx_0e2_op81(s32 mode, Fx0e2Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);
s32 fx_0e2_op83(s32 mode, SVECTOR *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);
s32 fx_0e2_op84(s32 mode, Fx0e2Work84 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

static void fxm_tintScreenLerp(s32 from, s32 to, s32 t, s32 n, s32 abr, s32 ot);
static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);

typedef struct {
    s16 x, y, z;
} Fx0e2Point;

typedef struct {
    s16 unk0; /* not read by the module */
    u16 count2; /* count */
    Fx0e2Point *keys4;
} Fx0e2Path;
static void fxm_posPathInit(Fx0e2Path *path, u16 *data);
extern Fx0e2Path fx_0e2_pieceTracks[FX_FENRIR_TOWER_PIECES]; /* piece tracks */
static void fxm_drawScrollMoveX(s32 x, s32 dx, s32 dy, s32 w, s32 h, s32 sx, s32 sy);

typedef struct {
    u8 pad0[0x54];
    struct {
        u8 pad0[0x180];
        BtlMotion *motion180[8]; /* summon motions */
    } *summon54; /* g_btlfxLoader.summon */
    u8 pad58[8];
    s32 summonModel60; /* summon model */
} Fx0e2Summon;

typedef s32 (*Fx0e2RegTexList)(BtlModelHeader *, BtlTexInfo *, s16 *);

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8[FX_FENRIR_HEADS];
    Pool6CEntry *emit1C;
    Pool6CEntry *emit20;
    Pool6CEntry *emit24;
    Pool6CEntry *emit28[FX_FENRIR_HEADS];
    SVECTOR rot3C; /* summon rotation */
    VECTOR pos44; /* summon position */
    VECTOR pos54; /* model position */
    VECTOR scale64; /* scale */
    s32 summonRgb74;
    s32 rgb78;
    SVECTOR rot7C;
    VECTOR pos84;
    VECTOR scale94;
    VECTOR unkA4; /* unused (runtime watch, round 33) */
    s32 rgbB4;
    SVECTOR rotB8;
    VECTOR posC0;
    VECTOR scaleD0;
    SVECTOR rotE0;
    VECTOR posE8;
    VECTOR scaleF8;
    u8 pad108[8];
    SVECTOR emitPos110;
    SVECTOR emitPos118;
    SVECTOR emitPos120;
    s32 unk128; /* unused (runtime watch, round 33) */
    s32 loopMotion12C; /* Hi_DrawSummonModel flags bit 0: 1 loops the summon motion, 0 holds its last frame */
    s32 unk130; /* unused (runtime watch, round 33) */
    s32 unk134; /* unused (runtime watch, round 33) */
    s32 morphStep138;
    s32 morphStep13C;
    s32 fadeTimer140;
    s32 fadeInTimer144;
    s32 fadeOutTimer148;
    s16 scrollStep14C;
    s16 scrollSpeed14E;
    u16 model150[14]; /* models */
} Fx0e2Work80; /* 0x16C */

typedef struct {
    Fx0e2Path pathA0;
    Fx0e2Path pathB8;
    s16 unk10; /* write-only (cleared at init) */
    s16 fade12;
    s16 scale14;
    s16 width16;
} Fx0e2Ribbon80;

typedef struct {
    Fx0e2Path path0;
    s16 base8;
    s16 countA;
    s16 unkC; /* write-only (0x1000) */
    s16 unkE; /* write-only (0x1000) */
    s16 width10;
} Fx0e2Path80;

typedef struct {
    s32 pos0[2][4]; /* positions */
    s32 start20[3]; /* start position */
    u8 pad2C[4];
    s16 vel30[3]; /* velocity */
    s16 pad36;
} Fx0e2Drop; /* 0x38 */

typedef struct {
    u8 data[0x98];
} Fx0e2Trail98;

typedef struct {
    u8 data[0x1E0];
} Fx0e2Tex1E0;

static void fxm_rotPosPathInit(Fx0e2Path *path, u16 *data);
static s32 fxm_posPathGet(Fx0e2Path *path, s32 t, SVECTOR *out);
extern Fx0e2Drop fx_0e2_drops[FX_FENRIR_DROPS];
extern u16 fx_0e2_dropBlocks[FX_FENRIR_DROPS];
extern u16 fx_0e2_stripTrackBlocks[FX_FENRIR_STRIP_KEYS];
extern u16 fx_0e2_emitBlocks0[FX_FENRIR_HEADS];
extern u16 fx_0e2_emitBlocks1[FX_FENRIR_HEADS];
extern BtlTexInfo fx_0e2_texPages[];
extern s16 fx_0e2_texPos[];
extern BtlModelBuf fx_0e2_effModels[];
extern Fx0e2Ribbon80 fx_0e2_stripBlend;
extern Fx0e2Path fx_0e2_stripBlendPathB;
extern s16 fx_0e2_stripBlendFade;
extern Fx0e2Path80 fx_0e2_stripPath0;
extern Fx0e2Path80 fx_0e2_stripPath1;
extern Fx0e2Trail98 fx_0e2_strips[FX_FENRIR_HEADS];
extern Fx0e2Tex1E0 fx_0e2_op80Trails0[FX_FENRIR_HEADS];
extern u8 fx_0e2_op80Trails1[];

typedef struct {
    Fx0e2Path pathA0;
    Fx0e2Path pathB8;
    SVECTOR scale10; /* scale */
    s16 unk18; /* passed to fxm_projectEdgesBlend, which ignores that argument (cleared at init) */
    s16 length1A; /* length */
    s16 bright1C; /* brightness */
    s16 width1E; /* width */
    s16 blend20;
    s16 rotY22;
    u8 pad24[0xC];
} Fx0e2Ribbon; /* 0x30 */

typedef struct {
    Fx0e2Path path0;
    SVECTOR scale8; /* scale */
    s16 unk10; /* passed to fxm_projectEdges, which ignores that argument (cleared at init) */
    s16 length12; /* length */
    s16 bright14; /* brightness */
    s16 width16; /* width */
    s16 rotY18;
} Fx0e2Ribbon1;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    SVECTOR rotC; /* model rotations */
    SVECTOR rot14;
    VECTOR pos1C; /* model position */
    VECTOR scale2C; /* model scale */
    s32 rgb3C; /* model brightness */
    SVECTOR pos40; /* effect positions */
    SVECTOR pos48;
    SVECTOR center50; /* target centre, 0x3E8 up */
    s32 fadeTimer58; /* fade counters */
    s32 fadeTimer5C;
    s32 fadeTimer60;
    s16 scrollStep64;
    s16 scrollSpeed66;
    u16 model68; /* models */
    u16 model6A;
} Fx0e2Work82; /* 0x6C */

extern Fx0e2Ribbon fx_0e2_ribbon0;
extern Fx0e2Ribbon fx_0e2_ribbon1;
extern Fx0e2Ribbon fx_0e2_ribbon2;
extern Fx0e2Ribbon1 fx_0e2_ribbon3;
extern SVECTOR fx_0e2_stripHeads[FX_FENRIR_HEADS];
extern SVECTOR fx_0e2_emitPos0;
extern SVECTOR fx_0e2_emitPos1;
extern SVECTOR fx_0e2_emitPos2;
extern SVECTOR fx_0e2_emitPos3[5];
extern SVECTOR fx_0e2_emitPos4[5];

/* propagated */

typedef struct {
    s16 pos0[3]; /* position */
    s16 rot6[3]; /* rotation */
} Fx0e2Key; /* 0xC */

typedef struct {
    s16 unk0; /* not read by the module */
    u16 count2; /* keys */
    Fx0e2Key *keys4;
} Fx0e2Path_0b4;
static s32 fxm_rotPosPathGet(Fx0e2Path_0b4 *path, s32 t, SVECTOR *rot, SVECTOR *pos);
static s32 fxm_offsetTableEntry(u8 *base, u16 idx);
static s32 fxm_moveUnitHome(s32 unit, s32 t, s32 n, SVECTOR *start);

typedef struct {
    s16 *data0;
    s16 start4; /* start frame */
    s16 left6; /* frames left, -1 = free */
    s16 chan8; /* channel */
    u16 dstA; /* destination slots, 4 bits each */
    u16 stepC;
    u16 stepsE;
} Fx0e2Track; /* 0x10 */
extern Fx0e2Track fx_0e2_camTracks[FX_FENRIR_CAM_KEY_SLOTS];
#define FX_0E2_KEY_FREE (-1) /* left6: slot not in use (fxm_*Reset sets it, fxm_*Add takes such a slot) */
extern s32 fx_0e2_camChanFrame[];
static void fxm_camChanReset(void);
static void fxm_camChanAdd(s16 start, u16 dst, s16 ch, s16 *data);
static void fxm_camChanUpdate(s32 ch);

#endif
