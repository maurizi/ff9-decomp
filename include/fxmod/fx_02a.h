#ifndef FX_02A_H
#define FX_02A_H

/* Effect module fx_02a (dir 13 file 0x2A). */
#include "btlfx_mod.h"

typedef struct {
    s32 stop0; /* dead early-out flag, always 0 (see Fx046Work.stop0) */
    SVECTOR pos4; /* target body position */
    s32 unkC; /* unused (runtime watch, round 33) */
} Fx02aWork;

extern Unk800F78A4 *fx_02a_env;

#endif
