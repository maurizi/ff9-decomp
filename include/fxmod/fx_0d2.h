#ifndef FX_0D2_H
#define FX_0D2_H

/* Effect module fx_0d2 (dir 13 file 0xD2): Fenrir, full summon. Slice models on key paths, path trails
 * (fxm_buildPathTrail) and camera key channels (fxm_camChanUpdate). Evidence: btlfx_start and btlfx_loadFile get
 * the same id for summons (docs/runtime-findings.md). */
#include "btlfx_mod.h"

/* fx_0d2 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_FENRIR_ORANGE_LIGHT_LINE     0x1 /* emitter: orbiting fan, a thin horizontal orange-pink light line */
#define FX_FENRIR_DUST_CLOUD            0x2 /* emitter: tan dust clouds and an orange gouraud ring */
#define FX_FENRIR_BLUE_RINGS            0x3 /* emitter: white-blue gouraud rings */
#define FX_FENRIR_ORANGE_RING           0x4 /* emitter: orange gouraud ring */
#define FX_FENRIR_BACKDROP              0x5 /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */
#define FX_FENRIR_UNIT_PATHS            0x8 /* table: offset table of per-unit position paths (op83) */
#define FX_FENRIR_MORPH_PATH            0x9 /* table: rotation / position path of the morph models (fx_0d2_morphPath) */
#define FX_FENRIR_UNIT_OFFSET_PATH      0x14 /* table: position path (fx_0d2_unitOffsetPath) */
#define FX_FENRIR_UNIT_SCALE_PATH       0x15 /* table: vector path (fx_0d2_unitScalePath) */
#define FX_FENRIR_MORPH_CURVE           0x19 /* table: value path (fx_0d2_morphCurve2) */
#define FX_FENRIR_MORPH_CURVE_2         0x1A /* table: alternative value path (fx_0d2_morphCurve2) */
#define FX_FENRIR_ROCK_STREAK           0x1B /* trail: brown rock-textured streak, drawn along 9 paths */
#define FX_FENRIR_CAMERA_KEYS_2         0x1D /* table: camera channel 2 keys from frame 0xDE (fxm_camChanAdd) */
#define FX_FENRIR_CAMERA_KEYS_3         0x1E /* table: camera channel 3 keys from frame 0xDD */
#define FX_FENRIR_CAMERA_KEYS_1         0x1F /* table: camera channel 1 keys from frame 0xDE */
#define FX_FENRIR_CAMERA_KEYS_4         0x20 /* table: camera channel 4 keys from frame 0xDD */
#define FX_FENRIR_DUST_DEBRIS           0x21 /* emitter: tan dust clouds and rock debris quads */
#define FX_FENRIR_ROCK_STREAK_2         0x25 /* trail: brown rock-textured streak, drawn along 11 paths */
#define FX_FENRIR_SLICE_PATH_2          0x28 /* table: rotation / position path (fx_0d2_slicePath2) */
#define FX_FENRIR_SUMMON_STREAK_PATHS_0 0x80 /* summon data entry 0: offset table of the rock-streak position paths */
#define FX_FENRIR_SUMMON_STREAK_PATHS_1 0x81 /* summon data entry 1: offset table of the rock-streak position paths */
#define FX_FENRIR_SUMMON_EMIT_PATH_2    0x82 /* summon data entry 2: position path of the emitters (fx_0d2_emitPath) */
#define FX_FENRIR_COLOR_RINGS           0x83 /* summon data entry 3: emitter, white-teal and white-blue gouraud rings */
#define FX_FENRIR_SUMMON_RING_PATH_4    0x84 /* summon data entry 4: rotation / position path (fx_0d2_ringPath) */
#define FX_FENRIR_WINGED_CREATURE       0x85 /* summon data entry 5: 'so' model, brown furred creature with spread wings and a green eye */
#define FX_FENRIR_ROCK_TERRACE          0x86 /* summon data entry 6: 'so' model, stepped ground terrace with a sand-rock texture */
#define FX_FENRIR_ROCK_TERRACE_2        0x8D /* summon data entry 13: 'so' model, stepped ground terrace with a sand-rock texture (same size as entry 6) */
#define FX_FENRIR_ROCK_CRAG             0x8E /* summon data entry 14: 'so' model, rough brown rock crag */
#define FX_FENRIR_SUMMON_MORPH_CURVE_15 0x8F /* summon data entry 15: value path (fx_0d2_morphCurve) */
#define FX_FENRIR_SUMMON_SLICE_PATHS_16 0x90 /* summon data entry 16: offset table of six rotation / position paths (fx_0d2_slicePaths) */
#define FX_FENRIR_SUMMON_SLICE_PATHS_17 0x91 /* summon data entry 17: offset table of six rotation / position paths (fx_0d2_slicePaths) */
#define FX_FENRIR_SUMMON_PATH_18        0x92 /* summon data entry 18: position path of the summon (fx_0d2_summonPath) */
#define FX_FENRIR_SUMMON_CAMERA_KEYS_19 0x93 /* summon data entry 19: camera channel keys (fxm_camChanAdd, op80) */
#define FX_FENRIR_SUMMON_CAMERA_KEYS_20 0x94 /* summon data entry 20: camera channel keys (fxm_camChanAdd, op80) */
#define FX_FENRIR_SUMMON_CAMERA_KEYS_21 0x95 /* summon data entry 21: camera channel keys (fxm_camChanAdd, op80) */
#define FX_FENRIR_SUMMON_CAMERA_KEYS_22 0x96 /* summon data entry 22: camera channel keys (fxm_camChanAdd, op80) */
#define FX_FENRIR_SUMMON_CAMERA_KEYS_23 0x97 /* summon data entry 23: camera channel keys (fxm_camChanAdd, op80) */
#define FX_FENRIR_SUMMON_CAMERA_KEYS_24 0x98 /* summon data entry 24: camera channel keys (fxm_camChanAdd, op80) */
#define FX_FENRIR_SUMMON_CAMERA_KEYS_25 0x99 /* summon data entry 25: camera channel keys (fxm_camChanAdd, op80) */
#define FX_FENRIR_SUMMON_CAMERA_KEYS_26 0x9A /* summon data entry 26: camera channel keys (fxm_camChanAdd, op80) */
#define FX_FENRIR_SUMMON_CAMERA_KEYS_27 0x9B /* summon data entry 27: camera channel keys (fxm_camChanAdd, op80) */
#define FX_FENRIR_SUMMON_CAMERA_KEYS_28 0x9C /* summon data entry 28: camera channel keys (fxm_camChanAdd, op80) */
#define FX_FENRIR_SUMMON_CAMERA_KEYS_29 0x9D /* summon data entry 29: camera channel keys (fxm_camChanAdd, op80) */
#define FX_FENRIR_GOLD_HORN             0x9E /* summon data entry 30: 'so' model, gold gouraud bent sheet (horn-shaped) */
#define FX_FENRIR_SUMMON_PATH_31        0x9F /* summon data entry 31: position path of the summon (fx_0d2_summonPath) */
#define FX_FENRIR_SUMMON_CAMERA_KEYS_32 0xA0 /* summon data entry 32: camera channel keys (fxm_camChanAdd, op80) */
#define FX_FENRIR_SUMMON_CAMERA_KEYS_33 0xA1 /* summon data entry 33: camera channel keys (fxm_camChanAdd, op80) */
#define FX_FENRIR_SUMMON_CAMERA_KEYS_34 0xA2 /* summon data entry 34: camera channel keys (fxm_camChanAdd, op80) */
#define FX_FENRIR_SUMMON_CAMERA_KEYS_35 0xA3 /* summon data entry 35: camera channel keys (fxm_camChanAdd, op80) */
#define FX_FENRIR_MOON                  0xA4 /* summon data entry 36: emitter, a pink-orange full-moon sprite */
#define FX_FENRIR_STAR_FLARE            0xA5 /* summon data entry 37: emitter, a cyan-white star flare particle */
#define FX_FENRIR_DUST_CLOUDS           0xA6 /* summon data entry 38: emitter, orbiting dust clouds and gold debris specks */
#define FX_FENRIR_SUMMON_SLICE_PATH_39  0xA7 /* summon data entry 39: rotation / position path (fx_0d2_slicePath) */

