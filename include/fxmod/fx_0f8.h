#ifndef FX_0F8_H
#define FX_0F8_H

/* Effect module fx_0f8 (dir 13 file 0x0F8). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    SVECTOR unk4;
    SVECTOR unkC;
    SVECTOR unk14;
} Fx0f8Work80;

typedef struct {
    Pool6CEntry *unk0[4];
    Pool6CEntry *unk10[4];
    Pool6CEntry *unk20;
    Pool6CEntry *unk24;
    SVECTOR unk28;
    SVECTOR unk30[4];
} Fx0f8Work81;

typedef struct {
    u16 unk0; /* file */
    u16 unk2; /* frame */
} Fx0f8Spawn;

extern Fx0f8Spawn D_801E7B78[4];
extern Unk800F78A4 *D_801E7BD0;

#endif
