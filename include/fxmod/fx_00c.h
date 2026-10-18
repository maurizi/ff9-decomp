#ifndef FX_00C_H
#define FX_00C_H

/* Effect module fx_00c (dir 13 file 0x0C). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    SVECTOR unk8; /* target position */
    SVECTOR unk10; /* target body position */
} Fx00cWork;

extern Unk800F78A4 *D_801E787C;

#endif
