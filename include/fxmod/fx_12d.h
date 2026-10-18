#ifndef FX_12D_H
#define FX_12D_H

/* Effect module fx_12d (dir 13 file 0x12D). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    SVECTOR unkC;
} Fx12dWork80;

typedef struct {
    Pool6CEntry *unk0;
    s32 unk4;
    Pool6CEntry *unk8;
    Pool6CEntry *unkC;
    SVECTOR unk10;
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
} Fx12dWork81;

typedef struct {
    SVECTOR unk0; /* model rotation */
    VECTOR unk8; /* model position */
    VECTOR unk18; /* model scale */
    s32 unk28;
    SVECTOR unk2C; /* caster position */
    u16 unk34; /* model */
} Fx12dWork82;

extern BtlTexInfo D_801E7CC0;
extern s16 D_801E7CD8[6][2]; /* D_801E7CC0's entries in reverse order */
extern BtlTexInfo D_801E7CF0;
extern s16 D_801E7D08[6][2]; /* D_801E7CF0's entries in reverse order */
extern Unk800F78A4 *D_801E7D68;
extern BtlModel D_801E7D78[];
extern SVECTOR D_801E7E40;
extern s32 D_801E7E48[];
extern s32 D_801E8388[];

void func_801E7C24(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out);
s32 func_801E7C68(s32 from, s32 to, s32 t, s32 n);

#endif
