#ifndef FX_1A4_H
#define FX_1A4_H

/* Effect module fx_1a4 (dir 13 file 0x1a4). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    SVECTOR pos14;
    SVECTOR pos1C;
    SVECTOR pos24; /* caster position */
    SVECTOR unk2C;
    s32 unk34; /* brightness */
    s32 unk38; /* flag */
    s16 unk3C; /* countdowns */
    s16 unk3E;
    s16 unk40;
} Fx1a4Work80; /* 0x44 */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR rot8; /* model rotations */
    SVECTOR rot10;
    SVECTOR rot18;
    VECTOR pos20; /* model position */
    VECTOR scale30; /* model scale */
    s32 unk40; /* brightness */
    SVECTOR pos44; /* caster body position */
    s16 unk4C; /* counter */
    s16 unk4E;
    s16 unk50;
    s16 unk52; /* fade countdown */
    u16 model54; /* models */
    u16 model56;
    u16 model58;
} Fx1a4Work81; /* 0x5C */

extern Unk800F78A4 *fx_1a4_env;
extern BtlModel fx_1a4_effModels[];
extern SVECTOR D_801E83D0;
extern SVECTOR D_801E83D8;

static void fxm_lerpSvec(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);

/* copied */
extern Unk800F78A4 *fx_1a4_env;
/* end copied */

#endif
