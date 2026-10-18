#ifndef FX_17F_H
#define FX_17F_H

/* Effect module fx_17f (dir 13 file 0x17f). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0[3]; /* target scale */
    SVECTOR unkC; /* target position */
    SVECTOR unk14; /* target rotation */
    BtlUnit9C8 *unk1C;
    SVECTOR unk20; /* caster position */
} Fx17fWork;

extern Unk800F78A4 *D_801E7C50;
extern s32 D_801E7C60[0x2BC];
extern s32 D_801E88E0[];

void func_801E7704(SVECTOR *v, s32 i);

#endif
