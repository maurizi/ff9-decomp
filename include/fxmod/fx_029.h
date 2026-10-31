#ifndef FX_029_H
#define FX_029_H

/* Effect module fx_029 (dir 13 file 0x29). */
#include "btlfx_mod.h"

typedef struct {
    s32 stop0; /* dead early-out flag, always 0 (see Fx046Work.stop0) */
    Pool6CEntry *emit4;
    SVECTOR pos8;
} Fx029Work;

extern Unk800F78A4 *fx_029_env;

#endif
