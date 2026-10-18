#ifndef FX_109_H
#define FX_109_H

/* Effect module fx_109 (dir 13 file 0x109). */
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
} Fx109Work80;

typedef struct {
    Pool6CEntry *unk0;
    SVECTOR unk4;
} Fx109Work81;

extern Unk800F78A4 *D_801E7BD4;
extern BtlModel D_801E7BE4[];

#endif
