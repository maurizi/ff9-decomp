#ifndef FX_00B_H
#define FX_00B_H

/* Effect module fx_00b (dir 13 file 0x0B). */
#include "btlfx_mod.h"

typedef struct {
    s32 stop0; /* dead early-out flag, always 0 (see Fx046Work.stop0) */
    SVECTOR pos4; /* target body position */
    s32 unkC; /* unused (runtime watch, round 33) */
} Fx00bWork;

extern Unk800F78A4 *fx_00b_env;

#endif
