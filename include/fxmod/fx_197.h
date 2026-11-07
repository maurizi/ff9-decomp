#ifndef FX_197_H
#define FX_197_H

/* Effect module fx_197 (dir 13 file 0x197): Shiva, short summon. Evidence: btlfx_start and btlfx_loadFile get the same id for summons (docs/runtime-findings.md). */
#include "btlfx_mod.h"

/* fx_197 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s6.md; "(look pending)" = by kind, appearance not settled). */
#define FX_SHIVA_STAR_GLINTS   0x4 /* emitter: blue glows, white / green star glints, blue glow domes (byte-identical to fx_026 entry 0x7) */
#define FX_SHIVA_SHATTER_RINGS 0xB /* emitter: blue gouraud rings + ice chunk sprites, sparks (pool item per piece; byte-identical to fx_026 entry 0x1A) */
#define FX_SHIVA_ICE_DEBRIS_2  0xC /* emitter: small ice chunk sprites (byte-identical to fx_026 entry 0x21) */
#define FX_SHIVA_BLUE_TRAIL    0xD /* emitter: spiralling trail, blue strip texture (byte-identical to fx_026 entry 0x1F) */
#define FX_SHIVA_ICE_DEBRIS    0xE /* emitter: small ice chunk sprites (byte-identical to fx_026 entry 0x20) */
#define FX_SHIVA_CYAN_RING     0xF /* emitter: cyan-white gouraud ring (byte-identical to fx_026 entry 0x22) */
#define FX_SHIVA_ICE_CHUNKS    0x1A /* emitter: falling ice chunk sprites, faint sparkles (byte-identical to fx_026 entry 0x6) */
#define FX_SHIVA_ICICLE 0xD8 /* 'so' model: long ice shard, pale cracked-ice texture (0xC0+ = battle load buffer, entry 0x18) */
#define FX_SHIVA_ICICLE_2 0xD9 /* 'so' model: long ice shard, dark cracked-ice texture (0xC0+ = battle load buffer, entry 0x19) */

