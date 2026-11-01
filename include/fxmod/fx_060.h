#ifndef FX_060_H
#define FX_060_H

/* Effect module fx_060 (dir 13 file 0x60). */
#include "btlfx_mod.h"

typedef struct {
    s32 hold0; /* nonzero: the op idles and never ends by itself (runtime); always 0 in the game (see Fx046Work.hold0) */
    SVECTOR pos4; /* target body position */
} Fx060Work;

extern Unk800F78A4 *fx_060_env;
extern s32 fx_060_trails2[];
extern s32 fx_060_trails[];

#endif
