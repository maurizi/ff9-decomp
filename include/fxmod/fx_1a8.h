#ifndef FX_1A8_H
#define FX_1A8_H

/* Effect module fx_1a8 (dir 13 file 0x1a8). */
#include "btlfx_mod.h"

extern Unk800F78A4 *D_801E97CC;
extern s32 D_801E97D4[2]; /* models */

void func_801E7704(s32 arg0, s32 arg1, SVECTOR *pos, SVECTOR *rot, s32 *scale, s32 arg5, s32 arg6, s32 t, s32 n);
void func_801E7828(s32 arg0, s32 arg1, SVECTOR *pos, SVECTOR *rot, s32 *scale, s32 arg5, s32 arg6, s32 t, s32 n);
void func_801E7950(SVECTOR *pos, SVECTOR *rot, VECTOR *scale, s32 r0, s32 r1, s32 z0, s32 z1, s32 col, s32 tpage,
                   s32 clut, s32 u, s32 v, s32 ang, s32 h, s32 abr, s32 step);

#endif
