#ifndef FX_0D2_H
#define FX_0D2_H

/* Effect module fx_0d2 (dir 13 file 0xD2): Fenrir, full summon. Slice models on key paths, path trails
 * (fxm_buildPathTrail) and camera key channels (fxm_camChanUpdate). Evidence: btlfx_start and btlfx_loadFile get
 * the same id for summons (docs/runtime-findings.md). */
#include "btlfx_mod.h"

/* fx_0d2 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_FENRIR_FAN_EMITTER           0x1 /* emitter: orbiting fan element (not rendered) (look pending) */
#define FX_FENRIR_DUST_CLOUD            0x2 /* emitter: tan dust clouds and an orange gouraud ring */
#define FX_FENRIR_BLUE_RINGS            0x3 /* emitter: white-blue gouraud rings */
#define FX_FENRIR_ORANGE_RING           0x4 /* emitter: orange gouraud ring */
#define FX_FENRIR_BACKDROP              0x5 /* table: background layers for btlfx_drawBackdrop */
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
#define FX_FENRIR_SUMMON_EMITTER_3      0x83 /* summon data entry 3: emitter (look pending) */
#define FX_FENRIR_SUMMON_RING_PATH_4    0x84 /* summon data entry 4: rotation / position path (fx_0d2_ringPath) */
#define FX_FENRIR_SUMMON_MODEL_5        0x85 /* summon data entry 5: 'so' model (look pending) */
#define FX_FENRIR_SUMMON_MODEL_6        0x86 /* summon data entry 6: 'so' model (look pending) */
#define FX_FENRIR_SUMMON_MODEL_13       0x8D /* summon data entry 13: 'so' model (look pending) */
#define FX_FENRIR_SUMMON_MODEL_14       0x8E /* summon data entry 14: 'so' model (look pending) */
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
#define FX_FENRIR_SUMMON_MODEL_30       0x9E /* summon data entry 30: 'so' model (look pending) */
#define FX_FENRIR_SUMMON_PATH_31        0x9F /* summon data entry 31: position path of the summon (fx_0d2_summonPath) */
#define FX_FENRIR_SUMMON_CAMERA_KEYS_32 0xA0 /* summon data entry 32: camera channel keys (fxm_camChanAdd, op80) */
#define FX_FENRIR_SUMMON_CAMERA_KEYS_33 0xA1 /* summon data entry 33: camera channel keys (fxm_camChanAdd, op80) */
#define FX_FENRIR_SUMMON_CAMERA_KEYS_34 0xA2 /* summon data entry 34: camera channel keys (fxm_camChanAdd, op80) */
#define FX_FENRIR_SUMMON_CAMERA_KEYS_35 0xA3 /* summon data entry 35: camera channel keys (fxm_camChanAdd, op80) */
#define FX_FENRIR_SUMMON_EMITTER_36     0xA4 /* summon data entry 36: emitter (look pending) */
#define FX_FENRIR_SUMMON_EMITTER_37     0xA5 /* summon data entry 37: emitter (look pending) */
#define FX_FENRIR_SUMMON_EMITTER_38     0xA6 /* summon data entry 38: emitter (look pending) */
#define FX_FENRIR_SUMMON_SLICE_PATH_39  0xA7 /* summon data entry 39: rotation / position path (fx_0d2_slicePath) */

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
    Pool6CEntry *emit0[7];
    Fx0d2Model xform1C[4];
    SVECTOR posBC[7];
    u16 modelF4[10]; /* models: 0 ring, 1-2 morph pair, 3-8 slices, 9 */
} Fx0d2Work82; /* 0x108 */

extern Fx0d2Keys fx_0d2_ringPath;
extern Fx0d2Keys fx_0d2_morphCurve;
extern Fx0d2Keys fx_0d2_slicePaths[6];
extern u16 fx_0d2_sliceBlocks[6];
extern SVECTOR fx_0d2_spawnRot;
extern Fx0d2Spawn fx_0d2_spawns[7];
extern Fx0d2Track fx_0d2_camChans[64];
#define FX_0D2_KEY_FREE (-1) /* left6: slot not in use (fxm_*Reset sets it, fxm_*Add takes such a slot) */
extern s32 fx_0d2_camChanFrame[4];

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
    u16 model44[6]; /* slice models */
} Fx0d2Work81; /* 0x50 */

extern Fx0d2Trail fx_0d2_streak;
extern Fx0d2Trail fx_0d2_streak2;
extern u32 fx_0d2_bgBuf[];
extern u8 fx_0d2_streakBufs[9][0x90];
extern u8 fx_0d2_streakBufs2[11][0x50];
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
    u16 model136[9]; /* models */
} Fx0d2Work83; /* 0x148 */

extern u16 fx_0d2_morphBlocks[3];
extern s16 fx_0d2_texPages[];
extern s16 fx_0d2_uvOffsets[];
extern u8 fx_0d2_pieceModels[];
extern Unk800E8F50 fx_0d2_sliceModel;
extern Fx0d2Keys fx_0d2_slicePath2;
extern Fx0d2Keys fx_0d2_morphPath;
extern Fx0d2Keys fx_0d2_morphCurve2;
extern Fx0d2Keys fx_0d2_piecePaths[10];
extern Fx0d2Keys fx_0d2_unitOffsetPath;
extern Fx0d2Keys fx_0d2_unitScalePath;
s32 fx_0d2_op83(s32 mode, Fx0d2Work83 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

#endif
