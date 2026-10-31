#ifndef FX_0C6_H
#define FX_0C6_H

/* Effect module fx_0c6 (dir 13 file 0xC6). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR unk8; /* unit position */
    s32 unk10; /* unit */
} Fx0c6Work;

extern Unk800F78A4 *fx_0c6_env;

static void fxm_setSignal(Unk800F78A4 *env, s32 i, s32 v);

#endif
