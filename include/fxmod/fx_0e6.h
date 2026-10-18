#ifndef FX_0E6_H
#define FX_0E6_H

/* Effect module fx_0e6 (dir 13 file 0x0E6). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* model */
    VECTOR unk4; /* scale */
    VECTOR unk14; /* position */
    SVECTOR unk24; /* rotation */
    SVECTOR unk2C;
    SVECTOR unk34;
} Fx0e6Work;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    SVECTOR unk4[25];
} Fx0e6Line;

extern Unk800F78A4 *D_801E7A88;
extern BtlModel D_801E7A98[];
extern Fx0e6Line D_801E7B68;

#endif
