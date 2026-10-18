#ifndef FX_105_H
#define FX_105_H

/* Effect module fx_105 (dir 13 file 0x105). */
#include "btlfx_mod.h"

extern Unk800F78A4 *D_801EAB18;
typedef struct {
    s16 vx, vy, vz, pad, unk8, unkA;
} Fx105Spark; /* 0xC */

extern Fx105Spark D_801EC168[];

void func_801E7D9C(s32 z, s32 tpage, s32 y0, s32 y1, CVECTOR *rgb);
void func_801E9E1C(void);

#endif