/* fx_197 choreography (docs/fxdata.md, "Choreography literals"; fx_026's names). */
#define FX_SHIVA_MODELS 0x20 /* btlfx_initEffModels slots */
#define FX_SHIVA_BARRIER_STATE_BUILD 0 /* op81: the pieces fly in (pool items) */
#define FX_SHIVA_BARRIER_STATE_SHATTER 1 /* the shatter line sweeps down, the pieces and the core break */
#define FX_SHIVA_BUILD_FRAMES 0x10 /* FX_SHIVA_BARRIER_STATE_BUILD's length */
#define FX_SHIVA_SHATTER_OP_FRAMES 0x7E /* FX_SHIVA_BARRIER_STATE_SHATTER's length (the op ends) */
#define FX_SHIVA_BARRIER_POOL_COUNT 7 /* op81 poolCount */
#define FX_SHIVA_PIECE_COUNT 6 /* barrier pieces (pool items 0-5, fx_197_pieceModelFileIds rows) */
#define FX_SHIVA_PIECE_CORE 6 /* pool item piece index of the core (model6C[6]) */
#define FX_SHIVA_PIECE_STATE_FLY 0 /* piece pool item: flies along its fx_197_piecePathFileIds track, */
#define FX_SHIVA_PIECE_FLY_FRAMES 3 /* for this many frames; then a blue tint and FX_SHIVA_SHATTER_RINGS */
#define FX_SHIVA_PIECE_STATE_STAND 1 /* grows through its stages (FX_SHIVA_PIECE_FORM_FRAMES) until the shatter */
#define FX_SHIVA_PIECE_STATE_SHATTER 2 /* breaks into shards and fades out */
#define FX_SHIVA_PIECE_STAGES 3 /* morph-stage models per piece (fx_197_pieceModelFileIds rows) */
#define FX_SHIVA_PIECE_FORM_FRAMES 0x10 /* FX_SHIVA_PIECE_STATE_STAND: fx_197_pieceModelFrames entries; then the piece's model6C is drawn too */
#define FX_SHIVA_PIECE_FADE_FRAME 0xE /* FX_SHIVA_PIECE_STATE_SHATTER: fades out from this frame, */
#define FX_SHIVA_PIECE_FADE_FRAMES 6 /* over this many; the item ends */
#define FX_SHIVA_PIECE_SHARD_LIFE 0x14 /* frames a piece's shard flies (fxm_shatterInit life) */
#define FX_SHIVA_CORE_STATE_STAND 0 /* core pool item: fades in (8 frames), waits for the shatter */
#define FX_SHIVA_CORE_STATE_SHATTER 1 /* breaks into shards */
#define FX_SHIVA_CORE_FADE_FRAME 0x37 /* FX_SHIVA_CORE_STATE_SHATTER: fades out from this frame (over 32, << 12 >> 5), */
#define FX_SHIVA_CORE_FRAMES 0x57 /* ends at this one */
#define FX_SHIVA_CORE_SHARD_LIFE 0x57 /* frames a core shard flies */
#define FX_SHIVA_PIECE_TINT_RGB 0, 0x20, 0x46 /* additive screen tint (blue) as a piece lands */
#define FX_SHIVA_SHATTER_LINE_Y 0x400 /* op81: the shatter line's start height; it sweeps up 0x80 per frame, triangles below it break off */
#define FX_SHIVA_CYAN_RING_FRAME 0x12 /* FX_SHIVA_BARRIER_STATE_SHATTER: FX_SHIVA_CYAN_RING starts */
#define FX_SHIVA_DARKEN_FRAME 0x13 /* the screen is darkened (FX_SHIVA_DARKEN_RGB) */
#define FX_SHIVA_SHATTER_FRAME 0x14 /* the pieces 0-4 and the core shatter, the debris and blue trail start, blue flash */
#define FX_SHIVA_SHATTER_5_FRAME 0x18 /* piece 5 shatters */
#define FX_SHIVA_STAR_GLINTS_FRAME 0x20 /* FX_SHIVA_STAR_GLINTS start */
#define FX_SHIVA_ICE_CHUNKS_FRAME 0x2C /* FX_SHIVA_ICE_CHUNKS start */
#define FX_SHIVA_FLASH_FADE_FRAMES 0x24 /* the blue flash fades over this many frames ((frame - SHATTER_FRAME) << 7 >> 4) */
#define FX_SHIVA_DARKEN_RGB 0x80, 0x80, 0x80 /* subtractive screen tint */
#define FX_SHIVA_FLASH_RGB 0, 0x80, 0xFF /* additive screen tint (blue) */
#define FX_SHIVA_FLASH_LEVEL 0x80 /* the fading flash's blue (green = blue / 4) */
#define FX_SHIVA_BLUE_TRAIL_TRAIL_SLOTS 0xC /* FX_SHIVA_BLUE_TRAIL's trail buffer fx_197_op81Trails: slots of */
#define FX_SHIVA_BLUE_TRAIL_TRAIL_STRIDE 0x4C /* this many bytes (Pool6CEntry trailSlots / trailStride) */
/* The icicles' 8-bit page (0x280, 0x100): every op81 frame btlfx_drawScrollMove copies (ICICLE_2_SRC_X, _Y) into
 * ICICLE_2's rect, scrolled; in the shatter state also ICICLE's texture into the v 128 rect below (BARRIER_VRAM). */
#define FX_SHIVA_ICICLE_2_VRAM_X 0x280
#define FX_SHIVA_ICICLE_2_VRAM_Y 0x100
#define FX_SHIVA_ICICLE_2_VRAM_W 0x40
#define FX_SHIVA_ICICLE_2_VRAM_H 0x80
#define FX_SHIVA_ICICLE_2_SRC_X 0x280
#define FX_SHIVA_ICICLE_2_SRC_Y 0x180
#define FX_SHIVA_BARRIER_VRAM_X 0x2C0
#define FX_SHIVA_BARRIER_VRAM_Y 0x180
#define FX_SHIVA_BARRIER_VRAM_W 0x40
#define FX_SHIVA_BARRIER_VRAM_H 0x80
#define FX_SHIVA_BARRIER_SRC_X 0x2C0
#define FX_SHIVA_BARRIER_SRC_Y 0x100

