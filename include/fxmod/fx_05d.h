#ifndef FX_05D_H
#define FX_05D_H

/* Effect module fx_05d (dir 13 file 0x5D). */
#include "btlfx_mod.h"

/* keyframe tracks read from the effect file: count, then the keys */
typedef struct {
    s16 unk0, unk2, unk4; /* position */
    s16 unk6, unk8, unkA; /* rotation */
} Fx05dKey12;

typedef struct {
    s16 x, y, z;
} Fx05dKey6;

typedef struct {
    u16 unk0; /* count */
    Fx05dKey12 *unk4;
} Fx05dTrack12;

typedef struct {
    u16 unk0; /* count */
    Fx05dKey6 *unk4;
} Fx05dTrack6;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    SVECTOR unk10; /* target position */
    SVECTOR unk18; /* model rotation */
    VECTOR unk20; /* model position */
    VECTOR unk30; /* model scale */
    s32 unk40;
    SVECTOR unk44; /* model position, for the trail */
    SVECTOR unk4C;
    s32 unk54; /* frames into the current morph */
    s32 unk58;
    u16 unk5C[6]; /* morph models */
} Fx05dWork; /* 0x68 */

extern u8 D_801E8048[6];
extern Unk800F78A4 *fx_05d_env;
extern BtlModel fx_05d_effModels[];
extern s32 D_801E8558[];
extern SVECTOR D_801E8DC0;
extern Fx05dTrack12 D_801E8DA0;
extern Fx05dTrack12 D_801E8DA8;
extern Fx05dTrack6 D_801E8DB0;
extern Fx05dTrack6 D_801E8DB8;

static void fxm_rotPosTrackInit(Fx05dTrack12 *track, u16 *data);
static s32 fxm_rotPosTrackGet(Fx05dTrack12 *track, s32 t, SVECTOR *rot, SVECTOR *pos);
static void fxm_vecTrackInit(Fx05dTrack6 *track, u16 *data);
static s32 fxm_vecTrackGet(Fx05dTrack6 *track, s32 t, VECTOR *out);

#endif
