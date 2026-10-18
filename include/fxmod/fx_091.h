#ifndef FX_091_H
#define FX_091_H

/* Effect module fx_091 (dir 13 file 0x91). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* frame for btlfx_drawBackdrop */
    Pool6CEntry *unk4;
    SVECTOR unk8; /* func_800DC858(0x10, 0) */
    SVECTOR unk10; /* caster body position */
    SVECTOR unk18; /* angles from unk8 to unk10 */
} Fx091Work;

extern Unk800F78A4 *D_801E7908;

#endif
