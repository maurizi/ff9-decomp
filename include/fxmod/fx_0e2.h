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

typedef struct {
    DVECTOR unk0; /* screen xy of one edge end */
    DVECTOR unk4; /* screen xy of the other */
    s32 unk8; /* ot z */
} Fx0e2Edge;

typedef struct {
    SVECTOR unk0; /* unit position */
    SVECTOR unk8; /* unit rotation */
    s16 unk10; /* distance from the centre */
    s16 unk12; /* angle around it */
} Fx0e2Orbit;

extern Unk800F78A4 *D_801ECC0C;
extern Fx0e2Orbit D_801EE8FC[];
extern Fx0e2Edge D_801EE62C[60];
extern SVECTOR D_801F0FEC; /* trail origin */
extern SVECTOR D_801F0FF4; /* trail head position */
extern SVECTOR D_801F0FFC; /* trail head rotation */
extern SVECTOR D_801F1004; /* trail aim target */

void func_801E77C0(void *trackA, void *trackB, s32 t, Fx0e2Trail *trail, s32 size, s32 pad);
void func_801E7A0C(void *track, Fx0e2Trail *trail, s32 size, s32 base, s32 pad);
void func_801E7BE8(void *track, Fx0e2Trail *trail, s32 size, s32 base, s32 count, s32 pad);
void func_801E7D5C(void *trackA, void *trackB, Fx0e2Trail *trail, s32 size, s32 base, s32 count, s32 pad, s32 t,
                   s32 rotY);
void func_801E7FBC(void *trackA, void *trackB, s32 arg2, s32 arg3, SVECTOR *scale, s32 w, s32 t, s32 rotY, s32 twist);
void func_801E83B0(void *track, s32 arg1, s32 arg2, SVECTOR *scale, s32 w, s32 rotY);
void func_801E86D0(s32 n, s32 c, s32 tpage, s32 clut, u16 v0, u16 v1);
s32 fx_0e2_op83(s32 mode, SVECTOR *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);
s32 func_801EC4A4(void *track, s32 i, SVECTOR *pos);
s32 func_801EC520(void *track, s32 i, SVECTOR *rot, SVECTOR *pos);

s32 func_801EC5B4(s32 r, s32 g, s32 t, s32 n, s32 arg4, s32 arg5);

#endif
