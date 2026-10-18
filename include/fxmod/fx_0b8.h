#ifndef FX_0B8_H
#define FX_0B8_H

/* Effect module fx_0b8 (dir 13 file 0xB8). */
#include "btlfx_mod.h"

typedef struct {
    u8 u;
    u8 v;
} Fx0b8UV;

extern SVECTOR D_801EA7B0[32];

void func_801EA1E0(s32 ot, s32 y, s32 h, s32 x, s32 c0, s32 c1, s32 tpage, s32 clut, s32 abr);
void func_801EA424(s32 ot);
void func_801EA4E0(s32 a, s32 b, s32 t, s32 n, s32 mode, s32 ot);
s32 func_801EA558(Pool30Entry *pt, s32 a1, SVECTOR *pos, s32 a3, s32 frame, s32 n);
extern Unk800F78A4 *D_801EA7A0;
extern SVECTOR D_801EA680[]; /* colour table */
void func_801E9D80(SVECTOR *pos, SVECTOR *rot, s32 n, s32 tpage, s32 clut, s32 abr);
s32 func_801E7704(s32 curve, s32 n, s32 t, s32 from, s32 to);

#endif
