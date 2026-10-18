#ifndef FX_1FD_H
#define FX_1FD_H

/* Effect module fx_1fd (dir 13 file 0x1fd). */
#include "btlfx_mod.h"

typedef struct {
    s16 x, y, z;
} Fx1fdPoint;

typedef struct {
    s16 unk0;
    u16 unk2; /* count */
    Fx1fdPoint *unk4;
} Fx1fdPath;

/* copied */
extern Unk800F78A4 *D_801E9E4C;
/* end copied */

#endif
