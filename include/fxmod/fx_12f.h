#ifndef FX_12F_H
#define FX_12F_H

/* Effect module fx_12f (dir 13 file 0x12F). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR unk0;
    SVECTOR unk8;
    SVECTOR unk10;
    SVECTOR unk18;
    SVECTOR unk20[4];
    Pool6CEntry *unk40;
} Fx12fWork;

extern Unk800F78A4 *D_801E7A54;

#endif