/* fx_0d2 choreography (docs/fxdata.md, "Choreography literals"): the ops' keyframes and lengths (op80 the summon,
 * op81 the rock slices and streaks, op82 the crag, op83 the shattered ground and the white-out, op84 the units'
 * drop, op85 the fade back), and the counts of their pieces. */
#define FX_FENRIR_MODEL_COUNT     0x20 /* op80: btlfx_initEffModels slots */
#define FX_FENRIR_MOTION_0_FRAME  1    /* op80: the summon's first motion, SUMMON_PATH_18 and FX_FENRIR_MOON start */
#define FX_FENRIR_MOTION_1_FRAME  0x29 /* op80: the summon's second motion */
#define FX_FENRIR_MOTION_2_FRAME  0x51 /* op80: the summon's third motion (looping) */
#define FX_FENRIR_STAR_FLARE_FRAME 0x52 /* op80: the two FX_FENRIR_STAR_FLAREs start */
#define FX_FENRIR_SHOT_2_FRAME    0x69 /* op80: the summon grows and follows SUMMON_PATH_31; MOON and the flares stop,
                                        * the two FX_FENRIR_COLOR_RINGS start along SUMMON_EMIT_PATH_2 */
#define FX_FENRIR_SUMMON_FRAMES   0x83 /* op80: the summon model is drawn from frame 1 for this many frames */
#define FX_FENRIR_SLICE_FRAME     0xB6 /* op80: the winged creature follows SUMMON_SLICE_PATH_39 from here */
#define FX_FENRIR_CREATURE_FRAMES 0xDE /* op80: FX_FENRIR_WINGED_CREATURE is drawn from frame 1 for this many frames */
#define FX_FENRIR_FRAMES          0x12C /* op80: the task ends at this frame */
#define FX_FENRIR_CAM_SHOT_2_FRAME 0x68 /* op80: the second set of camera channel keys starts (channels 3, 4 a frame earlier), */
#define FX_FENRIR_CAM_SHOT_3_FRAME 0xB6 /* the third, */
#define FX_FENRIR_CAM_SHOT_4_FRAME 0xDE /* the fourth (the file's own CAMERA_KEYS_n) */
#define FX_FENRIR_CAM_KEY_SLOTS   64   /* fx_0d2_camChans: camera key tracks (fxm_camChanAdd / fxm_camChanUpdate) */
#define FX_FENRIR_CAM_CHANNELS    4    /* fx_0d2_camChanFrame: frame counters per channel set (fxm_camChanUpdate's ch) */
#define FX_FENRIR_SLICES          6    /* rock-slice models (fx_0d2_sliceBlocks) and their paths (fx_0d2_slicePaths) */
#define FX_FENRIR_SLICE_FRAMES    0x19 /* op81: the slices are drawn for this many frames, then freed */
#define FX_FENRIR_UNITS_FRAME     0x2C /* op81: the targets' positions and scales are saved (fx_0d2_unitPos / unitScale) */
#define FX_FENRIR_STREAK_FRAME    0x40 /* op81: the FX_FENRIR_ROCK_STREAK trails grow from here */
#define FX_FENRIR_STREAK_FRAMES   0x15 /* for this many frames */
#define FX_FENRIR_STREAKS         9    /* FX_FENRIR_ROCK_STREAK paths (SUMMON_STREAK_PATHS_0, fx_0d2_streakBufs) */
#define FX_FENRIR_STREAK_MAX_LEN  0x10 /* points a streak grows to */
#define FX_FENRIR_BG_FRAME        0x41 /* op81: btlfx_drawBgOffscreen */
#define FX_FENRIR_STREAK_2_FRAME  0x43 /* op81: ORANGE_LIGHT_LINE, ORANGE_RING start; the ROCK_STREAK_2 trails grow */
#define FX_FENRIR_STREAK_2_FRAMES 0xF  /* for this many frames */
#define FX_FENRIR_STREAKS_2       11   /* FX_FENRIR_ROCK_STREAK_2 paths (SUMMON_STREAK_PATHS_1, fx_0d2_streakBufs2) */
#define FX_FENRIR_STREAK_2_MAX_LEN 9   /* points a streak grows to */
#define FX_FENRIR_ROCKS_FRAMES    0x64 /* op81: the task ends at this frame */
#define FX_FENRIR_SPAWNS          7    /* op82: emitters started from fx_0d2_spawns (the last one in front) */
#define FX_FENRIR_CRAG_FRAMES     0x29 /* op82: CRAG, the terrace morph, the slices and GOLD_HORN are drawn until this frame,
                                        * then freed and the spawned emitters stopped */
