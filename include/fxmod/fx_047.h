#ifndef FX_047_H
#define FX_047_H

/* Effect module fx_047 (dir 13 file 0x47). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0;
    SVECTOR unk4; /* above unkC */
    SVECTOR unkC; /* target on the ground */
    Pool6CEntry *unk14;
} Fx047Work;

extern Unk800F78A4 *D_801E796C;
extern s32 D_801E797C[];

#endif
