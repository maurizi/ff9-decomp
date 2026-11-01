#ifndef FX_051_H
#define FX_051_H

/* Effect module fx_051 (dir 13 file 0x51). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* target body position */
    SVECTOR rot10; /* target rotation */
    s32 target18; /* target index */
} Fx051Work;

extern SVECTOR fx_051_emitRot;
extern Unk800F78A4 *fx_051_env;

static void fxm_setSignal(Unk800F78A4 *env, s32 signal, s32 value);

#endif
