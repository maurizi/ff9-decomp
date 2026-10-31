#ifndef FX_01A_H
#define FX_01A_H

/* Effect module fx_01a (dir 13 file 0x1A). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    SVECTOR posC; /* target position */
} Fx01aWork;

extern Unk800F78A4 *fx_01a_env;
extern s32 D_801E7920[];
extern s32 D_801E7D80[];

#endif
