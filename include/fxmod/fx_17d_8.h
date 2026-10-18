#ifndef FX_17D_8_H
#define FX_17D_8_H

/* Effect module fx_17d_8 (dir 13 file 0x17d). */
#include "btlfx_mod.h"

typedef struct {
    s16 unk0;
    s16 unk2; /* scale */
    s16 unk4; /* position */
    s16 unk6;
    s16 unk8;
    s16 padA;
    SVECTOR unkC; /* rotation */
} Fx17d8Obj;

typedef struct {
    u8 pad0[0x78];
    s32 unk78; /* model */
} Fx17d8Inst;

extern s32 D_801E8FCC;

#endif
