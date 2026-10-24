#ifndef FX_0CA_H
#define FX_0CA_H

/* Effect module fx_0ca (dir 13 file 0xCA). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    SVECTOR unk8; /* unk10 raised by 0x190 */
    SVECTOR unk10; /* position: env->targets->centerX..Z */
} Fx0caWork;

extern Unk800F78A4 *D_801E78B8;
extern s32 D_801E78C8[];

#endif
