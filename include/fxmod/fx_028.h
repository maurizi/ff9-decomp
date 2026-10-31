#ifndef FX_028_H
#define FX_028_H

/* Effect module fx_028 (dir 13 file 0x28). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    Pool6CEntry *emit14;
    SVECTOR pos18; /* target body position */
} Fx028Work;

extern Unk800F78A4 *fx_028_env;

#endif
