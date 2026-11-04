#ifndef FX_0EC_H
#define FX_0EC_H

/* Effect module fx_0ec (dir 13 file 0x0EC): Doom. Evidence: the file btlfx_loadFile loads for the spell (docs/runtime-findings.md, btlfx_loadFile). */
#include "btlfx_mod.h"

/* fx_0ec effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s3.md; "(look pending)" = by kind, appearance not settled). */
#define FX_DOOM_GOLD_FRAME       0x1 /* model: ornate gold frame, rises sliced */
#define FX_DOOM_GOLD_PANEL       0x3 /* model: carved gold panel, rises sliced */
#define FX_DOOM_REAPER_PICTURE   0x4 /* model: panel with the picture of Death */
#define FX_DOOM_PARTICLES        0x6 /* particles; the render still reads garbled blue texels (texture not in the file) (look pending) */
#define FX_DOOM_GOLD_LATTICE     0x7 /* model: capsule with a gold lattice texture */
#define FX_DOOM_PINK_RINGS       0x8 /* emitter: pink-white rings (+ particles, texture not in the file) */
#define FX_DOOM_CAMERA_POINT     0x9 /* table: SVECTOR whose pad sets the camera point height */
#define FX_DOOM_RED_RING         0xB /* emitter: glowing red ring sprite */
#define FX_DOOM_PINK_SMOKE       0xC /* emitter: pink-violet smoke */
#define FX_DOOM_GLOW_SHELL       0xD /* model: capsule, additive, texture page computed by the C (not resolved by the tool), drawn twice (look pending) */
#define FX_DOOM_CYAN_SPARKLES    0xE /* emitter: cyan sparkles in a tube */
#define FX_DOOM_REAPER_PICTURE_2 0xF /* model: second panel with the picture of Death */
#define FX_DOOM_WHITE_SHELL      0x10 /* model: capsule on 15-bit texture page 0x119 (filled at run time; white in the render) (look pending) */
#define FX_DOOM_BACKDROP         0x11 /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */
#define FX_DOOM_GOURAUD_SHELL    0x13 /* model: gouraud capsule; the render draws it black (colours set by the C at run time?) (look pending) */

typedef struct {
    s16 modelCount0; /* split models */
    s16 triCount2; /* vertices */
    s32 triCounts4[2]; /* vertex counts */
    u8 *trisC; /* vertex buffer */
    u8 *next10; /* next free vertex */
    u8 *modelTris14[2]; /* model vertices */
} Fx0ecSplit;

typedef struct {
    u8 pad0[6];
    s16 split6; /* vertex 0's pad: split off (fxm_splitUpdate sets it; cleared here) */
    u8 pad8[0x18 - 8];
} Fx0ecVertex; /* 0x18 */

typedef struct {
    s32 vx, vy, vz;
} Fx0ecVec3;

typedef struct {
    Fx0ecVec3 pos0; /* position (.12) */
    SVECTOR velC; /* velocity; unkC.pad: scale */
    s16 spinX14; /* spin */
    s16 spinY16;
    s16 age18; /* age, < 0: free */
    s16 delayOrTri1A; /* start delay (fxm_shatterModelUpdate) or triangle (fxm_splitUpdate) */
} Fx0ecShard; /* 0x1C */

typedef struct {
    SVECTOR verts0[3]; /* vertices; verts0[0].pad: split off */
} Fx0ecTri;

extern Fx0ecTri fx_0ec_shatterTris[0x70];
extern Fx0ecShard fx_0ec_shatterShards[0x70];

typedef struct {
    u8 pad0[0x10];
    Unk800DA1B8 *pool10; /* particle pool */
} Fx0ecWork;

extern Unk800F78A4 *fx_0ec_env;
extern Fx0ecWork *fx_0ec_task;
extern SVECTOR fx_0ec_modelRot; /* model transform of fx_0ec_spawnImpact */
extern SVECTOR fx_0ec_modelPos;
extern VECTOR fx_0ec_modelScale;
extern Fx0ecSplit fx_0ec_split;
extern Fx0ecShard fx_0ec_splitShards[300];

static s32 fx_0ec_op80PoolTask(s32 mode, SVECTOR *pos, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);
void fx_0ec_spawnImpact(SVECTOR *pos);
static void fxm_shatterModel(s32 model, SVECTOR *center);
void fxm_shatterModelUpdate(void);
s32 fxm_splitModelInit(u8 *buf);
static s32 fxm_splitModelAdd(s32 model);
Fx0ecShard *fxm_splitFreeShard(void);
s32 fxm_splitRelease(s32 lineY, SVECTOR *center);
void fxm_splitUpdate(void);

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    SVECTOR pos8; /* caster position */
    SVECTOR rot10; /* caster rotation */
    SVECTOR pos18; /* summon position */
    SVECTOR pos20;
    SVECTOR emitPos28[2];
    s32 model38; /* models */
    s32 model3C;
    s32 model40[3];
    s32 model4C;
    s32 model50;
    s32 model54;
    s32 model58;
    s32 scale5C; /* scale */
    Pool6CEntry *emit60;
} Fx0ecWork80; /* 0x64 */

extern s32 fx_0ec_pointBlocks[2];
extern u8 fx_0ec_splitBuf[];
extern BtlModelBuf fx_0ec_effModels[];

s32 fx_0ec_op80(s32 mode, Fx0ecWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init);

#endif
