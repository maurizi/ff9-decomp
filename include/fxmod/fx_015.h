#ifndef FX_015_H
#define FX_015_H

/* Effect module fx_015 (dir 13 file 0x15). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* above the target */
    SVECTOR pos10; /* target position */
} Fx015Work;

extern Unk800F78A4 *fx_015_env;
extern s32 D_801E78F0[];

#endif
