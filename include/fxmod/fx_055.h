#ifndef FX_055_H
#define FX_055_H

/* Effect module fx_055 (dir 13 file 0x55). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* target position */
} Fx055Work;

extern Unk800F78A4 *fx_055_env;
extern s32 D_801E78CC[];

#endif
