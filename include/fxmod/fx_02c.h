#ifndef FX_02C_H
#define FX_02C_H

/* Effect module fx_02c (dir 13 file 0x2C). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* caster body position */
    SVECTOR pos10;
    SVECTOR aim18; /* angles from pos8 to pos10 */
} Fx02cWork;

extern Unk800F78A4 *fx_02c_env;

#endif
