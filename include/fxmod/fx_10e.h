#ifndef FX_10E_H
#define FX_10E_H

/* Effect module fx_10e (dir 13 file 0x10E). */
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
    u16 unk88; /* models */
    u16 unk8A;
} Fx10eWork80;

typedef struct {
    Pool6CEntry *unk0;
    SVECTOR unk4;
} Fx10eWork81;

extern Unk800F78A4 *D_801E7BAC;
extern BtlModel D_801E7BBC[];

#endif
