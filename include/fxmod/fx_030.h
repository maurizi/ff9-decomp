#ifndef FX_030_H
#define FX_030_H

/* Effect module fx_030 (dir 13 file 0x30). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR unk0;
    SVECTOR unk8;
    SVECTOR unk10; /* angles from unk0 to unk8 */
    SVECTOR unk18; /* angles from unk8 to unk0 */
    Pool6CEntry *unk20;
    Pool6CEntry *unk24;
    Pool6CEntry *unk28;
    SVECTOR unk2C[8]; /* target body positions */
} Fx030Work; /* 0x6C */

extern Unk800F78A4 *D_801E7A80;

#endif
