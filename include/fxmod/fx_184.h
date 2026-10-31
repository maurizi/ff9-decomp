#ifndef FX_184_H
#define FX_184_H

/* Effect module fx_184 (dir 13 file 0x184). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR pos0; /* caster body position */
    SVECTOR pos8; /* target body position */
    SVECTOR unk10; /* from */
    SVECTOR rot18; /* model rotation */
    VECTOR pos20; /* model position */
    VECTOR scale30; /* model scale */
    u16 model40; /* model */
} Fx184Work80;

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4;
} Fx184Work81;

typedef struct {
    SVECTOR pos0;
    SVECTOR unk8;
    SVECTOR pos10; /* target position */
    SVECTOR unk18; /* destination */
    SVECTOR unk20; /* angles */
    SVECTOR unk28;
    s32 unk30; /* state */
    s32 unk34; /* steps */
    s32 unk38; /* step */
} Fx184Work82;

extern Unk800F78A4 *fx_184_env;
extern BtlModel fx_184_effModels[];

static void fxm_lerpSvec(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);

/* propagated */
static s32 fxm_moveUnitArc(s32 unit, SVECTOR *dst, s32 frame, s32 n, s32 height, SVECTOR *start);

#endif
