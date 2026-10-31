#ifndef FX_00D_H
#define FX_00D_H

/* Effect module fx_00d (dir 13 file 0x0D). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    SVECTOR posC; /* target body position */
} Fx00dWork;

extern Unk800F78A4 *fx_00d_env;
extern s32 fx_00d_trails[];

#endif
