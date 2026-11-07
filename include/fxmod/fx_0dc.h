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

/* fx_0dc choreography (docs/fxdata.md, "Choreography literals"; as fx_097 Blizzaga without its ground bulge):
 * op80's states, their lengths and keyframes, the crystal / shard / piece counts and sizes, the screen tints, the
 * model textures and the VRAM rect of the reflection map; op84's sword flash. */
#define FX_SWDMAG_BLIZZAGA_STATE_CRYSTAL 2 /* op80 (starts here): ICE_CRYSTAL + CRYSTAL_SHEEN appear with a white-blue flash */
#define FX_SWDMAG_BLIZZAGA_STATE_SHARDS  3 /* a shard pool item every other frame around the crystal */
#define FX_SWDMAG_BLIZZAGA_STATE_SPIKES  4 /* FX_SWDMAG_BLIZZAGA_ICE_SPIKES */
#define FX_SWDMAG_BLIZZAGA_STATE_HOLD    5 /* the crystal stands; at the end it shatters */
#define FX_SWDMAG_BLIZZAGA_STATE_SHATTER 6 /* the pieces fly, FX_SWDMAG_BLIZZAGA_SHATTER_WAVE */
#define FX_SWDMAG_BLIZZAGA_CRYSTAL_FRAMES       4    /* CRYSTAL's length */
#define FX_SWDMAG_BLIZZAGA_CRYSTAL_FLASH_FRAMES 6    /* CRYSTAL: additive white-blue flash fading over this many frames */
#define FX_SWDMAG_BLIZZAGA_SPIKES_FRAMES        8    /* SPIKES's length */
#define FX_SWDMAG_BLIZZAGA_HOLD_FRAMES          10   /* HOLD's length; at its end the flares, bursts and pieces start */
#define FX_SWDMAG_BLIZZAGA_SHATTER_WAVE_FRAME   8    /* SHATTER: FX_SWDMAG_BLIZZAGA_SHATTER_WAVE starts */
#define FX_SWDMAG_BLIZZAGA_SHATTER_FLASH_FRAMES 8    /* SHATTER: additive white-blue flash (fades by >> 3) */
#define FX_SWDMAG_BLIZZAGA_RING_FADE_FRAMES     18   /* SHATTER: the snow / frost rings fade out */
#define FX_SWDMAG_BLIZZAGA_SHATTER_FRAMES       24   /* SHATTER's length (the op ends) */
#define FX_SWDMAG_BLIZZAGA_SHARD_COUNT          6    /* shards spawned in SHARDS (fx_0dc_shardAngles entries) */
#define FX_SWDMAG_BLIZZAGA_WIDE_SHARDS          3    /* shards 0..2 are ICE_CRYSTAL_3 / CRYSTAL_SHEEN_3 and break into the second piece set */
#define FX_SWDMAG_BLIZZAGA_CRYSTAL_PIECES       6    /* the main crystal's pieces: model20[0..5] */
#define FX_SWDMAG_BLIZZAGA_SHARD_PIECES         4    /* a shard's pieces: model20[6..9], the wide shards' model20[10..13] */
#define FX_SWDMAG_BLIZZAGA_PIECE_MODELS         14   /* model20 / fx_0dc_shardModelBlocks entries */
#define FX_SWDMAG_BLIZZAGA_SHARD_STATE_START 0 /* pool item: FX_SWDMAG_BLIZZAGA_SHARD_MIST starts */
#define FX_SWDMAG_BLIZZAGA_SHARD_STATE_GROW  1 /* pool item: the shard grows */
#define FX_SWDMAG_BLIZZAGA_SHARD_STATE_HOLD  2 /* pool item: it stands until split78, then breaks into its pieces */
#define FX_SWDMAG_BLIZZAGA_SHARD_GROW_FRAMES    4    /* pool item: SHARD_STATE_GROW's length */
#define FX_SWDMAG_BLIZZAGA_SHARD_RING_FRAMES    12   /* pool item: FX_SWDMAG_BLIZZAGA_SNOW_RING_2 at the shard's tip */
#define FX_SWDMAG_BLIZZAGA_PIECE_FRAMES         22   /* pool item: a piece flies and fades */
#define FX_SWDMAG_BLIZZAGA_SHARD_SCALE          0x8AA /* a shard's full scale */
#define FX_SWDMAG_BLIZZAGA_CRYSTAL_SCALE        0x1DFE /* the main crystal's x / y scale */
#define FX_SWDMAG_BLIZZAGA_CRYSTAL_SCALE_Z      0x1354
#define FX_SWDMAG_BLIZZAGA_BURST_HEIGHT         0x22A /* pos60 (SNOW_BURST, FREEZE_BURST, SHATTER_WAVE ...) is this far above pos58 */
#define FX_SWDMAG_BLIZZAGA_MIST_HEIGHT          0x5C6 /* pos68 (FX_SWDMAG_BLIZZAGA_MIST_HIGH) is this far above pos58 */
#define FX_SWDMAG_BLIZZAGA_POOL_COUNT           0x20 /* shard and piece pool items */
#define FX_SWDMAG_BLIZZAGA_MODEL_COUNT          0x19 /* btlfx_initEffModels slots (20 registered: 3 crystals, 3 sheens, 14 pieces) */
#define FX_SWDMAG_BLIZZAGA_DARKEN_RGB           0xFF, 0xFF, 0    /* subtractive screen tint (the screen turns blue): init, HOLD's end */
#define FX_SWDMAG_BLIZZAGA_BLUE_TINT_RGB        0, 0, 0x80       /* additive, with the init darken */
#define FX_SWDMAG_BLIZZAGA_WHITE_FLASH_RGB      0xFF, 0xFF, 0xFF /* additive, the first frame of CRYSTAL and SHATTER */
/* Hi_RegisterTexEffModel textures: the ICE_CRYSTAL models and the 14 pieces (ice texture), the CRYSTAL_SHEEN models
 * (the reflection rect below) */
