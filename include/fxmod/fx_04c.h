#ifndef FX_04C_H
#define FX_04C_H

/* Effect module fx_04c (dir 13 file 0x4C). */
#include "btlfx_mod.h"

typedef struct {
    s32 stop0; /* dead early-out flag, always 0 (see Fx046Work.stop0) */
    SVECTOR pos4;
} Fx04cWork80;

typedef struct {
    s32 state0; /* state */
    SVECTOR pos4[4]; /* target positions */
    Pool6CEntry *emit24[4];
    s32 count34; /* target count */
    s32 target38; /* current target */
} Fx04cWork81; /* 0x3C */

extern Unk800F78A4 *fx_04c_env;

#endif
