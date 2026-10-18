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

typedef struct {
    s16 vx, vy, vz;
    s16 unk6; /* width */
} Fx07aPt;

/* lightning strip passed to btlfx_drawTrail */
typedef struct {
    u8 unk0; /* count - 1 */
    u8 unk1;
    u8 unk2;
    u8 pad3;
    Fx07aPt unk4[11];
    s32 unk5C; /* end offset */
    s32 unk60;
    s32 unk64;
    s32 pad68;
    s32 unk6C[20]; /* jitter */
    s32 unkBC[20]; /* jitter speed */
    s32 unk10C; /* timer */
} Fx07aStrip; /* 0x110 */

typedef struct {
    s32 unk0; /* model */
    SVECTOR unk4; /* rotation */
    VECTOR unkC; /* scale */
} Fx07aModel; /* 0x1C */

typedef struct {
    s32 unk0[16]; /* timers */
    Fx07aModel unk40[3];
    VECTOR unk94; /* position */
    SVECTOR unkA4[8]; /* unit home positions */
    s32 unkE4[8]; /* unit delays */
    s32 pad104;
} Fx07aWork; /* 0x108 */

extern BtlModel D_801E9934[];
extern SVECTOR D_801E9B8C;
extern SVECTOR D_801E9B94;
extern Fx07aStrip D_801E9BA4[5];
extern s32 D_801E9504;
extern u32 D_801E9534[];

#endif
