#ifndef FX_01A_H
#define FX_01A_H

/* Effect module fx_01a (dir 13 file 0x1A). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    SVECTOR unkC; /* target position */
} Fx01aWork;

extern Unk800F78A4 *D_801E7910;
extern s32 D_801E7920[];
extern s32 D_801E7D80[];

#endif
