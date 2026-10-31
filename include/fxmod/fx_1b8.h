#ifndef FX_1B8_H
#define FX_1B8_H

/* Effect module fx_1b8 (dir 13 file 0x1b8). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4;
} Fx1b8Work80;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    SVECTOR pos10;
    SVECTOR pos18;
    SVECTOR pos20;
} Fx1b8Work81;

extern Unk800F78A4 *fx_1b8_env;

#endif
