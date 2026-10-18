#ifndef FX_0A8_H
#define FX_0A8_H

/* Effect module fx_0a8 (dir 13 file 0xA8). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    SVECTOR unk8; /* unit 0x10 body position */
    SVECTOR unk10; /* unit 0x10 rotation */
} Fx0a8Work80;

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    Pool6CEntry *unkC;
    Pool6CEntry *unk10;
    s32 unk14;
    Pool6CEntry *unk18;
    s32 unk1C;
    SVECTOR unk20; /* position: env->unkC->unk38..3C */
} Fx0a8Work81;

extern Unk800F78A4 *D_801E7B68;
extern s32 D_801E7B78[];

#endif
