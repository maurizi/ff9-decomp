#ifndef FX_03D_H
#define FX_03D_H

/* Effect module fx_03d (dir 13 file 0x3D). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* caster body position */
    SVECTOR unk10; /* caster rotation */
} Fx03dWork80;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    u8 pad10[8];
    SVECTOR pos18;
} Fx03dWork81; /* 0x20 */

typedef struct {
    SVECTOR rot0; /* model rotation */
    VECTOR pos8; /* model position */
    VECTOR scale18; /* model 1 scale */
    VECTOR scale28; /* model 2 scale */
    s32 unk38; /* model brightness */
    s16 unk3C; /* fade-in countdown */
    s16 unk3E; /* fade-out countdown */
    u16 model40; /* models */
    u16 model42;
} Fx03dWork82; /* 0x44 */

extern Unk800F78A4 *fx_03d_env;
extern BtlModel fx_03d_effModels[];
extern s32 D_801E8000[];

static void fxm_lerpSvec(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);

#endif
