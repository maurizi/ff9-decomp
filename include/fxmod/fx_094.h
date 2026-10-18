#ifndef FX_094_H
#define FX_094_H

/* Effect module fx_094 (dir 13 file 0x94). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR unk0; /* func_800DC858(0x10, 0) */
    SVECTOR unk8; /* caster body position */
    SVECTOR unk10; /* caster position */
    SVECTOR unk18; /* angles from unk0 to unk8 */
    Pool6CEntry *unk20;
} Fx094Work;

extern Unk800F78A4 *D_801E79AC;

#endif
