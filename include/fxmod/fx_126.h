#ifndef FX_126_H
#define FX_126_H

/* Effect module fx_126 (dir 13 file 0x126): enemy Boomerang (Ark). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    s32 model0; /* models */
    s32 model4;
    VECTOR pos8; /* model positions */
    VECTOR pos18;
    SVECTOR rot28; /* model rotations */
    SVECTOR rot30;
    VECTOR scale38; /* model scale */
    u8 pad48[0x50 - 0x48];
    SVECTOR pos50; /* curve control points */
    SVECTOR pos58;
    SVECTOR pos60;
    SVECTOR pos68;
    SVECTOR pos70;
    u8 pad78[0x7C - 0x78];
    SVECTOR pos7C[4]; /* unit body positions */
    s32 done9C[4]; /* unit hit */
} Fx126Work; /* 0xAC */

extern Unk800F78A4 *fx_126_env;
extern BtlModelBuf fx_126_effModels[];

s32 fx_126_op80(s32 mode, Fx126Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);
void fx_126_quarticBezier(s32 len, s32 t, SVECTOR *p0, SVECTOR *p1, SVECTOR *p2, SVECTOR *p3, SVECTOR *p4, VECTOR *out);

#endif
