#ifndef FX_04C_H
#define FX_04C_H

/* Effect module fx_04c (dir 13 file 0x4C). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0;
    SVECTOR unk4;
} Fx04cWork80;

typedef struct {
    s32 unk0; /* state */
    SVECTOR unk4[4]; /* target positions */
    Pool6CEntry *unk24[4];
    s32 unk34; /* target count */
    s32 unk38; /* current target */
} Fx04cWork81; /* 0x3C */

extern Unk800F78A4 *D_801E7CB4;

#endif
