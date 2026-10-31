#ifndef FX_09D_H
#define FX_09D_H

/* Effect module fx_09d (dir 13 file 0x9D): Blind. Evidence: the file btlfx_loadFile loads for the spell (docs/runtime-findings.md, btlfx_loadFile). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR pos0; /* position: env->targets->centerX, -0x190, unk3C */
} Fx09dWork;

extern Unk800F78A4 *fx_09d_env;

#endif
