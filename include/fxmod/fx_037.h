#ifndef FX_037_H
#define FX_037_H

/* Effect module fx_037 (dir 13 file 0x37). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    SVECTOR pos14; /* target body position */
} Fx037Work;

extern Unk800F78A4 *fx_037_env;
extern s32 fx_037_trails[];
extern s32 fx_037_trails2[];

#endif
