#ifndef FX_047_H
#define FX_047_H

/* Effect module fx_047 (dir 13 file 0x47). */
#include "btlfx_mod.h"

typedef struct {
    s32 hold0; /* nonzero: the op idles and never ends by itself (runtime); always 0 in the game (see Fx046Work.hold0) */
    SVECTOR pos4; /* above posC */
    SVECTOR posC; /* target on the ground */
    Pool6CEntry *emit14;
} Fx047Work;

extern Unk800F78A4 *fx_047_env;
extern s32 fx_047_trails[];

#endif
