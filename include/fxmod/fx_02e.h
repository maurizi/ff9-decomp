#ifndef FX_02E_H
#define FX_02E_H

/* Effect module fx_02e (dir 13 file 0x2E). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    Pool6CEntry *unkC;
    SVECTOR unk10; /* caster body position */
    SVECTOR unk18; /* target body position */
    SVECTOR unk20; /* angles to unk18 */
} Fx02eWork; /* 0x28 */

extern Unk800F78A4 *D_801E7A64;

#endif
