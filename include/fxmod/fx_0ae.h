#ifndef FX_0AE_H
#define FX_0AE_H

/* Effect module fx_0ae (dir 13 file 0xAE). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    Pool6CEntry *unk4;
    SVECTOR unk8; /* unit 0x10 bone position */
    SVECTOR unk10; /* caster body position */
    SVECTOR unk18; /* caster position */
    SVECTOR unk20; /* angles from unk8 to unk10 */
} Fx0aeWork;

extern Unk800F78A4 *D_801E79BC;
extern s32 D_801E79CC[];

#endif
