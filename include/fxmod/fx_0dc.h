#ifndef FX_0DC_H
#define FX_0DC_H

/* Effect module fx_0dc (dir 13 file 0xDC): Sword Magic Blizzaga. Ice shards (pool items) that break into flying
 * pieces. Evidence: [data] the party ability / item tables and enemy sequences that play this file id
 * (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0dc effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s3.md; "(look pending)" = by kind, appearance not settled). */
#define FX_SWDMAG_BLIZZAGA_CRYSTAL_SHEEN_2 0xF /* model: crystal 2 shape, reflection-map texture (byte-identical to fx_097 entry 0xF) */
#define FX_SWDMAG_BLIZZAGA_CRYSTAL_SHEEN_3 0x10 /* model: crystal 3 shape, reflection-map texture (byte-identical to fx_097 entry 0x10) */
#define FX_SWDMAG_BLIZZAGA_CRYSTAL_SHEEN   0x11 /* model: main crystal shape, reflection-map texture (byte-identical to fx_097 entry 0x11) */
#define FX_SWDMAG_BLIZZAGA_ICE_CRYSTAL_2   0x12 /* model: ice crystal spike (hexagonal, ice texture) (byte-identical to fx_097 entry 0x12) */
#define FX_SWDMAG_BLIZZAGA_ICE_CRYSTAL_3   0x13 /* model: wide ice crystal spike (byte-identical to fx_097 entry 0x13) */
#define FX_SWDMAG_BLIZZAGA_ICE_CRYSTAL     0x14 /* model: main ice crystal spike (byte-identical to fx_097 entry 0x14) */
#define FX_SWDMAG_BLIZZAGA_FROST_RING      0x1B /* rings: blue gouraud ring on the ground */
#define FX_SWDMAG_BLIZZAGA_ICE_SPIKES      0x1C /* emitter: four fans of white-cyan icicle spikes (byte-identical to fx_097 entry 0x1C) */
#define FX_SWDMAG_BLIZZAGA_SNOW_BURST      0x1D /* emitter: rings textured with snow flakes (byte-identical to fx_097 entry 0x1D) */
#define FX_SWDMAG_BLIZZAGA_SHATTER_WAVE    0x1E /* emitter: blue ripple wave (byte-identical to fx_097 entry 0x1E) */
#define FX_SWDMAG_BLIZZAGA_ICE_FLARES_2    0x1F /* emitter: cyan ice glints (byte-identical to fx_097 entry 0x1F) */
#define FX_SWDMAG_BLIZZAGA_ICE_FLARES      0x20 /* emitter: cyan ice glints, four elements (byte-identical to fx_097 entry 0x20) */
#define FX_SWDMAG_BLIZZAGA_FREEZE_BURST    0x21 /* emitter: white-pink rings + spark sprites (byte-identical to fx_097 entry 0x21) */
#define FX_SWDMAG_BLIZZAGA_FROST_RING_2    0x22 /* emitter: pale blue gouraud rings (byte-identical to fx_097 entry 0x22) */
#define FX_SWDMAG_BLIZZAGA_SNOW_RING       0x23 /* rings: textured ring of blue snow flakes (byte-identical to fx_097 entry 0x23) */
#define FX_SWDMAG_BLIZZAGA_SNOW_RING_2     0x24 /* rings: textured ring of pale snow flakes (byte-identical to fx_097 entry 0x24) */
#define FX_SWDMAG_BLIZZAGA_SHARD_MIST      0x25 /* emitter: grey mist trailing each shard (pool item) (byte-identical to fx_097 entry 0x25) */
#define FX_SWDMAG_BLIZZAGA_MIST            0x26 /* emitter: grey frost mist at the crystal base (byte-identical to fx_097 entry 0x26) */
#define FX_SWDMAG_BLIZZAGA_MIST_HIGH       0x27 /* emitter: grey frost mist higher up (byte-identical to fx_097 entry 0x27) */
#define FX_SWDMAG_BLIZZAGA_SWORD_FLASH     0x28 /* rings: white 8-segment rings flashed rotating at the sword (op84) */
#define FX_SWDMAG_BLIZZAGA_SWORD_GLINT     0x29 /* rings: white 6-segment rings flashed at the sword (op84, first draw) */
#define FX_SWDMAG_BLIZZAGA_ICE_SPARKLES_2  0x2A /* emitter: small blue ice sparkles while the crystal grows (byte-identical to fx_097 entry 0x29) */
#define FX_SWDMAG_BLIZZAGA_ICE_SPARKLES    0x2B /* emitter: small blue ice sparkles at the shatter (byte-identical to fx_097 entry 0x28) */

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* time */
    SVECTOR pos8; /* caster bone position */
} Fx0dcWork84;

extern Unk800F78A4 *fx_0dc_env;

s32 fx_0dc_op84(s32 mode, Fx0dcWork84 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

/* propagated */

extern s16 fx_0dc_shardDists[];
void fx_0dc_shardPoint(s16 *a, s16 *b, s32 i, s16 *out);
typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx0dcVec3;

/* one shard, a pool item */
typedef struct {
    s16 state0; /* state; flying shards: piece index (< 6 picks the scale) */
    s16 flying2; /* flying */
    s32 shard4; /* index */
    s32 frame8; /* frame */
    Fx0dcVec3 posC; /* position << 4 */
    SVECTOR pos18; /* velocity */
    SVECTOR rot20; /* rotation */
    s16 spinX28; /* rotation speed */
    s16 spinY2A;
    s32 model2C; /* models */
    s32 model30;
    Pool6CEntry *emit34;
} Fx0dcItem; /* 0x38 */

typedef struct {
    u8 pad0[0x20];
    s32 model20[22]; /* models */
    s16 split78;
} Fx0dcWork;

extern SVECTOR fx_0dc_shardAngles[];
static s32 fx_0dc_op80PoolTask(s32 mode, Fx0dcItem *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, s32 a5,
                  Fx0dcWork *work);

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame */
    s32 model8; /* models */
    s32 modelC;
    s32 model10;
    s32 model14;
    s32 model18;
    s32 model1C;
    s32 model20[14]; /* shard models */
    SVECTOR pos58; /* target position */
    SVECTOR pos60;
    SVECTOR pos68;
    SVECTOR pos70;
    s16 split78;
    s16 spawnCount7A; /* shards spawned */
    s32 scrollFrame7C; /* frames of screen scroll (it slows over the first 11) */
    s32 scroll80; /* btlfx_drawScrollMove offset */
    Pool6CEntry *emit84;
} Fx0dcWork80; /* 0x88 */

extern s32 fx_0dc_shardModelBlocks[14];
extern BtlModelBuf fx_0dc_effModels[];

#endif
