#ifndef FX_08E_H
#define FX_08E_H

/* Effect module fx_08e (dir 13 file 0x8E). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    Pool6CEntry *emit14;
    Pool6CEntry *emit18;
    SVECTOR unk1C; /* model rotation */
    VECTOR unk24; /* model position */
    VECTOR unk34; /* model scale */
    s32 unk44; /* model brightness */
    SVECTOR unk48; /* target body position */
    SVECTOR unk50;
    SVECTOR unk58; /* caster position */
    s32 unk60; /* fade-in countdown */
    s32 unk64; /* fade-out countdown */
    u16 unk68; /* models */
    u16 unk6A;
} Fx08eWork; /* 0x6C */

extern Unk800F78A4 *fx_08e_env;
extern s16 D_801E7D9C[][2];
extern BtlModel fx_08e_effModels[];
extern s32 D_801E7FC0[];

static void fxm_lerpSvec(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);

#endif
