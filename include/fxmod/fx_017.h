#ifndef FX_017_H
#define FX_017_H

/* Effect module fx_017 (dir 13 file 0x17). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    SVECTOR unkC; /* target body position */
    SVECTOR unk14;
    SVECTOR unk1C; /* unkC on the ground */
} Fx017Work;

extern Unk800F78A4 *D_801E78CC;

#endif
