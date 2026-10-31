#ifndef FX_0C0_H
#define FX_0C0_H

/* Effect module fx_0c0 (dir 13 file 0xC0). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    SVECTOR pos10; /* unit 0x10 body position */
    SVECTOR pos18; /* caster body position */
} Fx0c0Work;

extern Unk800F78A4 *fx_0c0_env;

#endif
