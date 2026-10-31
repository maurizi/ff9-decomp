#ifndef FX_060_H
#define FX_060_H

/* Effect module fx_060 (dir 13 file 0x60). */
#include "btlfx_mod.h"

typedef struct {
    s32 stop0; /* dead early-out flag, always 0 (see Fx046Work.stop0) */
    SVECTOR pos4; /* target body position */
} Fx060Work;

extern Unk800F78A4 *fx_060_env;
extern s32 fx_060_trails2[];
extern s32 fx_060_trails[];

#endif
