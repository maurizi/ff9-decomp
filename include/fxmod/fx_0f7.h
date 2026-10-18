#ifndef FX_0F7_H
#define FX_0F7_H

/* Effect module fx_0f7 (dir 13 file 0x0F7). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* countdown */
    Pool6CEntry *unk4;
    s32 unk8;
    Pool6CEntry *unkC;
    Pool6CEntry *unk10;
    Pool6CEntry *unk14;
} Fx0f7Work;

typedef struct {
    s32 unk0; /* delay */
    s32 unk4; /* kind */
    s32 unk8;
    s32 unkC; /* alpha */
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C; /* age */
} Fx0f7Spark;

extern Fx0f7Spark D_801E7AE0[4];
extern Unk800F78A4 *D_801E7BA8;

#endif
