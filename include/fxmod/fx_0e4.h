#ifndef FX_0E4_H
#define FX_0E4_H

/* Effect module fx_0e4 (dir 13 file 0x0E4). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    SVECTOR unk8; /* model rotation */
    VECTOR unk10; /* model position */
    VECTOR unk20; /* model scale */
    s32 unk30; /* model brightness */
    SVECTOR unk34; /* target position */
    SVECTOR unk3C; /* target rotation */
    SVECTOR unk44;
    SVECTOR unk4C;
    SVECTOR unk54;
    s16 unk5C;
    s16 unk5E;
    s16 unk60; /* drop countdown */
    s16 unk62; /* fade countdown */
    s32 unk64; /* state */
    u16 unk68[3]; /* models */
} Fx0e4Work;

extern u16 D_801E7CF8[3];
extern Unk800F78A4 *D_801E7D48;
extern BtlModel D_801E7D58[];

void func_801E7C5C(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out);
s32 func_801E7CA0(s32 from, s32 to, s32 t, s32 n);

#endif
