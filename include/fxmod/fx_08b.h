#ifndef FX_08B_H
#define FX_08B_H

/* Effect module fx_08b (dir 13 file 0x8B). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    Pool6CEntry *unkC;
    s32 unk10;
    Pool6CEntry *unk14;
    SVECTOR unk18; /* unit 0x10 position */
    SVECTOR unk20; /* unit 0x10 rotation */
    SVECTOR unk28; /* unit 0x10 bone 5 position */
    SVECTOR unk30; /* unit 0x10 bone 0x22 position */
    SVECTOR unk38; /* unk18 moved 0x708 along unk20 */
    SVECTOR unk40; /* angles from unk28 to unk50 */
    SVECTOR unk48; /* angles from unk30 to unk50 */
    SVECTOR unk50; /* unk18 moved 0x7D0 along unk20 */
    s32 unk58; /* screen tint */
} Fx08bWork80;

typedef struct {
    Pool6CEntry *unk0;
    SVECTOR unk4; /* unit position */
    s32 unkC; /* unit */
} Fx08bWork81;

extern Unk800F78A4 *D_801E7D74;

void func_801E7D18(Unk800F78A4 *env, s32 i, s32 v);

#endif