#define FX_SWDMAG_BLIZZAGA_ICE_TPAGE   0x9A   /* 8-bit page (0x280, 0x100), abr 0 */
#define FX_SWDMAG_BLIZZAGA_ICE_CLUT    0x3DC0 /* CLUT at (0x0, 0xF7) */
#define FX_SWDMAG_BLIZZAGA_ICE_U       0x80
#define FX_SWDMAG_BLIZZAGA_ICE_V       0
#define FX_SWDMAG_BLIZZAGA_SHEEN_TPAGE 0xBA   /* 8-bit page (0x280, 0x100), abr 1 */
#define FX_SWDMAG_BLIZZAGA_SHEEN_CLUT  0x3D40 /* CLUT at (0x0, 0xF5) */
#define FX_SWDMAG_BLIZZAGA_SHEEN_U     0
#define FX_SWDMAG_BLIZZAGA_SHEEN_V     0x80
/* the CRYSTAL_SHEEN models sample the rect the file fills with a "dummy" placeholder; btlfx_drawScrollMove copies the
 * file's reflection texture from (SRC_X, SRC_Y) into it (scroll80 slows over the first SCROLL_SLOW_FRAMES). */
#define FX_SWDMAG_BLIZZAGA_REFLECTION_VRAM_X 0x280
#define FX_SWDMAG_BLIZZAGA_REFLECTION_VRAM_Y 0x180
#define FX_SWDMAG_BLIZZAGA_REFLECTION_VRAM_W 0x40
#define FX_SWDMAG_BLIZZAGA_REFLECTION_VRAM_H 0x80
#define FX_SWDMAG_BLIZZAGA_REFLECTION_SRC_X  0x280
#define FX_SWDMAG_BLIZZAGA_REFLECTION_SRC_Y  0x100
#define FX_SWDMAG_BLIZZAGA_SCROLL_SLOW_FRAMES 11
#define FX_SWDMAG_BLIZZAGA_SWORD_STATE_APPEAR  0 /* op84: SWORD_GLINT / SWORD_FLASH grow in at the weapon */
#define FX_SWDMAG_BLIZZAGA_SWORD_STATE_FLICKER 1 /* op84: they flicker and turn */
#define FX_SWDMAG_BLIZZAGA_SWORD_STATE_FADE    2 /* op84: they spread and fade (the op ends) */
#define FX_SWDMAG_BLIZZAGA_SWORD_APPEAR_FRAMES  3 /* SWORD_STATE_APPEAR's length */
#define FX_SWDMAG_BLIZZAGA_SWORD_FLICKER_FRAMES 9 /* SWORD_STATE_FLICKER's length */
#define FX_SWDMAG_BLIZZAGA_SWORD_FADE_FRAMES    9 /* SWORD_STATE_FADE's length */

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
