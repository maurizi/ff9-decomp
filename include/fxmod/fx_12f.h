#ifndef FX_12F_H
#define FX_12F_H

/* Effect module fx_12f (dir 13 file 0x12F). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR unk0;
    SVECTOR pos8;
    SVECTOR unk10;
    SVECTOR pos18;
    SVECTOR unk20[4];
    Pool6CEntry *emit40;
} Fx12fWork;

extern Unk800F78A4 *fx_12f_env;

#endif
