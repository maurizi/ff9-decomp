#ifndef FX_01F_H
#define FX_01F_H

/* Effect module fx_01f (dir 13 file 0x1F). */
#include "btlfx_mod.h"

typedef struct {
    s32 stop0; /* dead early-out flag, always 0 (see Fx046Work.stop0) */
    SVECTOR pos4;
} Fx01fWork;

extern Unk800F78A4 *fx_01f_env;

#endif
