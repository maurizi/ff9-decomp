#ifndef FX_1F1_H
#define FX_1F1_H

/* Effect module fx_1f1 (dir 13 file 0x1f1). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* delay */
    s32 unk4; /* countdown */
    s32 unk8; /* saved height */
    SVECTOR unkC; /* unit position */
    s32 unk14;
    s32 unk18; /* models */
    s32 unk1C;
    SVECTOR unk20; /* model rotations */
    SVECTOR unk28;
    VECTOR unk30; /* model scales */
    VECTOR unk40;
    VECTOR unk50; /* model position */
} Fx1f1Unit; /* 0x60 */

extern s32 D_801E7E9C[];
extern Unk800F78A4 *D_801E7F04;
extern Fx1f1Unit D_801E7F14[];
extern BtlModel D_801E8214[];

s32 func_801E7704(s32 curve, s32 n, s32 t, s32 from, s32 to);

#endif
