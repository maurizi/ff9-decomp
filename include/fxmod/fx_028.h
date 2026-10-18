#ifndef FX_028_H
#define FX_028_H

/* Effect module fx_028 (dir 13 file 0x28). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    Pool6CEntry *unkC;
    Pool6CEntry *unk10;
    Pool6CEntry *unk14;
    SVECTOR unk18; /* target body position */
} Fx028Work;

extern Unk800F78A4 *D_801E7A1C;

#endif
