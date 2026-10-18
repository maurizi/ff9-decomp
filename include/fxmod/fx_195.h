#ifndef FX_195_H
#define FX_195_H

/* Effect module fx_195 (dir 13 file 0x195). */
#include "btlfx_mod.h"

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    SVECTOR unk4[13];
} Fx195Trail; /* 0x6C */

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    SVECTOR unk4[57];
} Fx195Trail57;

extern Unk800F78A4 *D_801EA278;

#endif
