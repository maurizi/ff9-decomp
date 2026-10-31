#ifndef FX_09C_H
#define FX_09C_H

/* Effect module fx_09c (dir 13 file 0x9C). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR unk0; /* unk10 + random */
    SVECTOR unk8; /* unk10 + random */
    SVECTOR pos10; /* caster body position */
    SVECTOR pos18; /* caster body position */
    Pool6CEntry *emit20;
    u8 pad24[0x2C - 0x24];
    SVECTOR pos2C;
    SVECTOR unk34; /* angles */
    Pool6CEntry *emit3C;
} Fx09cWork;

extern Unk800F78A4 *fx_09c_env;

#endif
