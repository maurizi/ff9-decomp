#ifndef FX_078_H
#define FX_078_H

/* Effect module fx_078 (dir 13 file 0x78). */
#include "btlfx_mod.h"

/* keyframe tracks read from the effect file: count, then the keys (as in fx_05d) */
typedef struct {
    s16 unk0, unk2, unk4; /* position */
    s16 unk6, unk8, unkA; /* rotation */
} Fx078Key12;

typedef struct {
    s16 x, y, z;
} Fx078Key6;

typedef struct {
    u16 unk0; /* count */
    Fx078Key12 *unk4;
} Fx078Track12;

typedef struct {
    u16 unk0; /* count */
    Fx078Key6 *unk4;
} Fx078Track6;

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
    SVECTOR unk44; /* trail position */
    u16 model4C[10]; /* models */
} Fx078Work; /* 0x60 */

extern u8 D_801E7D18[10];
extern Unk800F78A4 *fx_078_env;
extern BtlModel fx_078_effModels[];
extern s32 D_801E854C[];
extern Fx078Track12 D_801E8EAC;
extern Fx078Track6 D_801E8EB4;
extern Fx078Track6 D_801E8EBC;
extern SVECTOR D_801E8EC4;

static void fxm_rotPosTrackInit(Fx078Track12 *track, u16 *data);
static s32 fxm_rotPosTrackGet(Fx078Track12 *track, s32 t, SVECTOR *rot, SVECTOR *pos);
static void fxm_posTrackInit(Fx078Track6 *track, u16 *data);
static s32 fxm_posTrackGet(Fx078Track6 *track, s32 t, SVECTOR *out);
static void fxm_vecTrackInit(Fx078Track6 *track, u16 *data);
static s32 fxm_vecTrackGet(Fx078Track6 *track, s32 t, VECTOR *out);

#endif
