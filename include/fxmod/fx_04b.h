#ifndef FX_04B_H
#define FX_04B_H

/* Effect module fx_04b (dir 13 file 0x4B). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    SVECTOR unk8; /* target position */
    SVECTOR unk10;
    SVECTOR unk18;
    SVECTOR unk20; /* angles from unk10 to unk8 */
} Fx04bWork80; /* 0x28 */

typedef struct {
    Pool6CEntry *unk0;
    SVECTOR unk4; /* above the target */
    SVECTOR unkC; /* angles from D_801E7B9C to unk4 */
    s32 unk14; /* target index */
} Fx04bWork81; /* 0x18 */

extern Unk800F78A4 *D_801E7B94;
extern SVECTOR D_801E7B9C;
extern s32 D_801E7BA4[];
extern s32 D_801E87D4[];

#endif
