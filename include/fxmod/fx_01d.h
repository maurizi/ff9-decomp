#ifndef FX_01D_H
#define FX_01D_H

/* Effect module fx_01d (dir 13 file 0x1D). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    Pool6CEntry *unkC;
    Pool6CEntry *unk10;
    SVECTOR unk14; /* caster position */
    SVECTOR unk1C; /* caster rotation */
    SVECTOR unk24; /* bone 7 point */
    SVECTOR unk2C; /* bone 15 point */
    SVECTOR unk34; /* in front of the caster */
    SVECTOR unk3C; /* angles from unk24 to unk34 */
    SVECTOR unk44; /* angles from unk2C to unk34 */
    s32 unk4C; /* screen tint */
} Fx01dWork80; /* 0x50 */

typedef struct {
    Pool6CEntry *unk0;
    SVECTOR unk4; /* target position */
    s32 unkC; /* target index */
} Fx01dWork81;

extern Unk800F78A4 *D_801E7D34;

void func_801E7CD8(Unk800F78A4 *env, s32 i, s32 v);

#endif
