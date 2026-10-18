#ifndef FX_037_H
#define FX_037_H

/* Effect module fx_037 (dir 13 file 0x37). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    Pool6CEntry *unkC;
    Pool6CEntry *unk10;
    SVECTOR unk14; /* target body position */
} Fx037Work;

extern Unk800F78A4 *D_801E79B4;
extern s32 D_801E79C4[];
extern s32 D_801E81A4[];

#endif
