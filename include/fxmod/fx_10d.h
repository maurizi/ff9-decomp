#ifndef FX_10D_H
#define FX_10D_H

/* Effect module fx_10d (dir 13 file 0x10D). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR unk0; /* caster */
    SVECTOR unk8; /* target */
    SVECTOR unk10;
    SVECTOR unk18; /* model rotation */
    VECTOR unk20; /* model position */
    VECTOR unk30; /* model scale */
    SVECTOR unk40[3]; /* rotation trail */
    VECTOR unk58[3]; /* position trail */
    u16 model88; /* models */
    u16 model8A;
} Fx10dWork80;

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR unk4;
} Fx10dWork81;

extern Unk800F78A4 *fx_10d_env;
extern BtlModel fx_10d_effModels[];

#endif
