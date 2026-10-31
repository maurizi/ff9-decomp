#ifndef FX_0FF_H
#define FX_0FF_H

/* Effect module fx_0ff (dir 13 file 0x0FF). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR unk8[6]; /* model rotations */
    VECTOR unk38[6]; /* model positions */
    VECTOR scale98; /* model scale */
    s32 unkA8; /* model brightness */
    SVECTOR posAC; /* target body position */
    SVECTOR posB4; /* target position */
    s32 unkBC; /* fade-in countdown */
    s32 unkC0; /* fade-out countdown */
    u16 modelC4[3]; /* models */
} Fx0ffWork;

typedef struct {
    s16 unk0; /* radius */
    s16 unk2; /* angle */
    s16 unk4; /* spin */
} Fx0ffOrbit;

extern u16 D_801E7C68[3];
extern Unk800F78A4 *fx_0ff_env;
extern BtlModel fx_0ff_effModels[];
extern Fx0ffOrbit D_801E7F28[6];
static void fxm_lerpSvec(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);

#endif
