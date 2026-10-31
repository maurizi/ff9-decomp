#ifndef FX_1A5_H
#define FX_1A5_H

/* Effect module fx_1a5 (dir 13 file 0x1a5). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR unk8;
    s32 unk10; /* unit */
} Fx1a5Work;

extern Unk800F78A4 *fx_1a5_env;

static void fxm_setSignal(Unk800F78A4 *env, s32 i, s32 v);

#endif
