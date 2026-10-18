#ifndef FX_1A1_H
#define FX_1A1_H

/* Effect module fx_1a1 (dir 13 file 0x1a1). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* steps left */
    s32 unk4; /* steps */
    SVECTOR unk8;
    SVECTOR unk10; /* from */
    SVECTOR unk18; /* to */
    SVECTOR unk20; /* current */
    SVECTOR unk28; /* angles */
} Fx1a1Work;

extern Unk800F78A4 *D_801E7A38;

#endif
