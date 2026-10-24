#ifndef FX_0F5_H
#define FX_0F5_H

/* Effect module fx_0f5 (dir 13 file 0x0F5). */
#include "btlfx_mod.h"

extern Unk800F78A4 *D_801E8A74;
extern s32 D_801E8A28; /* flicker phase */
extern u8 D_801E89F8[];

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame */
    SVECTOR unk8; /* base position */
    SVECTOR unk10; /* position */
    SVECTOR unk18; /* rotation */
    s32 unk20; /* model */
} Fx0f5Work80; /* 0x24 */

extern BtlModel D_801E8A84[];

void func_801E7704(SVECTOR *pos, SVECTOR *rot, VECTOR *scale, s32 r0, s32 r1, s32 c, s32 tpage, s32 clut, s32 u, s32 v,
                   s32 dr, s32 ang, s32 dang, s32 dseg, s32 abr, s32 flicker);

#endif
