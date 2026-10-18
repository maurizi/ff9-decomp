#ifndef FX_024_H
#define FX_024_H

/* Effect module fx_024 (dir 13 file 0x024). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    Pool6CEntry *unkC;
    Pool6CEntry *unk10;
    Pool6CEntry *unk14;
    SVECTOR unk18; /* caster position */
    SVECTOR unk20; /* caster rotation */
    SVECTOR unk28; /* bone 7 point */
    SVECTOR unk30; /* bone 15 point */
    SVECTOR unk38; /* in front of the caster */
    SVECTOR unk40; /* angles from unk24 to unk34 */
    SVECTOR unk48; /* angles from unk2C to unk34 */
    s32 unk50; /* screen tint */
} Fx024Work80; /* 0x54 */

typedef struct {
    Pool6CEntry *unk0;
    SVECTOR unk4; /* target position */
    s32 unkC; /* target index */
} Fx024Work81;

extern Unk800F78A4 *D_801E7D60;

void func_801E7D04(Unk800F78A4 *env, s32 i, s32 v);

#endif
