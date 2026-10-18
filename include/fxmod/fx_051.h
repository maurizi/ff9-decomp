#ifndef FX_051_H
#define FX_051_H

/* Effect module fx_051 (dir 13 file 0x51). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    SVECTOR unk8; /* target body position */
    SVECTOR unk10; /* caster rotation */
    s32 unk18; /* target index */
} Fx051Work;

extern SVECTOR D_801E799C;
extern Unk800F78A4 *D_801E79EC;

void func_801E7988(Unk800F78A4 *env, s32 i, s32 v);

#endif
