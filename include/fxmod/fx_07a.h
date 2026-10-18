#ifndef FX_07A_H
#define FX_07A_H

/* Effect module fx_07a (dir 13 file 0x7A). */
#include "btlfx_mod.h"

extern Unk800F78A4 *D_801E94F4;
extern s32 D_801E94F8;
extern s32 D_801E94FC;
extern s32 D_801E9500;
extern s32 D_801E9508;
extern s32 D_801E950C;
extern s32 D_801E9514[];

s32 func_801E77A8(s32 curve, s32 n, s32 t, SVECTOR *from, SVECTOR *to, SVECTOR *out);
s32 func_801E78E4(s32 curve, s32 n, s32 t, s32 from, s32 to);
void func_801E8A88(SVECTOR *pos, s32 ot, s32 tpage, s32 clut, s32 abr);

#endif
