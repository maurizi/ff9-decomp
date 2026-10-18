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

extern BtlModel D_801EC15C[];
extern u32 D_801EDADC[];
extern SVECTOR D_801EDEDC[8]; /* unit positions */
extern VECTOR D_801EDF1C[8]; /* unit scales */

void func_801E8AB8(void);
void func_801E8B04(s32 arg0, s32 arg1, s32 arg2, s32 data);
void func_801E8B60(s32 arg0);

/* copied */
extern Unk800F78A4 *D_801EC14C;
/* end copied */

#endif
