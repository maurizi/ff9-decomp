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
    s32 model18[6][3]; /* models of the six pieces */
    s32 pad60[3];
    s32 model6C[7];
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
extern s32 fx_197_pieceModelFileIds[6][3];
extern s32 fx_197_modelFileIds[7];
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
