#ifndef FX_0C1_H
#define FX_0C1_H

/* Effect module fx_0c1 (dir 13 file 0xC1). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    Pool6CEntry *unkC;
    SVECTOR unk10; /* unit 0x10 position */
    SVECTOR unk18; /* unit 0x10 rotation */
    SVECTOR unk20; /* caster body position */
    SVECTOR unk28; /* unit 0x10 bone 0x17 position */
    SVECTOR unk30; /* unk10 moved 0x12C along unk18 */
    SVECTOR unk38; /* func_800DC818(0x10, 0x17) */
    SVECTOR unk40; /* copy of unk48 */
    SVECTOR unk48; /* angles from unk38 to unk20 */
} Fx0c1Work;

extern Unk800F78A4 *D_801E7C18;
extern SVECTOR D_801E7C20; /* particle path start */
extern SVECTOR D_801E7C28; /* particle path end */
extern SVECTOR D_801E7C30;

s32 func_801E7704(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);

#endif
