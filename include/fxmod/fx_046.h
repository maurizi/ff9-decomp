#ifndef FX_046_H
#define FX_046_H

/* Effect module fx_046 (dir 13 file 0x46). */
#include "btlfx_mod.h"

typedef struct {
    s32 stop0; /* dead early-out flag: zeroed in mode 1 and tested == 0 every frame, nothing else writes it
                * (runtime watch, round 33; the same in 21 other modules). The role is a guess (medium),
                * "always 0" is certain. */
    SVECTOR pos4;
} Fx046Work;

extern Unk800F78A4 *fx_046_env;

#endif
