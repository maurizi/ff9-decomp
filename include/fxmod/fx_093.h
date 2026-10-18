#ifndef FX_093_H
#define FX_093_H

/* Effect module fx_093 (dir 13 file 0x93). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR unk0; /* func_800DC858(0x10, 0) */
    SVECTOR unk8; /* unit 0x10 bone position */
    SVECTOR unk10; /* caster body position */
    SVECTOR unk18; /* caster position */
    SVECTOR unk20; /* angles from unk0 to unk8 */
    Pool6CEntry *unk28;
} Fx093Work;

extern Unk800F78A4 *D_801E79F4;

#endif
