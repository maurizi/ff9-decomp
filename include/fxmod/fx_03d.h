#ifndef FX_03D_H
#define FX_03D_H

/* Effect module fx_03d (dir 13 file 0x3D). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    SVECTOR unk8; /* caster body position */
    SVECTOR unk10; /* caster rotation */
} Fx03dWork80;

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    Pool6CEntry *unkC;
    u8 pad10[8];
    SVECTOR unk18;
} Fx03dWork81; /* 0x20 */

typedef struct {
    SVECTOR unk0; /* model rotation */
    VECTOR unk8; /* model position */
    VECTOR unk18; /* model 1 scale */
    VECTOR unk28; /* model 2 scale */
    s32 unk38; /* model brightness */
    s16 unk3C; /* fade-in countdown */
    s16 unk3E; /* fade-out countdown */
    u16 unk40; /* models */
    u16 unk42;
} Fx03dWork82; /* 0x44 */

extern Unk800F78A4 *D_801E7E60;
extern BtlModel D_801E7E70[];
extern s32 D_801E8000[];

void func_801E7D7C(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out);
s32 func_801E7DC0(s32 from, s32 to, s32 t, s32 n);

#endif
