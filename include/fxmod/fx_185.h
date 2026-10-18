#ifndef FX_185_H
#define FX_185_H

/* Effect module fx_185 (dir 13 file 0x185). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR unk0; /* from */
    SVECTOR unk8; /* to */
    SVECTOR unk10; /* model rotation */
    VECTOR unk18; /* model position */
    VECTOR unk28; /* model scale */
    s32 unk38; /* index */
    u16 unk3C; /* model */
} Fx185Work80;

typedef struct {
    Pool6CEntry *unk0;
    SVECTOR unk4;
} Fx185Work81;

extern Unk800F78A4 *D_801E7AD4;
extern BtlModel D_801E7AE4[];
extern SVECTOR D_801E8764;

#endif
