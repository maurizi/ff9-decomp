#ifndef FX_0A5_H
#define FX_0A5_H

/* Effect module fx_0a5 (dir 13 file 0xA5). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    SVECTOR unk8; /* btlfx_getUnitArgBonePos(0x10, 0) */
    SVECTOR unk10; /* angles from unk8 to env->targets->centerX */
    SVECTOR unk18; /* lerp start: env position at x = 0x5DC */
    SVECTOR unk20; /* lerp end: env position at x = -0x5DC */
    SVECTOR unk28; /* lerp output */
} Fx0a5Work;

extern Unk800F78A4 *D_801E7A2C;
extern s32 D_801E7A3C[];

#endif
