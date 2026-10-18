#ifndef FX_12A_H
#define FX_12A_H

/* Effect module fx_12a (dir 13 file 0x12A). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    Pool6CEntry *unkC;
    Pool6CEntry *unk10;
    Pool6CEntry *unk14;
    Pool6CEntry *unk18;
    Pool6CEntry *unk1C;
    SVECTOR unk20; /* bone positions */
    SVECTOR unk28;
    SVECTOR unk30;
    SVECTOR unk38;
    SVECTOR unk40; /* aim targets */
    SVECTOR unk48;
    SVECTOR unk50; /* aim angles */
    SVECTOR unk58;
} Fx12aWork80;

typedef struct {
    Pool6CEntry *unk0;
    SVECTOR unk4;
} Fx12aWork81;

typedef struct {
    Pool6CEntry *unk0;
    s32 unk4;
    SVECTOR unk8;
    s32 unk10; /* target index */
} Fx12aWork82;

extern Unk800F78A4 *D_801E7EFC;

void func_801E7EA0(Unk800F78A4 *env, s32 i, s32 value);

#endif
