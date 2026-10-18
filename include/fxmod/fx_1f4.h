#ifndef FX_1F4_H
#define FX_1F4_H

/* Effect module fx_1f4 (dir 13 file 0x1f4). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* fade-out countdown */
    s32 unk4; /* fade-in countdown */
    SVECTOR unk8;
} Fx1f4Work;

extern Unk800F78A4 *D_801E79B8;
extern s32 D_801E79C8[];
extern s32 D_801E8C18[];

#endif
