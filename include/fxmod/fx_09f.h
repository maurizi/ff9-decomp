#ifndef FX_09F_H
#define FX_09F_H

/* Effect module fx_09f (dir 13 file 0x9F). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    SVECTOR pos4; /* caster position */
    SVECTOR posC; /* caster body position */
} Fx09fWork;

extern Unk800F78A4 *fx_09f_env;

#endif
