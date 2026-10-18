#ifndef FX_032_H
#define FX_032_H

/* Effect module fx_032 (dir 13 file 0x32). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR unk0; /* target body position */
    SVECTOR unk8;
    SVECTOR unk10;
    SVECTOR unk18;
    SVECTOR unk20;
    SVECTOR unk28;
    SVECTOR unk30;
    SVECTOR unk38;
    SVECTOR unk40;
    Pool6CEntry *unk48;
    Pool6CEntry *unk4C;
    Pool6CEntry *unk50;
    Pool6CEntry *unk54;
} Fx032Work; /* 0x58 */

extern Unk800F78A4 *D_801E7C60;

#endif
