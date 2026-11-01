#ifndef FX_01F_H
#define FX_01F_H

/* Effect module fx_01f (dir 13 file 0x1F). */
#include "btlfx_mod.h"

typedef struct {
    s32 hold0; /* nonzero: the op idles and never ends by itself (runtime); always 0 in the game (see Fx046Work.hold0) */
    SVECTOR pos4;
} Fx01fWork;

extern Unk800F78A4 *fx_01f_env;

#endif
