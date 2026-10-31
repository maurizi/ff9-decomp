#ifndef FX_0BC_H
#define FX_0BC_H

/* Effect module fx_0bc (dir 13 file 0xBC). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR unk4; /* caster body position */
} Fx0bcWork80;

typedef struct {
    SVECTOR unk0; /* unit 0x10 position */
} Fx0bcWork81;

extern Unk800F78A4 *fx_0bc_env;

#endif
