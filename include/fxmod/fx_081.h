#ifndef FX_081_H
#define FX_081_H

/* Effect module fx_081 (dir 13 file 0x81). */
#include "btlfx_mod.h"

extern Unk800F78A4 *fx_081_env;
extern SVECTOR D_801EA728;
extern SVECTOR D_801EA730;
extern Pool6CEntry *D_801EA738;
extern Unk80100C78Entry *fx_081_task;
extern s32 D_801EA73C; /* glow level */
extern Pool6CEntry *D_801EA740;
extern SVECTOR D_801EA748; /* caster body position */
extern BtlModel fx_081_effModels[];
extern u8 D_801E8EE8[];

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx081Vec3;

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame */
    s32 model8; /* model */
    s32 modelC; /* model */
    SVECTOR unk10;
    Fx081Vec3 unk18;
    SVECTOR unk24;
    Pool6CEntry *emit2C;
    Pool6CEntry *emit30;
    Pool6CEntry *emit34;
} Fx081Work80; /* 0x38 */

static void fx_081_op80ElemDraw(Pool30Entry *pt, Pool6CEntry *p, SVECTOR *pos, s32 arg3, s32 *arg4, s32 arg5, s32 arg6, s32 frame,
                   s32 n);

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame */
    s32 unk8; /* priority base */
    SVECTOR unkC; /* target position */
    SVECTOR unk14;
    Fx081Vec3 unk1C;
    s32 model28; /* model */
} Fx081Work81; /* 0x2C */

extern s16 D_801E8E7C[10]; /* ring point counts */
s32 fx_081_op81(s32 mode, Fx081Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

#endif
