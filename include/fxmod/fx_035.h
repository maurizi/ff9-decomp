#ifndef FX_035_H
#define FX_035_H

/* Effect module fx_035 (dir 13 file 0x35). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    SVECTOR unkC; /* target body position */
    SVECTOR unk14;
    SVECTOR unk1C; /* unkC on the ground */
} Fx035Work;

extern Unk800F78A4 *D_801E78FC;

#endif
