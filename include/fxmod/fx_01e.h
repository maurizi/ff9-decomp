#ifndef FX_01E_H
#define FX_01E_H

/* Effect module fx_01e (dir 13 file 0x1E). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0;
    Pool6CEntry *unk4;
    SVECTOR unk8; /* caster body position */
    SVECTOR unk10; /* target body position */
    SVECTOR unk18; /* angles from caster to target */
} Fx01eWork;

extern Unk800F78A4 *D_801E791C;
extern s32 D_801E792C[];

#endif
