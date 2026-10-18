#ifndef FX_0D1_H
#define FX_0D1_H

/* Effect module fx_0d1 (dir 13 file 0xD1). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    Pool6CEntry *unkC;
    Pool6CEntry *unk10;
    SVECTOR unk14; /* unit 0x10 body position */
    SVECTOR unk1C; /* caster body position */
} Fx0d1Work;

extern Unk800F78A4 *D_801E7990;

#endif
