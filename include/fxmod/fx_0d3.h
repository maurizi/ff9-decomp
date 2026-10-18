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

void func_801E8A10(s32 a0, s32 frame, s32 a2, s32 a3, s16 *v);
s32 func_801E9050(SVECTOR *v);
void func_801E90D4(s16 *v);
typedef struct {
    s16 unk0;
    s16 unk2; /* count */
    s16 unk4[1]; /* keys: 1, 3, 6 or 9 halfwords each */
} Fx0d3Track;
void func_801E8470(Fx0d3Track *track, s32 t, SVECTOR *out);
void func_801E84C0(Fx0d3Track *track, s32 t, SVECTOR *a, SVECTOR *b);
void func_801E8548(Fx0d3Track *track, s32 t, SVECTOR *a, SVECTOR *b, SVECTOR *c);
s32 func_801E85FC(Fx0d3Track *track, s32 t);

#endif
