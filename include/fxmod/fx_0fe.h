#ifndef FX_0FE_H
#define FX_0FE_H

/* Effect module fx_0fe (dir 13 file 0x0FE). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR unk4[6]; /* model rotations */
    VECTOR unk34[6]; /* model positions */
    VECTOR unk94; /* model scale */
    s32 unkA4; /* model brightness */
    SVECTOR unkA8; /* target body position */
    s32 unkB0; /* fade-in countdown */
    s32 unkB4; /* fade-out countdown */
    u16 unkB8[3]; /* models */
} Fx0feWork;

typedef struct {
    s16 unk0; /* radius */
    s16 unk2; /* angle */
    s16 unk4; /* spin */
} Fx0feOrbit;

extern u16 D_801E7BF4[3];
extern Unk800F78A4 *fx_0fe_env;
extern BtlModel fx_0fe_effModels[];
extern Fx0feOrbit D_801E7EB4[6];

static void fxm_lerpSvec(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);

#endif
