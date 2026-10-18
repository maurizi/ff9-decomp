#ifndef FX_023_H
#define FX_023_H

/* Effect module fx_023 (dir 13 file 0x23). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    Pool6CEntry *unkC;
    Pool6CEntry *unk10;
    SVECTOR unk14; /* target position */
    SVECTOR unk1C; /* target home position */
    SVECTOR unk24;
    SVECTOR unk2C;
    s16 unk34; /* rise countdown */
    s16 unk36; /* fall countdown */
    s16 unk38; /* held up */
} Fx023Work; /* 0x3C */

extern Unk800F78A4 *D_801E7C30;
extern s32 D_801E7C40[];

void func_801E7B4C(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out);
s32 func_801E7B90(s32 from, s32 to, s32 t, s32 n);

#endif
