#ifndef FX_104_H
#define FX_104_H

/* Effect module fx_104 (dir 13 file 0x104). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    s32 unk4;
    SVECTOR unk8;
    SVECTOR unk10;
} Fx104Work80;

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    SVECTOR unkC;
} Fx104Work81;

extern Unk800F78A4 *D_801E79F4;
extern s32 D_801E7A04[];

#endif
