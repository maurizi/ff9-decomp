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

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx1a8Vec3;

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame */
    SVECTOR unk8;
    SVECTOR unk10;
    u8 pad18[0x20 - 0x18];
    SVECTOR unk20; /* ring position */
    s32 unk28; /* model */
    SVECTOR unk2C; /* rotation */
    Fx1a8Vec3 unk34; /* scale */
    Fx1a8Vec3 unk40; /* position */
    s32 unk4C[4]; /* models */
    s32 unk5C;
    s32 unk60;
    s32 unk64;
    s32 unk68;
    u8 pad6C[0x74 - 0x6C];
} Fx1a8Work80; /* 0x74 */

extern SVECTOR D_801E937C;
extern u32 D_801E93CC[];
extern BtlModel D_801E97DC[];
extern u8 D_801EA13C[];

#endif
