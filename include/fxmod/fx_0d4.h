#ifndef FX_0D4_H
#define FX_0D4_H

/* Effect module fx_0d4 (dir 13 file 0xD4). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    Pool6CEntry *unk4;
    SVECTOR unk8; /* caster body position */
    SVECTOR unk10; /* caster position */
} Fx0d4Work80;

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame counter */
    SVECTOR unk8; /* func_800E7D3C(0x10, 1) */
} Fx0d4Work84;

extern Unk800F78A4 *D_801E7C2C;

#endif
