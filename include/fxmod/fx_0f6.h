#ifndef FX_0F6_H
#define FX_0F6_H

/* Effect module fx_0f6 (dir 13 file 0x0F6). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    SVECTOR unk8;
    SVECTOR unk10;
} Fx0f6Work;

typedef struct {
    u16 unk0; /* file */
    u16 unk2; /* done */
    u16 unk4; /* first frame */
    u16 unk6; /* end frame */
    u16 unk8; /* frame */
    u16 unkA; /* fade */
} Fx0f6Layer;

extern Fx0f6Layer D_801E7984[4];
extern Unk800F78A4 *D_801E79FC;

#endif
