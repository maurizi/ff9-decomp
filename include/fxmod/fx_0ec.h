#ifndef FX_0EC_H
#define FX_0EC_H

/* Effect module fx_0ec (dir 13 file 0x0EC): Doom. Evidence: the file btlfx_loadFile loads for the spell (docs/runtime-findings.md, btlfx_loadFile). */
#include "btlfx_mod.h"

/* fx_0ec effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s3.md; "(look pending)" = by kind, appearance not settled). */
#define FX_DOOM_GOLD_FRAME       0x1 /* model: ornate gold frame, rises sliced */
#define FX_DOOM_GOLD_PANEL       0x3 /* model: carved gold panel, rises sliced */
#define FX_DOOM_REAPER_PICTURE   0x4 /* model: panel with the picture of Death */
#define FX_DOOM_VIOLET_HALO      0x6 /* particles: violet ring halo; runtime (docs/runtime-findings.md effect-data pass) */
#define FX_DOOM_GOLD_LATTICE     0x7 /* model: capsule with a gold lattice texture */
#define FX_DOOM_PINK_RINGS       0x8 /* emitter: pink-white rings (+ particles, texture not in the file) */
#define FX_DOOM_CAMERA_POINT     0x9 /* table: SVECTOR whose pad sets the camera point height */
#define FX_DOOM_RED_RING         0xB /* emitter: glowing red ring sprite */
#define FX_DOOM_PINK_SMOKE       0xC /* emitter: pink-violet smoke */
#define FX_DOOM_GLOW_SHELL       0xD /* model: capsule, additive, texture page computed by the C (not resolved by the tool), drawn twice; runtime (docs/runtime-findings.md effect-data pass): tpage computed by the C, not resolved (look pending) */
#define FX_DOOM_CYAN_SPARKLES    0xE /* emitter: cyan sparkles in a tube */
#define FX_DOOM_REAPER_PICTURE_2 0xF /* model: second panel with the picture of Death */
#define FX_DOOM_SWIRL_SHELL      0x10 /* model: shell on a grey marbled swirl page at run time; runtime (docs/runtime-findings.md effect-data pass, medium) */
#define FX_DOOM_BACKDROP         0x11 /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */
#define FX_DOOM_GOURAUD_SHELL    0x13 /* model: gouraud capsule; the render draws it black (colours set by the C at run time?); runtime (docs/runtime-findings.md effect-data pass): still black with the run-time page (look pending) */

/* fx_0ec choreography (docs/fxdata.md, "Choreography literals"): op80's states, their lengths, keyframes and screen
 * tints, the halo pool items, the shatter / split shard buffers, the model textures. */
