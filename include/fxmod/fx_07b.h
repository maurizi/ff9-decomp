#ifndef FX_07B_H
#define FX_07B_H

/* Effect module fx_07b (dir 13 file 0x7B: Protect, from runtime traces of btlfx_loadFile). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* target body position, moved back */
    SVECTOR rot10; /* target rotation */
} Fx07bWork;

extern SVECTOR fx_07b_emitRot;
extern Unk800F78A4 *fx_07b_env;
extern s32 fx_07b_trails[];

#endif
