#ifndef FX_001_H
#define FX_001_H

/* Effect module fx_001 (dir 13 file 0x01). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    s32 unk8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    Pool6CEntry *emit14;
    Pool6CEntry *emit18;
    Pool6CEntry *emit1C;
    SVECTOR rot20; /* model rotation */
    VECTOR pos28; /* model position */
    VECTOR scale38; /* model scale */
    s32 rgb48; /* model brightness */
    SVECTOR pos4C; /* target body position */
    SVECTOR pos54;
    SVECTOR pos5C; /* caster position */
    s32 fadeIn64; /* fade-in countdown */
    s32 fadeOut68; /* fade-out countdown */
    u16 model6C; /* models */
    u16 model6E;
} Fx001Work; /* 0x70 */

extern Unk800F78A4 *fx_001_env;
extern s16 fx_001_texAnimXY[][2];
extern BtlModel fx_001_effModels[];
extern s32 fx_001_trails[];

static void fxm_lerpSvec(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);

#endif
