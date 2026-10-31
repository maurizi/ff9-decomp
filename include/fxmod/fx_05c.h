#ifndef FX_05C_H
#define FX_05C_H

/* Effect module fx_05c (dir 13 file 0x5C). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    SVECTOR unkC; /* caster body position */
    SVECTOR unk14; /* target body position */
} Fx05cWork;

extern Unk800F78A4 *fx_05c_env;

#endif
