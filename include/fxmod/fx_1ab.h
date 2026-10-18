#ifndef FX_1AB_H
#define FX_1AB_H

/* Effect module fx_1ab (dir 13 file 0x1ab). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    SVECTOR unkC; /* model 1 rotation */
    VECTOR unk14; /* model 1 position */
    VECTOR unk24; /* model 1 scale */
    s32 unk34; /* brightness */
    SVECTOR unk38; /* model 2 rotation */
    VECTOR unk40; /* model 2 position */
    VECTOR unk50; /* model 2 scale */
    VECTOR unk60;
    SVECTOR unk70; /* target position */
    SVECTOR unk78;
    s16 unk80; /* fade-in countdown */
    s16 unk82; /* fade-out countdown */
    u16 unk84; /* models */
    u16 unk86;
} Fx1abWork; /* 0x88 */

extern Unk800F78A4 *D_801E7D80;
extern BtlModel D_801E7D90[];

void func_801E7C9C(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out);
s32 func_801E7CE0(s32 from, s32 to, s32 t, s32 n);

#endif
