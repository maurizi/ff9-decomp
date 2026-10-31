#ifndef FX_02F_H
#define FX_02F_H

/* Effect module fx_02f (dir 13 file 0x2F). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR aim0; /* beam angles */
    SVECTOR pos8;
    SVECTOR pos10;
    SVECTOR pos18; /* target position, on the ground */
    Pool6CEntry *emit20;
} Fx02fWork; /* 0x24 */

extern Unk800F78A4 *fx_02f_env;
extern SVECTOR fx_02f_targetPos[]; /* target body positions */
extern s32 fx_02f_trails[];

#endif
