#ifndef FX_0F2_H
#define FX_0F2_H

/* Effect module fx_0f2 (dir 13 file 0x0F2). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    SVECTOR unk8; /* model rotation */
    VECTOR unk10; /* model position */
    VECTOR unk20; /* model scale */
    s32 unk30; /* model brightness */
    s32 unk34; /* z speed */
    SVECTOR unk38;
    SVECTOR unk40;
    SVECTOR unk48;
    s32 unk50; /* enemy side */
    s16 unk54; /* fade-in countdown */
    s16 unk56; /* fade-out countdown */
    u16 unk58; /* model */
} Fx0f2Work;

extern Unk800F78A4 *D_801E7B80;
extern BtlModel D_801E7B90[];

void func_801E7A9C(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out);
s32 func_801E7AE0(s32 from, s32 to, s32 t, s32 n);

#endif