#define FX_FENRIR_CRAG_OP_FRAMES  0x2D /* op82: the task ends at this frame */
#define FX_FENRIR_DEBRIS_FRAME    3    /* op83: FX_FENRIR_DUST_DEBRIS starts (turned to rot128 until DEBRIS_STOP_FRAME) */
#define FX_FENRIR_BLUE_RINGS_FRAME 12  /* op83: FX_FENRIR_BLUE_RINGS start at the targets' average position */
#define FX_FENRIR_WHITE_IN_FRAME  22   /* op83: the additive white screen fade-in starts */
#define FX_FENRIR_DEBRIS_STOP_FRAME 38 /* op83: FX_FENRIR_DUST_DEBRIS stops */
#define FX_FENRIR_WHITE_OUT_FRAME 42   /* op83: the units get their scales back, the white fades out */
#define FX_FENRIR_WHITE_FADE_FRAMES 0x10 /* op83: each white fade (fxm_tintScreenLerp) takes this many frames */
#define FX_FENRIR_WHITE_RGB       0xFF, 0xFF, 0xFF /* op83: additive screen tint held between the fades */
#define FX_FENRIR_GROUND_SLICE_FRAMES 0x17 /* op83: fx_0d2_sliceModel follows SLICE_PATH_2 from frame 1 this long */
#define FX_FENRIR_MORPH_FRAMES    0x27 /* op83: the ground morph models are drawn from frame 1 this long, */
#define FX_FENRIR_MORPH_2_FRAME   4    /* morphing 1 -> 2 on MORPH_CURVE, from this frame 2 -> 3 on MORPH_CURVE_2 */
#define FX_FENRIR_MORPH_MODELS    3    /* op83: ground morph models (fx_0d2_morphBlocks, model136[1..3]) */
#define FX_FENRIR_PIECE_MODELS    5    /* op83: rock piece models (fx_0d2_pieceModels, model136[4..8]) */
#define FX_FENRIR_PIECES          10   /* op83: rock pieces flying along fx_0d2_piecePaths (the models in turn) */
#define FX_FENRIR_PIECES_FRAME    2    /* op83: the pieces fly from this frame */
#define FX_FENRIR_PIECES_FRAMES   0x18 /* for this many frames */
#define FX_FENRIR_UNIT_PATH_FRAMES 0x1C /* op83: the units follow UNIT_OFFSET_PATH / UNIT_SCALE_PATH this long */
#define FX_FENRIR_GROUND_FRAMES   0x3D /* op83: the task ends at this frame */
#define FX_FENRIR_DROP_HEIGHT     3000 /* op84: the units start this high above their home positions */
#define FX_FENRIR_DROP_FRAMES     4    /* op84: and drop home in this many frames (FX_FENRIR_DUST_CLOUD at each) */
#define FX_FENRIR_LAND_FRAMES     0x28 /* op84: the task ends at this frame */
#define FX_FENRIR_DARK_FADE_FRAMES 0x10 /* op85: subtractive screen fade from white to nothing (the op ends) */

