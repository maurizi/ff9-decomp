#ifndef FX_0F3_H
#define FX_0F3_H

/* Effect module fx_0f3 (dir 13 file 0x0F3). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* variant (0..2) */
    SVECTOR unk8; /* target body position */
    SVECTOR unk10;
    SVECTOR unk18;
    s32 unk20;
    Pool6CEntry *unk24;
} Fx0f3Work;

extern Unk800F78A4 *D_801E7CA0;
extern u8 D_801E7CB0[][0xEC];
extern u8 D_801E7F80[][0x2B0];

#endif
