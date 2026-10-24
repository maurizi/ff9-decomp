#ifndef FX_0AF_H
#define FX_0AF_H

/* Effect module fx_0af (dir 13 file 0xAF). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    Pool6CEntry *unk4;
    SVECTOR unk8; /* btlfx_getUnitArgBonePos(0x10, 0) */
    SVECTOR unk10; /* caster body position */
    SVECTOR unk18; /* angles from unk8 to unk10 */
} Fx0afWork;

extern Unk800F78A4 *D_801E798C;
extern s32 D_801E799C[];
extern s32 D_801E828C[];

#endif
