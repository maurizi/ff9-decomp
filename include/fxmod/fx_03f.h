#ifndef FX_03F_H
#define FX_03F_H

/* Effect module fx_03f (dir 13 file 0x3F: Shell, from runtime traces of btlfx_loadFile). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    Pool6CEntry *unkC;
    Pool6CEntry *unk10;
    Pool6CEntry *unk14;
    SVECTOR unk18; /* above the target */
    SVECTOR unk20; /* target body position */
    SVECTOR unk28; /* below the target */
} Fx03fWork; /* 0x30 */

extern Unk800F78A4 *D_801E7A58;

#endif