typedef struct {
    u16 unk0; /* not read by the module */
    u16 count2; /* count */
    s16 *keys4;
} Fx0d2Keys;

typedef struct {
    s16 *data0;
    s16 start4; /* start frame */
    s16 left6; /* frames left, -1 = free */
    s16 chan8; /* channel */
    u16 dstA; /* destination slots, 4 bits each: nibble n (1-10) = ((SVECTOR *)g_btlCamera)[n - 1], i.e. points[0-5]
               * then pointRots[0-3]; 0 = none. Runtime (Fenrir, g_btlCamera write watch): keys 1-4 write points[0-3],
               * 9 / 10 pointRots[2] / [3]; fx_0d2_op80 writes slot 7 (pointRots[1]) itself */
    u16 keyC;
    u16 keyCountE;
} Fx0d2Track; /* 0x10 */

typedef struct {
    Pool6CEntry *emit0[8];
    SVECTOR pos20[8]; /* unit positions */
    SVECTOR emitPos60[8];
    s16 doneA0[8]; /* done */
} Fx0d2Work84; /* 0xB0 */

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx0d2Vec3;

typedef struct {
    SVECTOR rot0; /* rotation */
    Fx0d2Vec3 pos8; /* position */
    s32 pad14;
    Fx0d2Vec3 scale18; /* scale */
    s32 pad24;
} Fx0d2Model; /* 0x28 */

