#ifndef FX_046_H
#define FX_046_H

/* Effect module fx_046 (dir 13 file 0x46). */
#include "btlfx_mod.h"

typedef struct {
    s32 hold0; /* nonzero: the op skips its frame logic and never ends itself, so the task stays alive until the
                * effect script ends (runtime: Fire with it poked to 1 ran the op 165 times instead of 63). Zeroed
                * in mode 1 and nothing else writes it, so it is always 0 in the game (same in 21 other modules). */
    SVECTOR pos4;
} Fx046Work;

extern Unk800F78A4 *fx_046_env;

#endif
