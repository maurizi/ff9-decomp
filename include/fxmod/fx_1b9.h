#ifndef FX_1B9_H
#define FX_1B9_H

/* Effect module fx_1b9 (dir 13 file 0x1b9). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    SVECTOR unk10;
    SVECTOR pos18;
    SVECTOR pos20;
} Fx1b9Work;

extern Unk800F78A4 *fx_1b9_env;

#endif