typedef struct {
    u16 block0; /* file id */
    u16 frame2; /* start frame */
    u16 done4; /* released */
    s16 pos6[3]; /* position */
    u16 unkC; /* not read by the module */
} Fx0d2Spawn; /* 0xE */

typedef struct {
    Pool6CEntry *emit0[FX_FENRIR_SPAWNS];
    Fx0d2Model xform1C[4];
    SVECTOR posBC[FX_FENRIR_SPAWNS];
    u16 modelF4[3 + FX_FENRIR_SLICES + 1]; /* models: 0 ring, 1-2 morph pair, 3-8 slices, 9 */
} Fx0d2Work82; /* 0x108 */

extern Fx0d2Keys fx_0d2_ringPath;
extern Fx0d2Keys fx_0d2_morphCurve;
extern Fx0d2Keys fx_0d2_slicePaths[FX_FENRIR_SLICES];
extern u16 fx_0d2_sliceBlocks[FX_FENRIR_SLICES];
extern SVECTOR fx_0d2_spawnRot;
extern Fx0d2Spawn fx_0d2_spawns[FX_FENRIR_SPAWNS];
extern Fx0d2Track fx_0d2_camChans[FX_FENRIR_CAM_KEY_SLOTS];
#define FX_0D2_KEY_FREE (-1) /* left6: slot not in use (fxm_*Reset sets it, fxm_*Add takes such a slot) */
extern s32 fx_0d2_camChanFrame[FX_FENRIR_CAM_CHANNELS];

static void fxm_buildPathTrail(Fx0d2Keys *keys, u8 *buf, s32 size, s32 start, s32 n, s32 from, s32 to, s32 clip);
static s32 fxm_offsetTableEntry(u8 *base, u16 idx);
static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);
static s32 fxm_valuePathGet(Fx0d2Keys *keys, s32 t, s32 *out);
static s32 fxm_moveUnitHome(s32 unit, s32 t, s32 n, SVECTOR *start);
static void fxm_camChanReset(void);
static void fxm_camChanAdd(s16 start, u16 dst, s16 ch, s16 *data);
static void fxm_camChanUpdate(s32 ch);
typedef struct {
    s16 x, y, z;
} Fx0d2Point;
typedef struct {
    s16 unk0; /* not read by the module */
    u16 count2; /* count */
    Fx0d2Point *points4;
} Fx0d2Path;
typedef struct {
    s16 pos0[3]; /* position */
    s16 rot6[3]; /* rotation */
} Fx0d2Key; /* 0xC */
typedef struct {
    s16 unk0; /* not read by the module */
    u16 count2; /* keys */
    Fx0d2Key *keys4;
} Fx0d2KeyPath;
typedef struct {
    s16 x, y, z;
} Fx0d2Key6;
typedef struct {
    u16 count0; /* count */
    Fx0d2Key6 *keys4;
} Fx0d2Track6;
static void fxm_posPathInit(Fx0d2Path *path, u16 *data);
static s32 fxm_posPathGet(Fx0d2Path *path, s32 t, SVECTOR *out);
static void fxm_rotPosPathInit(Fx0d2Path *path, u16 *data);
static s32 fxm_rotPosPathGet(Fx0d2KeyPath *path, s32 t, SVECTOR *rot, SVECTOR *pos);
static void fxm_valuePathInit(Fx0d2Path *path, u16 *data);
static void fxm_vecPathInit(Fx0d2Path *path, u16 *data);
static s32 fxm_vecPathGet(Fx0d2Path *track, s32 t, VECTOR *out);
extern Unk800F78A4 *fx_0d2_env;
static void fxm_tintScreenLerp(s32 from, s32 to, s32 t, s32 n, s32 abr, s32 ot);
typedef struct {
    Fx0d2Keys keys;
    s16 start8; /* start */
    s16 lengthA; /* points */
    s16 fadeC; /* btlfx_drawTrail fade (depth cue toward the far colour), 0x1000 */
    s16 scaleE; /* btlfx_drawTrail width scale, 0x1000 (ignored: these calls push no point) */
    s16 from10;
    s16 to12;
} Fx0d2Trail;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    SVECTOR rotC; /* slice rotation */
    Fx0d2Vec3 pos14; /* slice position */
    s32 pad20;
    Fx0d2Vec3 scale24; /* slice scale */
    s32 pad30;
    SVECTOR pos34;
    SVECTOR pos3C;
    u16 model44[FX_FENRIR_SLICES]; /* slice models */
} Fx0d2Work81; /* 0x50 */

