#ifndef FX_10A_H
#define FX_10A_H

/* Effect module fx_10a (dir 13 file 0x10A). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR unk0; /* caster */
    SVECTOR unk8; /* target */
    SVECTOR unk10;
    SVECTOR rot18; /* model rotation */
    VECTOR pos20; /* model position */
    VECTOR scale30; /* model scale */
    SVECTOR unk40[3]; /* rotation trail */
    VECTOR unk58[3]; /* position trail */
    u16 model88; /* models */
    u16 model8A;
} Fx10aWork80;

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4;
} Fx10aWork81;

extern Unk800F78A4 *fx_10a_env;
extern BtlModel fx_10a_effModels[];

#endif
