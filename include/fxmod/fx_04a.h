#ifndef FX_04A_H
#define FX_04A_H

/* Effect module fx_04a (dir 13 file 0x4A). */
#include "btlfx_mod.h"

typedef struct {
    s32 stop0; /* dead early-out flag, always 0 (see Fx046Work.stop0) */
    SVECTOR pos4; /* target body position */
} Fx04aWork;

extern Unk800F78A4 *fx_04a_env;

#endif
