#ifndef FX_0A7_H
#define FX_0A7_H

/* Effect module fx_0a7 (dir 13 file 0xA7). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    s32 unk8;
    Pool6CEntry *unkC;
    Pool6CEntry *unk10;
    Pool6CEntry *unk14;
    SVECTOR unk18; /* unit 0x10 body position */
    SVECTOR unk20; /* caster body position */
    SVECTOR unk28; /* caster position */
    SVECTOR unk30; /* btlfx_getUnitArgBonePos(0x10, 0) */
    SVECTOR unk38; /* unk30 moved 0x12C along unk48 */
    SVECTOR unk40; /* unk30 moved 0xC8 along unk48 */
    SVECTOR unk48; /* angles from unk30 to unk20 */
    SVECTOR unk50; /* angles from unk20 to unk30 */
} Fx0a7Work;

extern Unk800F78A4 *D_801E7C78;
extern s32 D_801E7C88[];
extern s32 D_801E7F48[];
extern s32 D_801E8388[];

#endif
