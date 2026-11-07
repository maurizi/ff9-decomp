#ifndef FX_1CC_H
#define FX_1CC_H

/* Effect module fx_1cc (dir 13 file 0x1cc): Black Waltz 1's "Sealion appears". Fire / ice drawn as a grid of texel
 * quads (fxm_drawTexelQuads). Evidence: [data] the party ability / item tables and enemy sequences that play this
 * file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_1cc effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s7.md; "(look pending)" = by kind, appearance not settled). */
#define FX_SEALION_APPEARS_FREEZE_BURST    0x1 /* emitter: white-pink rings + spark sprites (same call site as fx_097 entry 0x21) (byte-identical to fx_098 entry 0x27) */
#define FX_SEALION_APPEARS_FROST_RING_2    0x5 /* emitter: pale blue gouraud rings (byte-identical to fx_097 entry 0x22) */
#define FX_SEALION_APPEARS_ICE_FLARES      0x6 /* emitter: cyan ice glints, four elements (byte-identical to fx_097 entry 0x20) */
#define FX_SEALION_APPEARS_ICE_FLARES_2    0x7 /* emitter: cyan ice glints (byte-identical to fx_097 entry 0x1F) */
#define FX_SEALION_APPEARS_ICE_SPARKLES_2  0xB /* emitter: small blue ice sparkles while the crystal grows (same call site as fx_097 entry 0x29) (byte-identical to fx_098 entry 0x29) */
#define FX_SEALION_APPEARS_MIST            0xD /* emitter: grey frost mist at the crystal base (same call site as fx_097 entry 0x26) (byte-identical to fx_098 entry 0x1C) */
#define FX_SEALION_APPEARS_SHARD_MIST      0xE /* emitter: grey mist trailing each shard (pool item) (same call site as fx_097 entry 0x25) (byte-identical to fx_098 entry 0x26) */
#define FX_SEALION_APPEARS_MIST_HIGH       0xF /* emitter: grey frost mist higher up (same call site as fx_097 entry 0x27) (byte-identical to fx_098 entry 0x28) */
#define FX_SEALION_APPEARS_CRYSTAL_SHEEN_2 0x1E /* model: crystal 2 shape, reflection-map texture (byte-identical to fx_097 entry 0xF) */
#define FX_SEALION_APPEARS_CRYSTAL_SHEEN_3 0x1F /* model: crystal 3 shape, reflection-map texture (byte-identical to fx_097 entry 0x10) */
#define FX_SEALION_APPEARS_CRYSTAL_SHEEN   0x20 /* model: main crystal shape, reflection-map texture (byte-identical to fx_097 entry 0x11) */
#define FX_SEALION_APPEARS_SNOW_RING       0x24 /* rings: textured ring of blue snow flakes */
#define FX_SEALION_APPEARS_FROST_RING      0x25 /* rings: lilac-blue gouraud ring on the ground */
#define FX_SEALION_APPEARS_SNOW_RING_2     0x26 /* rings: textured ring of pale blue snow flakes */
#define FX_SEALION_APPEARS_ICE_CRYSTAL_2   0x27 /* model: ice crystal spike (hexagonal, ice texture) (byte-identical to fx_097 entry 0x12) */
#define FX_SEALION_APPEARS_ICE_CRYSTAL_3   0x28 /* model: wide ice crystal spike (byte-identical to fx_097 entry 0x13) */
#define FX_SEALION_APPEARS_ICE_CRYSTAL     0x29 /* model: main ice crystal spike (byte-identical to fx_097 entry 0x14) */

/* fx_1cc choreography (docs/fxdata.md, "Choreography literals"; partial: op80's state lengths, the pool and work
 * sizes, the model textures and the primitive words; the effect keys inside the states stay numeric). */
