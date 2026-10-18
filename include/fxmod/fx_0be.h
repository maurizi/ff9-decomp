#ifndef FX_0BE_H
#define FX_0BE_H

/* Effect module fx_0be (dir 13 file 0xBE). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    SVECTOR unk8; /* caster body position */
} Fx0beWork80;

typedef struct {
    SVECTOR unk0; /* unit 0x10 position */
} Fx0beWork81;

extern Unk800F78A4 *D_801E793C;

#endif
