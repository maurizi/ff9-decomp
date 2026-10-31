#ifndef FX_0E8_H
#define FX_0E8_H

/* Effect module fx_0e8 (dir 13 file 0x0E8). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    SVECTOR unk14;
    u8 pad1C[0x2C - 0x1C];
    VECTOR unk2C; /* unit scale */
    SVECTOR unk3C;
} Fx0e8Work;

extern Unk800F78A4 *fx_0e8_env;
extern s32 D_801E7A80[];

#endif
