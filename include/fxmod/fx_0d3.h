#ifndef FX_0D3_H
#define FX_0D3_H

/* Effect module fx_0d3 (dir 13 file 0xD3). */
#include "btlfx_mod.h"

extern Unk800F78A4 *D_801ED534;
extern s32 D_801EB084[5];
extern s32 D_801EB098;
extern u16 D_801EB0C4[3]; /* file ids for op87 tracks, end frame */
extern s32 D_801EDF44;
extern s32 D_801EDF48;

void func_801E8470(u8 *data, s32 frame, SVECTOR *out);
void func_801E8A10(s32 a0, s32 frame, s32 a2, s32 a3, s16 *v);
s32 func_801E9050(SVECTOR *v);
void func_801E90D4(s16 *v);

#endif
