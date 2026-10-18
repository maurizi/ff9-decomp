#ifndef FX_1BF_2_H
#define FX_1BF_2_H

/* Effect module fx_1bf_2 (dir 13 file 0x1bf). */
#include "btlfx_mod.h"

typedef struct {
    s16 unk0;
    s16 unk2; /* scale */
    s16 unk4; /* position */
    s16 unk6;
    s16 unk8;
    s16 padA;
    SVECTOR unkC; /* rotation */
} Fx1bf2Obj;

typedef struct {
    u8 pad0[0x78];
    s32 unk78; /* model */
} Fx1bf2Inst;

/* copied */
extern s32 D_801E8FCC;
/* end copied */

#endif
