#ifndef FX_197_H
#define FX_197_H

/* Effect module fx_197 (dir 13 file 0x197): Shiva, short summon. Evidence: btlfx_start and btlfx_loadFile get the same id for summons (docs/runtime-findings.md). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR unk0; /* pad: already split off */
    SVECTOR unk8;
    SVECTOR unk10;
} Fx197Tri; /* 0x18 */

typedef struct {
    s16 unk0;
    s16 unk2; /* triangle count */
    s32 unk4; /* triangles of the last model */
    Fx197Tri *unk8;
    Fx197Tri *unkC; /* next free triangle */
    Fx197Tri *unk10; /* first triangle of the last model */
} Fx197Shatter; /* 0x14 */

typedef struct {
    s32 unk0; /* position (fixed 20.12) */
    s32 unk4;
    s32 unk8;
    s16 unkC; /* velocity */
    s16 unkE;
    s16 unk10;
    s16 unk12;
    s16 unk14; /* spin */
    s16 unk16;
    s16 unk18; /* frame, -1 = free */
    s16 unk1A; /* triangle */
} Fx197Shard; /* 0x1C */

typedef struct {
    s16 unk0;
    s16 unk2; /* count */
    s16 unk4[1]; /* keys: 1, 3, 6 or 9 halfwords each */
} Fx197Track;

typedef struct {
    s32 unk0; /* camera path */
    SVECTOR unk4; /* camera target */
    SVECTOR unkC; /* camera position */
} Fx197Work87; /* 0x14 */

/* camera paths: position track, target track, length */
typedef struct {
    u16 unk0;
    u16 unk2;
    u16 unk4;
    u16 unk6;
} Fx197CamPath;

extern Fx197CamPath D_801E9270[];

static void fxm_keyGetVec(Fx197Track *track, s32 t, SVECTOR *out);
typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame */
    SVECTOR unk8; /* position */
    SVECTOR pos10;
    s32 unk18[6][3]; /* models of the six pieces */
    s32 pad60[3];
    s32 model6C[7];
} Fx197Work81; /* 0x88 */

typedef struct {
    s16 unk0; /* state */
    s16 unk2; /* piece */
    s32 pad4;
    s32 unk8;
    SVECTOR posC; /* position */
} Fx197Item81;

typedef struct {
    s32 unk0; /* shatter buffer, as an offset into env->unk30 */
    s32 unk4; /* triangle buffer */
} Fx197Offsets;

extern Fx197Offsets D_801E9194[];
extern s32 D_801E91D4[]; /* piece path file ids */
extern SVECTOR D_801E91EC[]; /* piece landing offsets */
extern s32 D_801E921C[]; /* model frame per animation frame */
extern Fx197Shatter D_801EAFC0;
extern Fx197Tri D_801EC5C0[];
extern BtlModel fx_197_effModels[];
extern s32 D_801EAF70;
extern s32 D_801EAF74;
extern s32 D_801E9130[6][3];
extern s32 D_801E9178[7];
extern SVECTOR D_801E9128;
extern s32 D_801E925C[];
extern s32 D_801E91C4;
extern s32 D_801E91C8;
extern s32 D_801E91CC;
extern s32 D_801E91D0;
extern u8 D_801E92D0[];

static s32 fx_197_op81PoolTask(s32 mode, Fx197Item81 *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame,
                  Unk800DA1B8 *pool, Fx197Work81 *work);

/* copied */
extern Unk800F78A4 *fx_197_env;
extern Fx197Shatter *D_801EAF80[8];
extern Fx197Shard *D_801EAFA0[8];
extern s32 D_801EAFBC; /* shatter line */
static Fx197Shard *fxm_shatterFreeShard(s32 i);
/* end copied */

#endif
