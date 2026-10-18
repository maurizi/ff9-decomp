#ifndef FX_0C0_H
#define FX_0C0_H

/* Effect module fx_0c0 (dir 13 file 0xC0). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    Pool6CEntry *unkC;
    SVECTOR unk10; /* unit 0x10 body position */
    SVECTOR unk18; /* caster body position */
} Fx0c0Work;

extern Unk800F78A4 *D_801E7974;

#endif
