#ifndef FX_102_H
#define FX_102_H

/* Effect module fx_102 (dir 13 file 0x102). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4[8];
    Pool6CEntry *unk24;
    SVECTOR unk28;
    SVECTOR unk30[8];
} Fx102Work;

extern u16 D_801E7924[8];
extern Unk800F78A4 *D_801E797C;

#endif
