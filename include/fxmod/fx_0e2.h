#ifndef FX_0E2_H
#define FX_0E2_H

/* Effect module fx_0e2 (dir 13 file 0x0E2). */
#include "btlfx_mod.h"

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    SVECTOR unk4[1]; /* (size - 4) / 8 points */
} Fx0e2Trail;

extern Unk800F78A4 *D_801ECC0C;
extern SVECTOR D_801F0FEC; /* trail origin */
extern SVECTOR D_801F0FF4; /* trail head position */
extern SVECTOR D_801F0FFC; /* trail head rotation */
extern SVECTOR D_801F1004; /* trail aim target */

void func_801E77C0(void *trackA, void *trackB, s32 t, Fx0e2Trail *trail, s32 size, s32 pad);
void func_801E7A0C(void *track, Fx0e2Trail *trail, s32 size, s32 base, s32 pad);
void func_801E7BE8(void *track, Fx0e2Trail *trail, s32 size, s32 base, s32 count, s32 pad);
void func_801E7D5C(void *trackA, void *trackB, Fx0e2Trail *trail, s32 size, s32 base, s32 count, s32 pad, s32 t,
                   s32 rotY);
s32 func_801EC4A4(void *track, s32 i, SVECTOR *pos);
s32 func_801EC520(void *track, s32 i, SVECTOR *rot, SVECTOR *pos);

s32 func_801EC5B4(s32 r, s32 g, s32 t, s32 n, s32 arg4, s32 arg5);

#endif
