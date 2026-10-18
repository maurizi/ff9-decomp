#ifndef FX_0BD_H
#define FX_0BD_H

/* Effect module fx_0bd (dir 13 file 0xBD). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    SVECTOR unk4; /* caster body position */
} Fx0bdWork80;

typedef struct {
    SVECTOR unk0; /* unit 0x10 position */
} Fx0bdWork81;

extern Unk800F78A4 *D_801E78DC;

#endif
