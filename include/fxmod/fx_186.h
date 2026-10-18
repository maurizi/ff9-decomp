#ifndef FX_186_H
#define FX_186_H

/* Effect module fx_186 (dir 13 file 0x186). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    s16 pad4;
    s16 unk6; /* height */
    s16 unk8; /* velocity */
    s16 unkA;
    SVECTOR unkC; /* position */
    SVECTOR unk14; /* rotation */
    s16 unk1C; /* spin */
    s16 unk1E;
    s32 unk20; /* model */
} Fx186Rock;

#endif
