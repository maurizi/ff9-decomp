#ifndef FX_002_H
#define FX_002_H

/* Effect module fx_002 (dir 13 file 0x02). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0;
    SVECTOR unk4; /* target position */
    SVECTOR unkC; /* target body position */
} Fx002Work;

extern Unk800F78A4 *D_801E7AF0;
extern s32 D_801E7B00[];

#endif
