#ifndef FX_1F5_H
#define FX_1F5_H

/* Effect module fx_1f5 (dir 13 file 0x1f5). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    SVECTOR unk4;
} Fx1f5Work80;

typedef struct {
    Pool6CEntry *unk0;
    SVECTOR unk4;
    s32 unkC; /* unit */
} Fx1f5Work82;

extern Unk800F78A4 *D_801E79E8;
extern s32 D_801E79F8[];

#endif
