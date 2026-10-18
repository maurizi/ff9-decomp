#ifndef FX_08A_H
#define FX_08A_H

/* Effect module fx_08a (dir 13 file 0x8A). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    SVECTOR unk4; /* caster body position */
} Fx08aWork;

extern Unk800F78A4 *D_801E7938;
extern s32 D_801E7948[];

#endif
