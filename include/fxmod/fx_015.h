#ifndef FX_015_H
#define FX_015_H

/* Effect module fx_015 (dir 13 file 0x15). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    SVECTOR unk8; /* above the target */
    SVECTOR unk10; /* target position */
} Fx015Work;

extern Unk800F78A4 *D_801E78E0;
extern s32 D_801E78F0[];

#endif
