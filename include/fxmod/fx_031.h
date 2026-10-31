#ifndef FX_031_H
#define FX_031_H

/* Effect module fx_031 (dir 13 file 0x31). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR unk0; /* target body position */
    SVECTOR unk8;
    SVECTOR unk10;
    SVECTOR unk18;
    SVECTOR unk20;
    SVECTOR unk28;
    Pool6CEntry *emit30;
    Pool6CEntry *emit34;
} Fx031Work; /* 0x38 */

extern Unk800F78A4 *fx_031_env;

#endif
