#ifndef FX_008_H
#define FX_008_H

/* Effect module fx_008 (dir 13 file 0x08). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    SVECTOR unk4; /* target position */
    SVECTOR unkC; /* target body position */
    u16 unk14;
    u16 unk16;
} Fx008Work;

extern Unk800F78A4 *D_801E78EC;
extern s32 D_801E78FC[];

#endif
