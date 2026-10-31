#ifndef FX_103_H
#define FX_103_H

/* Effect module fx_103 (dir 13 file 0x103). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR unk0;
    SVECTOR unk8;
    SVECTOR pos10;
    SVECTOR pos18;
    SVECTOR unk20;
    SVECTOR unk28;
    SVECTOR pos30;
    Pool6CEntry *emit38;
    Pool6CEntry *emit3C;
} Fx103Work;

extern Unk800F78A4 *fx_103_env;

#endif
