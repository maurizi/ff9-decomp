#ifndef FX_092_H
#define FX_092_H

/* Effect module fx_092 (dir 13 file 0x92). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    Pool6CEntry *unkC;
    SVECTOR unk10; /* unit 0x10 body position */
    SVECTOR unk18; /* unit 0x10 rotation */
    SVECTOR unk20; /* caster position */
    SVECTOR unk28; /* func_800DC858(0x10, 0), moved 0xC8 along unk18 */
    SVECTOR unk30; /* unk20 raised by 0x1C2 */
    SVECTOR unk38; /* unk20 raised by 0xC8 */
    SVECTOR unk40; /* angles from env->camera->unk54 to unk28 */
} Fx092Work;

extern Unk800F78A4 *D_801E7A1C;

#endif
