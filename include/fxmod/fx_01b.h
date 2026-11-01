#ifndef FX_01B_H
#define FX_01B_H

/* Effect module fx_01b (dir 13 file 0x1B: Float, from runtime traces of btlfx_loadFile). */
#include "btlfx_mod.h"

typedef struct {
    s32 hold0; /* nonzero: the op idles and never ends by itself (runtime); always 0 in the game (see Fx046Work.hold0) */
    SVECTOR pos4; /* target position */
} Fx01bWork;

extern Unk800F78A4 *fx_01b_env;

#endif
