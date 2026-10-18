#ifndef FX_1FC_H
#define FX_1FC_H

/* Effect module fx_1fc (dir 13 file 0x1fc). */
#include "btlfx_mod.h"

typedef struct {
    s16 x, y, z;
} Fx1fcKey6;

typedef struct {
    s16 unk0;
    u16 unk2; /* count */
    Fx1fcKey6 *unk4;
} Fx1fcTrack6;

typedef struct {
    s16 x, y, z;
} Fx1fcPoint;

typedef struct {
    s16 unk0;
    u16 unk2; /* count */
    Fx1fcPoint *unk4;
} Fx1fcPath;

/* copied */
extern Unk800F78A4 *D_801EC14C;
/* end copied */

#endif
