#ifndef FX_0E5_H
#define FX_0E5_H

/* Effect module fx_0e5 (dir 13 file 0x0E5). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8;
    SVECTOR pos10;
    SVECTOR pos18;
    SVECTOR rot20;
    SVECTOR unk28;
    SVECTOR pos30;
    SVECTOR unk38;
    u16 unk40;
    u16 unk42;
} Fx0e5Work;

extern Unk800F78A4 *fx_0e5_env;

#endif
