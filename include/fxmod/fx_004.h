#ifndef FX_004_H
#define FX_004_H

/* Effect module fx_004 (dir 13 file 0x04). */
#include "btlfx_mod.h"

/* fx_004 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_BLIZZARA_ICE_REFLECTION  0xB /* model: crystal shell drawn with the reflection map */
#define FX_BLIZZARA_FROST_MIST      0xD /* emitter: grey frost mist puffs */
#define FX_BLIZZARA_SHARD_MIST      0xE /* emitter: grey mist trailing each flying shard (pool item) */
#define FX_BLIZZARA_ICE_FLARES_2    0xF /* emitter: cyan ice glints */
#define FX_BLIZZARA_ICE_FLARES      0x10 /* emitter: cyan ice glints, four elements */
#define FX_BLIZZARA_FREEZE_BURST    0x11 /* emitter: white-pink rings + spark sprites */
#define FX_BLIZZARA_FROST_RING      0x12 /* emitter: lilac-blue gouraud rings */
#define FX_BLIZZARA_ICE_CRYSTAL     0x13 /* model: long ice crystal spike (ice texture), splits in three */
#define FX_BLIZZARA_WHITE_RING      0x14 /* emitter: white gouraud ring at the start */
#define FX_BLIZZARA_CRYSTAL_PIECE_0 0x15 /* model: first piece of the split crystal */
#define FX_BLIZZARA_CRYSTAL_PIECE_1 0x16 /* model: second piece of the split crystal */
#define FX_BLIZZARA_CRYSTAL_PIECE_2 0x17 /* model: third piece of the split crystal */
#define FX_BLIZZARA_SHATTER_WAVE    0x19 /* emitter: blue ripple wave */

/* fx_004 choreography (docs/fxdata.md, "Choreography literals"): op80's states and their lengths, the crystal
 * pool items' phases, and the VRAM rect of the reflection map. */
#define FX_BLIZZARA_STATE_CRYSTALS 0 /* one FX_BLIZZARA_ICE_CRYSTAL pool item spawns per frame */
#define FX_BLIZZARA_STATE_GLEAM    1 /* the crystals stand, the reflection map scrolls slower and slower */
#define FX_BLIZZARA_STATE_SHATTER  2 /* split28 set: the crystals split into their three pieces and fly apart */
#define FX_BLIZZARA_CRYSTALS_FRAMES    9    /* FX_BLIZZARA_STATE_CRYSTALS's length (spawns on frames 0..9) */
#define FX_BLIZZARA_GLEAM_FRAMES       14   /* FX_BLIZZARA_STATE_GLEAM's length; at its end the freeze burst, frost ring and flares start */
#define FX_BLIZZARA_SHATTER_WAVE_FRAME 0    /* FX_BLIZZARA_STATE_SHATTER: FX_BLIZZARA_SHATTER_WAVE starts */
#define FX_BLIZZARA_SHATTER_FRAMES     21   /* FX_BLIZZARA_STATE_SHATTER's length (the op ends) */
#define FX_BLIZZARA_CRYSTAL_COUNT      10   /* crystals: pool items, fx_004_pieceRots entries */
#define FX_BLIZZARA_CRYSTAL_GROW_FRAMES 4   /* pool item: the crystal grows for this many frames, then holds until the split */
#define FX_BLIZZARA_PIECES_FRAMES      20   /* pool item: the split pieces fly and fade for this many frames */
#define FX_BLIZZARA_MODEL_COUNT        6    /* btlfx_initEffModels slots (5 registered: crystal, 3 pieces, reflection) */
/* FX_BLIZZARA_ICE_REFLECTION samples (tpage 0xBA = 8-bit page (0x280, 0x100), v 0x80) the rect the file fills with a
 * "dummy" placeholder; every frame btlfx_drawScrollMove copies the file's reflection texture from (SRC_X, SRC_Y)
 * into it, scrolled vertically by scroll30. */
#define FX_BLIZZARA_REFLECTION_VRAM_X  0x280
#define FX_BLIZZARA_REFLECTION_VRAM_Y  0x180
#define FX_BLIZZARA_REFLECTION_VRAM_W  0x40
#define FX_BLIZZARA_REFLECTION_VRAM_H  0x80
#define FX_BLIZZARA_REFLECTION_SRC_X   0x280
#define FX_BLIZZARA_REFLECTION_SRC_Y   0x100

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx004Pos;

/* pool item: a model that grows, then splits into three pieces that fly apart */
typedef struct {
    s32 piece0; /* index into fx_004_pieceRots */
    s32 state4; /* state */
    s32 frame8; /* frame */
    Fx004Pos posC[3];
    SVECTOR rot30; /* rotation once split */
    s16 spin38; /* rotation speed */
    s16 spin3A;
    s32 size3C; /* size */
    Pool6CEntry *emit40;
} Fx004Item; /* 0x44 */

typedef struct {
    s32 state0;
    s32 model4; /* models */
    s32 model8;
    s32 modelC;
    s32 model10;
    s32 model14;
    SVECTOR pos18;
    SVECTOR pos20;
    s16 split28; /* split now */
    s16 count2A;
    s32 frameCount2C; /* incremented every update, never read */
    s32 scroll30;
    u8 pad34[4]; /* unused: op80 asks for 0x38 bytes (fx_04d's work is 0x34) */
} Fx004Work; /* 0x38 */

extern SVECTOR fx_004_pieceRots[];
extern Unk800F78A4 *fx_004_env;
extern BtlModelBuf fx_004_effModels[];

static s32 fx_004_op80PoolTask(s32 mode, Fx004Item *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame,
                  Unk800DA1B8 *pool, Fx004Work *work);

#endif
