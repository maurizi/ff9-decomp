#ifndef FX_097_H
#define FX_097_H

/* Effect module fx_097 (dir 13 file 0x97): Blizzaga, one target (party and enemy). Fire / ice drawn as a grid of
 * texel quads (fxm_drawTexelQuads). Evidence: [data] the party ability / item tables and enemy sequences that play
 * this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_097 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_BLIZZAGA_CRYSTAL_SHEEN_2 0xF /* model: crystal 2 shape, reflection-map texture */
#define FX_BLIZZAGA_CRYSTAL_SHEEN_3 0x10 /* model: crystal 3 shape, reflection-map texture */
#define FX_BLIZZAGA_CRYSTAL_SHEEN   0x11 /* model: main crystal shape, reflection-map texture */
#define FX_BLIZZAGA_ICE_CRYSTAL_2   0x12 /* model: ice crystal spike (hexagonal, ice texture) */
#define FX_BLIZZAGA_ICE_CRYSTAL_3   0x13 /* model: wide ice crystal spike */
#define FX_BLIZZAGA_ICE_CRYSTAL     0x14 /* model: main ice crystal spike */
#define FX_BLIZZAGA_FROST_RING      0x1B /* rings: blue gouraud ring on the ground */
#define FX_BLIZZAGA_ICE_SPIKES      0x1C /* emitter: four fans drawn as narrowing cyan, green, violet and blue icicle-shaped light spikes */
#define FX_BLIZZAGA_SNOW_BURST      0x1D /* emitter: rings textured with snow flakes */
#define FX_BLIZZAGA_SHATTER_WAVE    0x1E /* emitter: blue ripple wave */
#define FX_BLIZZAGA_ICE_FLARES_2    0x1F /* emitter: cyan ice glints */
#define FX_BLIZZAGA_ICE_FLARES      0x20 /* emitter: cyan ice glints, four elements */
#define FX_BLIZZAGA_FREEZE_BURST    0x21 /* emitter: white-pink rings + spark sprites */
#define FX_BLIZZAGA_FROST_RING_2    0x22 /* emitter: pale blue gouraud rings */
#define FX_BLIZZAGA_SNOW_RING       0x23 /* rings: textured ring of blue snow flakes */
#define FX_BLIZZAGA_SNOW_RING_2     0x24 /* rings: textured ring of pale snow flakes */
#define FX_BLIZZAGA_SHARD_MIST      0x25 /* emitter: grey mist trailing each shard (pool item) */
#define FX_BLIZZAGA_MIST            0x26 /* emitter: grey frost mist at the crystal base */
#define FX_BLIZZAGA_MIST_HIGH       0x27 /* emitter: grey frost mist higher up */
#define FX_BLIZZAGA_ICE_SPARKLES    0x28 /* emitter: small blue ice sparkles at the shatter */
#define FX_BLIZZAGA_ICE_SPARKLES_2  0x29 /* emitter: small blue ice sparkles while the crystal grows */

/* fx_097 choreography (docs/fxdata.md, "Choreography literals"): op80's states, their lengths and keyframes, the
 * crystal / shard / piece counts and sizes, the screen tints, and the VRAM rect of the reflection map. */
