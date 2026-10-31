#ifndef FX_0B9_H
#define FX_0B9_H

/* Effect module fx_0b9 (dir 13 file 0xB9). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* position: env->targets->centerX..Z */
    SVECTOR unk10; /* btlfx_getUnitArgBonePos(0x10, 0) */
    SVECTOR pos18; /* btlfx_getUnitArgBonePos(0x10, 0), every frame */
    SVECTOR unk20; /* angles from unk10 to unk8 */
} Fx0b9Work80;

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4; /* unit position, raised by 0xC8 */
    SVECTOR unkC; /* angles from D_801E7B9C to unk4 */
    s32 unk14; /* unit */
} Fx0b9Work81;

extern Unk800F78A4 *fx_0b9_env;
extern SVECTOR D_801E7B9C;
extern s32 D_801E7BA4[];
extern s32 D_801E87D4[];

#endif
