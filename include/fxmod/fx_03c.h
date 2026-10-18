#ifndef FX_03C_H
#define FX_03C_H

/* Effect module fx_03c (dir 13 file 0x3C). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    SVECTOR unk4; /* caster body position */
    SVECTOR unkC; /* target body position */
    SVECTOR unk14;
    u8 pad1C[8];
} Fx03cWork; /* 0x24 */

extern Unk800F78A4 *D_801E786C;
extern s32 D_801E787C[];

#endif
