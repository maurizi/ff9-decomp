#ifndef FX_08D_H
#define FX_08D_H

/* Effect module fx_08d (dir 13 file 0x8D). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    SVECTOR unkC; /* caster body position */
    SVECTOR unk14; /* caster position */
    u16 unk1C;
    u16 unk1E;
} Fx08dWork;

extern Unk800F78A4 *D_801E79A0;
extern s32 D_801E79B0[];

#endif
