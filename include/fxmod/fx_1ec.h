#ifndef FX_1EC_H
#define FX_1EC_H

/* Effect module fx_1ec (dir 13 file 0x1ec). */
#include "btlfx_mod.h"

typedef struct {
    s16 x, y, z;
} Fx1ecPoint;

/* copied */
typedef struct {
    s16 unk0;
    u16 unk2; /* count */
    Fx1ecPoint *unk4;
} Fx1ecPath;

extern Unk800F78A4 *D_801E8E94;
extern Fx1ecPath D_801E9034[0x12];
s32 func_801E8C60(Fx1ecPath *path, s32 t, SVECTOR *out);
extern SVECTOR D_801EFB64;
extern Fx1ecPath D_801E90C4[0x1E];
extern SVECTOR D_801EFB6C;
void func_801E8CC4(s32 unit, s32 motion);
/* end copied */

#endif
