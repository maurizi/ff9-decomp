#ifndef FX_10F_H
#define FX_10F_H

/* Effect module fx_10f (dir 13 file 0x10F). */
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
} Fx10fWork80;

typedef struct {
    Pool6CEntry *unk0;
    SVECTOR unk4;
} Fx10fWork81;

extern Unk800F78A4 *D_801E7BAC;
extern BtlModel D_801E7BBC[];

#endif