#define FX_BLIZZAGA_STATE_BULGE   0 /* the ground bulges (grid drawn with the reflection map) */
#define FX_BLIZZAGA_STATE_RISE    1 /* the bulge rises, FX_BLIZZAGA_SNOW_RING_2 spreads */
#define FX_BLIZZAGA_STATE_CRYSTAL 2 /* FX_BLIZZAGA_ICE_CRYSTAL + CRYSTAL_SHEEN appear with a white-blue flash */
#define FX_BLIZZAGA_STATE_SHARDS  3 /* a shard pool item every other frame around the crystal */
#define FX_BLIZZAGA_STATE_SPIKES  4 /* FX_BLIZZAGA_ICE_SPIKES */
#define FX_BLIZZAGA_STATE_HOLD    5 /* the crystal stands; at the end it shatters */
#define FX_BLIZZAGA_STATE_SHATTER 6 /* the pieces fly, FX_BLIZZAGA_SHATTER_WAVE */
#define FX_BLIZZAGA_BULGE_FRAMES        6    /* BULGE's length */
#define FX_BLIZZAGA_RISE_FRAMES         10   /* RISE's length; at its end the mists and ICE_SPARKLES_2 start */
#define FX_BLIZZAGA_CRYSTAL_FRAMES      4    /* CRYSTAL's length */
#define FX_BLIZZAGA_CRYSTAL_FLASH_FRAMES 6   /* CRYSTAL: additive white-blue flash fading over this many frames */
#define FX_BLIZZAGA_SPIKES_FRAMES       8    /* SPIKES's length */
#define FX_BLIZZAGA_HOLD_FRAMES         10   /* HOLD's length; at its end the flares, bursts and pieces start */
#define FX_BLIZZAGA_SHATTER_WAVE_FRAME  5    /* SHATTER: FX_BLIZZAGA_SHATTER_WAVE starts */
#define FX_BLIZZAGA_SHATTER_FLASH_FRAMES 8   /* SHATTER: additive white-blue flash (fades by >> 3) */
#define FX_BLIZZAGA_RING_FADE_FRAMES    18   /* SHATTER: the snow / frost rings fade out */
#define FX_BLIZZAGA_SHATTER_FRAMES      0x18 /* SHATTER's length (the op ends) */
#define FX_BLIZZAGA_SHARD_COUNT         6    /* shards spawned in SHARDS (fx_097_shardRot entries) */
#define FX_BLIZZAGA_WIDE_SHARDS         3    /* shards 0..2 are ICE_CRYSTAL_3 / CRYSTAL_SHEEN_3 and break into the second piece set */
#define FX_BLIZZAGA_CRYSTAL_PIECES      6    /* the main crystal's pieces: model20[0..5] */
#define FX_BLIZZAGA_SHARD_PIECES        4    /* a shard's pieces: model20[6..9], the wide shards' model20[10..13] */
#define FX_BLIZZAGA_PIECE_MODELS        14   /* model20 / fx_097_pieceBlocks entries */
#define FX_BLIZZAGA_SHARD_GROW_FRAMES   4    /* pool item: a shard grows */
#define FX_BLIZZAGA_SHARD_RING_FRAMES   12   /* pool item: FX_BLIZZAGA_SNOW_RING_2 at the shard's tip */
#define FX_BLIZZAGA_PIECE_FRAMES        22   /* pool item: a piece flies and fades */
#define FX_BLIZZAGA_SHARD_SCALE         0x8AA /* a shard's full scale */
#define FX_BLIZZAGA_CRYSTAL_SCALE       0x1DFE /* the main crystal's x / y scale */
#define FX_BLIZZAGA_CRYSTAL_SCALE_Z     0x1354
#define FX_BLIZZAGA_BURST_HEIGHT        0x22A /* pos60 (SNOW_BURST, FREEZE_BURST, SHATTER_WAVE ...) is this far above pos58 */
#define FX_BLIZZAGA_MIST_HEIGHT         0x5C6 /* pos68 (FX_BLIZZAGA_MIST_HIGH) is this far above pos58 */
#define FX_BLIZZAGA_POOL_COUNT          0x20 /* shard and piece pool items */
#define FX_BLIZZAGA_MODEL_COUNT         0x19 /* btlfx_initEffModels slots (20 registered: 3 crystals, 3 sheens, 14 pieces) */
#define FX_BLIZZAGA_BULGE_GRID          14   /* fxm_drawBulgeGrid14: quads per side */
#define FX_BLIZZAGA_DARKEN_RGB          0xFF, 0xFF, 0    /* subtractive screen tint (the screen turns blue): RISE's end, HOLD's end */
#define FX_BLIZZAGA_BLUE_TINT_RGB       0, 0, 0x80       /* additive, with the RISE-end darken */
#define FX_BLIZZAGA_WHITE_FLASH_RGB     0xFF, 0xFF, 0xFF /* additive, the first frame of CRYSTAL and SHATTER */
/* CRYSTAL_SHEEN models (tpage 0xBA, v 0x80) and the bulge grid (BULGE_TPAGE, v 0x80) sample the rect the file fills
 * with a "dummy" placeholder; btlfx_drawScrollMove copies the file's reflection texture from (SRC_X, SRC_Y) into it. */
