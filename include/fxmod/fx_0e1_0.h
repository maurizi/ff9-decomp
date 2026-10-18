#ifndef FX_0E1_0_H
#define FX_0E1_0_H

/* Effect module fx_0e1_0 (dir 13 file 0x0E1). */
#include "btlfx_mod.h"

/* the effect environment block as these callbacks read it */
typedef struct {
    u8 pad0[0x60];
    s32 unk60; /* summon model */
} Fx0e10EnvB;

typedef struct {
    SVECTOR unk0; /* pad: point valid */
    SVECTOR unk8;
} Fx0e10Seg; /* 0x10: one step of the ribbon history */

extern s32 D_801E9D84[];
extern s32 D_801E9DB8;
extern s32 D_801E9DBC;
extern Unk800F78A4 *D_801E9E38;

void func_801E7CB4(SVECTOR *a, SVECTOR *b, s32 n, s32 tpage, s32 clut, s32 u, s32 v, s32 w, s32 h, s32 p,
                   Fx0e10Seg *buf, s32 abr, s32 phase);
void func_801E80A8(s32 ot, s32 x, s32 y, s32 nx, s32 ny, s32 dx, s32 dy, s32 abr, s32 sy, s32 tpage, s32 clut, s32 u,
                   s32 v, s32 w, s32 h, s32 c, s32 amp, s32 ph, s32 dphx, s32 dphy);
s32 func_801E8844(SVECTOR *out);
void func_801E88F0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s16 *v);

#endif