extern Fx0d2Trail fx_0d2_streak;
extern Fx0d2Trail fx_0d2_streak2;
extern u32 fx_0d2_bgBuf[];
extern u8 fx_0d2_streakBufs[FX_FENRIR_STREAKS][0x90];
extern u8 fx_0d2_streakBufs2[FX_FENRIR_STREAKS_2][0x50];
extern SVECTOR fx_0d2_unitPos[8];
extern VECTOR fx_0d2_unitScale[8];
s32 fx_0d2_op81(s32 mode, Fx0d2Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);
typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    SVECTOR rot14; /* summon rotation */
    Fx0d2Vec3 summonPos1C; /* summon position */
    s32 pad28;
    Fx0d2Vec3 summonScale2C; /* summon scale */
    s32 pad38;
    SVECTOR rot3C; /* slice rotation */
    Fx0d2Vec3 pos44; /* slice position */
    s32 pad50;
    Fx0d2Vec3 scale54; /* slice scale */
    u8 pad60[0x6C - 0x60];
    SVECTOR pos6C;
    SVECTOR pos74;
    SVECTOR pos7C;
    SVECTOR pos84;
    SVECTOR pos8C;
    s32 motion94;
    s32 loop98;
    s32 pathFrame9C; /* path step */
    s32 padA0;
    u16 modelA4; /* slice model */
} Fx0d2Work80; /* 0xA8 */

/* the summon slot behind env->unk4 */
typedef struct {
    u8 pad0[0x54];
    struct {
        u8 pad0[0x180];
        BtlMotion *motion180;
        BtlMotion *motion184;
        BtlMotion *motion188;
    } *summon54; /* g_btlfxLoader.summon */
    u8 pad58[8];
    s32 model60; /* summon model */
} Fx0d2Summon;

extern BtlModelBuf fx_0d2_effModels[];
extern Fx0d2Keys fx_0d2_summonPath;
extern Fx0d2Keys fx_0d2_slicePath;
extern Fx0d2Keys fx_0d2_emitPath;
extern SVECTOR fx_0d2_emitPos;
extern SVECTOR fx_0d2_emitPos2[2];
s32 fx_0d2_op80(s32 mode, Fx0d2Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);
typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR rot8; /* slice rotation */
    Fx0d2Vec3 pos10; /* slice position */
    s32 pad1C;
    Fx0d2Vec3 scale20; /* slice scale */
    s32 pad2C;
    SVECTOR rot30;
    Fx0d2Vec3 pos38;
    s32 pad44;
    Fx0d2Vec3 scale48;
    s32 pad54;
    SVECTOR unitPos58[8]; /* unit positions */
    VECTOR unitScale98[8]; /* unit scales */
    SVECTOR pos118;
    SVECTOR pos120;
    SVECTOR rot128;
    s16 fadeOut130; /* fade counters */
    s16 fadeIn132;
    s16 white134;
    u16 model136[1 + FX_FENRIR_MORPH_MODELS + FX_FENRIR_PIECE_MODELS]; /* models: 0 slice, 1-3 morph, 4-8 pieces */
} Fx0d2Work83; /* 0x148 */

extern u16 fx_0d2_morphBlocks[FX_FENRIR_MORPH_MODELS];
extern s16 fx_0d2_texPages[];
extern s16 fx_0d2_uvOffsets[];
extern u8 fx_0d2_pieceModels[];
extern Unk800E8F50 fx_0d2_sliceModel;
extern Fx0d2Keys fx_0d2_slicePath2;
extern Fx0d2Keys fx_0d2_morphPath;
extern Fx0d2Keys fx_0d2_morphCurve2;
extern Fx0d2Keys fx_0d2_piecePaths[FX_FENRIR_PIECES];
extern Fx0d2Keys fx_0d2_unitOffsetPath;
extern Fx0d2Keys fx_0d2_unitScalePath;
s32 fx_0d2_op83(s32 mode, Fx0d2Work83 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

#endif
