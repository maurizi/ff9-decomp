#ifndef FX_125_H
#define FX_125_H

/* Effect module fx_125 (dir 13 file 0x125). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    SVECTOR pos4; /* target position */
    SVECTOR posC;
    SVECTOR pos14; /* target body position */
    SVECTOR unk1C;
    s32 model24; /* models */
    s32 model28[2];
} Fx125Work;

extern Unk800F78A4 *fx_125_env;
extern BtlModel fx_125_effModels[];

#endif