#define FX_DOOM_STATE_RISE    0 /* op80: the gold frame and panel rise sliced around the caster */
#define FX_DOOM_STATE_GLOW    1 /* they turn, GLOW_SHELL flickers */
#define FX_DOOM_STATE_PICTURE 2 /* REAPER_PICTURE fades in, the shell fades, RED_RING at both ends */
#define FX_DOOM_STATE_SHATTER 3 /* the frame and panel split into shards (fxm_split*), the picture shatters (the op ends) */
#define FX_DOOM_RISE_FRAMES         22 /* STATE_RISE's length */
#define FX_DOOM_GLOW_FRAMES         20 /* STATE_GLOW's length */
#define FX_DOOM_PICTURE_FRAMES      30 /* STATE_PICTURE's length */
#define FX_DOOM_PICTURE_FADE_FRAMES 8  /* STATE_PICTURE: REAPER_PICTURE brightens over this many frames */
#define FX_DOOM_SHELL_FADE_FRAMES   16 /* STATE_PICTURE: GLOW_SHELL fades over this many */
#define FX_DOOM_RED_RING_FRAME      20 /* STATE_PICTURE: FX_DOOM_RED_RING at both emitPos28 points */
#define FX_DOOM_FLASH_FRAME         8  /* STATE_SHATTER: RED_FLASH and CYAN_SPARKLES; REAPER_PICTURE_2 brightens until then */
#define FX_DOOM_RELEASE_FRAMES      47 /* STATE_SHATTER: the split shards are released over this many frames */
#define FX_DOOM_SHATTER_FRAMES      71 /* STATE_SHATTER's length (the op ends) */
#define FX_DOOM_DARKEN_RGB    100, 100, 0 /* STATE_PICTURE's end: subtractive screen tint (the screen turns blue) */
#define FX_DOOM_RED_FLASH_RGB 0xC8, 0, 0  /* STATE_SHATTER: additive (red) */
#define FX_DOOM_HALO_SLOTS    0x80 /* op80 pool items: a VIOLET_HALO (SVECTOR position each) */
#define FX_DOOM_HALO_FRAMES   6    /* pool item: a halo spreads and fades over this many frames */
#define FX_DOOM_SHATTER_TRIS  0x70 /* fxm_shatterModel: triangles / shards of the shattered picture */
#define FX_DOOM_SHATTER_FLY_AGE 8  /* a shatter shard moves (btlfx_stepModuleParticle) from this age */
#define FX_DOOM_SHATTER_LIFE  0x47 /* and is freed at this one */
#define FX_DOOM_SPLIT_SHARDS  300  /* fxm_split*: shards of the split frame and panel */
#define FX_DOOM_SPLIT_LIFE    0x20 /* a split shard is freed at this age */
#define FX_DOOM_MODEL_COUNT   9    /* btlfx_initEffModels slots */
#define FX_DOOM_GOLD_FRAME_TPAGE 0x97 /* Hi_RegisterTexEffModel texture: 8-bit page (0x1C0, 0x100), abr 0, gold */
#define FX_DOOM_GOLD_FRAME_CLUT  0x3D40 /* CLUT at (0x0, 0xF5) */
#define FX_DOOM_GOLD_FRAME_U     0x80 /* texture origin (u, v) in the page */
#define FX_DOOM_GOLD_FRAME_V     0x80
#define FX_DOOM_GOLD_PANEL_TPAGE 0x97 /* Hi_RegisterTexEffModel texture: 8-bit page (0x1C0, 0x100), abr 0, gold */
#define FX_DOOM_GOLD_PANEL_CLUT  0x3D40 /* CLUT at (0x0, 0xF5) */
#define FX_DOOM_GOLD_PANEL_U     0x80 /* texture origin (u, v) in the page */
#define FX_DOOM_GOLD_PANEL_V     0x80
#define FX_DOOM_REAPER_PICTURE_TPAGE 0xB7 /* Hi_RegisterTexEffModel texture: 8-bit page (0x1C0, 0x100), abr 1 */
#define FX_DOOM_REAPER_PICTURE_CLUT  0x3DC0 /* CLUT at (0x0, 0xF7) */
#define FX_DOOM_REAPER_PICTURE_U     0x0 /* texture origin (u, v) in the page */
#define FX_DOOM_REAPER_PICTURE_V     0x80
#define FX_DOOM_REAPER_PICTURE_2_TPAGE 0x97 /* Hi_RegisterTexEffModel texture: 8-bit page (0x1C0, 0x100), abr 0 */
#define FX_DOOM_REAPER_PICTURE_2_CLUT  0x3DC0 /* CLUT at (0x0, 0xF7) */
#define FX_DOOM_REAPER_PICTURE_2_U     0x0 /* texture origin (u, v) in the page */
#define FX_DOOM_REAPER_PICTURE_2_V     0x80
#define FX_DOOM_GOLD_LATTICE_TPAGE 0x39 /* Hi_RegisterTexEffModel texture: 4-bit page (0x240, 0x100), abr 1 */
#define FX_DOOM_GOLD_LATTICE_CLUT  0x3D01 /* CLUT at (0x10, 0xF4) */
#define FX_DOOM_GOLD_LATTICE_U     0x0 /* texture origin (u, v) in the page */
#define FX_DOOM_GOLD_LATTICE_V     0x0
#define FX_DOOM_SWIRL_SHELL_TPAGE 0x119 /* Hi_RegisterTexEffModel texture: 15-bit page (0x240, 0x100), abr 0, run-time swirl page */
#define FX_DOOM_SWIRL_SHELL_CLUT  0xFFFF /* no CLUT (15-bit page) */
#define FX_DOOM_SWIRL_SHELL_U     0x0 /* texture origin (u, v) in the page */
#define FX_DOOM_SWIRL_SHELL_V     0x90
#define FX_DOOM_GLOW_SHELL_CLUT 0xFFFF /* GLOW_SHELL samples the two display buffers (tpage from env->dispEnv, 15-bit, additive): no CLUT, */
#define FX_DOOM_GLOW_SHELL_U    0x40   /* texture origin (u, v) */
#define FX_DOOM_GLOW_SHELL_V    0

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

extern Fx0ecTri fx_0ec_shatterTris[FX_DOOM_SHATTER_TRIS];
extern Fx0ecShard fx_0ec_shatterShards[FX_DOOM_SHATTER_TRIS];

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
extern Fx0ecShard fx_0ec_splitShards[FX_DOOM_SPLIT_SHARDS];

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
