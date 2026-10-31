#ifndef FX_110_H
#define FX_110_H

/* Effect module fx_110 (dir 13 file 0x110). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR pos0; /* caster */
    SVECTOR pos8; /* target */
    SVECTOR pos10;
    SVECTOR rot18; /* model rotation */
    VECTOR pos20; /* model position */
    VECTOR scale30; /* model scale */
    SVECTOR unk40[4]; /* rotation trail */
    VECTOR unk60[4]; /* position trail */
    u16 modelA0; /* models */
    u16 modelA2;
} Fx110Work80;

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4;
} Fx110Work81;

extern Unk800F78A4 *fx_110_env;
extern BtlModel fx_110_effModels[];

#endif
