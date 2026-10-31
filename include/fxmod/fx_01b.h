#ifndef FX_01B_H
#define FX_01B_H

/* Effect module fx_01b (dir 13 file 0x1B: Float, from runtime traces of btlfx_loadFile). */
#include "btlfx_mod.h"

typedef struct {
    s32 stop0; /* dead early-out flag, always 0 (see Fx046Work.stop0) */
    SVECTOR pos4; /* target position */
} Fx01bWork;

extern Unk800F78A4 *fx_01b_env;

#endif
