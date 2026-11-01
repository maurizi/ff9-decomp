#ifndef FX_00B_H
#define FX_00B_H

/* Effect module fx_00b (dir 13 file 0x0B). */
#include "btlfx_mod.h"

typedef struct {
    s32 hold0; /* nonzero: the op idles and never ends by itself (runtime); always 0 in the game (see Fx046Work.hold0) */
    SVECTOR pos4; /* target body position */
    s32 unkC; /* unused (runtime watch, round 33) */
} Fx00bWork;

extern Unk800F78A4 *fx_00b_env;

#endif