#define FX_BLIZZAGA_BULGE_TPAGE         0x9A   /* 8-bit page (0x280, 0x100) */
#define FX_BLIZZAGA_BULGE_CLUT          0x3D40 /* CLUT (0, 0xF5) */
#define FX_BLIZZAGA_REFLECTION_VRAM_X   0x280
#define FX_BLIZZAGA_REFLECTION_VRAM_Y   0x180
#define FX_BLIZZAGA_REFLECTION_VRAM_W   0x40
#define FX_BLIZZAGA_REFLECTION_VRAM_H   0x80
#define FX_BLIZZAGA_REFLECTION_SRC_X    0x280
#define FX_BLIZZAGA_REFLECTION_SRC_Y    0x100

extern s16 fx_097_pieceDist[];
extern Unk800F78A4 *fx_097_env;

void fx_097_piecePos(s16 *a, s16 *b, s32 i, s16 *out);
typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx097Vec3;

/* one shard, a pool item */
typedef struct {
    s16 state0; /* state */
    s16 flying2; /* flying */
    s32 index4; /* index */
    s32 frame8; /* frame */
    Fx097Vec3 posC; /* position << 4 */
    SVECTOR pos18; /* velocity */
    SVECTOR rot20; /* rotation */
    s16 spinX28; /* rotation speed */
    s16 spinY2A;
    s32 model2C; /* models */
    s32 model30;
    Pool6CEntry *emit34;
} Fx097Item; /* 0x38 */

typedef struct {
    u8 pad0[0x20];
    s32 models20[22]; /* models */
    s16 shatter78;
} Fx097Work;

extern SVECTOR fx_097_shardRot[];
static s32 fx_097_op80PoolTask(s32 mode, Fx097Item *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, s32 pool,
                  Fx097Work *work);

typedef struct {
    u32 state0; /* state */
    s32 frame4; /* frame */
    s32 model8; /* models */
    s32 modelC;
    s32 model10;
    s32 model14;
    s32 model18;
    s32 model1C;
    s32 model20[14];
    SVECTOR pos58; /* summon position */
    SVECTOR pos60;
    SVECTOR pos68;
    SVECTOR unk70; /* unused: only the -= 0x80 in mode 1 reads it */
    s16 shatter78;
    s16 shardCount7A; /* shards spawned */
    s32 scrollStep7C;
    s32 scroll80;
    Pool6CEntry *emit84;
} Fx097Work80; /* 0x88 */

extern BtlModelBuf fx_097_effModels[];
extern s32 fx_097_pieceBlocks[FX_BLIZZAGA_PIECE_MODELS];

/* propagated */

/* PsyQ gte_stszotz: SZ3 / 4 */
#define gte_stszotz(r0) __asm__ volatile( \
    "mfc2\t$12, $19;" \
    "nop;" \
    "sra\t$12, $12, 2;" \
    "sw\t$12, 0(%0)" \
    : : "r"(r0) : "$12", "memory")
/* VZ0 = VZ1 = VZ2 = 0 */
#define gte_ldVZ012_0() __asm__ volatile( \
    "mtc2\t$0, $1;" \
    "mtc2\t$0, $3;" \
    "mtc2\t$0, $5")

/* V0..V2 x and y only (VZ0..VZ2 are loaded once with gte_ldVZ012_0) */
#define gte_ldv3xy(r0, r1, r2) __asm__ volatile( \
    "lwc2\t$0, 0(%0);" \
    "lwc2\t$2, 0(%1);" \
    "lwc2\t$4, 0(%2)" \
    : : "r"(r0), "r"(r1), "r"(r2))
#define gte_ldv0xy(r0) __asm__ volatile( \
    "lwc2\t$0, 0(%0)" \
    : : "r"(r0))
static void fxm_drawTexelQuads(SVECTOR *pos, SVECTOR *rot, s32 tp, s32 x, s32 y, s32 div, s32 idx, s32 size, s32 w, s32 h,
                   s32 scale, s32 bright, s32 abr, u16 *colors, s32 skip);

#endif
