#ifndef FX_0B1_H
#define FX_0B1_H

/* Effect module fx_0b1 (dir 13 file 0xB1): Carbuncle, full summon. Evidence: btlfx_start and btlfx_loadFile get the same id for summons (docs/runtime-findings.md). */
#include "btlfx_mod.h"

/* fx_0b1 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_CARBUNCLE_RING_BURST_2     0x1 /* emitter: cone of white-red, lilac, cyan and blue-grey gouraud rings, fan */
#define FX_CARBUNCLE_LENS_FLARE       0x9 /* emitter: orbiting white glow trail with a blue lens streak */
#define FX_CARBUNCLE_ORBIT_GLOW       0xA /* emitter: orbiting particle + sprite, two copies; the render reads unrelated texels (noise), texture probably drawn at run time (look pending) */
#define FX_CARBUNCLE_RED_RING         0xC /* emitter: white-red gouraud rings, fans */
#define FX_CARBUNCLE_WHITE_LIGHT_STREAK 0xD /* emitter: fan, a white horizontal light streak */
#define FX_CARBUNCLE_LENS_STREAK      0xE /* emitter: blue-white lens streak, white-red ring */
#define FX_CARBUNCLE_CAMERA_KEYS      0xF /* table: camera key path for fxm_camKeyAdd (target 1) */
#define FX_CARBUNCLE_LIGHT_BEAM       0x10 /* trail: blue-white light beam, drawn twice (fx_0b1_beams) */
#define FX_CARBUNCLE_RING_BURST       0x11 /* emitter: cone of white-yellow, lilac, cyan and blue-grey gouraud rings, fan */
#define FX_CARBUNCLE_CRYSTAL_SPARKLES 0x12 /* emitter: white crystal-flake sprites and blue sparkles, fans */
#define FX_CARBUNCLE_FLAME_GLINT      0x13 /* emitter: orange-blue flame-shaped glints, red gouraud rings, fans */
#define FX_CARBUNCLE_RING_BURST_3     0x14 /* emitter: cone of white-red, lilac, cyan and blue-grey gouraud rings, fan */
#define FX_CARBUNCLE_LENS_STREAK_2    0x15 /* emitter: blue-white lens streak, white-red ring, orange sprites */
#define FX_CARBUNCLE_ORBIT_GLOW_2     0x16 /* emitter: orbiting particle + sprite; the render reads unrelated texels (noise), texture probably drawn at run time (look pending) */
#define FX_CARBUNCLE_CAMERA_KEYS_3    0x17 /* table: camera key path for fxm_camKeyAdd (target 3) */
#define FX_CARBUNCLE_COLOR_RINGS      0x18 /* emitter: pink / green / pink gouraud rings at each target body */
#define FX_CARBUNCLE_CAMERA_KEYS_2    0x19 /* table: camera key path for fxm_camKeyAdd (target 2) */
#define FX_CARBUNCLE_PINK_RING        0x21 /* emitter: pink gouraud ring and swirl-ring particles at each target body */
#define FX_CARBUNCLE_BACKDROP         0x22 /* table: background layers for btlfx_drawBackdrop */
#define FX_CARBUNCLE_STAR_CRYSTAL     0x23 /* model: four-point star crystal (texture 0x177), one per target */
#define FX_CARBUNCLE_ORBIT_GLOW_3     0x24 /* emitter: orbiting particle + sprite; the render reads unrelated texels (noise), texture probably drawn at run time (look pending) */

typedef struct {
    s32 start0; /* start frame */
    s32 framesLeft4; /* frames left, -1 = free */
    u32 dst8; /* destination slots, 4 bits each (1-based) */
    u32 srcC; /* source slots, 4 bits each (1-based, 0 = none) */
    s16 *data10; /* data */
} Fx0b1Track; /* 0x14 */

extern s32 fx_0b1_camKeyFrame; /* frame */
extern Fx0b1Track fx_0b1_camKeys[16];
extern SVECTOR fx_0b1_camKeyPoints[];
extern s32 fx_0b1_fadeTimer; /* fade timer */
extern s32 fx_0b1_models[4]; /* models */
extern SVECTOR fx_0b1_modelRot; /* model rotation */
extern VECTOR fx_0b1_modelScale; /* model scale */
extern VECTOR fx_0b1_modelPos[4]; /* model positions */
extern SVECTOR fx_0b1_bodyPos[4]; /* unit body positions */
extern BtlModelBuf fx_0b1_effModels[];

static void fxm_camKeyReset(void);
static void fxm_camKeyAdd(s32 start, u32 dst, u32 src, s16 *data);
static void fxm_camKeyUpdate(void);
extern Unk800F78A4 *fx_0b1_env;
static s32 fxm_easeLerp(s32 curve, s32 n, s32 t, s32 from, s32 to);

typedef struct {
    u8 pad0[0x180];
    BtlMotion *motion180;
} Fx0b1SummonData;

typedef struct {
    u8 pad0[0x54];
    Fx0b1SummonData *summon54; /* g_btlfxLoader.summon */
    u8 pad58[0x60 - 0x58];
    s32 model60; /* summon model */
} Fx0b1Summon;

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR pts4[21];
} Fx0b1Trail; /* 0xAC */

#define SUMMON0B1(env) ((Fx0b1Summon *)(env)->loader)

extern SVECTOR fx_0b1_summonRot;
extern VECTOR fx_0b1_summonPos;
extern VECTOR fx_0b1_summonScale;
extern SVECTOR fx_0b1_startPos;
extern s32 fx_0b1_timers[8];
extern Pool6CEntry *fx_0b1_emitter0;
extern SVECTOR fx_0b1_emitPos0;
extern Pool6CEntry *fx_0b1_emitter1;
extern SVECTOR fx_0b1_emitPos1;
extern Pool6CEntry *fx_0b1_emitter2;
extern SVECTOR fx_0b1_emitPos2;
extern s32 fx_0b1_trails[];
extern Pool6CEntry *fx_0b1_emitter3;
extern SVECTOR fx_0b1_emitPos3;
extern SVECTOR fx_0b1_emitStartPos3;
extern Pool6CEntry *fx_0b1_emitter4;
extern SVECTOR fx_0b1_emitPos4;
extern Pool6CEntry *fx_0b1_emitter5;
extern SVECTOR fx_0b1_emitPos5;
extern Pool6CEntry *fx_0b1_emitter6;
extern SVECTOR fx_0b1_emitPos6;
extern Pool6CEntry *fx_0b1_emitter7;
extern SVECTOR fx_0b1_emitPos7;
extern Pool6CEntry *fx_0b1_emitter8;
extern SVECTOR fx_0b1_emitPos8;
extern Pool6CEntry *fx_0b1_emitter9;
extern SVECTOR fx_0b1_emitPos9;
extern Pool6CEntry *fx_0b1_emitter10;
extern SVECTOR fx_0b1_emitPos10;
extern Fx0b1Trail fx_0b1_beams[2];

#endif
