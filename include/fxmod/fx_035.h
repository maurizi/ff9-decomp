#ifndef FX_035_H
#define FX_035_H

/* Effect module fx_035 (dir 13 file 0x35). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    SVECTOR posC; /* target body position */
    SVECTOR pos14;
    SVECTOR pos1C; /* posC on the ground */
} Fx035Work;

extern Unk800F78A4 *fx_035_env;

#endif
