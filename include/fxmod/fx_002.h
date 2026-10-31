#ifndef FX_002_H
#define FX_002_H

/* Effect module fx_002 (dir 13 file 0x02). */
#include "btlfx_mod.h"

typedef struct {
    s32 stop0; /* dead early-out flag, always 0 (see Fx046Work.stop0) */
    SVECTOR pos4; /* target position */
    SVECTOR posC; /* target body position */
} Fx002Work;

extern Unk800F78A4 *fx_002_env;
extern s32 fx_002_trails[];

#endif
