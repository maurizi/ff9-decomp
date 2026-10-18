#ifndef FX_08E_H
#define FX_08E_H

/* Effect module fx_08e (dir 13 file 0x8E). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    Pool6CEntry *unkC;
    Pool6CEntry *unk10;
    Pool6CEntry *unk14;
    Pool6CEntry *unk18;
    SVECTOR unk1C; /* model rotation */
    VECTOR unk24; /* model position */
    VECTOR unk34; /* model scale */
    s32 unk44; /* model brightness */
    SVECTOR unk48; /* target body position */
    SVECTOR unk50;
    SVECTOR unk58; /* caster position */
    s32 unk60; /* fade-in countdown */
    s32 unk64; /* fade-out countdown */
    u16 unk68; /* models */
    u16 unk6A;
} Fx08eWork; /* 0x6C */

extern Unk800F78A4 *D_801E7E20;
extern s16 D_801E7D9C[][2];
extern BtlModel D_801E7E30[];
extern s32 D_801E7FC0[];

void func_801E7D00(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out);
s32 func_801E7D44(s32 from, s32 to, s32 t, s32 n);

#endif
