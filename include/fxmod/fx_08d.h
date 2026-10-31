#ifndef FX_08D_H
#define FX_08D_H

/* Effect module fx_08d (dir 13 file 0x8D). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    SVECTOR posC; /* caster body position */
    SVECTOR pos14; /* caster position */
    u16 unk1C;
    u16 unk1E;
} Fx08dWork;

extern Unk800F78A4 *fx_08d_env;
extern s32 D_801E79B0[];

#endif
