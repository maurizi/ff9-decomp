#ifndef FX_07C_H
#define FX_07C_H

/* Effect module fx_07c (dir 13 file 0x7C). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    SVECTOR unk4; /* position: env->targets->centerX..Z */
} Fx07cWork;

extern Unk800F78A4 *D_801E7818;

#endif
