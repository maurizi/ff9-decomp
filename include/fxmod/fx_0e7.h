#ifndef FX_0E7_H
#define FX_0E7_H

/* Effect module fx_0e7 (dir 13 file 0x0E7). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    SVECTOR unk4; /* target body position */
    SVECTOR unkC;
    u16 unk14;
    u16 unk16;
} Fx0e7Work80;

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    SVECTOR unkC; /* model rotations */
    SVECTOR unk14;
    VECTOR unk1C; /* model position */
    VECTOR unk2C; /* model scale */
    s32 unk3C; /* model brightness */
    SVECTOR unk40; /* target position */
    SVECTOR unk48; /* target rotation */
    SVECTOR unk50;
    SVECTOR unk58;
    SVECTOR unk60;
    s32 unk68;
    s32 unk6C; /* fade countdown */
    s32 unk70; /* drop countdown */
    s32 unk74; /* tilt countdown */
    u16 unk78; /* models */
    u16 unk7A;
} Fx0e7Work81;

extern s16 D_801E7EFC[4];
extern s16 D_801E7F04[4];
extern Unk800F78A4 *D_801E7F54;
extern BtlModel D_801E7F64[];

void func_801E7E60(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out);
s32 func_801E7EA4(s32 from, s32 to, s32 t, s32 n);

#endif
