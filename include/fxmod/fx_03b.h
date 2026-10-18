#ifndef FX_03B_H
#define FX_03B_H

/* Effect module fx_03b (dir 13 file 0x3B). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    Pool6CEntry *unkC;
    SVECTOR unk10; /* target position */
    SVECTOR unk18; /* above the target */
    SVECTOR unk20; /* target body position */
    SVECTOR unk28;
} Fx03bWork;

extern Unk800F78A4 *D_801E7970;
extern s32 D_801E7980[];
extern s32 D_801E7E80[];

#endif
