#ifndef FX_110_H
#define FX_110_H

/* Effect module fx_110 (dir 13 file 0x110). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR unk0; /* caster */
    SVECTOR unk8; /* target */
    SVECTOR unk10;
    SVECTOR unk18; /* model rotation */
    VECTOR unk20; /* model position */
    VECTOR unk30; /* model scale */
    SVECTOR unk40[4]; /* rotation trail */
    VECTOR unk60[4]; /* position trail */
    u16 unkA0; /* models */
    u16 unkA2;
} Fx110Work80;

typedef struct {
    Pool6CEntry *unk0;
    SVECTOR unk4;
} Fx110Work81;

extern Unk800F78A4 *D_801E7BB0;
extern BtlModel D_801E7BC0[];

#endif
