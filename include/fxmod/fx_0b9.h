#ifndef FX_0B9_H
#define FX_0B9_H

/* Effect module fx_0b9 (dir 13 file 0xB9). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    SVECTOR unk8; /* position: env->unkC->unk38..3C */
    SVECTOR unk10; /* func_800DC858(0x10, 0) */
    SVECTOR unk18; /* func_800DC858(0x10, 0), every frame */
    SVECTOR unk20; /* angles from unk10 to unk8 */
} Fx0b9Work80;

typedef struct {
    Pool6CEntry *unk0;
    SVECTOR unk4; /* unit position, raised by 0xC8 */
    SVECTOR unkC; /* angles from D_801E7B9C to unk4 */
    s32 unk14; /* unit */
} Fx0b9Work81;

extern Unk800F78A4 *D_801E7B94;
extern SVECTOR D_801E7B9C;
extern s32 D_801E7BA4[];
extern s32 D_801E87D4[];

#endif
