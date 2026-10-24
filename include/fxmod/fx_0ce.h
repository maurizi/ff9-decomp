#ifndef FX_0CE_H
#define FX_0CE_H

/* Effect module fx_0ce (dir 13 file 0xCE). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame counter */
    SVECTOR unk8; /* caster body position, raised by 0x80 */
    SVECTOR unk10; /* caster position */
    SVECTOR unk18; /* btlfx_getUnitArgBonePos(0x10, 0) */
    s32 unk20; /* model */
    SVECTOR unk24; /* angles from unk18 to unk8 */
    Pool6CEntry *unk2C;
} Fx0ceWork;

extern Unk800F78A4 *D_801E7D20;
extern BtlModel D_801E7D30[];
extern s32 D_801E7E00[];

#endif
