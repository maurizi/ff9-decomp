#ifndef FX_061_H
#define FX_061_H

/* Effect module fx_061 (dir 13 file 0x61: Confuse, from runtime traces of btlfx_loadFile). */
#include "btlfx_mod.h"

typedef struct {
    s32 hold0; /* nonzero: the op idles and never ends by itself (runtime); always 0 in the game (see Fx046Work.hold0) */
    SVECTOR pos4;
} Fx061Work;

extern Unk800F78A4 *fx_061_env;
extern s32 fx_061_trails2[];
extern s32 fx_061_trails[];

#endif
