#ifndef FX_036_H
#define FX_036_H

/* Effect module fx_036 (dir 13 file 0x36). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    Pool6CEntry *unkC;
    Pool6CEntry *unk10;
    Pool6CEntry *unk14;
    Pool6CEntry *unk18;
    SVECTOR unk1C;
    SVECTOR unk24;
} Fx036Work; /* 0x2C */

extern s16 D_801E7BCC[];
extern s32 D_801E7BDC; /* particles spawned */
extern Unk800F78A4 *D_801E7C28;
extern SVECTOR D_801E7C30; /* target position */

s32 func_801E7704(SVECTOR *out, SVECTOR *arg1, s32 arg2, s32 arg3);

#endif