typedef struct {
    SVECTOR vert0; /* pad: already split off */
    SVECTOR vert8;
    SVECTOR vert10;
} Fx197Tri; /* 0x18 */

typedef struct {
    s16 duration0;
    s16 count2; /* triangle count */
    s32 count4; /* triangles of the last model */
    Fx197Tri *tris8;
    Fx197Tri *nextTriC; /* next free triangle */
    Fx197Tri *modelTris10; /* first triangle of the last model */
} Fx197Shatter; /* 0x14 */

typedef struct {
    s32 pos0; /* position (fixed 20.12) */
    s32 pos4;
    s32 pos8;
    s16 velC; /* velocity */
    s16 velE;
    s16 vel10;
    s16 unk12; /* write-only (0x1000) */
    s16 spin14; /* spin */
    s16 spin16;
    s16 frame18; /* frame, -1 = free */
    s16 index1A; /* triangle */
} Fx197Shard; /* 0x1C */

typedef struct {
    s16 unk0; /* not read by the module */
    s16 count2; /* count */
    s16 keys4[1]; /* keys: 1, 3, 6 or 9 halfwords each */
} Fx197Track;

typedef struct {
    s32 index0; /* camera path */
    SVECTOR camPos4; /* camera target */
    SVECTOR camPosC; /* camera position */
} Fx197Work87; /* 0x14 */

/* camera paths: position track, target track, length */
typedef struct {
    u16 fileId0;
    u16 fileId2;
    u16 duration4;
    u16 unk6; /* not read by the module */
} Fx197CamPath;

extern Fx197CamPath fx_197_camPaths[];

static void fxm_keyGetVec(Fx197Track *track, s32 t, SVECTOR *out);
typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame */
    SVECTOR pos8; /* position */
    SVECTOR pos10;
    s32 model18[FX_SHIVA_PIECE_COUNT][FX_SHIVA_PIECE_STAGES]; /* models of the six pieces */
    s32 pad60[3];
    s32 model6C[FX_SHIVA_PIECE_COUNT + 1];
} Fx197Work81; /* 0x88 */

typedef struct {
    s16 state0; /* state */
    s16 index2; /* piece */
    s32 pad4;
    s32 unk8; /* write-only (cleared at spawn) */
    SVECTOR posC; /* position */
} Fx197Item81;

typedef struct {
    s32 shatterOfs0; /* shatter buffer, as an offset into env->unk30 */
    s32 trisOfs4; /* triangle buffer */
} Fx197Offsets;

extern Fx197Offsets fx_197_shatterBufs[];
extern s32 fx_197_piecePathFileIds[]; /* piece path file ids */
extern SVECTOR fx_197_pieceOffsets[]; /* piece landing offsets */
extern s32 fx_197_pieceModelFrames[]; /* model frame per animation frame */
extern Fx197Shatter fx_197_shatterBuf6;
extern Fx197Tri fx_197_shatterTris6[];
extern BtlModelBuf fx_197_effModels[];
extern s32 fx_197_model0;
extern s32 fx_197_model1;
extern s32 fx_197_pieceModelFileIds[FX_SHIVA_PIECE_COUNT][FX_SHIVA_PIECE_STAGES];
extern s32 fx_197_modelFileIds[FX_SHIVA_PIECE_COUNT + 1];
extern SVECTOR fx_197_basePos;
extern s32 fx_197_spawnPieces[];
extern s32 fx_197_overlayOn; /* 1 in the module image, which is reloaded for every cast (runtime, round 33) */
extern s32 fx_197_shatterFlag0;
extern s32 fx_197_shatterFlag1;
extern s32 fx_197_shatterFlag2;
extern u8 fx_197_op81Trails[];

static s32 fx_197_op81PoolTask(s32 mode, Fx197Item81 *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame,
                  Unk800DA1B8 *pool, Fx197Work81 *work);

/* copied */
extern Unk800F78A4 *fx_197_env;
extern Fx197Shatter *fx_197_shatters[8];
extern Fx197Shard *fx_197_shards[8];
extern s32 fx_197_shatterHeight; /* shatter line */
static Fx197Shard *fxm_shatterFreeShard(s32 slot);
/* end copied */

#endif
