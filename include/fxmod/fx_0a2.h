#ifndef FX_0A2_H
#define FX_0A2_H

/* Effect module fx_0a2 (dir 13 file 0xA2). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* position: env->targets->centerX..Z */
} Fx0a2Work;

extern Unk800F78A4 *fx_0a2_env;
extern s32 D_801E7894[];

#endif
