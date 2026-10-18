#ifndef FX_0F0_H
#define FX_0F0_H

/* Effect module fx_0f0 (dir 13 file 0x0F0). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    Pool6CEntry *unkC;
    Pool6CEntry *unk10;
    SVECTOR unk14;
    SVECTOR unk1C;
    SVECTOR unk24;
} Fx0f0Work80;

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    SVECTOR unkC;
} Fx0f0Work81;

extern Unk800F78A4 *D_801E7C20;
extern s32 D_801E7C30[];
extern s32 D_801E8D40[];
extern s32 D_801E9E50[];

#endif