#define FX_SEALION_APPEARS_BULGE_FRAMES  6    /* op80 state 0: the screen bulge grid (fxm_drawBulgeGrid14) swells */
#define FX_SEALION_APPEARS_MIST_FRAMES   10   /* state 1: the grid, SNOW_RING_2, MIST, MIST_HIGH, ICE_SPARKLES_2, a tint */
#define FX_SEALION_APPEARS_FLASH_FRAMES  4    /* state 2: ICE_CRYSTAL / CRYSTAL_SHEEN and a fading blue-white flash (6 frames) */
#define FX_SEALION_APPEARS_HOLD_FRAMES   8    /* state 4: the crystal models only */
#define FX_SEALION_APPEARS_BURST_FRAMES  10   /* state 5: ICE_FLARES(_2), FREEZE_BURST, FROST_RING_2, the pieces fly */
#define FX_SEALION_APPEARS_RINGS_FRAMES  24   /* state 6: SNOW_RING, FROST_RING, a fading flash; then the op ends */
#define FX_SEALION_APPEARS_POOL          0x20 /* op80: pool of Fx1ccObj (op80PoolTask) */
#define FX_SEALION_APPEARS_MODEL_SLOTS   0x19 /* op80: btlfx_initEffModels slots */
#define FX_SEALION_APPEARS_PIECES        14   /* op80: ice piece models (fx_1cc_pieceFileIds, model20) */
#define FX_SEALION_APPEARS_ICE_TPAGE     0x9A /* ICE_CRYSTAL(_2 / _3) and the pieces (Hi_RegisterTexEffModel): 8-bit page (0x280, 0x100) */
#define FX_SEALION_APPEARS_ICE_CLUT      0x3DC0 /* CLUT at (0x0, 0xF7) */
#define FX_SEALION_APPEARS_ICE_U         0x80
#define FX_SEALION_APPEARS_ICE_V         0
#define FX_SEALION_APPEARS_SHEEN_TPAGE   0xBA /* CRYSTAL_SHEEN(_2 / _3): 8-bit page (0x280, 0x100), abr 1 */
#define FX_SEALION_APPEARS_SHEEN_CLUT    0x3D40 /* CLUT at (0x0, 0xF5) */
#define FX_SEALION_APPEARS_SHEEN_U       0
#define FX_SEALION_APPEARS_SHEEN_V       0x80

extern Unk800F78A4 *fx_1cc_env;
extern s16 fx_1cc_pieceDists[];

typedef struct Fx1ccObj {
    s16 state0; /* state; flying pieces: piece index (< 6 picks the scale) */
    s16 kind2; /* 0: main object, 1: piece */
    s32 index4; /* direction index */
    s32 frame8; /* frame */
    s32 posXC; /* position, 28.4 (low halves used by the main object) */
    s32 posY10;
    s32 posZ14;
    s16 velX18; /* velocity (main object: SVECTOR position, the emitter origin) */
    s16 velY1A;
    s16 velZ1C;
    s16 unk1E; /* unused (pad of the main object's position) */
    SVECTOR rot20; /* rotation */
    s16 spin28; /* rotation speed */
    s16 spin2A;
    s32 model2C; /* model */
    s32 model30; /* model */
    Pool6CEntry *emit34; /* particle */
} Fx1ccObj;

typedef struct {
    u8 pad0[0x20];
    s32 model20[FX_SEALION_APPEARS_PIECES]; /* models */
    u8 pad58[0x78 - 0x58];
    s16 flag78; /* burst */
} Fx1ccInst;

extern SVECTOR fx_1cc_dirRots[];
void fx_1cc_piecePos(s16 *from, s16 *to, s32 i, s16 *out);

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    s32 model8; /* models */
    s32 modelC;
    s32 model10;
    s32 model14;
    s32 model18;
    s32 model1C;
    s32 model20[FX_SEALION_APPEARS_PIECES]; /* piece models */
    SVECTOR pos58; /* position */
    SVECTOR pos60;
    SVECTOR pos68;
    SVECTOR unk70; /* unused: only the -= 0x80 in mode 1 reads it */
    s16 flag78;
    s16 index7A; /* spawned directions */
    s32 frame7C;
    s32 scroll80;
    u8 unused84[0x88 - 0x84]; /* pad to the 0x88 bytes op80 asks for */
} Fx1ccWork80; /* 0x88 */

extern BtlModelBuf fx_1cc_effModels[];
extern s32 fx_1cc_pieceFileIds[14];
static s32 fx_1cc_op80PoolTask(s32 mode, Fx1ccObj *obj, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, s32 pool,
                  Fx1ccInst *inst);

s32 fx_1cc_op80(s32 mode, Fx1ccWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init);

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
static void fxm_drawBulgeGrid14(SVECTOR *pos, SVECTOR *rot, s32 scale, s32 bright, s32 tpage, s32 clut, s32 u, s32 v, s32 amp, s32 abr);

#endif
