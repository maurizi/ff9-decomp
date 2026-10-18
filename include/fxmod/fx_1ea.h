#ifndef FX_1EA_H
#define FX_1EA_H

/* Effect module fx_1ea (dir 13 file 0x1ea). */
#include "btlfx_mod.h"

typedef struct {
    s16 x, y, z;
} Fx1eaPoint;

typedef struct {
    s16 unk0;
    u16 unk2; /* count */
    Fx1eaPoint *unk4;
} Fx1eaPath;

/* copied */
extern Unk800F78A4 *D_801E8DD8;
/* end copied */

#endif
