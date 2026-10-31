#ifndef FX_030_H
#define FX_030_H

/* Effect module fx_030 (dir 13 file 0x30). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR pos0;
    SVECTOR pos8;
    SVECTOR aim10; /* angles from pos0 to pos8 */
    SVECTOR aim18; /* angles from pos8 to pos0 */
    Pool6CEntry *emit20;
    Pool6CEntry *emit24;
    Pool6CEntry *emit28;
    SVECTOR pos2C[8]; /* target body positions */
} Fx030Work; /* 0x6C */

extern Unk800F78A4 *fx_030_env;

#endif
