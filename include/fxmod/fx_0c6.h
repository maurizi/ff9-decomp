#ifndef FX_0C6_H
#define FX_0C6_H

/* Effect module fx_0c6 (dir 13 file 0xC6). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    SVECTOR unk8; /* unit position */
    s32 unk10; /* unit */
} Fx0c6Work;

extern Unk800F78A4 *D_801E7950;

void func_801E78F4(Unk800F78A4 *env, s32 index, s32 value);

#endif
