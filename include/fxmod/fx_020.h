#ifndef FX_020_H
#define FX_020_H

/* Effect module fx_020 (dir 13 file 0x20). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR unk8; /* target position */
} Fx020Work;

extern SVECTOR D_801E7894;
extern Unk800F78A4 *fx_020_env;
extern s32 D_801E78F4[];

#endif
