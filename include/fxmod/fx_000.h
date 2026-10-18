#ifndef FX_000_H
#define FX_000_H

/* Effect module fx_000 (dir 13 file 0x00). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0;
    Pool6CEntry *unk4;
    SVECTOR unk8; /* target body position */
    SVECTOR unk10; /* target position */
} Fx000Work;

extern Unk800F78A4 *D_801E78EC;

#endif
