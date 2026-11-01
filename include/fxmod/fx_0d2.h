#ifndef FX_0D2_H
#define FX_0D2_H

/* Effect module fx_0d2 (dir 13 file 0xD2): Fenrir, full summon. Slice models on key paths, path trails
 * (fxm_buildPathTrail) and camera key channels (fxm_camChanUpdate). Evidence: btlfx_start and btlfx_loadFile get
 * the same id for summons (docs/runtime-findings.md). */
#include "btlfx_mod.h"

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
static s32 fxm_offsetTableEntry(u8 *base, u16 i);
static void fxm_lerpSvec(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);
static s32 fxm_valuePathGet(Fx0d2Keys *keys, s32 t, s32 *out);
static s32 fxm_moveUnitHome(s32 a0, s32 t, s32 n, SVECTOR *b);
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
