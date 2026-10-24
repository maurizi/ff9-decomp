#ifndef FX_0A1_H
#define FX_0A1_H

/* Effect module fx_0a1 (dir 13 file 0xA1). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    s32 unk4;
    SVECTOR unk8; /* caster body position */
    SVECTOR unk10; /* caster position */
    SVECTOR unk18; /* btlfx_getUnitArgBonePos(0x10, 0) */
    SVECTOR unk20; /* btlfx_getUnitArgBonePos(0x10, 0), every frame */
    SVECTOR unk28; /* angles from unk8 to unk18 */
} Fx0a1Work;

extern Unk800F78A4 *D_801E7C60;
extern u8 D_801E7C70[];
extern s32 D_801E7CA0[];
extern s32 D_801E8A00[];

#endif
