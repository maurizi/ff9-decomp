#ifndef FX_090_H
#define FX_090_H

/* Effect module fx_090 (dir 13 file 0x90). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    s32 unk4;
    s32 unk8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    Pool6CEntry *emit14;
    Pool6CEntry *emit18;
    Pool6CEntry *emit1C;
    Pool6CEntry *emit20;
    SVECTOR unk24; /* model rotation */
    VECTOR unk2C; /* model position */
    VECTOR unk3C; /* model scale */
    s32 unk4C; /* model brightness */
    SVECTOR unk50; /* position: env->targets->centerX..Z */
    SVECTOR unk58; /* unk50 raised by 0x320 */
    SVECTOR unk60; /* unk50 raised by 0x4B0 */
    s32 unk68; /* fade-in countdown */
    s32 unk6C; /* fade-out countdown */
    u16 model70; /* models */
    u16 model72;
} Fx090Work; /* 0x74 */

extern s16 D_801E7DB4[][2];
extern Unk800F78A4 *fx_090_env;
extern BtlModel fx_090_effModels[];
extern s32 D_801E7FD8[];

static void fxm_lerpSvec(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);

#endif
