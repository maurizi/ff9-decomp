#ifndef FX_17D_0_H
#define FX_17D_0_H

/* Effect module fx_17d_0 (dir 13 file 0x17D): Ark, full summon (load group 0). Streak field (fxm_drawStreakField).
 * Evidence: btlfx_start and btlfx_loadFile get the same id for summons (docs/runtime-findings.md). */
#include "btlfx_mod.h"

/* fx_17d_0 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_ARK_CAMERA_KEYS_2    0x19 /* table: per-frame position keys for camera point 1 (op 0x82) */
#define FX_ARK_CAMERA_KEYS      0x1A /* table: per-frame position keys for camera point 0 (op 0x82) */
#define FX_ARK_MORPH_KEYS       0x1B /* table: per-frame morph weights, first morph pair */
#define FX_ARK_NOISE_SPHERE 0x1C /* 'so' model registered at the start of op 0x80: a sphere, its texture reads as white noise in the render (look pending) */
#define FX_ARK_RED_ROCK_LAYER   0x1D /* sprite: red-brown rock / cloud texture drawn on screen */
#define FX_ARK_MORPH_PATH       0x1E /* table: per-frame position + rotation keys for the first morph models */
#define FX_ARK_MORPH_PATH_2     0x1F /* table: per-frame position + rotation keys for the second morph models */
#define FX_ARK_MORPH_KEYS_2     0x20 /* table: per-frame morph weights, second morph pair */
#define FX_ARK_MORPH_KEYS_3     0x21 /* table: per-frame morph weights for the second morph models */
#define FX_ARK_GREY_TRAIL       0x22 /* trail: grey noise strip (pool task) */
#define FX_ARK_VIOLET_RING      0x23 /* emitter: white-violet-blue and white-cream-purple gouraud rings */
#define FX_ARK_CAMERA_ROLL_KEYS 0x25 /* table: per-frame keys whose third value is camera point 0 roll (op 0x82) */

typedef struct {
    s16 unk0; /* write-only (cleared at spawn) */
    s16 speed2;
    s32 index4; /* line index */
    SVECTOR pos8;
    s16 unk10; /* unused */
} Fx17d0Part;

typedef struct {
    s16 file0;
    s16 morphFile2;
    s16 keys4; /* key file */
    s16 morphKeys6; /* morph key file */
} Fx17d0Piece;

extern Fx17d0Piece fx_17d_0_pieceFiles[21];
extern Unk800F78A4 *fx_17d_0_env;
extern u8 fx_17d_0_partTrails[][0x24];

void fx_17d_0_drawCenterBandY(u32 *ot, s32 abr, s32 h, s32 r, s32 g, s32 b);
static void fxm_randDiscPoints(s32 n, s32 radius, SVECTOR *pts);
static void fxm_drawStreakField(u32 *ot, s32 n, s32 projH, s32 dz, SVECTOR *rot, SVECTOR *pts, CVECTOR *rgb, s32 fade, s32 abr);
void fx_17d_0_drawPieces(Unk80100C78Entry *entry, SVECTOR *origin, s32 frame, s32 models, s16 *morph, s32 scale);
s32 fx_17d_0_op82(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);
static s32 fx_17d_0_op80PoolTask(s32 mode, Fx17d0Part *p, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);

/* the PsyQ gte_ReadGeomScreen: store the projection distance H */
#define gte_ReadGeomScreen(r0) __asm__ volatile( \
    "cfc2\t$12, $26;" \
    "nop;" \
    "sw\t$12, 0(%0)" \
    : : "r"(r0) : "$12", "memory")

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx17d0Vec3;

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame */
    SVECTOR pos8; /* summon position */
    SVECTOR pos10; /* pieces origin */
    Fx17d0Vec3 pos18; /* model position */
    u8 pad24[0x30 - 0x24];
    SVECTOR unk30; /* write-only (cleared at init) */
    SVECTOR rot38; /* model rotation */
    SVECTOR pos40; /* spawn position */
    SVECTOR pos48; /* camera */
    u8 pad50[0x58 - 0x50];
    s32 count58; /* spark count */
    s16 model5C[21]; /* piece models */
    s16 model86[21]; /* piece morph models */
    s16 modelB0[5]; /* body models */
    s16 modelBA; /* model */
} Fx17d0Work80; /* 0xBC */

typedef struct {
    u8 pad0[0x18];
    u16 texInfo18[6];
    u16 texInfo24[6];
    u16 texPos30[6];
} Fx17d0SummonData;

typedef struct {
    u8 pad0[0x54];
    Fx17d0SummonData *summon54; /* g_btlfxLoader.summon */
} Fx17d0Summon;

extern SVECTOR fx_17d_0_basePos;
extern s16 fx_17d_0_texModelFileIds[];
extern s16 fx_17d_0_texSlots[];
extern SVECTOR fx_17d_0_streakPoints[];
extern BtlModelBuf fx_17d_0_effModels[];
extern VECTOR fx_17d_0_modelScale;

#endif
