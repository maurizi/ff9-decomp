#ifndef FX_000_H
#define FX_000_H

/* Effect module fx_000 (dir 13 file 0x00). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* target body position */
    SVECTOR pos10; /* target position */
} Fx000Work;

extern Unk800F78A4 *fx_000_env;

#endif
