#ifndef FX_043_H
#define FX_043_H

/* Effect module fx_043 (dir 13 file 0x43). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    u8 pad4[8];
    SVECTOR unkC;
    SVECTOR unk14; /* above the target body */
} Fx043Work80; /* 0x1C */

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    SVECTOR unk8; /* target body position */
    SVECTOR unk10; /* target position */
    SVECTOR unk18;
    SVECTOR unk20; /* angles from unk8 to D_801E7BD0 */
} Fx043Work81; /* 0x28 */

extern Unk800F78A4 *D_801E7BC8;
extern SVECTOR D_801E7BD0; /* particle start */
extern SVECTOR D_801E7BD8; /* particle end */
extern SVECTOR D_801E7BE0;
extern s32 D_801E7BE8[];
extern s32 D_801E7D48[];

s32 func_801E7704(VECTOR *out, s32 arg1, SVECTOR *pos, s32 arg3, s32 t, s32 n);

#endif
