#ifndef FX_00D_H
#define FX_00D_H

/* Effect module fx_00d (dir 13 file 0x0D). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    SVECTOR unkC; /* target body position */
} Fx00dWork;

extern Unk800F78A4 *D_801E7908;
extern s32 D_801E7918[];

#endif
