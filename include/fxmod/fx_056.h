#ifndef FX_056_H
#define FX_056_H

/* Effect module fx_056 (dir 13 file 0x56). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    SVECTOR unk8; /* target position */
    s32 unk10; /* target index */
} Fx056Work;

extern Unk800F78A4 *D_801E79A8;
extern s32 D_801E79B8[][0x168]; /* per target */

void func_801E794C(Unk800F78A4 *env, s32 i, s32 v);

#endif
