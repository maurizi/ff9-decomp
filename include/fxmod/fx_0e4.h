#ifndef FX_0E4_H
#define FX_0E4_H

/* Effect module fx_0e4 (dir 13 file 0x0E4). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR rot8; /* model rotation */
    VECTOR pos10; /* model position */
    VECTOR scale20; /* model scale */
    s32 unk30; /* model brightness */
    SVECTOR pos34; /* target position */
    SVECTOR rot3C; /* target rotation */
    SVECTOR pos44;
    SVECTOR unk4C;
    SVECTOR unk54;
    s16 unk5C;
    s16 unk5E;
    s16 unk60; /* drop countdown */
    s16 unk62; /* fade countdown */
    s32 unk64; /* state */
    u16 model68[3]; /* models */
} Fx0e4Work;

extern u16 D_801E7CF8[3];
extern Unk800F78A4 *fx_0e4_env;
extern BtlModel fx_0e4_effModels[];

static void fxm_lerpSvec(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);

#endif
