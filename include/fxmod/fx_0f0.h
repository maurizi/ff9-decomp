#ifndef FX_0F0_H
#define FX_0F0_H

/* Effect module fx_0f0 (dir 13 file 0x0F0). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    SVECTOR pos14;
    SVECTOR pos1C;
    SVECTOR pos24;
} Fx0f0Work80;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    SVECTOR posC;
} Fx0f0Work81;

extern Unk800F78A4 *fx_0f0_env;
extern s32 D_801E7C30[];
extern s32 D_801E8D40[];
extern s32 D_801E9E50[];

#endif
