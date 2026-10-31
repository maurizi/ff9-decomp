#ifndef FX_04E_H
#define FX_04E_H

/* Effect module fx_04e (dir 13 file 0x4E: Sleep, from runtime traces of btlfx_loadFile). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    SVECTOR unkC; /* position: env->targets->centerX..Z */
} Fx04eWork;

extern Unk800F78A4 *fx_04e_env;
extern s32 D_801E7940[];
extern s32 D_801E7DA0[];

#endif
