#ifndef FX_18C_H
#define FX_18C_H

/* Effect module fx_18c (dir 13 file 0x18c). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR unk0; /* home position */
    SVECTOR unk8; /* unk8.vy: height */
} Fx18cWork;

extern Unk800F78A4 *D_801E7B10;
extern s32 D_801E7B20[3]; /* countdowns */

s32 func_801E7704(s32 curve, s32 n, s32 t, s32 from, s32 to);

#endif
