#ifndef FX_00F_H
#define FX_00F_H

/* Effect module fx_00f (dir 13 file 0x0F). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0;
    SVECTOR pos4; /* target body position */
} Fx00fWork;

extern Unk800F78A4 *fx_00f_env;
extern s32 fx_00f_trails[];

#endif
