#ifndef FX_0E6_H
#define FX_0E6_H

/* Effect module fx_0e6 (dir 13 file 0x0E6). */
#include "btlfx_mod.h"

typedef struct {
    s32 model0; /* model */
    VECTOR scale4; /* scale */
    VECTOR pos14; /* position */
    SVECTOR rot24; /* rotation */
    SVECTOR pos2C;
    SVECTOR pos34;
} Fx0e6Work;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    SVECTOR unk4[25];
} Fx0e6Line;

extern Unk800F78A4 *fx_0e6_env;
extern BtlModel fx_0e6_effModels[];
extern Fx0e6Line D_801E7B68;

#endif
