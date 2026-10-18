#ifndef FX_126_H
#define FX_126_H

/* Effect module fx_126 (dir 13 file 0x126). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* models */
    s32 unk4;
    VECTOR unk8; /* model positions */
    VECTOR unk18;
    SVECTOR unk28; /* model rotations */
    SVECTOR unk30;
    VECTOR unk38; /* model scale */
    u8 pad48[0x50 - 0x48];
    SVECTOR unk50; /* curve control points */
    SVECTOR unk58;
    SVECTOR unk60;
    SVECTOR unk68;
    SVECTOR unk70;
    u8 pad78[0x7C - 0x78];
    SVECTOR unk7C[4]; /* unit body positions */
    s32 unk9C[4]; /* unit hit */
} Fx126Work; /* 0xAC */

extern Unk800F78A4 *D_801E80DC;
extern BtlModel D_801E80EC[];

s32 fx_126_op80(s32 mode, Fx126Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);
void func_801E7D48(s32 len, s32 t, SVECTOR *p0, SVECTOR *p1, SVECTOR *p2, SVECTOR *p3, SVECTOR *p4, VECTOR *out);

#endif
