#ifndef FX_18D_H
#define FX_18D_H

/* Effect module fx_18d (dir 13 file 0x18d). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* steps left */
    s32 unk4; /* steps */
    SVECTOR unk8;
    SVECTOR unk10; /* from */
    SVECTOR unk18; /* to */
    SVECTOR unk20; /* current */
    SVECTOR unk28; /* angles */
} Fx18dWork;

extern Unk800F78A4 *D_801E7A38;

#endif
