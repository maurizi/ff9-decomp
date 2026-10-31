#ifndef FX_03B_H
#define FX_03B_H

/* Effect module fx_03b (dir 13 file 0x3B). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    SVECTOR pos10; /* target position */
    SVECTOR pos18; /* above the target */
    SVECTOR pos20; /* target body position */
    SVECTOR pos28;
} Fx03bWork;

extern Unk800F78A4 *fx_03b_env;
extern s32 D_801E7980[];
extern s32 D_801E7E80[];

#endif
